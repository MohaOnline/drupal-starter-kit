<?php

/**
 * @file
 * Theme-related functions for the API module.
 */

/**
 * Preprocesses theme('api_defined').
 *
 * Sets up variables for the file link and summary line.
 */
function api_preprocess_api_defined(&$variables) {
  $object = $variables['object'];
  $branch = $variables['branch'];
  $variables['file_link'] = api_file_link($object);
  $file_info = api_object_load((int) $object->file_did, $branch, 'file');

  $variables['file_summary'] = '';
  if (isset($file_info)) {
    $variables['file_summary'] = api_link_documentation($file_info->summary, $branch, $object->file_did, NULL, FALSE, FALSE, FALSE);
  }
}

/**
 * Preprocesses theme('api_class_section').
 *
 * Sets up variables for the class link and summary line.
 */
function api_preprocess_api_class_section(&$variables) {
  $class = $variables['class'];
  $branch = $variables['branch'];
  $variables['class_link'] = l($class->title, api_url($class));
  $variables['class_info'] = api_link_documentation($class->summary, $branch, $class->file_did, $class->class_did, FALSE, FALSE, $class->is_drupal);
}

/**
 * Preprocesses a documentation item page.
 */
function api_preprocess_api_object_page(&$variables) {
  // Note: branch is not defined for File pages, so don't try to create the
  // 'defined' section of the page.
  if (isset($variables['branch'])) {
    $variables['defined'] = theme('api_defined', array('branch' => $variables['branch'], 'object' => $variables['object']));
  }

  $object = $variables['object'];

  // Find the namespace.
  if (isset($object->namespace) && strlen($object->namespace)) {
    $variables['namespace'] = l($object->namespace, api_namespace_url($object->branch_id, $object->namespace));
  }
  else {
    $variables['namespace'] = '';
  }

  _api_find_alternatives($object, $variables);

  // See if this is an override of another method/etc.
  $branches = api_get_branches();
  $variables['override'] = '';
  if (!empty($object->overrides_did)) {
    $overrides = api_object_load((int) $object->overrides_did, $branches[$object->branch_id], array(
      'function',
      'property',
      'constant',
    ));
    if (isset($overrides)) {
      $variables['override'] = '<p class="api-override">' . t('Overrides !link', array('!link' => l($overrides->title, api_url($overrides)))) . '</p>';
    }
  }
}

/**
 * Preprocesses a namespace page.
 */
function api_preprocess_api_namespace_page(&$variables) {
  $namespace = $variables['name'];
  $branch = $variables['branch'];
  $branches = api_get_branches();

  // Do something like _api_find_alternatives, but namespaces are not in
  // the api_documentation table.
  $query = db_select('api_namespace', 'an')
    ->condition('an.class_name', $namespace)
    ->condition('an.object_type', 'namespace');
  $query->leftJoin('api_documentation', 'ad', 'an.did = ad.did');
  $query->fields('ad', array('branch_id'));
  $query->condition('ad.branch_id', $branch->branch_id, '<>');
  $query->groupBy('ad.branch_id');
  $query->leftJoin('api_branch', 'b', 'ad.branch_id = b.branch_id');
  $query->condition('b.project', $branch->project);

  $count = 0;
  $alternatives = array(
    '#prefix' => '<ol class="api-alternatives">',
    '#suffix' => '</ol>',
  );
  foreach ($query->execute() as $alternative) {
    $this_branch = $branches[$alternative->branch_id];
    $label = $this_branch->branch_name . ' ' . $namespace;
    $alternatives[$this_branch->project][] = array(
      '#prefix' => '<li>',
      '#markup' => l($label, api_namespace_url($alternative->branch_id, $namespace)),
      '#weight' => $this_branch->weight,
      '#suffix' => '</li>',
    );
    $count++;
  }

  if ($count) {
    $alts['other_branches'] = array(
      '#prefix' => '<details class="api-alternatives"><summary>' . t('Same name in other branches') . '</summary>',
      [$alternatives],
      '#suffix' => '</details>',
    );
  }

  $variables['alternatives'] = drupal_render($alts);
}

/**
 * Finds alternative versions of an object for the object page.
 *
 * @param object $object
 *   Object to find alternatives for.
 * @param array $variables
 *   Variables array, passed by reference, to put the alternatives in.
 */
function _api_find_alternatives($object, array &$variables) {
  switch ($object->object_type) {
    case 'file':
      $labels = array(
        'within' => t('Same filename in this branch'),
        'other' => t('Same filename and directory in other branches'),
      );
      break;

    default:
      $labels = array(
        'within' => t('Same name in this branch'),
        'other' => t('Same name and namespace in other branches'),
      );
  }

  $alternatives = array();

  // Find objects of the same name within this branch.
  if ($object->object_type == 'file') {
    // For files, the object name includes the path, so match on the title.
    $query = db_select('api_documentation', 'ad')
      ->fields('ad', array(
        'branch_id',
        'file_name',
        'object_type',
        'object_name',
      ))
      ->condition('ad.title', $object->title);
  }
  else {
    // For other objects, match on the object name and find matching names
    // within the same branch.
    $query = db_select('api_documentation', 'ad')
      ->fields('ad', array(
        'branch_id',
        'file_name',
        'object_type',
        'object_name',
        'namespaced_name',
      ))
      ->condition('ad.object_name', $object->object_name);
  }
  $query
    ->condition('ad.object_type', $object->object_type)
    ->condition('ad.branch_id', $object->branch_id);
  $within_branch = _api_make_alternative_section($query->execute(), $object, 2);

  if ($within_branch) {
    $alternatives['same_name'] = array(
      '#prefix' => '<details class="api-alternatives"><summary>' . $labels['within'] . '</summary>',
      [$within_branch],
      '#suffix' => '</details>',
    );
  }

  // Find matches within the project in other branches, using a more
  // strict matching.
  if ($object->object_type == 'file') {
    // For files, match on the object name to include the path.
    $query = db_select('api_documentation', 'ad')
      ->fields('ad', array(
        'branch_id',
        'file_name',
        'object_type',
        'object_name',
      ))
      ->condition('ad.object_name', $object->object_name);
  }
  else {
    // For non-file objects, match on the namespaced name.
    $query = db_select('api_documentation', 'ad')
      ->fields('ad', array(
        'branch_id',
        'file_name',
        'object_type',
        'object_name',
        'namespaced_name',
      ))
      ->condition('ad.namespaced_name', $object->namespaced_name);
  }
  $query
    ->condition('ad.object_type', $object->object_type)
    ->condition('ad.branch_id', $object->branch_id, '<>');

  $branch = api_get_branch_by_id($object->branch_id);
  $query->leftJoin('api_branch', 'b', 'ad.branch_id = b.branch_id');
  $query->condition('b.project', $branch->project);
  $other_branches = _api_make_alternative_section($query->execute(), $object, 1);
  if ($other_branches) {
    $alternatives['other_branches'] = array(
      '#prefix' => '<details class="api-alternatives"><summary>' . $labels['other'] . '</summary>',
      [$other_branches],
      '#suffix' => '</details>',
    );
  }

  $variables['alternatives'] = drupal_render($alternatives);
}

/**
 * Makes a set of alternative links from a query result.
 *
 * @param object $result
 *   Database query result to turn into alternatives list.
 * @param object $object
 *   Object we are making alternatives list for.
 * @param int $min_count
 *   Minimum count the alternatives list must have to consider it not empty.
 *   Typically 1 for other branches, 2 within this branch (meaning one besides
 *   the object that is being shown).
 */
function _api_make_alternative_section($result, $object, $min_count) {
  $branches = api_get_branches();
  $count = 0;
  $alternatives = array(
    '#prefix' => '<ol class="api-alternatives">',
    '#suffix' => '</ol>',
  );
  foreach ($result as $alternative) {
    // Construct link label.
    $label = $branches[$alternative->branch_id]->branch_name . ' ' . $alternative->file_name;
    $suffix = ($object->object_type == 'file') ? '' : ' ' . $alternative->namespaced_name;
    if ($object->object_type == 'function') {
      $suffix .= '()';
    }

    $alternatives[$branches[$alternative->branch_id]->project][] = array(
      '#prefix' => '<li>',
      '#markup' => l($label, api_url($alternative)) . $suffix,
      '#weight' => $branches[$alternative->branch_id]->weight,
      '#suffix' => '</li>',
    );
    $count++;
  }

  if ($count < $min_count) {
    return '';
  }
  return $alternatives;
}

/**
 * Returns HTML for a reference link on a function or file page.
 *
 * This theme hook is used for the "N functions call function()" and "N
 * functions implement hook()" links on function pages.
 *
 * @param array $variables
 *   Associative array of variables to use, with the following elements:
 *   - type: The type of reference link. See api_find_references() for list.
 *   - count: The number of referenced items.
 *   - function: The function, class, or file object being referenced.
 *   - override_text: Text to override the link text.
 *
 * @ingroup themeable
 */
function theme_api_function_reference_link(array $variables) {
  module_load_include('inc', 'api', 'api.pages');
  $type = $variables['type'];
  $count = $variables['count'];
  $function = $variables['function'];
  if (!isset($function->branch_id)) {
    return '';
  }

  $link_title = (isset($variables['override_text']) && !empty($variables['override_text'])) ? $variables['override_text'] : api_reference_text($type, $count, $function);

  // Create the link path.
  $processed_file_name = str_replace(API_FILEPATH_SEPARATOR, API_FILEPATH_SEPARATOR_REPLACEMENT, $function->file_name);
  $branch = api_get_branch_by_id($function->branch_id);

  if ($function->object_type == 'file') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/' . $type . '/' . $branch->branch_name;
  }
  elseif ($function->object_type == 'class') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/class/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }
  elseif ($function->object_type == 'interface') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/interface/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }
  elseif ($function->object_type == 'trait') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/trait/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }
  elseif ($function->object_type == 'service') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/service/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }
  elseif ($function->object_type == 'constant') {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/constant/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }
  else {
    $link_path = 'api/' . $branch->project . '/' . $processed_file_name . '/function/' . $type . '/' . $function->object_name . '/' . $branch->branch_name;
  }

  return l($link_title, $link_path);
}

/**
 * Preprocesses variables for api-views-view-dl-list.tpl.php.
 */
function api_preprocess_views_view_dl_list(&$variables) {
  $handler = $variables['view']->style_plugin;

  $class = explode(' ', $handler->options['class']);
  $class = array_map('views_clean_css_identifier', $class);

  $wrapper_class = explode(' ', $handler->options['wrapper_class']);
  $wrapper_class = array_map('views_clean_css_identifier', $wrapper_class);

  $variables['list_class'] = implode(' ', $class);
  $variables['wrapper_class'] = implode(' ', $wrapper_class);
  $variables['wrapper_prefix'] = '';
  $variables['wrapper_suffix'] = '';
  if ($variables['wrapper_class']) {
    $variables['wrapper_prefix'] = '<div class="' . $variables['wrapper_class'] . '">';
    $variables['wrapper_suffix'] = '</div>';
  }

  template_preprocess_views_view_unformatted($variables);
}
