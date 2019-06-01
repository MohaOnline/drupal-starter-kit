<?php
/**
 * @file
 * maintenance-page.vars.php
 */

/**
 * Implements hook_preprocess_maintenance_page().
 *
 * @see maintenance-page.tpl.php
 */
function wetkit_bootstrap_preprocess_maintenance_page(&$variables) {
  global $conf;
  $db_down = FALSE;

  // WxT Settings.
  $wxt_active = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');
  $library_path = libraries_get_path($wxt_active, TRUE);
  $wxt_active = str_replace('-', '_', $wxt_active);
  $wxt_active = str_replace('theme_', '', $wxt_active);

  // Fluid container.
  if(bootstrap_setting('fluid_container') == 1) {
    $variables['container_class'] = 'container-fluid';
  }
  else {
    $variables['container_class'] = 'container';
  }

  // Dead databases will show error messages so supplying this template will
  // allow themers to override the page and the content completely
  // also aware db_is_active is deprecated but still used in theme.inc for D7.
  if (isset($variables['db_is_active']) && !$variables['db_is_active'] && MAINTENANCE_MODE != 'update') {
    $db_down = TRUE;
  }

  // Might want to leverage this variable in tpl files.
  $variables['db_down'] = $db_down;

  // Maintenance Page logic for when the site is in maintenance mode
  // and the database is down / not available in this instance we
  // will have to define our own translations.
  if ($db_down) {
    // Can override these values via $conf in settings.php
    $variables['head_title'] = isset($conf['head_title']) ? $conf['head_title'] : 'Site not available / Site non disponible';
    $variables['site_name'] = isset($conf['site_name']) ? $conf['site_name'] : 'Site not available / Site non disponible';
    $variables['wxt_title_en'] = isset($conf['wxt_title_en']) ? $conf['wxt_title_en'] : 'Site not available';
    $variables['wxt_content_en'] = isset($conf['wxt_content_en']) ? $conf['wxt_content_en'] : 'The site is currently not available due to technical problems.';
    $variables['wxt_title_fr'] = isset($conf['wxt_title_fr']) ? $conf['wxt_title_fr'] : 'Site non disponible';
    $variables['wxt_content_fr'] = isset($conf['wxt_content_fr']) ? $conf['wxt_content_fr'] : 'Le site n\'est pas disponible actuellement en raison de problèmes techniques.';

    // Since database is down determine which theme to use based on
    // conf variable in settings.php else revert to using the WxT theme.
    $variables['theme_hook_suggestion'] = isset($conf['maintenance_theme_suggestion']) ? $conf['maintenance_theme_suggestion'] : 'maintenance_page__wet_boew';
  }

  // Maintenance Page logic for when the site is in maintenance mode.
  // but database is not down.
  if (!$db_down) {
    if ($wxt_active == 'base') {
      $variables['theme_hook_suggestion'] = 'maintenance_page__' . $wxt_active;
    }
    elseif ($wxt_active == 'gc_intranet') {
      $variables['theme_hook_suggestion'] = 'maintenance_page__' . $wxt_active;
    }
    elseif ($wxt_active == 'gcweb') {
      $variables['theme_hook_suggestion'] = 'maintenance_page__' . $wxt_active;
    }
    elseif ($wxt_active == 'gcweb_v5') {
      $variables['theme_hook_suggestion'] = 'maintenance_page__' . $wxt_active;
    }
    elseif ($wxt_active == 'gcwu_fegc') {
      $variables['theme_hook_suggestion'] = 'maintenance_page__' . $wxt_active;
    }
    elseif ($wxt_active == 'ogpl') {
      $variables['theme_hook_suggestion'] = 'maintenance_page__' . $wxt_active;
    }
    else {
      $variables['theme_hook_suggestion'] = 'maintenance_page__wet_boew';
    }
  }
}
