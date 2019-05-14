<?php

/**
 * Implements template_preprocess_node()
 */
function glazed_free_preprocess_node(&$vars) {
  // ALL NODES
  // remove "add comment link. it's silly because the comment form will be right there in our theme"
  if (isset($vars['content']['links']['comment'])) {
    unset($vars['content']['links']['comment']);
  }
  // Hide byline on all teasers
  if ($vars['teaser']) {
    $vars['display_submitted'] = FALSE;
  }
  // Next / Prev pager
  $vars['prevnext'] = '';
  if ($vars['view_mode'] == 'full') {
    $types = theme_get_setting('prevnext_content_types');
    $type = $vars['node']->type;
    if (isset($types[$type]) && !empty($types[$type])) {
      $prevnext = glazed_free_next_prev($vars['node']);
      $vars['prevnext'] = '<div class="prev-next small clear-both clearfix">';
      if (!empty($prevnext['prev'])) {
        $url = url('node/' . $prevnext['prev']['nid']);
        $title = check_plain($prevnext['prev']['title']);
        $vars['prevnext'] .= '<div class="prev pull-left"><a class="nextprev-link" href="' . $url . '" title="' . $title . '"><i class="fa fa-angle-left"></i><span class="nextprev-text">' . t('Previous') . '</span></a></div>';
      }
      if (!empty($prevnext['next'])) {
        $url = url('node/' . $prevnext['next']['nid']);
        $title = check_plain($prevnext['next']['title']);
        $vars['prevnext'] .= '<div class="next pull-right"><a class="nextprev-link" href="' . $url . '" title="' . $title . '"><span class="nextprev-text">' . t('Next') . '</span><i class="fa fa-angle-right"></i></a></div>';
      }
      $vars['prevnext'] .= '</div>';
    }
  }

  // BLOG NODES
  if ($vars['type'] == 'blog') {
    $_created_date = format_date($vars['node']->created, 'custom', 'M j, Y');
    $vars['submitted'] = '<em class="text-muted">' . $_created_date . ' ' . t('By') . ' </em> <span class="glazed-util-text-uppercase">' . $vars['node']->name . '</span>';
  }
  // NEWS NODES
  if ($vars['type'] == 'news') {
    $vars['submitted'] = t('Posted on !datetime', array( '!datetime' => format_date($vars['node']->created, 'custom', 'l, F j, Y')));
  }
  // EVENT NODES
  if ($vars['type'] == 'event') {
    if (isset($vars['field_event_registration']) && ($vars['field_event_registration']['und'][0]['registration_type']) && function_exists('registration_event_count')) {
      // @see registration.module ~ #646
      $entity_id = $vars['nid'];
      $entity_type = 'node';
      $settings = registration_entity_settings($entity_type, $entity_id);
      $count = registration_event_count($entity_type, $entity_id);
      $capacity = $settings['capacity'];
      if ($capacity == 0) {
        $capacity = t('No limit set');
        $available = t('No limit set');
      }
      else {
        $available = $settings['capacity'] - $count;
      }
        $vars['registration_event_count'] = '<h3 class="field-label">' . t('Spaces') . '</h3><div class="field-event-spaces">' . $capacity . '</div>';
        $vars['registration_event_capacity'] = '<h3 class="field-label">' . t('Available spaces') . '</h3><div class="field-event-spaces-available">' . $available . '</div>';
    }
  }
  // PORTFOLIO NODES
  if (($vars['type'] == 'portfolio') && ($vars['view_mode'] == 'full')) {
    // Hide portfolio images if hide mode is selected. this is useful if you
    // want to show the images in a custom case study layout
    $hide = FALSE;
    if (!empty($vars['field_image_display_style']) && $vars['field_image_display_style']['und'][0]['value'] == 'hide') {
      $hide = TRUE;
    }
    elseif (theme_get_setting('portfolio_images') == 'hide') {
      $hide = TRUE;
    }
    if ($hide && isset($vars['content']['group_portfolio_images']['field_portfolio_images'])) {
      hide($vars['content']['group_portfolio_images']['field_portfolio_images']);
    }
    // Load Lightbox assets if gallery grid is selected
    $lightbox = FALSE;
    if (!empty($vars['field_image_display_style']) && $vars['field_image_display_style']['und'][0]['value'] == 'grid') {
      $lightbox = TRUE;
    }
    elseif (theme_get_setting('portfolio_images') == 'grid') {
      $lightbox = TRUE;
    }
    if ($lightbox) {
      $path = drupal_get_path('theme', 'glazed_free');
      drupal_add_js($path . '/js/vendor/ilightbox/js/jquery.requestAnimationFrame.js');
      drupal_add_js($path . '/js/vendor/ilightbox/js/jquery.mousewheel.js');
      drupal_add_js($path . '/js/vendor/ilightbox/js/ilightbox.min.js');
      drupal_add_js($path . '/js/glazed-ilightbox.js');
      drupal_add_css($path . '/js/vendor/ilightbox/css/ilightbox.css');
    }
  }

  // Render node CSS

  // Get node object
  // Somehow node_load causes $node->field_glazed_content_design['und][0]['value']
  // to move to $node->field_glazed_content_design[0]['value'] in glazed_free_preprocess_html
  // @see glazed_free_preprocess_html.inc
  // We abort on finding arg(2) to avoid using fancy headers on /edit and /devel etc
  if ((arg(0) == 'node') && is_numeric(arg(1)) && !arg(2)) {
    // $node = node_load(arg(1));
    $node = $vars['node'];
    if (!empty($node->field_glazed_content_design)
      && isset($node->field_glazed_content_design['und'][0]['value'])
      && module_exists('field_collection')) {
      $field_array = array(
        'field_glazed_body_image',
        'field_glazed_header_image',
        'field_header_height',
        'field_glazed_header_style',
        'field_header_background',
        'field_header_text',
        'field_navbar_background',
        'field_header_block_background',
        'field_header_block_text_color',
        'field_menu_background',
        'field_menu_text_color',
        'field_menu_hover_background',
        'field_menu_hover_text_color',
        'field_dropdown_background',
        'field_dropdown_text_color',
        'field_dropdown_hover_background',
        'field_dropdown_hover_text_color',
        'field_side_header_background',
        'field_side_header_text_color',
        'field_side_header_width',
        'field_side_header_layout',
      );
      $entity_id = $node->field_glazed_content_design['und'][0]['value'];
      $content_design = glazed_free_rows_from_field_collection($entity_id, $field_array);

      // COLLECT CSS
      $CSS = '';
      // BODY BACKGROUND AND PAGE TITLE BACKGROUND
      if (isset($content_design['field_glazed_body_image'])) {
        $url = file_create_url($content_design['field_glazed_body_image']['uri']);
        $CSS .= "body { background-image: url('" . $url . "') !important; }\n\n";
      }
      if (isset($content_design['field_glazed_header_image'])) {
        $url = file_create_url($content_design['field_glazed_header_image']['uri']);
        $CSS .= ".page-title-full-width-container:after { background-image: url('" . $url . "') !important; }\n\n";
      }
      // STYLES THAT USE HEADER HEIGHT
      if ($header_top_height = $content_design['field_header_height']) {
        $header_top_height_scroll = theme_get_setting('header_top_height_scroll');
        $CSS .= <<<EOT
.body--glazed-nav-desktop #navbar.glazed-header--top .menu > li > a,
.body--glazed-nav-desktop #navbar.glazed-header--top .wrap-branding {
  height: {$header_top_height}px;
  line-height: {$header_top_height}px;
}
.body--glazed-nav-desktop #navbar.glazed-header--top #block-glazed-helper-full-screen-search {
  height: {$header_top_height}px;
  line-height: {$header_top_height}px;
}
.body--glazed-nav-desktop #navbar.glazed-header--top.affix .menu > li > a,
.body--glazed-nav-desktop #navbar.glazed-header--top.affix .navbar-header .wrap-branding {
  height: {$header_top_height_scroll}px;
  line-height: {$header_top_height_scroll}px;
}
.body--glazed-nav-desktop #navbar.glazed-header--top.affix #block-glazed-helper-full-screen-search {
    height: {$header_top_height_scroll}px;
    line-height: {$header_top_height_scroll}px;
}
.body--glazed-nav-desktop .nav-child-fixed-width {
  top: {$header_top_height}px;
}
.body--glazed-nav-desktop #navbar.glazed-header--top.affix .nav-child-fixed-width {
  top: $header_top_height_scroll;
}

EOT;
      }
      else {
        $header_top_height = theme_get_setting('header_top_height');
      }
      if ($content_design['field_glazed_header_style'] == 'navbar_pull_down') {
        $CSS .= ".body--glazed-nav-desktop.body--glazed-header-not-overlay #navbar.glazed-header--top .col-sm-12 {\n margin-bottom: -" . ($header_top_height / 2) . "px; }\n\n";
      }
      elseif ($content_design['field_glazed_header_style']) {
        $CSS .= ".body--glazed-nav-desktop.body--glazed-header-not-overlay #navbar.glazed-header--top .col-sm-12 {\n margin-bottom: 0; }\n\n";
      }


      // CUSTOM NAVIGATION COLORS
      if ($value = $content_design['field_header_background']) {
        $CSS .= ".html.body--glazed-nav-desktop  #navbar.glazed-header--top { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_header_text']) {
        $CSS .= ".html.body--glazed-nav-desktop  .glazed-header--top,.html.body--glazed-nav-desktop .glazed-header--top a { color: $value }\n\n";
      }
      if ($value = $content_design['field_navbar_background']) {
        $CSS .= "#navbar.glazed-header--top .container-row > .col-sm-12 { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_header_block_background']) {
        $CSS .= "#navbar.glazed-header--top .block { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_header_block_text_color']) {
        $CSS .= "#navbar.glazed-header--top .block, #navbar.glazed-header--top .block a, #navbar.glazed-header--top .block .btn-link { color: $value }\n\n";
      }
      if ($value = $content_design['field_menu_background']) {
        $CSS .= "#navbar.glazed-header--top .menu { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_menu_text_color']) {
        $CSS .= "#navbar.glazed-header--top .menu, #navbar.glazed-header--top .menu a { color: $value }\n\n";
      }
      if ($value = $content_design['field_menu_hover_background']) {
        $CSS .= ".html #navbar.glazed-header--top .menu a:hover, #navbar.glazed-header--top .menu a:focus { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_menu_hover_text_color']) {
        $CSS .= ".html #navbar.glazed-header--top .menu a:hover, #navbar.glazed-header--top .menu a:focus { color: $value }\n\n";
      }
      if ($value = $content_design['field_dropdown_background']) {
        $CSS .= "#navbar.glazed-header--top .dropdown-menu { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_dropdown_text_color']) {
        $CSS .= ".html #navbar.glazed-header--top .dropdown, #navbar.glazed-header--top .dropdown-menu a { color: $value }\n\n";
      }
      if ($value = $content_design['field_dropdown_hover_background']) {
        $CSS .= ".html #navbar.glazed-header--top .dropdown-menu a:hover,#navbar.glazed-header--top .dropdown-menu a:focus { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_dropdown_hover_text_color']) {
        $CSS .= ".html #navbar.glazed-header--top .dropdown-menu a:hover,#navbar.glazed-header--top .dropdown-menu a:focus { color: $value }\n\n";
      }
      // SIDE HEADER
      if ($value = $content_design['field_side_header_background']) {
        $CSS .= "#navbar.glazed-header--side .menu__level, #navbar.glazed-header--side .glazed-main-menu { background-color: $value }\n\n";
      }
      if ($value = $content_design['field_side_header_text_color']) {
        $CSS .= "#navbar.glazed-header--side .menu__level, #navbar.glazed-header--side .glazed-main-menu, #navbar.glazed-header--side a, #navbar.glazed-header--side h1, #navbar.glazed-header--side h2, #navbar.glazed-header--side h3, #navbar.glazed-header--side h4, #navbar.glazed-header--side h5, #navbar.glazed-header--side h6 { color: $value }\n\n";
      }
      if ($value = $content_design['field_side_header_width']) {
        $CSS .= "#navbar.glazed-header--side .glazed-main-menu, #navbar.glazed-header--side .menu__level, .body--glazed-nav-desktop #navbar.glazed-header--side  { width: {$value}px }\n\n";
        $CSS .= "html .body--glazed-nav-desktop.body--glazed-header-side { padding-left: {$value}px }\n\n";
      }
      drupal_add_css($CSS, array('type' => 'inline'));
    }
  }

}
