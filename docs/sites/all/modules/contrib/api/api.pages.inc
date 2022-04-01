<?php

/**
 * @file
 * Pages (except administrative) for the API module.
 *
 * Functions whose names start with _ in this file are only intended to be
 * called by other functions in this file.
 */

module_load_include('inc', 'api', 'api.formatting');
module_load_include('inc', 'api', 'api.utilities');

/**
 * Page callback: Redirects to an appropriate search page.
 *
 * @param ...
 *   Search terms.
 *
 * @see api_menu()
 */
function api_search_redirect() {
  // See what we should search for.
  $args = func_get_args();
  if (count($args) === 0 && isset($_GET['destination']) && strpos($_GET['destination'], 'apis/') !== 0) {
    // Handling 404: search for the unknown page path.
    $tail = $_GET['destination'];
  }
  else {
    // Called directly: search for whatever was after apis/ in the path.
    $tail = implode('/', $args);
  }

  $tail = trim($tail);

  // Find the first branch with an exact match to this term.
  $query = db_select('api_documentation', 'ad');
  $query->innerJoin('api_branch', 'b', 'ad.branch_id = b.branch_id');
  $results = $query
    ->condition('ad.title', $tail)
    ->orderBy('b.weight', 'DESC')
    ->range(0, 1)
    ->fields('b', array('project', 'branch_name'))
    ->execute();
  foreach ($results as $branch) {
    // If we found an exact result, redirect to that branch's search page.
    $_GET['destination'] = 'api/' . $branch->project . '/' . $branch->branch_name . '/search/' . $tail;
    drupal_goto();
  }

  // If we get here, there was not an exact match, so look for a partial match.
  $query = db_select('api_documentation', 'ad');
  $query->innerJoin('api_branch', 'b', 'ad.branch_id = b.branch_id');
  $results = $query
    ->condition('ad.title', '%' . $tail . '%', 'LIKE')
    ->orderBy('b.weight', 'DESC')
    ->range(0, 1)
    ->fields('b', array('project', 'branch_name'))
    ->execute();
  foreach ($results as $branch) {
    // We found a partial match, so redirect to that branch's search page.
    $_GET['destination'] = 'api/' . $branch->project . '/' . $branch->branch_name . '/search/' . $tail;
    drupal_goto();
  }

  // If we get here, there was not a match.
  return t('Sorry, %term cannot be found', array('%term' => $tail));
}

/**
 * Page callback: Prints out OpenSearch plugin discovery XML output.
 *
 * @see https://developer.mozilla.org/en/Creating_OpenSearch_plugins_for_Firefox
 * @see api_menu()
 */
function api_opensearch() {
  drupal_add_http_header('Content-Type', 'text/xml; charset=utf-8');

  $short_name = variable_get('api_opensearch_name', t('Drupal API'));
  $description = variable_get('api_opensearch_description', t('Drupal API documentation'));
  $search_url = url('apis', array('absolute' => TRUE)) . '/{searchTerms}';
  $suggest_url = url('api/suggest', array('absolute' => TRUE)) . '/{searchTerms}';
  $self_url = url($_GET['q'], array('absolute' => TRUE));

  // We need to use a theme function, so initialize the theme system.
  drupal_theme_initialize();
  if (!($image = theme_get_setting('favicon'))) {
    // Fall back on default favicon if the theme didn't provide one.
    $image = 'misc/favicon.ico';
  }
  $image = url($image, array('absolute' => TRUE));

  print <<<EOD
<?xml version="1.0" encoding="UTF-8"?>
<OpenSearchDescription xmlns="http://a9.com/-/spec/opensearch/1.1/">
  <ShortName>$short_name</ShortName>
  <Description>$description</Description>
  <Image width="16" height="16" type="image/x-icon">$image</Image>
  <Url type="text/html" rel="results" method="GET" template="$search_url" />
  <Url type="application/x-suggestions+json" rel="suggestions" template="$suggest_url"/>
  <Url type="application/opensearchdescription+xml" rel="self" template="$self_url" />
</OpenSearchDescription>
EOD;
}

/**
 * Page callback: Prints JSON-formatted potential matches for OpenSearch.
 *
 * @param string $search
 *   The string to search for.
 *
 * @see http://www.opensearch.org/Specifications/OpenSearch/Extensions/Suggestions/1.0
 * @see api_menu()
 */
function api_suggest($search = '') {
  $query = db_select('api_documentation', 'd')
    ->fields('d', array('title'));
  $matches = $query->condition('d.title', '%' . db_like($search) . '%', 'LIKE')
    ->range(0, 10)
    ->execute()
    ->fetchCol();
  print drupal_json_output(array($search, array_values(array_unique($matches))));
}

/**
 * Page callback: Prints a list of functions in text format, for use in IDEs.
 *
 * @param string $branch_name
 *   Name of the branch to list functions for.
 *
 * @see api_menu()
 */
function api_page_function_dump($branch_name) {
  $query = db_select('api_documentation', 'd')
    ->fields('d', array('summary'))
    ->orderBy('d.title');
  $query->innerJoin('api_function', 'f', 'd.did = f.did');
  $query->fields('f', array('signature'));
  $query->innerJoin('api_branch', 'b', 'd.branch_id = b.branch_id');
  $query->condition('b.branch_name', $branch_name)
    ->condition('d.class_did', 0)
    ->condition('d.object_type', 'function');
  $result = $query->execute();
  foreach ($result as $object) {
    print($object->signature);
    // Make sure the summary is free of HTML tags and newlines.
    $summary = $object->summary;
    $summary = strip_tags($summary);
    $summary = preg_replace('|\s+|', ' ', $summary);
    print(' ### ' . $summary . "\n");
  }
}

/**
 * Page callback: Outputs a list of all documentation objects in JSON format.
 *
 * Note that this function outputs the JSON data directly rather than returning
 * it. Also, query parameters 'limit' and 'page' can be used to limit to a
 * certain number of results, and to get subsequent pages (both are integers and
 * page numbering starts at 0; you can omit 'page' on the first page as well).
 *
 * @param object $branch
 *   (optional) Object representing the branch to dump objects for.
 * @param string $project
 *   If $branch is empty, load the default branch from this project.
 *
 * @see api_menu()
 */
function api_dump_full_list($branch, $project = '') {
  if (!$branch) {
    $branch = api_get_branch_by_name($project);
    if (!$branch) {
      return;
    }
  }

  $query = db_select('api_documentation', 'd')
    ->fields('d', array(
      'object_name',
      'namespaced_name',
      'member_name',
      'title',
      'summary',
      'object_type',
      'branch_id',
      'file_name',
    ))
    ->condition('d.branch_id', $branch->branch_id)
    ->condition('d.object_type', 'mainpage', '<>')
    ->orderBy('d.object_name');

  // See if there is a limit or a page number.
  $params = drupal_get_query_parameters();
  if (isset($params['limit'])) {
    $page = isset($params['page']) ? (int) $params['page'] : 0;
    $limit = (int) $params['limit'];
    $start = $page * $limit;

    $query->range($start, $limit);
  }

  $result = $query->execute();

  $objs = array();
  foreach ($result as $doc) {
    $objs[] = array(
      'object_name' => $doc->object_name,
      'namespaced_name' => $doc->namespaced_name,
      'title' => $doc->title,
      'member_name' => $doc->member_name,
      'summary' => $doc->summary,
      'object_type' => $doc->object_type,
      'url' => url(api_url($doc), array('absolute' => TRUE)),
    );
  }

  drupal_json_output($objs);
}

/**
 * Page callback: Performs a search.
 *
 * @param object $branch
 *   Object representing the branch to search.
 * @param ...
 *   The string to search for, encompassing the rest of the ... arguments.
 *
 * @return string|null
 *   HTML output for the search results. If there is exactly one result,
 *   redirects to that page instead of displaying the results page.
 *
 * @see api_menu()
 */
function api_search_listing($branch) {
  // Read the search text from the ... part of the arguments.
  $search_text = func_get_args();
  array_shift($search_text);
  $search_text = trim(implode('/', $search_text));

  // If there is exactly one exact match, go directly to that page.
  $query = db_select('api_documentation', 'ad');
  $query
    ->fields('ad')
    ->condition('branch_id', $branch->branch_id)
    ->condition('title', $search_text);

  // Unfortunately, MySQL is not case-sensitive. So, to figure out if there
  // is exactly one case-sensitive match, we need to look at the results.
  $results = $query->execute();
  $exact = NULL;
  foreach ($results as $item) {
    if ($item->title == $search_text) {
      if ($exact) {
        // We have now found multiple results. On to the next step.
        $exact = NULL;
        break;
      }
      else {
        // This is an exact match. Store it.
        $exact = $item;
      }
    }
  }

  if ($exact) {
    // We found one exact match.
    drupal_goto(api_url($exact));
  }

  // If we get here, there were multiple or zero matches.
  $title = t('Search for !search', array('!search' => $search_text));
  drupal_set_title($title);
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('API reference'), 'api/' . $branch->project),
    l($branch->title, 'api/' . $branch->project . '/' . $branch->branch_name),
  );
  drupal_set_breadcrumb($breadcrumb);
  $plain_title = t('Search for @search', array('@search' => $search_text));
  $page_title = array(
    check_plain($plain_title),
    check_plain($branch->title),
    check_plain(variable_get('site_name', 'Drupal')),
  );
  api_set_html_page_title(implode(' | ', $page_title));

  $output = api_display_view('api_search', 'block_search_results', array($branch->branch_id, $search_text));

  return _api_search_links($branch, $search_text) . $output;
}

/**
 * Renders a list of links to search other branches for the same text.
 *
 * @param object $branch
 *   The current branch being searched.
 * @param string $search_text
 *   The text being searched for.
 *
 * @return string
 *   HTML for links to search other branches for the same text, and a link to
 *   the Projects page.
 */
function _api_search_links($branch, $search_text) {
  $search_text = check_plain($search_text);
  $others = db_select('api_branch', 'b')
    ->fields('b')
    ->condition('b.project', $branch->project)
    ->condition('b.branch_id', $branch->branch_id, '<>')
    ->orderBy('b.branch_name')
    ->execute();

  $links = array();
  foreach ($others as $other) {
    $links[] = '<li>' . l(t('Search @branch_name for !search_text', array('@branch_name' => $other->title, '!search_text' => $search_text)), 'api/' . $other->project . '/' . $other->branch_name . '/search/' . $search_text) . '</li>';
  }

  $other = api_other_projects_link(FALSE);
  if (strlen($other)) {
    $links[] = '<li>' . $other . '</li>';
  }

  if (!count($links)) {
    return '';
  }

  return '<ol class="api-alternatives">' . implode("\n", $links) . '</ol>';
}

/**
 * Page callback: Displays a list of links to projects using a pager.
 *
 * @return string
 *   Themed HTML output for the page.
 */
function api_page_projects() {
  $output = array();

  $output['project_form'] = drupal_get_form('api_project_form');

  $links = api_display_view('api_projects', 'block_project_list', array());
  if ($links) {
    $output['project_list'] = array(
      '#type' => 'markup',
      '#markup' => $links,
    );
  }

  return $output;
}

/**
 * Form constructor for the project select autocomplete form.
 */
function api_project_form($form, $form_state) {
  $form['project'] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'api-project-autocomplete',
    '#title' => t('Project short name'),
    '#default_value' => !empty($form_state['values']['project']) ? $form_state['values']['project'] : '',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
  );

  return $form;
}

/**
 * Form submission handler for api_project_form().
 */
function api_project_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'api/' . $form_state['values']['project'];
}

/**
 * Page callback: Displays documentation for a file.
 *
 * @param object $file
 *   Documentation object representing the file to display.
 *
 * @return string
 *   HTML output for the file documentation page.
 *
 * @see api_menu()
 */
function api_page_file($file) {
  $branch = api_get_branch_by_id($file->branch_id);

  _api_object_title_and_breadcrumb($branch, $file);

  $documentation = api_link_documentation($file->documentation, $branch, $file->did, NULL, FALSE, FALSE, $file->is_drupal);
  $see = api_link_documentation($file->see, $branch, $file->did, NULL, TRUE, TRUE, $file->is_drupal);
  $deprecated = api_link_documentation($file->deprecated, $branch, $file->did, NULL, TRUE, FALSE, $file->is_drupal);
  $related_topics = api_display_view('api_references', 'block_related_topics', array($file->did));

  $code = api_link_code($file->code, $branch, $file->did, NULL, $file->is_drupal);
  $defined = $file->file_name;

  $types = array(
    'function' => t('Functions'),
    'constant' => t('Constants'),
    'global' => t('Globals'),
    'class' => t('Classes'),
    'interface' => t('Interfaces'),
    'trait' => t('Traits'),
    'service' => t('Services'),
  );

  $objects = array();
  foreach ($types as $type => $label) {
    // Render the view of this type of object in this file. Note that
    // Views arguments must not contain / characters, because it screws up
    // the parsing if you do an Ajax sort later on.
    $out = api_display_view('api_listings', 'block_items_file', array(
      str_replace('/', '!', $file->object_name),
      $type,
      $file->branch_id,
    ));
    // Only display if there was something there.
    if ($out) {
      $objects[$type] = array(
        '#type' => 'markup',
        '#markup' => '<h3>' . $label . '</h3>' . $out,
      );
    }
  }

  $objects = drupal_render($objects);

  // If this is a theme template or YML file, make reference link.
  $links = api_build_references_section(array(
    'theme_invokes',
    'yml_config',
    'yml_keys',
  ), $file, $branch);

  $output = theme('api_file_page', array(
    'object' => $file,
    'documentation' => $documentation,
    'objects' => $objects,
    'code' => $code,
    'see' => $see,
    'deprecated' => $deprecated,
    'related_topics' => $related_topics,
    'defined' => $defined,
    'call_links' => $links,
  ));
  $output .= _api_add_comments($file);
  return $output;
}

/**
 * Page callback: Generates the default documentation page for a branch.
 *
 * @param object $branch
 *   (optional) Branch object giving the branch to display documentation for.
 *   If NULL, $project must be specified, and the default branch for that
 *   project will be displayed.
 * @param string $project
 *   (optional) If $branch is not given, display this project's default branch.
 *   Ignored if $branch is given.
 *
 * @return array|null
 *   Render array for the page, or NULL if the page doesn't work.
 *
 * @see api_menu()
 */
function api_page_branch($branch, $project = '') {
  if (!$branch) {
    $branch = api_get_branch_by_name($project);
    if (!$branch) {
      return;
    }
  }

  // Set HTML page title.
  $title = t('API reference');
  $page_title = array(
    check_plain($branch->title),
    check_plain($title),
    check_plain(variable_get('site_name', 'Drupal')),
  );
  api_set_html_page_title(implode(' | ', $page_title));

  $output = array();
  $output['navigation'] = _api_make_navigation($branch);

  $docs = db_select('api_documentation', 'd')
    ->fields('d', array('documentation'))
    ->condition('branch_id', $branch->branch_id)
    ->condition('object_type', 'mainpage')
    ->execute()
    ->fetchObject();
  if ($docs) {
    $output['docs'] = array(
      '#type' => 'markup',
      '#markup' => api_link_documentation($docs->documentation, $branch),
    );
  }
  else {
    $output['docs'] = array(
      '#theme' => 'api_branch_default_page',
      '#branch' => $branch,
    );
  }

  return $output;
}

/**
 * Page callback: Generates a listing page for an object type.
 *
 * @param object $branch
 *   (optional) Object representing branch to generate the listing for.
 * @param string $object_type
 *   Type of object to list ('file', 'group', etc.), or 'deprecated' for the
 *   Deprecated page.
 * @param bool $is_page
 *   (optional) TRUE if this is on its own page (default), FALSE if it is
 *   included in another page.
 * @param string $project
 *   Name of the project. Used if $branch is NULL, meaning find the default
 *   branch for this project.
 *
 * @return array|null
 *   Render array for the listing page, or NULL if the page doesn't work.
 */
function api_page_listing($branch, $object_type, $is_page = TRUE, $project = '') {
  if (!$branch) {
    $branch = api_get_branch_by_name($project);
    if (!$branch) {
      return;
    }
  }

  // Set the HTML page title and breadcrumb.
  if ($is_page) {
    $breadcrumb = array(
      l(t('Home'), '<front>'),
      l(t('API reference'), 'api/' . $branch->project),
      l($branch->title, 'api/' . $branch->project . '/' . $branch->branch_name),
    );
    drupal_set_breadcrumb($breadcrumb);

    $titles = array(
      'class' => t('Classes, traits, and interfaces'),
      'constant' => t('Constants'),
      'file' => t('Files'),
      'function' => t('Functions'),
      'global' => t('Globals'),
      'group' => t('Topics'),
      'namespace' => t('Namespaces'),
      'deprecated' => t('Deprecated'),
      'service' => t('Services'),
      'element' => t('Form and render elements'),
    );
    $title = $titles[$object_type];
    drupal_set_title($title);

    $page_title = array(
      check_plain($title),
      check_plain($branch->title),
      check_plain(variable_get('site_name', 'Drupal')),
    );
    api_set_html_page_title(implode(' | ', $page_title));
  }

  $output = array();

  if ($is_page) {
    $output['navigation'] = _api_make_navigation($branch, $object_type);
  }

  // See if there is a special defgroup in this branch with the name
  // 'listing_page_OBJECT_TYPE'. If so, display its documentation.
  $defgroup = api_object_load('listing_page_' . $object_type, $branch, 'group');
  if ($defgroup) {
    $documentation = api_link_documentation($defgroup->documentation, $branch, $defgroup->file_did, NULL, FALSE, FALSE, $defgroup->is_drupal);
    $see = api_link_documentation($defgroup->see, $branch, $defgroup->file_did, NULL, TRUE, TRUE, $defgroup->is_drupal);
    $output['heading'] = array(
      '#theme' => 'api_group_page',
      '#documentation' => $documentation,
      '#see' => $see,
      '#object' => $defgroup,
      '#hide_alternatives' => TRUE,
    );
  }

  if ($object_type === 'function' || $object_type == 'constant' || $object_type == 'global') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_listings', 'block_listing', array($object_type, $branch->branch_id)),
    );
  }
  elseif ($object_type === 'group') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_listings', 'block_groups', array($branch->branch_id)),
    );
  }
  elseif ($object_type === 'service') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_listings', 'block_services', array($branch->branch_id)),
    );
  }
  elseif ($object_type === 'element') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_listings', 'block_elements', array($branch->branch_id)),
    );
  }
  elseif ($object_type === 'file') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_listings', 'block_files', array($branch->branch_id)),
    );
  }
  elseif ($object_type === 'class') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_listings', 'block_class', array($branch->branch_id)),
    );
  }
  elseif ($object_type === 'namespace') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_namespaces', 'block_namespace_list', array($branch->branch_id)),
    );
  }
  elseif ($object_type === 'deprecated') {
    $output['listing'] = array(
      '#type' => 'markup',
      '#markup' => api_display_view('api_listings', 'block_deprecated', array($branch->branch_id)),
    );
  }

  if ($is_page) {
    $output['suffix'] = array(
      '#type' => 'markup',
      '#markup' => api_other_projects_link(),
    );
  }

  return $output;
}

/**
 * Generates tab navigation for listing pages.
 *
 * @param object $branch
 *   Branch of the current page.
 * @param string $object_type
 *   (optional) Object type of the listing page; leave out for the branch
 *   home page.
 *
 * @return array
 *   Render array for tab navigation.
 */
function _api_make_navigation($branch, $object_type = '') {
  switch ($object_type) {
    case '':
      $suffix = '';
      break;

    case 'class':
      $suffix = 'classes';
      break;

    case 'deprecated':
      $suffix = 'deprecated';
      break;

    default:
      $suffix = $object_type . 's';
  }

  // Make links to all the other branches' listing pages within this
  // project.
  $branches = api_get_branches();
  $links = array();
  foreach ($branches as $other_branch) {
    if ($other_branch->project == $branch->project) {
      if ($suffix) {
        $path = 'api/' . $branch->project . '/' . $suffix . '/' . $other_branch->branch_name;
      }
      else {
        $path = 'api/' . $branch->project . '/' . $other_branch->branch_name;
      }
      $links[$other_branch->title . $other_branch->branch_id] = array(
        'title' => $other_branch->title,
        'href' => $path,
        'active' => ($other_branch->branch_id == $branch->branch_id),
      );
    }
  }

  if ($links) {
    ksort($links);
    // Output this as a Local tasks array.
    $output = array(
      '#theme' => 'menu_local_tasks',
      '#primary' => array(),
    );
    foreach ($links as $link) {
      $link['localized_options'] = array();
      $output['#primary'][] = array(
        '#theme' => 'menu_local_task',
        '#link' => $link,
        '#active' => $link['active'],
      );
    }
    return $output;
  }

  return NULL;
}

/**
 * Page callback: Generates a namespace page.
 *
 * @param string $project
 *   Short name of project to generate page for.
 * @param string $branch_name
 *   Short name of branch to generate page for.
 * @param string $namespace
 *   Namespace to generate page for (with substitution characters).
 *
 * @return string
 *   HTML for the namespace page.
 */
function api_page_namespace($project, $branch_name, $namespace) {

  $branch = api_get_branch_by_name($project, $branch_name);
  if (is_null($branch)) {
    return MENU_NOT_FOUND;
  }

  $namespace = str_replace(API_FILEPATH_SEPARATOR_REPLACEMENT, API_NAMESPACE_SEPARATOR, $namespace);

  // Set the HTML page title and breadcrumb.
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('API reference'), 'api/' . $branch->project),
    l($branch->title, 'api/' . $branch->project . '/' . $branch->branch_name),
  );
  drupal_set_breadcrumb($breadcrumb);

  // Do not use t() here since this is a PHP construct.
  $title = 'namespace ' . $namespace;
  drupal_set_title($title);

  $page_title = array(
    check_plain($title),
    check_plain($branch->title),
    check_plain(variable_get('site_name', 'Drupal')),
  );
  api_set_html_page_title(implode(' | ', $page_title));

  $output = theme('api_namespace_page', array(
    'branch' => $branch,
    'name' => $namespace,
    'listing' => api_display_view('api_namespaces', 'block_items_namespace', array($branch->branch_id, $namespace)),
  ));

  return $output;
}

/**
 * Generates a link to the projects page if there is more than one project.
 *
 * @param bool $wrap_in_p
 *   TRUE to wrap in a P tag; FALSE to not do that.
 *
 * @return string
 *   Link to the API projects page, maybe inside a P tag, or an empty string if
 *   there would only be one project to display on that page.
 */
function api_other_projects_link($wrap_in_p = TRUE) {
  // Figure out how many different projects there are.
  $count = db_select('api_project', 'p')
    ->countQuery()
    ->execute()
    ->fetchField();
  if ($count > 1) {
    $text = '';
    if ($wrap_in_p) {
      $text .= '<p class="api-switch"><strong>';
    }
    $text .= l(t('Other projects'), 'api/projects');
    if ($wrap_in_p) {
      $text .= '</strong></p>';
    }

    return $text;
  }

  return '';
}

/**
 * Page callback: Displays documentation for a function.
 *
 * @param object $function
 *   Documentation object representing the function to display.
 *
 * @return string
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_function($function) {
  $branches = api_get_branches();
  $branch = $branches[$function->branch_id];

  _api_object_title_and_breadcrumb($branch, $function);

  // Build the page sections.
  $documentation = api_link_documentation($function->documentation, $branch, $function->file_did, $function->class_did, FALSE, FALSE, $function->is_drupal);

  $parameters = api_link_documentation($function->parameters, $branch, $function->file_did, $function->class_did, TRUE, FALSE, $function->is_drupal);

  $return = api_link_documentation($function->return_value, $branch, $function->file_did, $function->class_did, TRUE, FALSE, $function->is_drupal);
  $see = api_link_documentation($function->see, $branch, $function->file_did, $function->class_did, TRUE, TRUE, $function->is_drupal);
  $deprecated = api_link_documentation($function->deprecated, $branch, $function->file_did, $function->class_did, TRUE, FALSE, $function->is_drupal);
  $throws = api_link_documentation($function->throws, $branch, $function->file_did, $function->class_did, TRUE, FALSE, $function->is_drupal);
  $code = api_link_code($function->code, $branch, $function->file_did, $function->class_did, $function->is_drupal);
  $related_topics = api_display_view('api_references', 'block_related_topics', array($function->did));
  $class = _api_class_section($function, $branch);

  // Build reference links.
  $links = api_build_references_section(array(
    'calls',
    'references',
    'implementations',
    'invokes',
    'theme_invokes',
    'overrides',
  ), $function, $branch);

  // Put it all together and theme the output.
  $output = theme('api_function_page', array(
    'branch' => $branch,
    'object' => $function,
    'documentation' => $documentation,
    'parameters' => $parameters,
    'return' => $return,
    'related_topics' => $related_topics,
    'call_links' => $links,
    'code' => $code,
    'see' => $see,
    'deprecated' => $deprecated,
    'throws' => $throws,
    'class' => $class,
  ));
  $output .= _api_add_comments($function);
  return $output;
}

/**
 * Page callback: Displays a list of instances where a function is called.
 *
 * @param object $function
 *   Documentation object representing the function to display.
 * @param string $type
 *   Type of page to generate. See api_find_references() for list.
 *
 * @return string
 *   HTML output for the page.
 */
function api_page_function_calls($function, $type) {
  $branch = api_get_branch_by_id($function->branch_id);

  $call_count = api_find_references($function, $branch, $type, TRUE, $function->did, 0, $function->is_drupal);
  $call_title = api_reference_text($type, $call_count, $function);
  _api_object_title_and_breadcrumb($branch, $function, $call_title, TRUE);

  return api_list_references($function, $branch, $type, 0, $function->is_drupal);
}

/**
 * Page callback: Displays a full class hierarchy.
 *
 * @param object $class
 *   Documentation object representing the class to display.
 *
 * @return string
 *   HTML output for the page.
 */
function api_page_class_hierarchy($class) {
  $branch = api_get_branch_by_id($class->branch_id);

  $call_title = api_reference_text('hierarchy', 0, $class);
  _api_object_title_and_breadcrumb($branch, $class, $call_title, TRUE);

  return _api_class_hierarchy($class, 'full');
}

/**
 * Page callback: Displays a full interface implementations list.
 *
 * @param object $class
 *   Documentation object representing the interface to display.
 *
 * @return string
 *   HTML output for the page.
 */
function api_page_interface_implements($class) {
  $branch = api_get_branch_by_id($class->branch_id);

  $call_title = api_reference_text('implements', 0, $class);
  _api_object_title_and_breadcrumb($branch, $class, $call_title, TRUE);

  // Make a list of all the classes that implement the interface.
  $to_process = array($class->did => $class);
  $processed = array();
  $found = array();
  while (count($to_process)) {
    $todo = array_shift($to_process);
    $processed[$todo->did] = $todo;
    if ($todo->object_type == 'class') {
      _api_class_process_inherits($todo, $to_process, $processed, $found);
    }
    else {
      _api_interface_process_inherits($todo, $to_process, $processed, $found);
    }
  }

  // Sort alphabetically and return it.
  if ($found) {
    ksort($found);
    return theme('item_list', array('items' => $found));
  }
  else {
    return t('No classes implement this interface');
  }
}

/**
 * Page callback: Displays documentation for an item (constant, global, etc.).
 *
 * This function can be used to display "simple" items (items without listings,
 * and functions are a special case as well, because they display signatures
 * and variations).
 *
 * @param object $item
 *   Documentation object representing the item to display.
 * @param string $type
 *   Type of item, one of: 'constant', 'global', 'property'.
 *
 * @return string
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_simple_item($item, $type) {
  $branch = api_get_branch_by_id($item->branch_id);

  _api_object_title_and_breadcrumb($branch, $item);

  $documentation = api_link_documentation($item->documentation, $branch, $item->file_did, $item->class_did, FALSE, FALSE, $item->is_drupal);
  $code = api_link_code($item->code, $branch, $item->file_did, $item->class_did, $item->is_drupal);
  $related_topics = api_display_view('api_references', 'block_related_topics', array($item->did));
  $see = api_link_documentation($item->see, $branch, $item->file_did, $item->class_did, TRUE, TRUE, $item->is_drupal);
  $deprecated = api_link_documentation($item->deprecated, $branch, $item->file_did, $item->class_did, TRUE, FALSE, $item->is_drupal);
  $var = '';
  if ($type == 'property') {
    $var = api_link_name($item->var, $branch, '', '', $item->file_did, $item->class_did, NULL, FALSE, TRUE, NULL, NULL, 'class');
  }

  $class = _api_class_section($item, $branch);

  $links = ($type == 'constant') ? api_build_references_section(array('constants'), $item, $branch) : array();

  $theme_hooks = array(
    'constant' => 'api_constant_page',
    'global' => 'api_global_page',
    'property' => 'api_property_page',
  );

  $output = theme($theme_hooks[$type], array(
    'branch' => $branch,
    'object' => $item,
    'documentation' => $documentation,
    'code' => $code,
    'related_topics' => $related_topics,
    'see' => $see,
    'deprecated' => $deprecated,
    'var' => $var,
    'class' => $class,
    'call_links' => $links,
  ));

  $output .= _api_add_comments($item);
  return $output;
}

/**
 * Page callback: Displays documentation for a class.
 *
 * @param object $class
 *   Documentation object representing the class to display.
 *
 * @return string
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_class($class) {
  $branch = api_get_branch_by_id($class->branch_id);

  _api_object_title_and_breadcrumb($branch, $class);

  $documentation = api_link_documentation($class->documentation, $branch, $class->file_did, $class->did, FALSE, FALSE, $class->is_drupal);
  $related_topics = api_display_view('api_references', 'block_related_topics', array($class->did));
  $code = api_link_code($class->code, $branch, $class->file_did, $class->did, $class->is_drupal);
  $see = api_link_documentation($class->see, $branch, $class->file_did, $class->did, TRUE, TRUE, $class->is_drupal);
  $deprecated = api_link_documentation($class->deprecated, $branch, $class->file_did, $class->did, TRUE, FALSE, $class->is_drupal);

  // Figure out the class hierarchy.
  $hierarchy = _api_class_hierarchy($class);

  // Find and render all the class members.
  $objects = array();
  $out = api_display_view('api_members', 'block_member_list', array($class->did));
  // Only display if there was something there.
  if ($out) {
    $objects['members'] = array(
      '#type' => 'markup',
      '#markup' => '<h3>' . t('Members') . '</h3>' . $out,
    );
  }

  $objects = drupal_render($objects);

  $reference_types = array('uses', 'references');
  if ($class->object_type == 'class') {
    $reference_types[] = 'annotations';
    $reference_types[] = 'element_invokes';
    $reference_types[] = 'services';
  }
  elseif ($class->object_type == 'interface') {
    $reference_types[] = 'implements';
    $reference_types[] = 'services';
  }

  $links = api_build_references_section($reference_types, $class, $branch);

  // Put it all together.
  $output = theme('api_class_page', array(
    'branch' => $branch,
    'object' => $class,
    'documentation' => $documentation,
    'hierarchy' => $hierarchy,
    'objects' => $objects,
    'code' => $code,
    'related_topics' => $related_topics,
    'see' => $see,
    'deprecated' => $deprecated,
    'call_links' => $links,
  ));
  $output .= _api_add_comments($class);

  return $output;
}

/**
 * Page callback: Displays documentation for a service.
 *
 * @param object $service
 *   Documentation object representing the service to display.
 *
 * @return string
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_service($service) {
  $branch = api_get_branch_by_id($service->branch_id);

  _api_object_title_and_breadcrumb($branch, $service);

  // The class name (if there is one), or some other information is stored
  // in the documentation field.
  $class = api_link_documentation($service->documentation, $branch, $service->file_did, 0, TRUE, FALSE, $service->is_drupal);
  $code = api_link_code($service->code, $branch, $service->file_did, $service->did, $service->is_drupal);

  // Make a list of the service tags. These are in the api_references table.
  $results = db_select('api_reference_storage', 'r')
    ->condition('r.object_type', 'service_tag')
    ->condition('r.from_did', $service->did)
    ->fields('r', array('object_name'))
    ->execute()
    ->fetchCol();
  if (count($results)) {
    $tags = theme('item_list', array('items' => $results));
  }
  else {
    $tags = FALSE;
  }

  $links = api_build_references_section(array('use'), $service, $branch);

  // Put it all together.
  $output = theme('api_service_page', array(
    'branch' => $branch,
    'object' => $service,
    'class' => $class,
    'code' => $code,
    'tags' => $tags,
    'call_links' => $links,
  ));
  $output .= _api_add_comments($service);

  return $output;
}

/**
 * Page callback: Displays documentation for a group (topic).
 *
 * @param object $group
 *   Documentation object representing the group (topic) to display.
 *
 * @return string
 *   HTML output for the page.
 *
 * @see api_menu()
 */
function api_page_group($group) {
  $branch = api_get_branch_by_id($group->branch_id);

  _api_object_title_and_breadcrumb($branch, $group);

  $documentation = api_link_documentation($group->documentation, $branch, $group->file_did, NULL, FALSE, FALSE, $group->is_drupal);
  $see = api_link_documentation($group->see, $branch, $group->file_did, NULL, TRUE, TRUE, $group->is_drupal);
  $related_topics = api_display_view('api_references', 'block_related_topics', array($group->did));

  // Find and render all items in this group.
  $types = array(
    'function' => t('Functions'),
    'constant' => t('Constants'),
    'global' => t('Globals'),
    'class' => t('Classes'),
    'interface' => t('Interfaces'),
    'trait' => t('Traits'),
    'file' => t('Files'),
    'group' => t('Sub-Topics'),
  );

  $objects = array();
  foreach ($types as $type => $label) {
    // Render the view of this type of object in this group.
    $out = api_display_view('api_references', 'block_items_in_group', array(
      $branch->branch_id,
      $group->object_name,
      $type,
    ));
    // Only display if there was something there.
    if ($out) {
      $objects[$type] = array(
        '#type' => 'markup',
        '#markup' => '<h3>' . $label . '</h3>' . $out,
      );
    }
  }

  $objects = drupal_render($objects);

  $output = theme('api_group_page', array(
    'branch' => $branch,
    'object' => $group,
    'documentation' => $documentation,
    'objects' => $objects,
    'see' => $see,
    'related_topics' => $related_topics,
  ));
  $output .= _api_add_comments($group);
  return $output;
}

/**
 * Sets the page title and breadcrumb for an object display page.
 *
 * @param object $branch
 *   Object representing the branch.
 * @param object $object
 *   Object representing the documentation item on the current page.
 * @param string|null $title
 *   A string to be used as a replacement for the title (optional). Assumed to
 *   already be sanitized.
 * @param bool $object_name_in_breadcrumb
 *   A boolean indicating whether to add the object name to the breadcrumb.
 */
function _api_object_title_and_breadcrumb($branch, $object, $title = NULL, $object_name_in_breadcrumb = FALSE) {
  // Allow the title to be overridden.
  if (empty($title)) {
    $title = check_plain($object->title);
    if ($object->object_type == 'group' || $object->object_type == 'file') {
      $h1title = $title;
    }
    else {
      $h1title = (empty($object->modifiers)) ? '' : check_plain($object->modifiers) . ' ';
      $h1title .= check_plain($object->object_type) . ' ' . $title;
    }
  }
  else {
    $h1title = $title;
  }
  drupal_set_title($h1title, PASS_THROUGH);

  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('API reference'), 'api/' . $branch->project),
    l($branch->title, 'api/' . $branch->project . '/' . $branch->branch_name),
  );
  $page_title = array(
    check_plain(variable_get('site_name', 'Drupal')),
    check_plain($branch->title),
  );

  if ($object->object_type !== 'file') {
    $breadcrumb[] = l(basename($object->file_name), api_url($object, TRUE));
    $page_title[] = check_plain(basename($object->file_name));
  }
  if (!empty($object->class_did)) {
    $branch = api_get_branch_by_id($object->branch_id);
    $class = api_object_load((int) $object->class_did, $branch, array(
      'interface',
      'class',
      'trait',
    ), $object->file_name);
    if (isset($class)) {
      $breadcrumb[] = l($class->object_name, api_url($class));
    }
    // Note that this is not needed in the page title, since the object name
    // includes the class.
  }

  // Add the object name to the breadcrumb. This is used for function calls and
  // hook implementations listing pages.
  if ($object_name_in_breadcrumb) {
    $breadcrumb[] = l($object->object_name, api_url($object));
  }

  $page_title[] = $title;

  drupal_set_breadcrumb($breadcrumb);
  api_set_html_page_title(implode(' | ', array_reverse($page_title)));
}

/**
 * Renders a class hierarchy, either full or partial.
 *
 * @param object $class
 *   Class object with at least did, branch_id, file_name, object_type, and
 *   object_name properties.
 * @param string $type
 *   One of the following strings to indicate what type of hierarchy:
 *   - 'full': Full hierarchy, showing all parents, siblings, children, etc.
 *   - 'ancestors': Only direct ancestors of this class.
 *
 * @return string
 *   HTML string containing the class hierarchy, or an empty string if the
 *   only thing to display would be the class itself.
 */
function _api_class_hierarchy($class, $type = 'ancestors') {
  // Avoid inheritance loops.
  $processed = array();

  $branch = api_get_branch_by_id($class->branch_id);

  // See if this class has any children. Note that this is only in the same
  // branch.
  $child_output = '';
  if ($type == 'full') {
    $children = _api_class_children($class, $class->did);
    if (count($children)) {
      $child_output = theme('item_list', array('items' => $children));
    }
  }

  // Find the direct-line extends ancestors of this class, only in the same
  // branch.
  $parent = $class;
  $current = $class;

  while ($parent) {
    $processed[] = $parent->did;

    // Query to find the next ancestor.
    $query = db_select('api_reference_storage', 'ars');
    $ad = $query->innerJoin('api_documentation', 'ad', 'ad.did = ars.extends_did');
    $query
      ->fields($ad, array(
        'did',
        'branch_id',
        'file_name',
        'object_type',
        'object_name',
        'namespaced_name',
      ))
      ->condition('ars.from_did', $parent->did)
      ->condition('ars.object_type', 'class')
      ->condition('ad.object_type', array('class', 'interface'));
    $results = $query->execute()->fetchAll();
    $count = count($results);
    if ($count < 1) {
      $parent = _api_class_hierarchy_line($current, $class->did);
      break;
    }
    $parent = array_pop($results);
    if ($count == 1 && $type == 'full') {
      $siblings = _api_class_children($parent, $class->did);
    }
    else {
      $siblings = array($current->did => _api_class_hierarchy_line($current, $class->did));
    }

    $siblings[$current->did] .= $child_output;
    $child_output = theme('item_list', array('items' => $siblings));
    $current = $parent;

    if ($count > 1) {
      // If we found more than one result, that means that either there were
      // multiple results with the same class name, or this was an interface
      // with multiple inheritance (hopefully not both!). So, if the results
      // have the same name, add a line indicating a search. If they have
      // different names, list them all. In either case, don't attempt any more
      // lines of hierarchy.
      $same = TRUE;
      foreach ($results as $item) {
        if ($item->object_name != $parent->object_name) {
          $same = FALSE;
          break;
        }
      }

      if ($same) {
        $parent = l(t('Multiple classes named @classname', array('@classname' => $parent->object_name)), 'api/' . $branch->project . '/' . $branch->branch_name . '/search/' . check_plain($parent->object_name));
      }
      else {
        $this_line = array();
        array_push($results, $parent);
        foreach ($results as $item) {
          $this_line[] = _api_class_hierarchy_line($item, $class->did);
        }
        $parent = implode('; ', $this_line);
      }
      break;
    }

    // If we get here, make sure we're not in an infinite loop.
    if (in_array($parent->did, $processed)) {
      $parent = _api_class_hierarchy_line($current, $class->did);
      break;
    }
  }

  $output = theme('item_list', array('items' => array($parent . $child_output)));

  if ($type != 'full' && $class->object_type != 'trait') {
    $output .= '<p>' . theme('api_function_reference_link', array(
      'type' => 'hierarchy',
      'count' => 0,
      'function' => $class,
    )) . '</p>';
  }

  if ($type != 'full' && $class->object_type == 'interface') {
    $output .= '<p>' . theme('api_function_reference_link', array(
      'type' => 'implements',
      'count' => 0,
      'function' => $class,
    )) . '</p>';
  }

  return $output;
}

/**
 * Finds and renders the children of a class within the same branch.
 *
 * @param object $class
 *   Documentation object for the class.
 * @param int $current_did
 *   Documentation ID of the class on the current page.
 *
 * @return array
 *   Array of children. Keys are class names, and values are output
 *   of _api_class_hierarchy_line().
 */
function _api_class_children($class, $current_did = 0) {
  $children = array();
  $query = db_select('api_reference_storage', 'ars');
  $ad = $query->innerJoin('api_documentation', 'ad', 'ad.did = ars.from_did');
  $query
    ->fields($ad, array(
      'did',
      'branch_id',
      'file_name',
      'object_type',
      'object_name',
      'namespaced_name',
    ))
    ->condition('ars.extends_did', $class->did)
    ->condition('ad.object_type', array('class', 'interface'))
    ->condition('ars.object_type', 'class')
    ->orderBy('ad.namespaced_name');
  $result = $query->execute();
  foreach ($result as $object) {
    $children[$object->did] = _api_class_hierarchy_line($object, $current_did);
  }

  return $children;
}

/**
 * Processes an interface for api_page_interface_implements().
 *
 * Finds all interfaces that extend it and classes that implement it, directly,
 * and adds them to the to do and found lists.
 *
 * @param object $interface
 *   Documentation object for the interface to check.
 * @param object[] $to_process
 *   List of classes and interfaces still to be processed, keyed by
 *   documentation ID.
 * @param object[] $processed
 *   List of classes and interfaces that have already been processed, keyed
 *   by documentation ID.
 * @param string[] $found
 *   Array of classes found to implement the interface. Keys are the
 *   a sort index; values are lines like in a class hierarchy.
 */
function _api_interface_process_inherits($interface, array &$to_process, array &$processed, array &$found) {
  // Find interfaces that directly extend this one.
  $exclude = array_merge(array_keys($to_process), array_keys($processed));
  $query = db_select('api_reference_storage', 'ars');
  $ad = $query->innerJoin('api_documentation', 'ad', 'ad.did = ars.from_did');
  $query
    ->fields($ad, array(
      'did',
      'branch_id',
      'file_name',
      'object_type',
      'object_name',
    ))
    ->condition('ars.extends_did', $interface->did)
    ->condition('ad.object_type', array('interface'))
    // In reference storage, any "extends" gets saved as type "class".
    ->condition('ars.object_type', 'class')
    ->condition('ad.did', $exclude, 'NOT IN');
  $result = $query->execute();
  foreach ($result as $object) {
    $to_process[$object->did] = $object;
  }

  // Find classes that directly implement this interface.
  $query = db_select('api_reference_storage', 'ars');
  $ad = $query->innerJoin('api_documentation', 'ad', 'ad.did = ars.from_did');
  $query
    ->fields($ad, array(
      'did',
      'branch_id',
      'file_name',
      'object_type',
      'object_name',
      'namespaced_name',
    ))
    ->condition('ars.extends_did', $interface->did)
    ->condition('ad.object_type', array('class'))
    // In reference storage, any "implements" gets saved as type "interface".
    ->condition('ars.object_type', 'interface')
    ->condition('ad.did', $exclude, 'NOT IN');
  $result = $query->execute();
  foreach ($result as $object) {
    $to_process[$object->did] = $object;
    $found[$object->namespaced_name . $object->did] = _api_class_hierarchy_line($object);
  }
}

/**
 * Processes a class for api_page_interface_implements().
 *
 * Finds all classes that extend it directly, and adds them to the to do and
 * found lists.
 *
 * @param object $class
 *   Documentation object for the class to check.
 * @param object[] $to_process
 *   List of classes and interfaces still to be processed, keyed by
 *   documentation ID.
 * @param object[] $processed
 *   List of classes and interfaces that have already been processed, keyed
 *   by documentation ID.
 * @param string[] $found
 *   Array of classes found to implement the interface. Keys are the
 *   a sort index; values are lines like in a class hierarchy.
 */
function _api_class_process_inherits($class, array &$to_process, array &$processed, array &$found) {
  // Find classes that directly extend this one.
  $exclude = array_merge(array_keys($to_process), array_keys($processed));
  $query = db_select('api_reference_storage', 'ars');
  $ad = $query->innerJoin('api_documentation', 'ad', 'ad.did = ars.from_did');
  $query
    ->fields($ad, array(
      'did',
      'branch_id',
      'file_name',
      'object_type',
      'object_name',
      'namespaced_name',
    ))
    ->condition('ars.extends_did', $class->did)
    ->condition('ad.object_type', array('class'))
    // In reference storage, any "extends" gets saved as type "class".
    ->condition('ars.object_type', 'class')
    ->condition('ad.did', $exclude, 'NOT IN');
  $result = $query->execute();
  foreach ($result as $object) {
    $to_process[$object->did] = $object;
    $found[$object->namespaced_name . $object->did] = _api_class_hierarchy_line($object);
  }
}

/**
 * Renders the class hierarchy line for a single class.
 *
 * @param object $class
 *   Documentation object for the class to render.
 * @param int $current_did
 *   Documentation ID of the class that the current page is showing. If this
 *   class is being shown, it will have an 'active' class on the link.
 *
 * @return string
 *   HTML for this class in the class hierarchy.
 */
function _api_class_hierarchy_line($class, $current_did = 0) {
  $classes = array();
  if ($class->did == $current_did) {
    $classes[] = 'active';
  }

  // See if this class implements any interfaces or uses traits.
  $interfaces = array();
  $traits = array();

  $query = db_select('api_reference_storage', 'ars');
  $query->addField('ars', 'object_name', 'ref_name');
  $ad = $query->leftJoin('api_documentation', 'ad', 'ad.did = ars.extends_did');
  $query->addField($ad, 'object_name', 'object_name');
  $query
    ->fields($ad, array('did', 'branch_id', 'file_name', 'object_type'))
    ->condition('ars.from_did', $class->did)
    ->condition('ars.object_type', array('interface', 'trait'))
    ->orderBy('ad.namespaced_name');
  $query->addField('ars', 'object_type', 'ars_object_type');
  $result = $query->execute();
  foreach ($result as $object) {
    if (isset($object->did) && $object->did) {
      // Interface/trait exists as an object in the database, make a link.
      if ($object->ars_object_type == 'interface') {
        if (!isset($interfaces[$object->object_name])) {
          $interfaces[$object->object_name] = l($object->object_name, api_url($object));
        }
      }
      else {
        if (!isset($traits[$object->object_name])) {
          $traits[$object->object_name] = l($object->object_name, api_url($object));
        }
      }
    }
    else {
      // This class was declared to implement this interface, or use this
      // trait, but it didn't get defined (probably a built-in PHP object
      // or from another project). So just display the name.
      if ($object->ars_object_type == 'interface') {
        $interfaces[] = $object->ref_name;
      }
      else {
        $traits[] = $object->ref_name;
      }
    }
  }

  // See if this class extends another class that isn't defined in our DB.
  // For interfaces, this could be multiple interfaces.
  $extends = '';
  $query = db_select('api_reference_storage', 'ars')
    ->fields('ars', array('object_name'))
    ->condition('ars.from_did', $class->did)
    ->condition('ars.object_type', 'class')
    ->condition('ars.extends_did', 0);
  $parents = $query->execute()->fetchCol();
  if (count($parents) > 0) {
    $extends = ' extends';
    foreach ($parents as $parent) {
      $extends .= ' ' . $parent;
    }
  }

  $namespace = substr($class->namespaced_name, 0, -strlen($class->object_name));
  $output = $class->object_type . ' ' . $namespace . l($class->object_name, api_url($class), array('attributes' => array('class' => $classes))) . $extends;
  if (count($interfaces) > 0) {
    $output .= ' implements ' . implode(', ', $interfaces);
  }
  if (count($traits) > 0) {
    $output .= ' uses ' . implode(', ', $traits);
  }

  return $output;
}

/**
 * Renders comments for a documentation object.
 *
 * @param object $documentation_object
 *   Object to render comments for.
 *
 * @return string
 *   Rendered comments to display with the object.
 */
function _api_add_comments($documentation_object) {
  $output = '';

  if (module_exists('comment') && user_access('access comments') && variable_get('comment_api', COMMENT_NODE_OPEN) != COMMENT_NODE_HIDDEN) {
    // Let the Comment module output the comments and comment form.
    $additions = comment_node_page_additions(node_load($documentation_object->did));
    // If the comment form wasn't included, see if authenticated users can post
    // comments, and add a link saying to log in to post.
    if (!isset($additions['comment_form'])) {
      $roles = user_roles(TRUE, 'post_comments');
      if (array_key_exists(DRUPAL_AUTHENTICATED_RID, $roles)) {
        $options = array(
          'query' => drupal_get_destination(),
          'fragment' => 'comment-form',
        );
        if (variable_get('user_register', 1)) {
          // Users can register themselves.
          $additions['login_link'] = array(
            '#type' => 'markup',
            '#value' => t('<a href="@login">Log in</a> or <a href="@register">register</a> to post comments', array(
              '@login' => url('user/login', $options),
              '@register' => url('user/register', $options),
            )),
          );
        }
        else {
          // Only admins can add new users, no public registration.
          $additions['login_link'] = array(
            '#type' => 'markup',
            '#value' => t('<a href="@login">Log in</a> to post comments', array('@login' => url('user/login', $options))),
          );
        }
      }
    }
    $output .= drupal_render($additions);
  }

  return $output;
}

/**
 * Returns a link to the file a documentation object is in.
 *
 * @param object $object
 *   Documentation object.
 *
 * @return string
 *   Formatted link to the file the object is in.
 */
function api_file_link($object) {
  return api_add_breaks(dirname($object->file_name) . '/') . l(basename($object->file_name), api_url($object, TRUE));
}

/**
 * Creates a section documenting which class a member is from.
 *
 * @param object $item
 *   Documentation item for the member.
 * @param object $branch
 *   Branch the item is in.
 *
 * @return string|false
 *   HTML for the section, or FALSE if it is not a class member.
 */
function _api_class_section($item, $branch) {
  if (!isset($item->class_did) || !$item->class_did) {
    return FALSE;
  }

  $class = api_object_load(
    (int) $item->class_did,
    $branch,
    array('class', 'interface', 'trait'));

  if (is_null($class)) {
    return FALSE;
  }

  return theme('api_class_section', array('class' => $class, 'branch' => $branch));
}

/**
 * Counts or lists references to a function or file.
 *
 * @param string $name
 *   Name of the function/file to check (see $type), or the object itself.
 * @param object $branch
 *   Object representing the branch to check.
 * @param string $type
 *   Type of reference. One of:
 *   - 'calls': $name is the name of a function, find function calls.
 *   - 'implementations': $name is the name of a hook, find implementations.
 *   - 'invokes': $name is the name of a hook, find invocations.
 *   - 'theme_invokes': $name is the name of a theme function or file, find
 *     theme calls.
 *   - 'element_invokes': $name is the name of an element, find #type uses of
 *     this element.
 *   - 'theme_references': $name is the name of a theme function or file, find
 *     string references to the theme hook name.
 *   - 'references': $name is the name of a function, find string references.
 *   - 'overrides': $exclude_did is the ID of a method, find overriding methods.
 *   - 'constants': $name is the name of a constant, find uses.
 *   - 'uses': $name is the fully-namespaced name of a class, find files that
 *     have use declarations.
 *   - 'annotations': $exclude_did is the ID of an annotation class. Find
 *     classes using it as annotation.
 *   - 'yml_config': $name is a YML config file name, find string references
 *     to the file name without .yml.
 *   - 'yml_keys': $name is a YML config file name, and $exclude_did is its
 *     documentation ID. Find references to keys in this file.
 *   - 'services': $name is a class, find services that reference it.
 *   - 'use': $name is a service, find functions that reference its name.
 * @param bool $count
 *   If TRUE (default), return a count. If FALSE, return an array.
 * @param int|null $exclude_did
 *   (optional) Document ID to exclude from the query, or to use as the ID for
 *   overrides and element_invokes.
 * @param int $limit
 *   (optional) Limit the number of references returned to this amount.
 * @param bool $is_drupal
 *   (optional) If set to FALSE, and $type is a Drupal-specific type (related
 *   to themes, hooks, or YAML), just return an empty list or zero count.
 *
 * @return int|array
 *   The number of references or an array of references.
 */
function api_find_references($name, $branch, $type, $count = TRUE, $exclude_did = 0, $limit = 0, $is_drupal = TRUE) {

  // Early return: if this is a Drupal type and $is_drupal is FALSE.
  $drupal_types = array(
    'implementations',
    'theme_invokes',
    'element_invokes',
    'theme_references',
    'invokes',
    'yml_config',
    'yml_keys',
  );
  if (!$is_drupal && in_array($type, $drupal_types)) {
    return ($count) ? 0 : array();
  }

  if (!is_string($name)) {
    // It should be an object. Derive the name.
    $obj = $name;
    $name = ($obj->class_did > 0 && ($type == 'calls' || $type == 'constants')) ? $obj->namespaced_name : $obj->title;
    // In a few cases, we want to look for either the base name or the
    // namespaced name.
    $altname = $obj->namespaced_name;
    if ($type == 'uses' || $type == 'services') {
      $name = $altname;
    }
  }

  $references_type = 'potential callback';
  $group = FALSE;
  if ($type == 'yml_config') {
    // Verify this is a *.yml file. If not, just return.
    if (strpos($name, '.yml') != strlen($name) - 4) {
      return ($count) ? 0 : array();
    }
    // Strip off the .yml and look for string references.
    $name = substr($name, 0, strlen($name) - 4);
    $type = 'references';
    $references_type = 'potential file';
  }

  $matches = array();
  $branch_id = $branch->branch_id;
  $base_query = NULL;

  if ($type == 'calls' || $type == 'constants') {
    // Use reference storage to find functions that call this one, or
    // use this constant.
    $base_query = db_select('api_reference_storage', 'r');
    $d = $base_query->leftJoin('api_documentation', 'd', 'r.from_did = d.did');
    $base_query->condition('d.branch_id', $branch_id)
      ->condition('d.did', $exclude_did, '<>')
      ->condition('r.object_type', array(
        'function',
        'constant',
        'member-class',
        'computed-member',
      ))
      ->condition('r.object_name', $name);
  }
  elseif ($type == 'overrides') {
    // Use overrides storage to find overrides of the ID passed in as "exclude".
    $base_query = db_select('api_overrides', 'o');
    $d = $base_query->leftJoin('api_documentation', 'd', 'o.did = d.did');
    $base_query->condition('d.branch_id', $branch_id)
      ->condition('o.overrides_did', $exclude_did);
  }
  elseif ($type == 'references') {
    // Use reference storage to find functions that use this name as a string.
    $base_query = db_select('api_reference_storage', 'r');
    $d = $base_query->innerJoin('api_documentation', 'd', 'r.from_did = d.did');
    $base_query->condition($d . '.branch_id', $branch_id)
      ->condition($d . '.did', $exclude_did, '<>')
      ->condition('r.object_type', $references_type);
    if (isset($altname)) {
      $base_query->condition('r.object_name', array($name, $altname));
    }
    else {
      $base_query->condition('r.object_name', $name);
    }
  }
  elseif ($type == 'use') {
    // Use reference storage to find functions that reference a service. It
    // could have a . in it, so it is either stored as 'potential file' or
    // 'potential callback'.
    $base_query = db_select('api_reference_storage', 'r');
    $d = $base_query->innerJoin('api_documentation', 'd', 'r.from_did = d.did');
    $base_query->condition($d . '.branch_id', $branch_id)
      ->condition($d . '.did', $exclude_did, '<>')
      ->condition('r.object_type', array('potential file', 'potential callback'));
    $base_query->condition('r.object_name', $name);
  }
  if ($type == 'services') {
    // Use reference storage to find services using this class.
    $base_query = db_select('api_reference_storage', 'r');
    $d = $base_query->leftJoin('api_documentation', 'd', 'r.from_did = d.did');
    $base_query->condition('d.branch_id', $branch_id)
      ->condition('d.did', $exclude_did, '<>')
      ->condition('r.object_type', array('service_class'))
      ->condition('r.object_name', $name);
  }
  elseif ($type == 'annotations') {
    // In this case, $exclude_did is the documentation ID of a class, presumably
    // with an 'annotation_class' reference in the table. Find other classes
    // that annotated with this same class name, which will have 'annotation'
    // references.
    $base_query = db_select('api_reference_storage', 'r_annotation')
      ->condition('r_annotation.from_did', $exclude_did)
      ->condition('r_annotation.object_type', 'annotation_class');
    $base_query->innerJoin('api_reference_storage', 'r_uses', "r_annotation.object_name = r_uses.object_name AND r_uses.object_type = 'annotation'");
    $base_query->innerJoin('api_documentation', 'd', 'r_uses.from_did = d.did');
    $base_query
      ->condition('d.branch_id', $branch_id);
    $group = TRUE;
  }
  elseif ($type == 'yml_keys') {
    // In this case, $exclude_did is the documentation ID of a file. We
    // want to find the string keys that file has in reference storage, and
    // then match those with 'potential callback' or 'potential file'
    // references.
    $base_query = db_select('api_reference_storage', 'r');
    $base_query->innerJoin('api_documentation', 'd', 'r.from_did = d.did');
    $base_query
      ->condition('d.branch_id', $branch_id)
      ->condition('r.object_type', array('potential callback', 'potential file'));
    $base_query->innerJoin('api_reference_storage', 'rstrings', 'r.object_name = rstrings.object_name');
    $base_query
      ->condition('rstrings.from_did', $exclude_did)
      ->condition('rstrings.object_type', 'yaml string');
    // A given function/method might have multiple matching YML keys, so we
    // need to group this query.
    $group = TRUE;
  }
  elseif ($type == 'uses') {
    // Use namespace storage to find files with use declarations for this class.
    // Note that $name probably starts with a backslash, while use declarations
    // do not.
    $name = substr(api_full_classname($name), 1);
    $base_query = db_select('api_namespace', 'n');
    $d = $base_query->leftJoin('api_documentation', 'd', 'n.did = d.did');
    $base_query->condition('d.branch_id', $branch_id)
      ->condition('d.did', $exclude_did, '<>')
      ->condition('n.object_type', 'use_alias')
      ->condition('n.class_name', $name);
  }
  elseif ($type == 'implementations') {
    // Use pattern matching to find functions that look like implementations of
    // this one. i.e. something_hookname, where "something" doesn't start with
    // an underscore. Note that LIKE uses _ as "match any one character", so
    // _ has to be escaped in this query. Limit this to Drupal functions.
    if (strpos($name, 'hook_') === 0) {
      $hook_name = substr($name, 5);
      // If the hook has an UPPER_CASE_SECTION, allow that to match anything,
      // for cases like hook_update_N() and hook_form_FORM_ID_alter(). Note
      // that the upper-case section could be at the start, middle, or end.
      $hook_name = preg_replace('/^[A-Z][A-Z_]*_/', '%\_', $hook_name);
      $hook_name = preg_replace('/_[A-Z][A-Z_]*_/', '\_%\_', $hook_name);
      $hook_name = preg_replace('/_[A-Z][A-Z_]*$/', '\_%', $hook_name);
      $base_query = db_select('api_documentation', 'd')
        ->condition('d.object_name', '%\_' . $hook_name, 'LIKE')
        ->condition('d.object_name', '\_%', 'NOT LIKE')
        ->condition('d.object_name', 'hook\_%', 'NOT LIKE')
        ->condition('d.object_type', 'function')
        ->condition('d.branch_id', $branch_id)
        ->condition('d.did', $exclude_did, '<>')
        ->condition('d.is_drupal', 1);
    }
  }
  elseif ($type == 'theme_invokes' || $type == 'theme_references') {
    // Use reference storage to find functions that call this theme, or
    // string references to this theme. Limit this to Drupal functions on the
    // calls.
    $theme_name = '';
    if (strpos($name, 'theme_') === 0) {
      // It's presumably a theme function.
      $theme_name = substr($name, 6);
    }
    elseif (strpos($name, '.tpl.php') == strlen($name) - 8) {
      // It's presumably a theme template file.
      $name = basename($name);
      $theme_name = str_replace('-', '_', substr($name, 0, strlen($name) - 8));
    }
    elseif (preg_match('|^(.*)\.[^.]*\.twig$|', $name, $matches)) {
      // It's a *.*.twig file.
      $name = basename($name);
      $theme_name = str_replace('-', '_', basename($matches[1]));
    }

    if (strlen($theme_name) > 0) {
      // We could have calls to things like theme('name') or theme('name__sub')
      // (or the corresponding strings) recorded in the references. Match
      // either. Note _ escaping for LIKE queries.
      $base_query = db_select('api_reference_storage', 'r');
      $d = $base_query->innerJoin('api_documentation', 'd', 'r.from_did = d.did');
      $base_query->condition($d . '.branch_id', $branch_id)
        ->condition($d . '.did', $exclude_did, '<>')
        ->condition('d.is_drupal', 1)
        ->condition(db_or()->condition('r.object_name', $theme_name)->condition('r.object_name', $theme_name . '\_\_%', 'LIKE'));
      if ($type == 'theme_invokes') {
        $base_query->condition('r.object_type', 'potential theme');
      }
      else {
        $base_query->condition('r.object_type', 'potential callback');
      }
    }
  }
  elseif ($type == 'invokes') {
    // Use reference storage to find functions that invoke this hook.
    // The reference storage holds the string that was actually found inside
    // the invoking function. So $name could be one of:
    // - "hook_$string",
    // - "hook_field_$string"
    // - "field_default_$string"
    // - "hook_user_$string"
    // - "hook_entity_$string"
    // - "hook_entity_bundle_$string"
    // - "hook_node_$string"
    // - "hook_$string_alter"
    // For these, {reference_storage} has object_type of 'potential hook',
    // 'potential fieldhook', etc. So, we need to build a query that will find
    // these matches between $string (field object_name) and $name (passed into
    // this function). And only do this for Drupal functions.
    $or_clauses = db_or();
    $found = FALSE;

    if (strpos($name, 'hook_') === 0) {
      $hook_name = substr($name, 5);
      $or_clauses->condition(db_and()->condition('r.object_type', 'potential hook')->condition('r.object_name', $hook_name));
      $found = TRUE;
      if (strpos($hook_name, 'user_') === 0) {
        $sub_hook_name = substr($hook_name, 5);
        $or_clauses->condition(db_and()->condition('r.object_type', 'potential userhook')->condition('r.object_name', $sub_hook_name));
      }
      elseif (strpos($hook_name, 'entity_') === 0) {
        $sub_hook_name = substr($hook_name, 7);
        $or_clauses->condition(db_and()->condition('r.object_type', 'potential entityhook')->condition('r.object_name', $sub_hook_name));
        if (strpos($sub_hook_name, 'bundle_') === 0) {
          $sub_sub_hook_name = substr($sub_hook_name, 7);
          $or_clauses->condition(db_and()->condition('r.object_type', 'potential entityhook')->condition('r.object_name', $sub_sub_hook_name));
        }
      }
      elseif (strpos($hook_name, 'field_') === 0) {
        $sub_hook_name = substr($hook_name, 6);
        $or_clauses->condition(db_and()->condition('r.object_type', 'potential fieldhook')->condition('r.object_name', $sub_hook_name));
      }
      elseif (strpos($hook_name, 'node_') === 0) {
        $sub_hook_name = substr($hook_name, 5);
        $or_clauses->condition(db_and()->condition('r.object_type', 'potential entityhook')->condition('r.object_name', $sub_hook_name));
      }
      elseif (strrpos($hook_name, '_alter') === strlen($hook_name) - 6) {
        $sub_hook_name = substr($hook_name, 0, strlen($hook_name) - 6);
        $or_clauses->condition(db_and()->condition('r.object_type', 'potential alter')->condition('r.object_name', $sub_hook_name));
      }
    }
    elseif (strpos($name, 'field_default_') === 0) {
      $hook_name = substr($name, 14);
      $or_clauses->condition(db_and()->condition('r.object_type', 'potential fieldhook')->condition('r.object_name', $hook_name));
      $found = TRUE;
    }

    // If we found at least one match, run this query we've built.
    if ($found) {
      $base_query = db_select('api_reference_storage', 'r');
      $d = $base_query->innerJoin('api_documentation', 'd', 'r.from_did = d.did');
      $base_query->condition('d.branch_id', $branch_id)
        ->condition('d.did', $exclude_did, '<>')
        ->condition('d.is_drupal', 1)
        ->condition($or_clauses);
    }
  }
  elseif ($type == 'element_invokes') {
    // $exclude_did is the ID of a potential element class. This must have
    // an 'element' reference in reference storage. Then we're looking for
    // 'potential element' references to that element machine name.
    $base_query = db_select('api_reference_storage', 'r')
      ->condition('r.from_did', $exclude_did)
      ->condition('r.object_type', 'element');
    $base_query->innerJoin('api_reference_storage', 'r2', 'r.object_name = r2.object_name AND r.branch_id = r2.branch_id');
    $base_query->condition('r2.object_type', 'potential element');
    $d = $base_query->innerJoin('api_documentation', 'd', 'r2.from_did = d.did');
  }

  // See if we built a query to execute.
  if (is_null($base_query)) {
    return ($count) ? 0 : array();
  }

  // Execute the query.
  if ($count) {
    // We're looking for a count here.
    if ($group) {
      $base_query->addExpression('COUNT(DISTINCT d.did)', 'num');
    }
    else {
      $base_query->addExpression('COUNT(d.did)', 'num');
    }
    return $base_query->execute()->fetchField();
  }

  // If we get here, we want to return a list.
  if ($group) {
    $base_query->groupBy('d.did');
  }
  $base_query->fields('d',
    array(
      'branch_id',
      'object_name',
      'title',
      'summary',
      'file_name',
      'object_type',
      'class_did',
      'is_drupal',
    ))
    ->orderBy('d.title', 'ASC');
  if ($limit > 0) {
    $base_query->range(0, $limit);
  }

  $result = $base_query->execute();
  $list = array();
  foreach ($result as $object) {
    $list[] = array(
      'function' => l($object->title, api_url($object)),
      'file' => api_file_link($object),
      'description' => api_link_documentation($object->summary, $branch, NULL, $object->class_did, FALSE, FALSE, $object->is_drupal),
    );
  }

  return $list;
}

/**
 * Generates a list of references as HTML.
 *
 * @param object $object
 *   Object to list references for.
 * @param object $branch
 *   Branch the object is in.
 * @param string $type
 *   Type of references: see api_find_references() for list.
 * @param int $limit
 *   (optional) Limit list to a few references.
 * @param bool $is_drupal
 *   (optional) If set to FALSE, and $type is a Drupal-specific type (such as
 *   theme- or hook- related), just return an empty list or zero count.
 *
 * @return string
 *   HTML for a list of references.
 */
function api_list_references($object, $branch, $type, $limit = 0, $is_drupal = FALSE) {
  $call_functions = api_find_references($object, $branch, $type, FALSE, $object->did, $limit, $is_drupal);

  $note = '';
  if ($type == 'implementations') {
    $note = '<p>' . t('Note: this list is generated by pattern matching, so it may include some functions that are not actually implementations of this hook.') . '</p>';
  }
  elseif ($type == 'theme_references') {
    $note = '<p>' . t('Note: this list is generated by looking for the string for this theme hook, so it may include some references that are not actually using this theme hook.') . '</p>';
  }

  return $note . theme('api_functions', array('functions' => $call_functions));
}

/**
 * Builds and returns a references section for a documentation object.
 *
 * @param string[] $types
 *   Types of references to make (see api_find_references() for the list).
 * @param object $object
 *   Documentation object to find references for.
 * @param object $branch
 *   Branch to find references in.
 *
 * @return string[]
 *   Array of HTML strings for the references section. Each one is a
 *   collapsible list of a few references, with a link to the full references
 *   page. Empty sections are omitted.
 */
function api_build_references_section(array $types, $object, $branch) {
  $output = array();
  foreach ($types as $type) {
    $count = api_find_references($object, $branch, $type, TRUE, $object->did, 0, $object->is_drupal);
    if ($count > 0) {
      $limit = variable_get('api_reference_limit', 5);
      $section = '';
      $section .= api_list_references($object, $branch, $type, $limit, $object->is_drupal);
      if ($count > $limit) {
        $section .= '<p>' . theme(
          'api_function_reference_link',
          array(
            'type' => $type,
            'count' => $count,
            'function' => $object,
            'override_text' => t('... See full list'),
          )) . '</p>';
      }
      $title = api_reference_text($type, $count, $object);
      $output[] = '<details class="api-references"><summary>' . $title . '</summary>' . $section . '</details>';
    }
  }

  return $output;
}

/**
 * Returns the page title or link text for a references page/link.
 *
 * @param string $type
 *   The type of reference link. This is either one of the values listed in
 *   api_find_references(), or:
 *   - 'hierarchy': Class hierarchy ($count is ignored).
 *   - 'implements': Interface implements list ($count is ignored).
 * @param int $count
 *   The number of referenced items.
 * @param object $function
 *   The function, class, or file object being referenced.
 *
 * @return string
 *   Text to be used as the link to the reference listing page, or the title
 *   of the page.
 */
function api_reference_text($type, $count, $function) {
  $name_to_use = check_plain($function->title);
  if ($type == 'references') {
    $name_to_use = "'" . $name_to_use . "'";
  }
  elseif ($type == 'yml_config') {
    $name_to_use = "'" . substr($name_to_use, 0, strlen($name_to_use) - 4) . "'";
    $type = 'references';
  }
  elseif ($function->object_type == 'function') {
    $name_to_use .= '()';
  }

  if ($type == 'calls') {
    return format_plural(
      $count,
      '1 call to !name',
      '@count calls to !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'constants') {
    return format_plural(
      $count,
      '1 use of !name',
      '@count uses of !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'implementations') {
    return format_plural(
      $count,
      '1 function implements !name',
      '@count functions implement !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'uses') {
    return format_plural(
      $count,
      '1 file declares its use of !name',
      '@count files declare their use of !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'overrides') {
    return format_plural(
      $count,
      '1 method overrides !name',
      '@count methods override !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'element_invokes') {
    return format_plural(
      $count,
      '1 #type use of !name',
      '@count #type uses of !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'theme_invokes') {
    return format_plural(
      $count,
      '1 theme call to !name',
      '@count theme calls to !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'theme_references') {
    return format_plural(
      $count,
      '1 string reference to the theme hook from !name',
      '@count string references to the theme hook from !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'invokes') {
    return format_plural(
      $count,
      '1 invocation of !name',
      '@count invocations of !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'references' || $type == 'use') {
    return format_plural(
      $count,
      '1 string reference to !name',
      '@count string references to !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'annotations') {
    return format_plural(
      $count,
      '1 class is annotated with !name',
      '@count classes are annotated with !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'services') {
    return format_plural(
      $count,
      '1 service uses !name',
      '@count services use !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'yml_keys') {
    return format_plural(
      $count,
      '1 string reference to YAML keys in !name',
      '@count string references to YAML keys in !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'hierarchy') {
    return t('Expanded class hierarchy of !name',
      array('!name' => $name_to_use));
  }
  if ($type == 'implements') {
    return t('All classes that implement !name',
      array('!name' => $name_to_use));
  }

  return '';
}
