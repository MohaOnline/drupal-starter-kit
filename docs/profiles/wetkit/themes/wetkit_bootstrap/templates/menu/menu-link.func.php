<?php
/**
 * @file
 * Stub file for bootstrap_menu_link() and suggestion(s).
 */

/**
 * Returns HTML for a menu link and submenu.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: Structured array data for a menu link.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see theme_menu_link()
 *
 * @ingroup theme_functions
 */
function wetkit_bootstrap_menu_link(array $variables) {
  $element = $variables['element'];
  $sub_menu = '';

  $options = !empty($element['#localized_options']) ? $element['#localized_options'] : array();

  // Check plain title if "html" is not set, otherwise, filter for XSS attacks.
  $title = empty($options['html']) ? check_plain($element['#title']) : filter_xss_admin($element['#title']);

  // Ensure "html" is now enabled so l() doesn't double encode. This is now
  // safe to do since both check_plain() and filter_xss_admin() encode HTML
  // entities. See: https://www.drupal.org/node/2854978
  $options['html'] = TRUE;

  $href = $element['#href'];
  $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();

  if ($element['#below']) {
    // Prevent dropdown functions from being added to management menu so it
    // does not affect the navbar module.
    if (($element['#original_link']['menu_name'] == 'management') && (module_exists('navbar'))) {
      $sub_menu = drupal_render($element['#below']);
    }
    elseif ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      $sub_menu = '<ul class="dropdown-menu">' . drupal_render($element['#below']) . '</ul>';

      // Generate as standard dropdown.
      $title .= ' <span class="caret"></span>';
      $attributes['class'][] = 'dropdown';
      $options['attributes']['class'][] = 'dropdown-toggle';
      $options['attributes']['data-toggle'] = 'dropdown';
    }
  }

  return '<li' . drupal_attributes($attributes) . '>' . l($title, $href, $options) . $sub_menu . "</li>\n";
}

/**
 * Overrides theme_menu_link() for the mega menu.
 */
function wetkit_bootstrap_menu_link__menu_block__main_menu(&$variables) {
  $element = $variables['element'];
  $sub_menu = '';
  $mb_mainlink = (!in_array($element['#href'], array('<nolink>')) ? '<li class="slflnk">' . l($element['#title'] . ' - ' . t('More'), $element['#href'], $element['#localized_options']) . '</li>' : '' );
  $depth = $element['#original_link']['depth'];
  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $library_path = libraries_get_path($wxt_active, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active);
  $wxt_active = str_replace('theme_', '', $wxt_active);

  if ($element['#below']) {
    if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
      // Add our own wrapper.
      unset($element['#below']['#theme_wrappers']);
      if (!theme_get_setting('wetkit_render_mb_main_link') && $wxt_active != 'gcweb_v5') {
        $sub_menu = '<ul class="sm list-unstyled" role="menu">' . drupal_render($element['#below']) . $mb_mainlink . '</ul>';
      }
      else {
        if ($wxt_active == 'gcweb_v5') {
          $sub_menu = '<ul role="menu">' . drupal_render($element['#below']) . '</ul>';
        }
        else {
          $sub_menu = '<ul class="sm list-unstyled" role="menu">' . drupal_render($element['#below']) . '</ul>';
        }
      }

      if ($wxt_active != 'gcweb_v5') {
        // Generate as standard dropdown.
        $element['#attributes']['class'][] = 'dropdown';
        $element['#localized_options']['html'] = TRUE;

        // Set dropdown trigger element to # to prevent inadvertant page loading
        // when a submenu link is clicked.
        $element['#localized_options']['attributes']['data-target'] = '#';
        $element['#localized_options']['attributes']['class'][] = 'item';
        $element['#localized_options']['attributes']['data-toggle'] = 'dropdown';
      }
    }
  }

  // GCWeb v5.
  if ($wxt_active == 'gcweb_v5') {
    if ($element['#below']) {
      $element['#localized_options']['html'] = TRUE;
      $element['#localized_options']['attributes']['role'] = 'menuitem';
      $element['#localized_options']['attributes']['aria-haspopup'] = 'true';
      $element['#localized_options']['attributes']['aria-expanded'] = 'false';
      $element['#localized_options']['attributes']['aria-controls'] = 'menu-rand-' . rand();
    }

    $element['#attributes']['role'] = 'presentation';
    $element['#localized_options']['html'] = TRUE;
    $element['#localized_options']['attributes']['role'] = 'menuitem';
  }

  // <nolink> handling for wxt.
  if (in_array($element['#href'], array('<nolink>'))) {
    $element['#href'] = '#';
    $element['#localized_options'] = array(
      'fragment' => 'wb-tphp',
      'external' => TRUE,
    );
    $element['#localized_options']['attributes']['class'][] = 'item';
  }

  $output = l($element['#title'], $element['#href'], $element['#localized_options']);
  return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
}

/**
 * Overrides theme_menu_link() for the mid footer menu.
 */
function wetkit_bootstrap_menu_link__menu_block__mid_footer_menu(&$variables) {
  global $counter;
  global $needs_closing;

  $element = $variables['element'];
  $sub_menu = '';

  // WxT Settings.
  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $library_path = libraries_get_path($wxt_active, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active);
  $wxt_active = str_replace('theme_', '', $wxt_active);

  // <nolink> handling for wxt.
  if (in_array($element['#href'], array('<nolink>'))) {
    $element['#href'] = '#';
    $element['#localized_options'] = array(
      'fragment' => 'wb-info',
      'external' => TRUE,
    );
  }

  if ($wxt_active != 'gcweb' && $wxt_active != 'gcweb_v5') {
    if ($element['#below']) {
      if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
        $sub_menu = '<ul class="list-unstyled">' . drupal_render($element['#below']) . '</ul>';
      }
    }
    if ($element['#original_link']['depth'] == 1) {
      $output = '<h3>' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</h3>';
      return '<section class="col-sm-3">' . $output . $sub_menu . '</section>';
    }
    else {
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
      return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";
    }
  }
  else {
    if ($element['#below']) {
      if ((!empty($element['#original_link']['depth'])) && ($element['#original_link']['depth'] == 1)) {
        // Add our own wrapper.
        unset($element['#below']['#theme_wrappers']);
        $sub_menu = drupal_render($element['#below']);
      }
    }
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    return '<li' . drupal_attributes($element['#attributes']) . '>' . $output . $sub_menu . "</li>\n";

  }
}

/**
 * Overrides theme_menu_link() for book module.
 */
function wetkit_bootstrap_menu_link__book_toc(array $variables) {
  $element = $variables['element'];
  $sub_menu = drupal_render($element['#below']);

  $title = $element['#title'];
  $href = $element['#href'];
  $options = !empty($element['#localized_options']) ? $element['#localized_options'] : array();
  $attributes = !empty($element['#attributes']) ? $element['#attributes'] : array();
  $attributes['role'] = 'presentation';

  // Header.
  $link = TRUE;
  if ($title && $href === FALSE) {
    $attributes['class'][] = 'dropdown-header';
    $link = FALSE;
  }
  // Divider.
  elseif ($title === FALSE && $href === FALSE) {
    $attributes['class'][] = 'divider';
    $link = FALSE;
  }
  // Active.
  elseif (($href == $_GET['q'] || ($href == '<front>' && drupal_is_front_page())) && (empty($options['language']))) {
    $attributes['class'][] = 'active';
  }

  // Convert to a link.
  if ($link) {
    $title = l($title, $href, $options);
  }

  // Otherwise, filter the title if "html" is not set, otherwise l() will automatically
  // sanitize using check_plain(), so no need to call that here.
  elseif (empty($options['html'])) {
    $title = filter_xss_admin($title);
  }

  return '<li' . drupal_attributes($attributes) . '>' . $title . $sub_menu . "</li>\n";
}
