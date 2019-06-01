<?php

/**
 * @file
 * Main template file for the WetKit Ember theme.
 */


/**
 * Override or insert variables into the page template.
 */
function wetkit_ember_preprocess_page(&$variables) {
  if (module_exists('wetkit_language')) {
    $variables['lang_bar'] = '<div class="breadcrumb-side"><ul><li>' . $variables['menu_lang_bar'] . '</li></ul></div>';
  }
}

/**
 * Override or insert variables into the html template.
 */
function wetkit_ember_preprocess_html(&$variables) {
  // Add conditional CSS for IE8 and below.
  drupal_add_css(path_to_theme() . '/css/ie/ie.css',
    array(
      'group' => CSS_THEME,
      'browsers' => array(
        'IE' => 'lte IE 9',
        '!IE' => FALSE,
      ),
      'weight' => 999,
      'preprocess' => FALSE,
    ));

  // Add conditional CSS for IE7 and below.
  drupal_add_css(path_to_theme() . '/css/ie/ie7.css',
    array(
      'group' => CSS_THEME,
      'browsers' => array(
        'IE' => 'lte IE 7',
        '!IE' => FALSE,
      ),
      'weight' => 999,
      'preprocess' => FALSE,
    ));

  // Add conditional CSS for IE6.
  drupal_add_css(path_to_theme() . '/css/ie/ie6.css',
    array(
      'group' => CSS_THEME,
      'browsers' => array(
        'IE' => 'lte IE 6',
        '!IE' => FALSE,
      ),
      'weight' => 999,
      'preprocess' => FALSE,
    ));
}

/**
 * Modify the menu local tasks to include language switcher.
 */
function wetkit_ember_menu_local_tasks(&$variables) {
  $output = '';
  $lang_bar = '';

  if (!empty($variables['primary'])) {
    if (!is_array($variables['primary'])) {
      $variables['primary'] = array();
    }

    $variables['primary']['#prefix'] = '<h2 class="element-invisible">' . t('Primary tabs') . '</h2>' . '<ul class="tabs primary">';
    $variables['primary']['#suffix'] = '</ul>';
    $output .= drupal_render($variables['primary']);
  }
  if (!empty($variables['secondary'])) {
    if (module_exists('wetkit_language')) {
      $lang_bar = '<li class="language-toggle">' . _wetkit_language_lang_switcher() . '</li>';
    }

    if (!is_array($variables['secondary'])) {
      $variables['secondary'] = array();
    }

    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>' . '<ul class="tabs secondary">';
    $variables['secondary']['#suffix'] =  $lang_bar . '</ul>';
    $output .= drupal_render($variables['secondary']);
  }
  else {
    if (module_exists('wetkit_language')) {
      $lang_bar = '<li class="language-toggle">' . _wetkit_language_lang_switcher() . '</li>';
    }

    if (!is_array($variables['secondary'])) {
      $variables['secondary'] = array();
    }

    $variables['secondary']['#prefix'] = '<h2 class="element-invisible">' . t('Secondary tabs') . '</h2>' . '<ul class="tabs secondary">';
    $variables['secondary']['#suffix'] = $lang_bar . '</ul>';
    $output .= drupal_render($variables['secondary']);
  }

  return $output;
}
