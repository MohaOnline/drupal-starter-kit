<?php

/**
 * Implements template_preprocess_html().
 */
function glazed_free_preprocess_html(&$vars) {
  global $theme;

  // Add body class for core toolbar to help with header spacing
  if (module_exists('admin_menu_toolbar') && user_access('access administration menu')) {
    $vars['classes_array'][] = 'admin-menu-toolbar';
  }
  // Load Glazed Content Design Overrides
  // We abort on finding arg(2) to avoid using fancy headers on /edit and /devel etc
  if ((arg(0) == 'node') && is_numeric(arg(1)) && !arg(2)) {
    $node = node_load(arg(1));
    if (isset($node->field_glazed_content_design)
      && !empty($node->field_glazed_content_design)
      && (!empty($node->field_glazed_content_design[0]) OR !empty($node->field_glazed_content_design['und']))
      && module_exists('field_collection')) {
      $field_array = array(
        'field_glazed_header_mode',
        'field_glazed_header_position',
        'field_glazed_header_style'
      );
      // @see render_glazed_free_content_css.inc node_load
      if (isset($node->field_glazed_content_design['und']) && isset($node->field_glazed_content_design['und'][0])) {
        $entity_id = $node->field_glazed_content_design['und'][0]['value'];
      }
      elseif (isset($node->field_glazed_content_design[0])) {
        $entity_id = $node->field_glazed_content_design[0]['value'];
      }
      $content_design = glazed_free_rows_from_field_collection($entity_id, $field_array);
    }
  }

  if (theme_get_setting('sticky_footer') && (theme_get_setting('boxed_layout') == FALSE)) {
    $vars['html_attributes_array']['class'] = 'html--glazed-sticky-footer';
  }

  if (isset($content_design['field_glazed_header_mode'])) {
    $header_position = $content_design['field_glazed_header_mode'];
  }
  else {
    if (theme_get_setting('header_position')) {
      $header_position = 'side';
    }
    else {
      $header_position = 'top';
    }
  }

  if ($header_position == 'side') {
    // Side Header
    $vars['classes_array'][] = 'body--glazed-header-side';
  }
  else {
    // Top Header
    $vars['classes_array'][] = 'body--glazed-header-top';
    if (isset($content_design['field_glazed_header_style'])) {
      if ($content_design['field_glazed_header_style'] != 'overlay') {
        $vars['classes_array'][] = 'body--glazed-header-not-overlay';
        $vars['classes_array'][] = 'body--glazed-header-' . $content_design['field_glazed_header_style'];
      }
      else {
        $vars['classes_array'][] = 'body--glazed-header-' . $content_design['field_glazed_header_style'];
      }
    }
    else {
      if (theme_get_setting('header_style') != 'overlay') {
        $vars['classes_array'][] = 'body--glazed-header-not-overlay';
        $vars['classes_array'][] = 'body--glazed-header-' . theme_get_setting('header_style');
      }
      else {
        $vars['classes_array'][] = 'body--glazed-header-' . theme_get_setting('header_style');
      }
    }
    if (isset($content_design['field_glazed_header_position'])) {

    }
    else {
      if (theme_get_setting('header_top_fixed') && (!theme_get_setting('header_top_sticky'))) {
        $vars['classes_array'][] = 'body--glazed-header-fixed';
      }
    }
  }
}
