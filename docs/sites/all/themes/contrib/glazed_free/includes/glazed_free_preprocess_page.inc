<?php

/**
 * Implements template_preprocess_page().
 */
function glazed_free_preprocess_page(&$vars) {
  global $theme;

  // Render field collection object into simple array structure
  // We abort on finding arg(2) to avoid using fancy headers on /edit and /devel etc
  if ((arg(0) == 'node') && is_numeric(arg(1)) && !arg(2)
    && !empty($vars['node']->field_glazed_content_design)
    && isset($vars['node']->field_glazed_content_design['und'][0]['value'])
    && module_exists('field_collection')) {
    $field_array = array(
      'field_page_layout',
      'field_header_layout',
      'field_menu_hover_effect',
      'field_full_width_header',
      'field_glazed_hide_regions',
      'field_glazed_content_region',
      'field_glazed_header_mode',
      'field_glazed_header_position',
      'field_glazed_header_style'
    );
    $entity_id = $vars['node']->field_glazed_content_design['und'][0]['value'];
    $content_design = glazed_free_rows_from_field_collection($entity_id, $field_array);
    if ($content_design) {
      if (in_array('page_title', $content_design['field_glazed_hide_regions'])) {
        $vars['hide_page_title'] = TRUE;
      }
      if (in_array('header', $content_design['field_glazed_hide_regions'])) {
        $vars['hide_header'] = TRUE;
      }
      if (in_array('navigation', $content_design['field_glazed_hide_regions'])) {
        $vars['hide_navigation'] = TRUE;
      }
      foreach ($content_design['field_glazed_hide_regions'] as $region) {
        unset($vars['page'][$region]);
      }
    }
  }

  // Boxed / Full-width Layout
  $vars['boxed_cnt'] = $vars['boxed_cnt_end'] = '';
  if (isset($content_design['field_page_layout'])) {
    if ($content_design['field_page_layout'] == 'boxed') {
      $vars['boxed_cnt'] = '<div class="glazed-boxed-container">';
      $vars['boxed_cnt_end'] = '</div>';
    }
  }
  elseif (theme_get_setting('boxed_layout')) {
    $vars['boxed_cnt'] = '<div class="glazed-boxed-container">';
    $vars['boxed_cnt_end'] = '</div>';
  }
  // Full width regions
  (theme_get_setting('full_width_containers')) ? $fwc = theme_get_setting('full_width_containers') : $fwc = array(
    'cnt_footer' => 0,
    'cnt_secondary_header' => 0,
    'cnt_header_nav' => 0,
    'cnt_page_title' => 0,
    'cnt_content_top' => 0,
    'cnt_content_bottom' => 0,
  );
  if (isset($content_design) && ($content_design['field_full_width_header'] == 'fullwidth')) {
    $fwc['cnt_header_nav'] = 1;
  }
  if (isset($content_design) && ($content_design['field_full_width_header'] == 'normal')) {
    $fwc['cnt_header_nav'] = 0;
  }
  foreach ($fwc as $key => $checked) {
    $vars["{$key}_squeeze_start"] = '<div class="row container-row"><div class="col-sm-12 container-col">';
    $vars["{$key}_squeeze_end"] = '</div></div>';
    if ($checked) {
      $vars[$key] = 'glazed-fluid';
      $vars["{$key}_squeeze_start"] = '';
      $vars["{$key}_squeeze_end"] = '';
    }
    else {
      $vars[$key] = 'container';
    }
  }
  // Full width content types
  if (!($types = theme_get_setting('full_width_content_types'))) {
    $sitetypes = array_keys(node_type_get_types());
    $types = array();
    foreach ($sitetypes as $value) {
      if (strpos($value, 'drag_drop') !== FALSE) {
        $types[$value] = $value;
      }
    }
  }

  $vars['content_row_class'] = ' class="row"';
  $vars['content_container_class'] = ' class="container main-container"';
  if ((isset($vars['node'])
      && (((array_key_exists($vars['node']->type, $types))
      && ($types[$vars['node']->type]))
      OR (isset($content_design) && ($content_design['field_glazed_content_region']))))
    && (arg(2) != 'edit')
    && empty($vars['page']['sidebar_first'])
    && empty($vars['page']['sidebar_second'])) {
    $vars['content_column_class'] = '';
    $vars['content_row_class'] = '';
    $vars['content_container_class'] = ' class="main-container"';
    $vars['cnt_page_title_full_width'] = ' glazed-no-margin';
  }
  else {
    $vars['cnt_page_title_full_width'] = '';
  }

  // Page Title Hiding
  if ((theme_get_setting('page_title_home_hide') !== 0) && $vars['is_front']) {
    $vars['title'] = '';
  }

  if ($vars['is_front'] && theme_get_setting('stlink')) {
    $vars['sooperthemes_attribution_link'] = glazed_free_attribution_link();
  } else {
    $vars['sooperthemes_attribution_link'] = '';
  }

  // Glazed Header
  $vars['glazed_free_header_classes'] = array('navbar', 'glazed-header');
  if (isset($content_design['field_glazed_header_mode'])) {
    $header_mode = $content_design['field_glazed_header_mode'];
  }
  else {
    if (theme_get_setting('header_mode')) {
      $header_mode = 'side';
    }
    else {
      $header_mode = 'top';
    }
  }
  if ($header_mode == 'side') {
    // Side Header
    $vars['glazed_free_header_classes'][] = 'glazed-header--side';
    $vars['glazed_free_header_classes'][] = theme_get_setting('header_side_align');
  }
  else {
    // Top Header
    $vars['glazed_free_header_classes'][] = 'glazed-header--top';

    // Header Layout Logo left/center/right etc
    if (isset($content_design['field_header_layout'])) {
      $header_layout = $content_design['field_header_layout'];
      $vars['glazed_free_header_classes'][] = 'glazed-header--' . drupal_html_class($header_layout);
    }
    elseif ($header_layout = theme_get_setting('header_top_layout')) {
      $vars['glazed_free_header_classes'][] = 'glazed-header--' . drupal_html_class($header_layout);
    }

    // Header Style Normal/Overlay/Pull-Down
    if (isset($content_design['field_glazed_header_style'])) {
      $header_style = $content_design['field_glazed_header_style'];
      $vars['glazed_free_header_classes'][] = 'glazed-header--' . drupal_html_class($header_style);
    }
    elseif ($header_style = theme_get_setting('header_style')) {
      $vars['glazed_free_header_classes'][] = 'glazed-header--' . drupal_html_class($header_style);
    }

    // Layout Position Static/Fixed/Sticky
    // Header Style Normal/Overlay/Pull-Down
    if (isset($content_design['field_glazed_header_position'])) {
      $vars['glazed_free_header_classes'][] = 'glazed-header--' . $content_design['field_glazed_header_position'];
      if ($content_design['field_glazed_header_position'] == 'sticky') {
        $scroll = theme_get_setting('header_top_height_sticky_offset');
        $vars['header_affix'] = ' data-spy="affix" data-offset-top="' . $scroll . '"';
      }
    }
    else {
      if (theme_get_setting('header_top_fixed')) {
        if (theme_get_setting('header_top_sticky')) {
          $vars['glazed_free_header_classes'][] = 'glazed-header--sticky';
          $scroll = theme_get_setting('header_top_height_sticky_offset');
          $vars['header_affix'] = ' data-spy="affix" data-offset-top="' . $scroll . '"';
        }
        else {
          $vars['glazed_free_header_classes'][] = 'glazed-header--fixed';
        }
      }

    }
  }
  // Menu Hover Style
  if (isset($content_design['field_menu_hover_effect'])) {
    $header_menu_hover_effect = $content_design['field_menu_hover_effect'];
    $vars['glazed_free_header_classes'][] = 'glazed-header--hover-' . drupal_html_class($header_menu_hover_effect);
  }
  elseif ($header_menu_hover_effect = theme_get_setting('menu_hover')) {
    $vars['glazed_free_header_classes'][] = 'glazed-header--hover-' . drupal_html_class($header_menu_hover_effect);
  }

  $vars['glazed_free_header_classes'] = implode(' ', $vars['glazed_free_header_classes']);

  // Glazed Secondary Header
  $vars['glazed_free_secondary_header_classes'] = array('glazed-secondary-header');
  switch (theme_get_setting('secondary_header_hide')) {
    case 'hidden_xs':
      $vars['glazed_free_secondary_header_classes'][] = 'hidden-xs';
      break;
    case 'hidden_sm':
     $vars['glazed_free_secondary_header_classes'] += array('hidden-xs', 'hidden-sm');
      break;
    case 'hidden_md':
      $vars['glazed_free_secondary_header_classes'] += array('hidden-xs', 'hidden-sm', 'hidden-md');
      break;
  }
  if (theme_get_setting('secondary_header_hide')) {
    $vars['glazed_free_secondary_header_classes'][] = drupal_html_class(theme_get_setting('secondary_header_hide'));
  }
  else {
    $vars['glazed_free_secondary_header_classes'][] = 'hidden-xs';
  }
  $vars['glazed_free_secondary_header_classes'] = implode(' ', $vars['glazed_free_secondary_header_classes']);

  // Page Title
  $vars['glazed_free_title_classes'] = array('page-title');
  if (theme_get_setting('page_title_animate')) {
    $vars['glazed_free_title_classes'][] = 'animated';
    $vars['glazed_free_title_classes'][] = theme_get_setting('page_title_animate');
  }
  $vars['glazed_free_title_classes'] = implode(' ', $vars['glazed_free_title_classes']);
}
