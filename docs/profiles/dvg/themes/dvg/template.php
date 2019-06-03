<?php

/**
 * Implements theme_css_alter().
 *
 * - Remove **some** Drupal CSS files so we can implement that ourselves (better!).
 */
function dvg_css_alter(&$css) {
  $remove = array(
    'user',
    'image',
    'system.theme',
    'system.messages',
    'system.menus',
    'node',
    'views',
    'field',
    'ctools',
    'taxonomy',
  );
  dvg_remove_css($css, $remove);
}

/**
 * Helper to remove CSS files from a CSS file list.
 *
 * You can use this function in your subtheme's hook_css_alter().
 *
 * NOTE the `$remove` argument is an array of stylesheet basenames **without** `.css`.
 */
function dvg_remove_css(&$css, $remove) {
  foreach ($css as $file => $meta) {
    if (in_array(substr(basename($file), 0, -4), $remove)) {
      unset($css[$file]);
    }
  }
}

/**
 * Implements theme_status_messages().
 *
 * Make sure `.messages > *` is **always** an UL, even if there's only 1 message.
 *
 * @see http://api.drupal.org/api/drupal/includes!theme.inc/function/theme_status_messages/7
 */
function dvg_status_messages($display = NULL) {
  $all_messages = drupal_get_messages();

  $output = '';
  foreach ($all_messages as $type => $messages) {
    if ($messages) {
      $output .= '<div class="messages ' . $type . '">' . "\n";
      $output .= '<ul class="menu">' . "\n";
      foreach ($messages as $message) {
        $output .= '<li>' . $message . "</li>\n";
      }
      $output .= "</ul>\n";
      $output .= "</div>\n";
    }
  }

  return $output;
}

/**
 * Implements template_preprocess_html().
 *
 * - Remove Drupal's "no-sidebars" class.
 * - Add body classes for active contexts.
 */
function dvg_preprocess_html(&$variables) {
  // Remove Drupal's "no-sidebars" class.
  if ($key = array_search('no-sidebars', $variables['classes_array'])) {
    unset($variables['classes_array'][$key]);
  }

  // Simpler frontpage title
  if (theme_get_setting('simpler_frontpage_title')) {
    if (drupal_is_front_page()) {
      $site_name = variable_get('site_name', 'Drupal');
      $variables['head_title_array'] = array($site_name);
    }

    $head_title_separator = theme_get_setting('head_title_separator') ? : ' | ';
    $variables['head_title'] = implode($head_title_separator, $variables['head_title_array']);
  }

  // Add body classes for active contexts.
  if (module_exists('context')) {
    $contexts = context_active_contexts();
    foreach ($contexts as $c_id => $c) {
      $variables['classes_array'][] = 'context-' . $c_id;
    }
  }

  // `with` / `without` classes for certain regions
  foreach (array('sidebar_left', 'sidebar_right') as $region_name) {
    $without = empty($variables['page'][$region_name]);
    $variables['classes_array'][] = ($without ? 'without' : 'with') . '-' . str_replace('_', '-', $region_name);
  }

  // Add information about the number of sidebars.
  if (!empty($variables['page']['sidebar_left']) && !empty($variables['page']['sidebar_right'])) {
    $variables['classes_array'][] = 'all-sidebars';
  }
  elseif (empty($variables['page']['sidebar_left']) && empty($variables['page']['sidebar_right'])) {
    $variables['classes_array'][] = 'no-sidebars';
  }

  $theme_path = drupal_get_path('theme', 'dvg');

  // Add conditional stylesheets for IE < 9
  drupal_add_css($theme_path . '/css/dvg-no-queries.css', array(
      'group' => CSS_THEME,
      'browsers' => array('IE' => 'lte IE 9', '!IE' => FALSE),
      'preprocess' => FALSE
    )
  );

  // Add modernizr library.
  $modernizr_path = libraries_get_path('modernizr');
  if ($modernizr_path) {
    drupal_add_js($modernizr_path . '/modernizr.js');
  }
}

/**
 * Implements template_preprocess_page().
 *
 * - Simple "is_node" variable for further preprocessing.
 */
function dvg_preprocess_page(&$variables) {
  $variables['is_node'] = 'node' == arg(0) && is_numeric(arg(1));

  $search_page_nid = functional_content_nid(_functional_content_item_name('search', 'block'));
  if (arg(0) == 'node' && arg(1) == $search_page_nid && !arg(2) && !empty($_GET['search'])) {
    $variables['title'] = t('Search results for <span>@term</span>', array('@term' => $_GET['search']));
  }

  // SVG Logo.
  $variables['svg_logo'] = base_path() . path_to_theme() . '/logo.svg';
}

/**
 * Implements template_preprocess_maintenance_page().
 */
function dvg_preprocess_maintenance_page(&$vars) {
  $vars['svg_logo'] = base_path() . path_to_theme() . '/logo.svg';
}

/**
 * Implements theme_preprocess_tabs().
 */
function dvg_process_tabs(&$vars) {
  $types = array('#primary', '#secondary');
  foreach ($types as $type) {
    if (isset($vars['tabs'][$type]) && is_array($vars['tabs'][$type])) {
      foreach ($vars['tabs'][$type] as $key => $tab) {
        $vars['tabs'][$type][$key]['#link']['localized_options']['attributes']['class'][] = drupal_clean_css_identifier($vars['tabs'][$type][$key]['#link']['path']);
      }
    }
  }
}

/**
 * Implements template_preprocess_node().
 *
 * Suggest a template based the view mode, for example: node--teaser.tpl.php
 */
function dvg_preprocess_node(&$vars) {
  $node = $vars['node'];

  // Add template suggestions.
  $vars['theme_hook_suggestions'][] = 'node__' . $node->type . '__' . $vars['view_mode'];

  // Adds same CSS class.
  $vars['classes_array'][] = drupal_clean_css_identifier('node-' . $node->type . '-' . $vars['view_mode']);

  // We do our own theming in the node template.
  unset($vars['content']['field_highlight_text']['#theme']);

  // Nodes on non-pages have a linkable H2 title
  if (!isset($vars['page_link'])) {
    $vars['page_link'] = TRUE;
  }

  if ($vars['view_mode'] == 'search_results') {
    global $_domain;
    $searchresult = '';

    if (isset($vars['domains']) && is_array($vars['domains']) && module_exists('dvg_domains')) {
      foreach ($vars['domains'] as $redirect_domain) {
        $redirect_domain = domain_load($redirect_domain);
        dvg_domains_domain_load($redirect_domain);
      }
      if ($node->type != 'external_link' && $_domain['machine_name'] != $redirect_domain['machine_name']) {
        // If the node isn't an external link add the domain path.
        $vars['node_url'] = $redirect_domain['path'] . $vars['node_url'];
        $searchresult = '<div class="other-domain">(' . t('opens website') . ' ' . $redirect_domain['sitename'] . ')</div>';
      }
    }

    if ($node->type == 'external_link') {
      $searchresult = '<div class="other-domain">(' . t('opens external website') . ')</div>';
    }

    if (isset($vars['content']['field_search_result'])) {
      $vars['content']['field_search_result'][0]['#markup'] = $searchresult . $vars['content']['field_search_result'][0]['#markup'];
    }
  }
}

/**
 * Implements theme_breadcrumb().
 */
function dvg_breadcrumb($variables) {
  $breadcrumb = $variables['breadcrumb'];

  if (!empty($breadcrumb)) {
    // Add a span around the breadcrumb symbol
    $output = '<div class="breadcrumb">' . implode(' <span class="breadcrumb-symbol">&gt;</span> ', $breadcrumb) . '</div>';
    return $output;
  }
}

/**
 * Implements theme_form_required_marker().
 */
function dvg_form_required_marker($variables) {
  return '';
}

/**
 * Implementation of hook_entity_view_alter().
 */
function dvg_entity_view_alter(&$build, $type) {
  $field = array(
    'field_body' => array(
      'type' => 'field_collection_item',
      'bundle' => 'field_sections'
    ),
  );

  // Re-structure the headings comform W3C standaards
  foreach ($field as $field_name => $info) {
    if ($info['type'] == $type && $info['bundle'] == $build['#bundle']) {
      $i = 0;
      while (isset($build[$field_name][$i])) {
        if (strpos($build[$field_name][$i]['#markup'], '<h2>') !== FALSE) {
          $build[$field_name][$i]['#markup'] = str_replace(
            array('<h3>', '</h3>', '<h2>', '</h2>'),
            array('<h4>', '</h4>', '<h3>', '</h3>'),
            $build[$field_name][$i]['#markup']
          );
        }
        $i++;
      }
    }
  }
}

/**
 * Implements theme_webform_progressbar().
 */
function dvg_webform_progressbar($vars) {
  if (isset($vars['progressbar_bar']) && !$vars['progressbar_bar']) {
    return '';
  }

  $vars['classes_array'][] = 'progressbar';
  $items = array();
  foreach ($vars['page_labels'] as $index => $label) {
    $classes = array();
    $replacements = array('@step' => ($index + 1), '@total' => $vars['page_count']);

    if ($index < ($vars['page_num'] - 1)) {
      $accessibility = t('Step @step of @total. Completed step', $replacements);
      $classes[] = 'completed';
    }
    elseif ($index == ($vars['page_num'] - 1)) {
      $accessibility = t('Step @step of @total. Active step', $replacements);
      $classes[] = 'current';
    }
    else {
      $accessibility = t('Step @step of @total', $replacements);
    }

    $items[] = array(
      'class' => $classes,
      'data' => '<span class="label">' . t('<span class="element-invisible">!accessibility: </span>@label', array(
          '@label' => $label,
          '!accessibility' => $accessibility,
        )) . '</span>',
    );
  }

  return theme('item_list', array(
    'items' => $items,
    'type' => 'ol',
    'attributes' => array(
      'class' => $vars['classes_array'],
    ),
  ));
}
