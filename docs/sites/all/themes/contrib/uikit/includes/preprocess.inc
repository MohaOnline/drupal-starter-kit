<?php

/**
 * @file
 * Set up variables to be placed within the template (.tpl.php) files.
 *
 * The variables set up here apply to both templates (.tpl.php) files and
 * functions (theme_HOOK). These are also used for providing
 * @link https://www.drupal.org/node/223440 template suggestions @endlink.
 *
 * @see process.inc
 */

use Drupal\uikit\UIkit;

/**
 * Implements template_preprocess_html().
 */
function uikit_preprocess_html(&$variables) {
  // Load UIkit assets, i.e. stylesheets and scripts.
  UIkit::getCdnAssets();

  // Create an HTML5 doctype variable.
  $variables['doctype'] = '<!DOCTYPE html>' . "\n";

  // Create an attributes array for the html element.
  $html_attributes_array = array(
    'lang' => $variables['language']->language,
    'dir' => $variables['language']->dir,
    'class' => array('uk-height-1-1'),
  );
  $variables['html_attributes_array'] = $html_attributes_array;

  // Add the uk-height-1-1 class to extend the <html> and <body> elements to the
  // full height of the page.
  $variables['classes_array'][] = 'uk-height-1-1';

  // Serialize RDF Namespaces into an RDFa 1.1 prefix attribute.
  if ($variables['rdf_namespaces']) {
    $rdf_namespaces = array();

    foreach (explode("\n  ", ltrim($variables['rdf_namespaces'])) as $namespace) {
      // Remove xlmns: and ending quote and fix prefix formatting.
      $rdf_namespaces[] = str_replace('="', ': ', substr($namespace, 6, -1));
    }

    $variables['rdf_namespaces'] = ' prefix="' . implode('  ', $rdf_namespaces) . '"';
  }

  // Add an http-equiv tag, if set.
  if (UIkit::getThemeSetting('x_ua_compatible')) {
    $meta_x_ua_compatible = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'x-ua-compatible',
        'content' => 'IE=' . UIkit::getThemeSetting('x_ua_compatible'),
      ),
      '#weight' => -9998,
    );

    drupal_add_html_head($meta_x_ua_compatible, 'uikit_x_ua_compatible');
  }

  // Get viewport metadata settings for mobile devices.
  $device_width_ratio = UIkit::getThemeSetting('viewport_device_width_ratio');
  $custom_device_width = UIkit::getThemeSetting('viewport_custom_width');
  $device_height_ratio = UIkit::getThemeSetting('viewport_device_height_ratio');
  $custom_device_height = UIkit::getThemeSetting('viewport_custom_height');
  $initial_scale = UIkit::getThemeSetting('viewport_initial_scale');
  $maximum_scale = UIkit::getThemeSetting('viewport_maximum_scale');
  $minimum_scale = UIkit::getThemeSetting('viewport_minimum_scale');
  $user_scalable = UIkit::getThemeSetting('viewport_user_scalable');
  $viewport_array = array();

  // Add each, if set, to the viewport array.
  if ($device_width_ratio == 'device-width') {
    $viewport_array['width'] = 'width=device-width';
  }
  elseif ($device_width_ratio) {
    $viewport_array['width'] = 'width=' . $custom_device_width;
  }
  if ($device_height_ratio == 1) {
    $viewport_array['height'] = 'height=' . $custom_device_height;
  }
  if ($initial_scale) {
    $viewport_array['initial-scale'] = 'initial-scale=' . $initial_scale;
  }
  if ($maximum_scale) {
    $viewport_array['maximum-scale'] = 'maximum-scale=' . $maximum_scale;
  }
  if ($minimum_scale) {
    $viewport_array['minimum-scale'] = 'minimum-scale=' . $minimum_scale;
  }
  if ($viewport_array && !$user_scalable) {
    $viewport_array['user-scalable'] = 'user-scalable=no';
  }

  if ($viewport_array) {
    // Add the viewport options to the viewport meta tag.
    $viewport_content = implode(', ', $viewport_array);

    $meta_viewport = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'name' => 'viewport',
        'content' => $viewport_content,
      ),
      '#weight' => -9997,
    );

    drupal_add_html_head($meta_viewport, 'uikit_viewport');
  }
}

/**
 * Implements template_preprocess_page().
 */
function uikit_preprocess_page(&$variables) {
  // Get various theme settings to add the correct attributes.
  $sidebar_first = isset($variables['page']['sidebar_first']) ? $variables['page']['sidebar_first'] : '';
  $sidebar_second = isset($variables['page']['sidebar_second']) ? $variables['page']['sidebar_second'] : '';
  $standard_layout = UIkit::getThemeSetting('standard_sidebar_positions');
  $tablet_layout = UIkit::getThemeSetting('tablet_sidebar_positions');
  $mobile_layout = UIkit::getThemeSetting('mobile_sidebar_positions');

  // Determine the standard layout.
  $standard_grail = $standard_layout === 'holy-grail';
  $standard_left = $standard_layout === 'sidebars-left';
  $standard_right = $standard_layout === 'sidebars-right';

  // Determine the tablet layout.
  $tablet_grail = $tablet_layout === 'holy-grail';
  $tablet_left = $tablet_layout === 'sidebars-left';
  $tablet_left_stacked = $tablet_layout === 'sidebar-left-stacked';
  $tablet_right = $tablet_layout === 'sidebars-right';
  $tablet_right_stacked = $tablet_layout === 'sidebar-right-stacked';

  // Determine the mobile layout.
  $mobile_stacked = $mobile_layout === 'sidebars-stacked';
  $mobile_vertical = $mobile_layout === 'sidebars-vertical';

  // Assign page container attributes.
  $page_container_attributes['id'] = 'page';
  if (UIkit::getThemeSetting('page_container')) {
    $page_container_attributes['class'][] = 'uk-container';
  }
  if (UIkit::getThemeSetting('page_margin')) {
    $page_container_attributes['class'][] = UIkit::getThemeSetting('page_margin');
  }
  $variables['page_container_attributes_array'] = $page_container_attributes;

  // Assign content attributes.
  $variables['content_attributes_array']['id'] = 'page-content';

  // Assign sidebar_first attributes.
  $variables['sidebar_first_attributes_array'] = array(
    'id' => 'sidebar-first',
    'class' => array('uk-width-1-4@l'),
  );

  // Assign sidebar_second attributes.
  $variables['sidebar_second_attributes_array'] = array(
    'id' => 'sidebar-second',
    'class' => array('uk-width-1-4@l'),
  );

  // Assign additional content attributes if either sidebar is not empty.
  if (!empty($sidebar_first) && !empty($sidebar_second)) {
    $variables['content_attributes_array']['class'][] = 'uk-width-1-2@l';
    $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-4@l';
    $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-4@l';

    if ($standard_grail) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-4@l';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-pull-1-2@l';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@l';
    }
    elseif ($standard_left) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-2@l';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-pull-1-2@l';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-pull-1-2@l';
    }
    elseif ($standard_right) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@l';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@l';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@l';
    }

    if ($tablet_grail || $tablet_left || $tablet_right) {
      $variables['content_attributes_array']['class'][] = 'uk-width-1-2@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-4@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-4@m';
    }
    elseif ($tablet_left_stacked || $tablet_right_stacked) {
      $variables['content_attributes_array']['class'][] = 'uk-width-3-4@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-4@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-1@m';
    }

    if ($tablet_grail) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-4@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-pull-1-2@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@m';
    }
    elseif ($tablet_left) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-2@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-pull-1-2@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-pull-1-2@m';
    }
    elseif ($tablet_right) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@m';
    }
    elseif ($tablet_left_stacked) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-4@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-pull-3-4@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@m';
    }
    elseif ($tablet_right_stacked) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@m';
    }

    if ($mobile_stacked || $mobile_vertical) {
      $variables['content_attributes_array']['class'][] = 'uk-width-1-1@s';
      $variables['content_attributes_array']['class'][] = 'uk-width-1-1';
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@s';
    }

    if ($mobile_stacked) {
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-1@s';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-1';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@s';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-1@s';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-1';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@s';
    }
    elseif ($mobile_vertical) {
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-2@s';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-2';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@s';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-2@s';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-2';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@s';
    }
  }
  elseif (!empty($sidebar_first)) {
    $variables['content_attributes_array']['class'][] = 'uk-width-3-4@l';
    $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-4@l';

    if ($standard_grail || $standard_left) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-4@l';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-pull-3-4@l';
    }
    elseif ($standard_right) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@l';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@l';
    }

    if ($tablet_layout) {
      $variables['content_attributes_array']['class'][] = 'uk-width-3-4@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-width-1-4@m';
    }

    if ($tablet_grail || $tablet_left || $tablet_left_stacked) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-4@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-pull-3-4@m';
    }
    elseif ($tablet_right || $tablet_right_stacked) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@m';
    }
  }
  elseif (!empty($sidebar_second)) {
    $variables['content_attributes_array']['class'][] = 'uk-width-3-4@l';
    $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-4@l';

    if ($standard_grail || $standard_right) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@l';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@l';
    }
    elseif ($standard_left) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-4@l';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-pull-3-4@l';
    }

    if ($tablet_grail || $tablet_right || $tablet_left) {
      $variables['content_attributes_array']['class'][] = 'uk-width-3-4@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-4@m';
    }
    elseif ($tablet_left_stacked || $tablet_right_stacked) {
      $variables['content_attributes_array']['class'][] = 'uk-width-1-1@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-width-1-1@m';
    }

    if ($tablet_grail || $tablet_right) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@m';
      $variables['sidebar_first_attributes_array']['class'][] = 'uk-push-pull-@m';
    }
    elseif ($tablet_left) {
      $variables['content_attributes_array']['class'][] = 'uk-push-1-4@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-pull-3-4@m';
    }
    elseif ($tablet_left_stacked || $tablet_right_stacked) {
      $variables['content_attributes_array']['class'][] = 'uk-push-pull-@m';
      $variables['sidebar_second_attributes_array']['class'][] = 'uk-push-pull-@m';
    }
  }
  elseif (empty($sidebar_first) && empty($sidebar_second)) {
    $variables['content_attributes_array']['class'][] = 'uk-width-1-1';
  }

  // Define header attributes.
  $variables['header_attributes_array'] = array(
    'id' => 'page-header',
  );

  // Define navbar attributes.
  $variables['navbar_attributes_array'] = array(
    'id' => 'page-navbar',
    'class' => array('uk-navbar-container'),
    'uk-navbar' => '',
  );

  // Move the main and secondary menus into variables to set the attributes
  // accordingly.
  $variables['navbar_primary'] = FALSE;
  $variables['navbar_secondary'] = FALSE;
  $variables['offcanvas_primary'] = FALSE;
  $variables['offcanvas_secondary'] = FALSE;

  // Provide menu trees and theme wrappers for the navbar and offcanvas menus.
  if ($variables['main_menu']) {
    // Primary navbar menu.
    $variables['navbar_primary'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    $variables['navbar_primary']['#theme_wrappers'] = array('menu_tree__navbar');

    // Primary offcanvas menu.
    $variables['offcanvas_primary'] = menu_tree(variable_get('menu_main_links_source', 'main-menu'));
    $variables['offcanvas_primary']['#theme_wrappers'] = array('menu_tree__offcanvas');

    // Primary offcanvas below menus.
    foreach ($variables['offcanvas_primary'] as $key => $value) {
      if (is_array($value) && isset($value['#below']) && !empty($value['#below'])) {
        $variables['offcanvas_primary'][$key]['#below']['#theme_wrappers'] = array('menu_tree__offcanvas__below');
      }
    }
  }
  if ($variables['secondary_menu']) {
    // Secondary navbar menu.
    $variables['navbar_secondary'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    $variables['navbar_secondary']['#theme_wrappers'] = array('menu_tree__navbar');

    // Secondary offcanvas menu.
    $variables['offcanvas_secondary'] = menu_tree(variable_get('menu_secondary_links_source', 'user-menu'));
    $variables['offcanvas_secondary']['#theme_wrappers'] = array('menu_tree__offcanvas');

    // Primary offcanvas below menus.
    foreach ($variables['offcanvas_secondary'] as $key => $value) {
      if (is_array($value) && isset($value['#below']) && !empty($value['#below'])) {
        $variables['offcanvas_secondary'][$key]['#below']['#theme_wrappers'] = array('menu_tree__offcanvas__below');
      }
    }
  }

  // Create variable for breadcrumb display setting.
  $variables['display_breadcrumb'] = UIkit::getThemeSetting('display_breadcrumbs');
}

/**
 * Implements template_preprocess_maintenance_page().
 */
function uikit_preprocess_maintenance_page(&$variables) {
  // Load UIkit assets, i.e. stylesheets and scripts.
  UIkit::getCdnAssets();

  // Add an http-equiv tag, if set.
  if (UIkit::getThemeSetting('x_ua_compatible')) {
    $meta_x_ua_compatible = array(
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => array(
        'http-equiv' => 'x-ua-compatible',
        'content' => 'IE=' . UIkit::getThemeSetting('x_ua_compatible'),
      ),
      '#weight' => -9998,
    );

    drupal_add_html_head($meta_x_ua_compatible, 'uikit_x_ua_compatible');
  }

  // Get viewport metadata settings for mobile devices.
  $device_width_ratio = UIkit::getThemeSetting('viewport_device_width_ratio', UIkit::getActiveTheme());
  $custom_device_width = UIkit::getThemeSetting('viewport_custom_width', UIkit::getActiveTheme());
  $device_height_ratio = UIkit::getThemeSetting('viewport_device_height_ratio', UIkit::getActiveTheme());
  $custom_device_height = UIkit::getThemeSetting('viewport_custom_height', UIkit::getActiveTheme());
  $initial_scale = UIkit::getThemeSetting('viewport_initial_scale', UIkit::getActiveTheme());
  $maximum_scale = UIkit::getThemeSetting('viewport_maximum_scale', UIkit::getActiveTheme());
  $minimum_scale = UIkit::getThemeSetting('viewport_minimum_scale', UIkit::getActiveTheme());
  $user_scalable = UIkit::getThemeSetting('viewport_user_scalable', UIkit::getActiveTheme());
  $viewport_array = array();

  // Add each, if set, to the viewport array.
  if ($device_width_ratio == 'device-width') {
    $viewport_array['width'] = 'width=device-width';
  }
  elseif ($device_width_ratio) {
    $viewport_array['width'] = 'width=' . $custom_device_width;
  }
  if ($device_height_ratio == 1) {
    $viewport_array['height'] = 'height=' . $custom_device_height;
  }
  if ($initial_scale) {
    $viewport_array['initial-scale'] = 'initial-scale=' . $initial_scale;
  }
  if ($maximum_scale) {
    $viewport_array['maximum-scale'] = 'maximum-scale=' . $maximum_scale;
  }
  if ($minimum_scale) {
    $viewport_array['minimum-scale'] = 'minimum-scale=' . $minimum_scale;
  }
  if ($viewport_array && !$user_scalable) {
    $viewport_array['user-scalable'] = 'user-scalable=no';
  }

  if ($viewport_array) {
    // Add the viewport options to the viewport meta tag.
    $viewport_content = implode(', ', $viewport_array);

    $meta_viewport = [
      '#type' => 'html_tag',
      '#tag' => 'meta',
      '#attributes' => [
        'name' => 'viewport',
        'content' => $viewport_content,
      ],
      '#weight' => -9997,
    ];

    drupal_add_html_head($meta_viewport, 'uikit_viewport');
  }

  // Assign content attributes.
  $content_attributes = array(
    'id' => 'page-content',
    'class' => array('uk-width-1-1@s'),
  );

  // Assign sidebar_first attributes.
  $sidebar_first_attributes = array(
    'id' => 'sidebar-first',
    'class' => array('uk-width-1-1@s'),
  );

  // Assign sidebar_second attributes.
  $sidebar_second_attributes = array(
    'id' => 'sidebar-second',
    'class' => array('uk-width-1-1@s'),
  );

  // Assign additional content attributes if either sidebar is not empty.
  $sidebar_first = $variables['sidebar_first'];
  $sidebar_second = $variables['sidebar_second'];
  if (!empty($sidebar_first) && !empty($sidebar_second)) {
    $content_attributes['class'][] = 'uk-width-1-2@m';
    $content_attributes['class'][] = 'uk-push-1-4@m';
    $sidebar_first_attributes['class'][] = 'uk-width-1-4@m';
    $sidebar_first_attributes['class'][] = 'uk-pull-1-2@m';
    $sidebar_second_attributes['class'][] = 'uk-width-1-4@m';
    $sidebar_second_attributes['class'][] = 'uk-push-pull-@m';
  }
  elseif (!empty($sidebar_first)) {
    $content_attributes['class'][] = 'uk-width-3-4@m';
    $content_attributes['class'][] = 'uk-push-1-4@m';
    $sidebar_first_attributes['class'][] = 'uk-width-1-4@m';
    $sidebar_first_attributes['class'][] = 'uk-pull-3-4@m';
  }
  elseif (!empty($sidebar_second)) {
    $content_attributes['class'][] = 'uk-width-3-4@m';
    $content_attributes['class'][] = 'uk-push-pull-@m';
    $sidebar_second_attributes['class'][] = 'uk-width-1-4@m';
    $sidebar_second_attributes['class'][] = 'uk-push-pull-@m';
  }

  // Add attributes.
  $variables['classes_array'][] = 'uk-height-1-1';
  $variables['content_attributes_array'] = $content_attributes;
  $variables['sidebar_first_attributes_array'] = $sidebar_first_attributes;
  $variables['sidebar_second_attributes_array'] = $sidebar_second_attributes;
}

/**
 * Implements template_preprocess_node().
 */
function uikit_preprocess_node(&$variables) {
  $node = $variables['node'];

  // Add the uk-text-lead class to all node titles.
  $variables['title_attributes_array']['class'][] = 'uk-text-lead';

  // Theme the submitted meta data.
  $datetime = date('F j, Y', $node->created);
  if (variable_get('node_submitted_' . $node->type, TRUE)) {
    $variables['submitted'] = t('Written by !username on !datetime', array('!username' => $variables['name'], '!datetime' => $datetime));
  }
}

/**
 * Implements template_preprocess_region().
 */
function uikit_preprocess_region(&$variables) {
  global $theme_key;
  $region = $variables['region'];
  $wrapper_id = str_replace('_', '-', $region);

  // Add a wrapper id and class to help themers.
  $variables['wrapper_attributes_array'] = array(
    'id' => 'region-' . $wrapper_id . '-wrapper',
    'class' => array('region-wrapper'),
  );

  // Add an attributes id.
  $variables['attributes_array'] = array(
    'id' => 'region-' . $wrapper_id,
  );

  // Get all regions for the theme.
  $regions = system_region_list($theme_key);

  foreach ($regions as $key => $value) {
    // Get the settings for each region being used.
    if ($region == $key) {
      $style_setting = UIkit::getThemeSetting($key . '_style');
      $card_style = UIkit::getThemeSetting($key . '_card_style');
      $region_style = $style_setting ? $style_setting : 0;
      $region_card_style = $card_style ? $card_style : 0;

      if ($region_style) {
        switch ($region_style) {
          case 'card':
            $variables['attributes_array']['class'][] = 'uk-card';
            $variables['attributes_array']['class'][] = 'uk-card-body';
            break;
        }
      }

      if ($region_card_style) {
        switch ($region_card_style) {
          case 'default':
            $variables['attributes_array']['class'][] = 'uk-card-default';
            break;

          case 'primary':
            $variables['attributes_array']['class'][] = 'uk-card-primary';
            break;

          case 'secondary':
            $variables['attributes_array']['class'][] = 'uk-card-secondary';
            break;
        }
      }
    }
  }
}

/**
 * Implements template_preprocess_block().
 */
function uikit_preprocess_block(&$variables) {
  $region = $variables['block']->region;

  if ($region == 'sidebar_first' || $region == 'sidebar_second') {
    $variables['classes_array'][] = 'uk-margin-bottom';
  }
}

/**
 * Implements template_preprocess_HOOK().
 */
function uikit_preprocess_breadcrumb(&$variables) {
  // Remove home link from breadcrumb if disabled in theme settings.
  if (!UIkit::getThemeSetting('breakcrumbs_home_link')) {
    array_shift($variables['breadcrumb']);
  }

  if (UIkit::getThemeSetting('breakcrumbs_current_page') && !drupal_is_front_page()) {
    // Display the current page title if enabled in the theme settings and set
    // it as being active.
    $page_title = UIkit::getPageTitle();
    $variables['breadcrumb'][] = '<span>' . $page_title . '</span>';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_button().
 */
function uikit_preprocess_button(&$variables) {
  // Add the uk-button-default class to all buttons.
  $variables['element']['#attributes']['class'][] = 'uk-button';
  $variables['element']['#attributes']['class'][] = 'uk-button-default';
}

/**
 * Implements template_preprocess_comment().
 */
function uikit_preprocess_comment(&$variables) {
  $comment = $variables['elements']['#comment'];
  $node = $variables['elements']['#node'];

  // Check if user picture in comments is enabled.
  $comment_picture = UIkit::getThemeSetting('toggle_comment_user_picture') ? 1 : 0;

  // Check if user pictures are enabled.
  $user_pictures = variable_get('user_pictures') ? 1 : 0;

  // Chech if a default picture has been set.
  $user_picture_default = !empty(variable_get('user_picture_default')) ? 1 : 0;

  // Add comment classes.
  $variables['classes_array'][] = 'uk-comment';
  if ($variables['elements']['#comment']->divs > 0) {
    $variables['classes_array'][] = 'uk-comment-primary';
  }
  $variables['title_attributes_array']['class'][] = 'uk-comment-title';
  $variables['title_attributes_array']['class'][] = 'uk-margin-remove';
  $variables['content_attributes_array']['class'][] = 'uk-comment-body';

  // Use the comment id as the permalink text in the comment title.
  $id = $variables['id'];
  $cid = $comment->cid;
  $uri = entity_uri('node', $node);
  $uri['options'] += array(
    'attributes' => array(
      'class' => array(
        'permalink',
        'uk-link-reset',
      ),
      'rel' => 'bookmark',
    ),
    'fragment' => "comment-$cid",
  );
  $variables['permalink'] = l(t('#@id', array('@id' => $id)), $uri['path'], $uri['options']);

  // Use the same uri for the title permalink.
  $variables['title'] = l(t('@subject', array('@subject' => $comment->subject)), $uri['path'], $uri['options']);

  // Use separate submitted by and date variables.
  $variables['submitted_user'] = t('!username', array('!username' => $variables['author']));
  $variables['submitted_date'] = t('!datetime', array('!datetime' => $variables['created']));

  if ($comment_picture && $user_pictures && $user_picture_default && empty($variables['picture'])) {
    // Provide a default image when the user does not have a picture uploaded.
    if (empty($variables['picture'])) {
      $default = ' data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0idXRmLTgiPz4NCjwhLS0gR2VuZXJhdG9yOiBBZG9iZSBJbGx1c3RyYXRvciAxNi4wLjQsIFNWRyBFeHBvcnQgUGx1Zy1JbiAuIFNWRyBWZXJzaW9uOiA2LjAwIEJ1aWxkIDApICAtLT4NCjwhRE9DVFlQRSBzdmcgUFVCTElDICItLy9XM0MvL0RURCBTVkcgMS4xLy9FTiIgImh0dHA6Ly93d3cudzMub3JnL0dyYXBoaWNzL1NWRy8xLjEvRFREL3N2ZzExLmR0ZCI+DQo8c3ZnIHZlcnNpb249IjEuMSIgaWQ9IkViZW5lXzEiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIgeG1sbnM6eGxpbms9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkveGxpbmsiIHg9IjBweCIgeT0iMHB4Ig0KCSB3aWR0aD0iNTBweCIgaGVpZ2h0PSI1MHB4IiB2aWV3Qm94PSIwIDAgNTAgNTAiIGVuYWJsZS1iYWNrZ3JvdW5kPSJuZXcgMCAwIDUwIDUwIiB4bWw6c3BhY2U9InByZXNlcnZlIj4NCjxyZWN0IGZpbGw9IiNGRkZGRkYiIHdpZHRoPSI1MCIgaGVpZ2h0PSI1MCIvPg0KPGc+DQoJPHBhdGggZmlsbD0iI0UwRTBFMCIgZD0iTTQ1LjQ1LDQxLjM0NWMtMC4yMDktMS4xNjYtMC40NzMtMi4yNDYtMC43OTEtMy4yNDJjLTAuMzE5LTAuOTk2LTAuNzQ3LTEuOTY3LTEuMjg2LTIuOTE0DQoJCWMtMC41MzgtMC45NDYtMS4xNTUtMS43NTMtMS44NTItMi40MmMtMC42OTktMC42NjctMS41NS0xLjItMi41NTYtMS41OThzLTIuMTE3LTAuNTk4LTMuMzMyLTAuNTk4DQoJCWMtMC4xNzksMC0wLjU5NywwLjIxNC0xLjI1NSwwLjY0MmMtMC42NTcsMC40MjktMS4zOTksMC45MDctMi4yMjYsMS40MzRjLTAuODI3LDAuNTI4LTEuOTAzLDEuMDA2LTMuMjI3LDEuNDM0DQoJCWMtMS4zMjUsMC40MjktMi42NTUsMC42NDMtMy45ODksMC42NDNjLTEuMzM0LDAtMi42NjQtMC4yMTQtMy45ODktMC42NDNjLTEuMzI1LTAuNDI4LTIuNDAxLTAuOTA2LTMuMjI3LTEuNDM0DQoJCWMtMC44MjgtMC41MjctMS41NjktMS4wMDUtMi4yMjYtMS40MzRjLTAuNjU4LTAuNDI4LTEuMDc2LTAuNjQyLTEuMjU1LTAuNjQyYy0xLjIxNiwwLTIuMzI2LDAuMTk5LTMuMzMyLDAuNTk4DQoJCWMtMS4wMDYsMC4zOTgtMS44NTgsMC45MzEtMi41NTQsMS41OThjLTAuNjk5LDAuNjY3LTEuMzE1LDEuNDc0LTEuODUzLDIuNDJjLTAuNTM4LDAuOTQ3LTAuOTY3LDEuOTE3LTEuMjg1LDIuOTE0DQoJCXMtMC41ODMsMi4wNzYtMC43OTIsMy4yNDJjLTAuMjA5LDEuMTY1LTAuMzQ5LDIuMjUxLTAuNDE4LDMuMjU2Yy0wLjA3LDEuMDA2LTAuMTA0LDIuMS0wLjEwNCwzLjE1NUMzLjkwMSw0OC41NCwzLjk4Nyw0OSw0LjE0Myw1MA0KCQloNDEuNTg5YzAuMTU2LTEsMC4yNDItMS40NiwwLjI0Mi0yLjI0M2MwLTEuMDU1LTAuMDM1LTIuMTE4LTAuMTA1LTMuMTI0QzQ1Ljc5OSw0My42MjcsNDUuNjYsNDIuNTEsNDUuNDUsNDEuMzQ1eiIvPg0KCTxwYXRoIGZpbGw9IiNFMEUwRTAiIGQ9Ik0yNC45MzgsMzIuNDg1YzMuMTY3LDAsNS44NzEtMS4xMjEsOC4xMTMtMy4zNjFjMi4yNDEtMi4yNDIsMy4zNjEtNC45NDUsMy4zNjEtOC4xMTMNCgkJcy0xLjEyMS01Ljg3Mi0zLjM2MS04LjExMmMtMi4yNDItMi4yNDEtNC45NDYtMy4zNjItOC4xMTMtMy4zNjJzLTUuODcyLDEuMTIxLTguMTEyLDMuMzYyYy0yLjI0MiwyLjI0MS0zLjM2Miw0Ljk0NS0zLjM2Miw4LjExMg0KCQlzMS4xMiw1Ljg3MSwzLjM2Miw4LjExM0MxOS4wNjUsMzEuMzY1LDIxLjc3MSwzMi40ODUsMjQuOTM4LDMyLjQ4NXoiLz4NCjwvZz4NCjwvc3ZnPg0K';
      $variables['picture'] = '<img class="uk-comment-avatar" width="50" height="50" src="' . $default . '">';
    }
  }
  $variables['content']['links']['#attributes']['class'][] = 'uk-float-right';
}

/**
 * Implements template_preprocess_comment_wrapper().
 */
function uikit_preprocess_comment_wrapper(&$variables) {
  // Remove margin from the comment wrapper element.
  $variables['classes_array'][] = 'uk-margin-top-remove';
}

/**
 * Implements hook_preprocess_HOOK() for theme_confirm_form().
 */
function uikit_preprocess_confirm_form(&$variables) {
  foreach ($variables['form']['actions'] as $key => $action) {
    $type = isset($action['#type']) ? $action['#type'] : 0;

    if ($type && $type == 'link') {
      // Add the uk-button-default class to form action links for consistent
      // styling with form action buttons.
      $variables['form']['actions'][$key]['#attributes']['class'][] = 'uk-button-default';
    }
  }
}

/**
 * Implements template_preprocess_field().
 */
function uikit_preprocess_field(&$variables) {
  $type = $variables['element']['#field_type'];
  $classes = $variables['classes_array'];

  // Add utility classes based on field type.
  switch ($type) {
    case 'image':
      $classes[] = 'uk-display-inline-block';
      $classes[] = 'uk-margin';
      break;
  }

  $variables['classes_array'] = $classes;

  if ($variables['element']['#field_name'] == 'field_tags') {
    foreach ($variables['items'] as $delta => $item) {
      $variables['item_attributes_array'][$delta]['class'][] = 'field-item';
      $variables['item_attributes_array'][$delta]['class'][] = $delta % 2 ? 'odd' : 'even';
    }
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_fieldset().
 */
function uikit_preprocess_fieldset(&$variables) {
  $collapsible = isset($variables['element']['#collapsible']) && $variables['element']['#collapsible'];
  $group_fieldset = isset($variables['element']['#group_fieldset']) && $variables['element']['#group_fieldset'];
  $format_fieldset = isset($variables['element']['format']);

  // Collapsible, non-grouped fieldsets will use UIkit's accordion components.
  if ($group_fieldset) {
    $variables['theme_hook_suggestions'][] = 'fieldset__grouped';
  }
  elseif ($collapsible) {
    $variables['theme_hook_suggestions'][] = 'fieldset__collapsible';
    $variables['element']['#attributes']['class'][] = 'uk-margin';
    $variables['element']['#attributes']['class'][] = 'uk-accordion';
    $variables['element']['#attributes']['uk-accordion'] = '';

    foreach ($variables['element']['#attributes']['class'] as $key => $class) {
      if ($class == 'collapsible' || $class == 'collapsed') {
        unset($variables['element']['#attributes']['class'][$key]);
        array_values($variables['element']['#attributes']['class']);
      }
      if ($class == 'collapsed') {
        $variables['element']['#attributes']['uk-accordion'] .= '{showfirst: false}';
      }
    }
  }
  elseif ($format_fieldset) {
    $variables['theme_hook_suggestions'][] = 'fieldset__format';
  }
  else {
    $variables['theme_hook_suggestions'][] = 'fieldset';
    $variables['element']['#attributes']['class'][] = 'uk-fieldset';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_form().
 */
function uikit_preprocess_form(&$variables) {
  $attributes = $variables['element']['#attributes'];

  // Make sure the class attribute is an array. If not we take the class set
  // and add it to an array. This is a special case issue caused by the
  // api_search module, and should not be a common problem.
  if (isset($attributes['class']) && !is_array($attributes['class'])) {
    $class = $attributes['class'];
    $variables['element']['#attributes']['class'] = array($class);
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_item_list().
 */
function uikit_preprocess_item_list(&$variables) {
  $pagination = isset($variables['pagination']) && $variables['pagination'];

  // Add the uk-list class to all but pagination item lists.
  if (!$pagination) {
    $variables['attributes']['class'][] = 'uk-list';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_links().
 */
function uikit_preprocess_links(&$variables) {
  $classes = isset($variables['attributes']['class']) ? $variables['attributes']['class'] : array();

  // Add uk-subnav class to inline links.
  $inline = in_array('inline', $classes);

  if ($inline) {
    $variables['attributes']['class'][] = 'uk-subnav';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_menu_local_tasks().
 */
function uikit_preprocess_menu_local_tasks(&$variables) {
  // Get the local task styles.
  $primary_style = UIkit::getThemeSetting('primary_tasks_style');
  $secondary_style = UIkit::getThemeSetting('secondary_tasks_style');

  // Set the default attributes.
  $variables['primary_attributes_array'] = array(
    'id' => 'primary-local-tasks',
    'class' => array('uk-subnav'),
  );
  $variables['secondary_attributes_array'] = array(
    'id' => 'secondary-local-tasks',
    'class' => array('uk-subnav'),
  );

  // Add additional styling from theme settings.
  if ($primary_style) {
    $variables['primary_attributes_array']['class'][] = $primary_style;
  }
  if ($secondary_style) {
    $variables['secondary_attributes_array']['class'][] = $secondary_style;
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_menu_tree().
 */
function uikit_preprocess_menu_tree(&$variables) {
  $shortcut_set = preg_grep("/^(menu_tree__shortcut_set)/", $variables['#tree']['#theme_wrappers']);

  if ($shortcut_set) {
    // Provide hook suggestion for shortcut set menu trees.
    $variables['theme_hook_suggestions'][] = 'menu_tree__shortcut_set';
  }
}

/**
 * Implements hook_preprocess_HOOK() for theme_password().
 */
function uikit_preprocess_password(&$variables) {
  // Define size attribute for password fields for consistent styling.
  $variables['element']['#attributes']['size'] = 25;
}

/**
 * Implements hook_preprocess_HOOK() for theme_table().
 */
function uikit_preprocess_table(&$variables) {
  $variables['attributes']['class'][] = 'uk-table';

  // Add some additional classes to the table for text format filter tips.
  $filter_tips = current_path() === 'filter/tips';
  if ($filter_tips) {
    $variables['attributes']['class'][] = 'uk-table-striped';
    $variables['attributes']['class'][] = 'table-filter-tips';
  }
}

/**
 * Implements template_preprocess_username().
 */
function uikit_preprocess_username(&$variables) {
  global $language;
  $user_language = isset($variables['attributes_array']['xml:lang']);

  if ($user_language) {
    // Provide a proper lang attribute to username elements.
    $lang = $variables['attributes_array']['xml:lang'];
    unset($variables['attributes_array']['xml:lang']);
    $variables['attributes_array']['lang'] = !empty($lang) ? $lang : $language->language;
  }
}
