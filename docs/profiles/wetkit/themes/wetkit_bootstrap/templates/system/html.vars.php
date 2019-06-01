<?php
/**
 * @file
 * Stub file for "html" theme hook [pre]process functions.
 */

/**
 * Pre-processes variables for the "html" theme hook.
 *
 * See template for list of available variables.
 *
 * @see html.tpl.php
 *
 * @ingroup theme_preprocess
 */
function wetkit_bootstrap_preprocess_html(&$variables, $hook) {
  // Backport from Drupal 8 RDFa/HTML5 implementation.
  // @see https://www.drupal.org/node/1077566
  // @see https://www.drupal.org/node/1164926

  global $theme_key;
  global $language;

  // WxT Settings.
  $wxt_active_orig = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $library_path = libraries_get_path($wxt_active_orig, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active_orig);
  $wxt_active = str_replace('theme_', '', $wxt_active);

  // Return early, so the maintenance page does not call any of the code below.
  if ($hook != 'html') {
    return;
  }

  // Create a dedicated attributes array for the HTML element.
  // By default, core does not provide a way to target the HTML element.
  // The only arrays currently available technically belong to the BODY element.
  $variables['html_attributes_array'] = array(
    'lang' => $variables['language']->language,
    'dir' => $variables['language']->dir,
  );

  // Override existing RDF namespaces to use RDFa 1.1 namespace prefix bindings.
  if (function_exists('rdf_get_namespaces')) {
    $rdf = array('prefix' => array());
    foreach (rdf_get_namespaces() as $prefix => $uri) {
      $rdf['prefix'][] = $prefix . ': ' . $uri;
    }
    if (!$rdf['prefix']) {
      $rdf = array();
    }
    $variables['rdf_namespaces'] = drupal_attributes($rdf);
  }

  // Create a dedicated attributes array for the BODY element.
  if (!isset($variables['body_attributes_array'])) {
    $variables['body_attributes_array'] = array();
  }

  // Ensure there is at least a class array.
  if (!isset($variables['body_attributes_array']['class'])) {
    $variables['body_attributes_array']['class'] = array();
  }

  // Navbar position.
  switch (bootstrap_setting('navbar_position')) {
    case 'fixed-top':
      $variables['body_attributes_array']['class'][] = 'navbar-is-fixed-top';
      break;

    case 'fixed-bottom':
      $variables['body_attributes_array']['class'][] = 'navbar-is-fixed-bottom';
      break;

    case 'static-top':
      $variables['body_attributes_array']['class'][] = 'navbar-is-static-top';
      break;
  }

  // Add the default body id needed
  // WetKit Layouts may have already set this variable.
  if (empty($variables['wetkit_col_array'])) {
    $variables['wetkit_col_array'] = 'wb-body';
  }

  // Add a body class for the active theme name.
  $variables['body_attributes_array']['class'][] = drupal_html_class($theme_key);

  // Add legacy WxT class for v5 spec.
  if ($wxt_active == "gcweb_v5") {
    $variables['body_attributes_array']['class'][] = 'theme-gcweb';
  }

  // Add the active WxT theme into body class.
  $variables['body_attributes_array']['class'][] = drupal_html_class($wxt_active_orig);

  // Assign skip link variables.
  $variables['wetkit_skip_link_id_1'] = theme_get_setting('wetkit_skip_link_id_1');
  $variables['wetkit_skip_link_text_1'] = t('Skip to main content');
  $variables['wetkit_skip_link_id_2'] = theme_get_setting('wetkit_skip_link_id_2');
  $variables['wetkit_skip_link_text_2'] = t('Skip to footer');

  // Splash Page.
  if (current_path() == 'splashify-splash') {
    if ($wxt_active == 'gcweb' || $wxt_active == 'gcweb_v5') {
      $variables['body_attributes_array']['class'][] = 'splash';
    }
    else {
      $variables['body_attributes_array']['class'][] = 'wb-sppe';
    }
  }
  if (($wxt_active == 'gcweb' || $wxt_active == 'gcweb_v5') && drupal_is_front_page()) {
    $variables['body_attributes_array']['class'][] = 'home';
  }
}
