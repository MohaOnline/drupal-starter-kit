<?php
/**
 * @file
 * menu.theme.inc
 */

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block($variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_megamenu($variables) {
  $element = $variables['element'];
  $depth = $element['#original_link']['depth'];
  $has_children = $element['#original_link']['has_children'];
  $has_nolink = (in_array($element['#href'], array('<nolink>')) ? $element['#title'] : l($element['#title'], $element['#href'], $element['#localized_options']));
  $mainlink_options = $element['#localized_options'];
  unset($mainlink_options['minipanel']);
  $mb_mainlink = '<div class="mb-main-link">' . l($element['#title'] . ' - ' . t('Main Page'), $element['#href'], $mainlink_options) . '</div>';

  // Recursion.
  $sub_menu = '';
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  // Depth 1.
  if ($depth == 1) {
    // Has children menu items.
    if ($has_children == 1) {
      if (function_exists('at_get_setting')) {
        if (!at_get_setting('render_mb_main_link')) {
          return '<li><section><h3>' . $has_nolink . '</h3><div class="mb-sm">' . $sub_menu . $mb_mainlink . '</div></section></li>';
        }
        else {
          return '<li><section><h3>' . $has_nolink . '</h3><div class="mb-sm">' . $sub_menu . '</div></section></li>';
        }
      }
      elseif (function_exists('omega_theme_get_setting')) {
        if (!omega_theme_get_setting('wetkit_render_mb_main_link')) {
          return '<li><section><h3>' . $has_nolink . '</h3><div class="mb-sm">' . $sub_menu . $mb_mainlink . '</div></section></li>';
        }
        else {
          return '<li><section><h3>' . $has_nolink . '</h3><div class="mb-sm">' . $sub_menu . '</div></section></li>';
        }
      }
    }
    // No children menu items.
    else {
      return '<li><div>' . $has_nolink . '</div></li>';
    }
  }
  // Depth 2.
  elseif ($depth == 2) {
    // Has children menu items.
    if ($has_children == 1) {
      return $has_nolink;
    }
    // No children menu items.
    else {
      return $has_nolink;
    }
  }
  // Default Depth.
  else {
    return '<li>' . $has_nolink . '</li>';
  }
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_megamenu_insert($variables) {
  $element = $variables['element'];
  $depth = $element['#original_link']['depth'];
  $has_children = $element['#original_link']['has_children'];
  $has_nolink = (in_array($element['#href'], array('<nolink>')) ? '<a href="#">' . $element['#title'] . '</a>' : l($element['#title'], $element['#href'], $element['#localized_options']));

  // Recursion.
  $sub_menu = '';
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  // Depth 1.
  if ($depth == 1) {
    if ($has_children == 1) {
      if (isset($element['#below']['#children'])) {
        $sub_menu = (preg_match('<h4>', $element['#below']['#children']) ? '<section><ul>' . $sub_menu . '</ul></section>' : '<div><ul>' . $sub_menu . '</ul></div>');
      }
      else {
        $sub_menu = '<div><ul>' . $sub_menu . '</ul></div>';
      }
      return '<section><h3 class="top-section">' . $has_nolink . '</h3>' . $sub_menu . '</section>';
    }
    else {
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $has_nolink . '</li>';
    }
  }
  // Depth 2.
  if ($depth == 2) {
    if ($has_children == 1) {
      return '<section><h4>' . $has_nolink . '</h4><ul>' . $sub_menu . '</ul></section>';
    }
    else {
      $element['#attributes']['class'][] = '';
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $has_nolink . '</li>';
    }
  }
  // Depth 3.
  if ($depth == 3) {
    if ($has_children == 1) {
      return '<li>' . $has_nolink . '</li><ul>' . $sub_menu . '</ul>';
    }
    else {
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $has_nolink . '</li>';
    }
  }
  // Depth 4+.
  else {
    if ($has_children == 1) {
      return '<ul><li>' . $has_nolink . '<ul>' . $sub_menu . '</ul></li></ul>';
    }
    else {
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $has_nolink . '</li>';
    }
  }
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_megamenu_focus($variables) {
  $element = $variables['element'];
  $depth = $element['#original_link']['depth'];
  $has_children = $element['#original_link']['has_children'];
  $has_nolink = (in_array($element['#href'], array('<nolink>')) ? $element['#title'] : l($element['#title'], $element['#href'], $element['#localized_options']));

  // Recursion.
  $sub_menu = '';
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  // Depth 1.
  if ($depth == 2) {
    if ($has_children == 1) {
      return '<h4>' . $has_nolink . '</h4><ul>' . $sub_menu . '</ul>';
    }
    else {
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $has_nolink . '</li>';
    }
  }
  // Depth 3+.
  else {
    return '<li' . drupal_attributes($element['#attributes']) . '>' . $has_nolink . '</li>';
  }
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_mid_footer($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  $has_children = $element['#original_link']['has_children'];

  // Custom Logic Based on Theme Selection (Internet, Intranet or Base).
  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');

  // Internet Theme.
  if ($wxt_active == 'theme-gcwu-fegc') {
    $theme_prefix = 'gcwu';
  }
  // Intranet Theme.
  elseif ($wxt_active == 'theme-gc-intranet') {
    $theme_prefix = 'gcwu';
  }
  // Base Theme.
  elseif ($wxt_active == 'theme-base') {
    $theme_prefix = 'base';
  }
  // OGPL Theme.
  elseif ($wxt_active == 'theme-ogpl') {
    $theme_prefix = 'ogpl';
  }
  // Canada Theme.
  elseif ($wxt_active == 'theme-gcweb' || $wxt_active == 'theme-gcweb-v5') {
    $theme_prefix = 'gcweb';
  }
  // WET Theme.
  else {
    $theme_prefix = 'wet';
  }

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  if ($element['#original_link']['depth'] == 1) {
    if ($has_children == 1) {
      if (in_array($element['#href'], array('<nolink>'))) {
        $output = '<h4 class="' . $theme_prefix . '-col-head">' . $element['#title'] . '</h4>';
      }
      else {
        $output = '<h4 class="' . $theme_prefix . '-col-head">' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</h4>';
      }
      return '<section><div class="span-2">' . $output . '<ul>' . $sub_menu . '</ul>' . "</div></section>\n";
    }
    else {
      if (in_array($element['#href'], array('<nolink>'))) {
        $output = '<div class="' . $theme_prefix . '-col-head">' . $element['#title'] . '</div>';
      }
      else {
        $output = '<div class="' . $theme_prefix . '-col-head">' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</div>';
      }
      return '<div class="span-2">' . $output . "</div>";
    }
  }
  else {
    if (in_array($element['#href'], array('<nolink>'))) {
      $output = $element['#title'];
    }
    else {
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    }
    return '<li>' . $output . $sub_menu . "</li>\n";
  }
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_sidebar($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  $sub_sub_menu = '';
  $sub_sub_sub_menu = '';
  $has_nolink = (in_array($element['#href'], array('<nolink>')) ? l($element['#title'], '#wb-tphp', $element['#localized_options']) : l($element['#title'], $element['#href'], $element['#localized_options']));
  $has_chevron = (theme_get_setting('wetkit_sidebar_no_chevron')) ? '' : ' <span class="expicon glyphicon glyphicon-chevron-down"></span>';

  if ($element['#below']  || ($element['#original_link']['has_children'] && $element['#original_link']['depth'] == 1 && count($element['#below']) == 0)) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
      if (isset($element['#below']['#below'])) {
        $sub_sub_menu = drupal_render($element['#below']['#below']);
      }
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      // Generate as standard dropdown.
      $element['#title'] .= $has_chevron;
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['html'] = TRUE;

      // Set dropdown trigger element to # to prevent inadvertant page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
      foreach ($element['#below'] as $key => $link) {
        if (is_numeric($key)) {
          if ($element['#below'][$key]['#original_link']['has_children'] && $element['#below'][$key]['#original_link']['expanded']) {
            $element['#below'][$key]['#title'] = $element['#below'][$key]['#title'] . $has_chevron;
            $element['#below'][$key]['#localized_options']['attributes']['data-target'] = '#';
            $element['#below'][$key]['#attributes']['class'][] = 'dropdown';
           $element['#below'][$key]['#localized_options']['html'] = TRUE;
         }
        }
      }
      $sub_menu = '<ul class="list-group menu list-unstyled">' . drupal_render($element['#below']) . '</ul>';
    }
    elseif ((!empty($element['#original_link']['depth'])) && $element['#original_link']['expanded'] && $element['#original_link']['depth'] == 2) {
      unset($element['#below']['#theme_wrappers']);
      // Generate as standard dropdown.
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['html'] = TRUE;

      // Set dropdown trigger element to # to prevent inadvertant page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
      foreach ($element['#below'] as $key => $link) {
        if (is_numeric($key)) {
          if ($element['#below'][$key]['#original_link']['has_children'] && $element['#below'][$key]['#original_link']['expanded'] && !count($element['#below'][$key]['#below']) ) {
            //Chevron class was already added by previous render, no need to add it when clicked.
            $element['#below'][$key]['#title'] = $element['#below'][$key]['#title'] . '<span class="expicon glyphicon glyphicon-chevron-down"></span>';
            $element['#below'][$key]['#localized_options']['attributes']['data-target'] = '#';
            $element['#below'][$key]['#attributes']['class'][] = 'dropdown';
            $element['#below'][$key]['#localized_options']['html'] = TRUE;
          }
        }
      }
      $sub_sub_menu = '<ul class="list-group menu list-unstyled">' . drupal_render($element['#below']) . '</ul>';
    }
    if ((!empty($element['#original_link']['depth'])) && $element['#original_link']['depth'] == 3) {
      unset($element['#below']['#theme_wrappers']);
      $sub_sub_sub_menu = '<ul class="list-group menu list-unstyled active">' . drupal_render($element['#below']) . '</ul>';
      // Generate as standard dropdown.
      $element['#title'] .= $has_chevron;
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['html'] = TRUE;

      // Set dropdown trigger element to # to prevent inadvertant page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
    }
  }

  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }

  // <nolink> handling for wxt.
  if (in_array($element['#href'], array('<nolink>'))) {
    $element['#href'] = '#';
    $element['#localized_options'] = array(
      'fragment' => 'wb-sec',
      'external' => TRUE,
      'html' => TRUE,
    );
  }

  if ($element['#original_link']['depth'] == 1) {
    $output = '<h3>' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</h3>';
  }
  else {
    $element['#localized_options']['attributes']['class'][] = 'list-group-item';
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . $sub_sub_menu . $sub_sub_sub_menu . "</li>\n";
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_section($variables) {
  $element = $variables['element'];
  $sub_menu = '';

  if ($element['#below']) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="list-unstyled">' . drupal_render($element['#below']) . '</ul>';
      // Generate as standard dropdown.
      //$element['#title'] .= ' <span class="caret"></span>';
      $element['#attributes']['class'][] = 'dropdown';
      $element['#localized_options']['html'] = TRUE;

      // Set dropdown trigger element to # to prevent inadvertant page loading
      // when a submenu link is clicked.
      $element['#localized_options']['attributes']['data-target'] = '#';
      $element['#localized_options']['attributes']['class'][] = 'dropdown-toggle';
      $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
    }
  }
  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }

  if ($element['#original_link']['depth'] == 1) {
    $output = '<h3>' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</h3>';
  }
  else {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  }
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_sidebar_right($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  if (isset($element['#below']) && is_array($element['#below'])) {
    unset($element['#below']['#theme_wrappers']);
    $sub_menu = '<ul class="list-group list-unstyled">' . drupal_render($element['#below']) . '</ul>';
  }
  $output = '';
  $element['#attributes']['class'][] = 'list-group-item';
  $output .= '<li' . drupal_attributes($element['#attributes']) . '>' . l($element['#title'], $element['#href'], $element['#localized_options']) . $sub_menu . "</li>\n";
  return $output;
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_link_menu_block_group_list($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  if ($element['#below']) {
      // Remove wrapper. They will be outputted on the same level.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = drupal_render($element['#below']);
  }
  // On primary navigation menu, class 'active' is not set on active menu item.
  // @see https://drupal.org/node/1896674
  if (($element['#href'] == $_GET['q'] || ($element['#href'] == '<front>' && drupal_is_front_page())) && (empty($element['#localized_options']['language']))) {
    $element['#attributes']['class'][] = 'active';
  }

  $element['#localized_options']['attributes']['class'][] = 'list-group-item';
  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return $output . "\n" . $sub_menu . "\n";
}

/**
 * Override or insert variables into the menu tree templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_tree__main_menu($variables) {
  return '<ul>' . $variables['tree'] . '</ul>';
}

/**
 * Override or insert variables into the menu tree templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_menu_menu_tree__menu_block($variables) {
  return $variables['tree'];
}

/**
 * Custom array_depth implementation.
 */
function _menu_depth(array $array) {
  $max_depth = 4;
  foreach ($array as $value) {
    if (isset($value['#below']) && is_array($value['#below'])) {
      $depth = _menu_depth($value['#below']) + 1;
      if ($depth > $max_depth) {
        $max_depth = $depth;
      }
    }
  }
  return $max_depth;
}
