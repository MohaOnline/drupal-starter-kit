<?php

/**
 * @file
 * Functionality to clear cache for Navbar Flush Cache.
 */

/**
 * Flush all caches or a specific one.
 *
 * @param string $name
 *   (optional) Name of cache to flush.
 */
function wetkit_navbar_flush_cache($name = NULL) {
  if (isset($name)) {
    $caches = module_invoke_all('wetkit_navbar_cache_info');
    if (!isset($caches[$name])) {
      return MENU_NOT_FOUND;
    }
  }
  else {
    $caches[$name] = array(
      'title' => t('Every'),
      'callback' => 'drupal_flush_all_caches',
    );
  }
  // Pass the cache to flush forward to the callback.
  $function = $caches[$name]['callback'];
  $function($name);

  drupal_set_message(t('!title cache cleared.', array('!title' => $caches[$name]['title'])));

  // The JavaScript injects a destination request parameter pointing to the
  // originating page, so the user is redirected back to that page. Without
  // destination parameter, the redirect ends on the front page.
  $root = trim(url(), '/ ');
  if(!empty($root) && isset($_GET['destination'])) {
    $dest = trim($_GET['destination'], '/ ');
    if(strpos($dest, $root) === 0) {
      $dest = substr($dest, strlen($root)+1);
      unset($_GET['destination']);
      drupal_goto($dest);
    }
  }
  drupal_goto();
}

/**
 * Implements hook_wetkit_navbar_cache_info().
 *
 * Performed on behalf of the System module
 */
function system_wetkit_navbar_cache_info() {
  $caches = array(
    'assets' => t('CSS and JavaScript'),
    'cache' => t('Page and else'),
    'menu' => t('Menu'),
    'registry' => t('Class registry'),
    'theme' => t('Theme registry'),
  );
  foreach ($caches as $name => $cache) {
    $caches[$name] = array(
      'title' => $cache,
      'callback' => '_wetkit_navbar_flush_cache',
    );
  }
  return $caches;
}

/**
 * Implements hook_wetkit_navbar_cache_info().
 *
 * Performed on behalf of Update module
 */
function update_wetkit_navbar_cache_info() {
  $caches['update'] = array(
    'title' => t('Update data'),
    'callback' => '_update_cache_clear',
  );
  return $caches;
}

/**
 * Flush all caches or a specific one.
 *
 * @param string $name
 *   (optional) Name of cache to flush.
 *
 * @see system_wetkit_navbar_cache_info()
 */
function _wetkit_navbar_flush_cache($name = NULL) {
  switch ($name) {
    case 'menu':
      menu_rebuild();
      break;

    case 'registry':
      registry_rebuild();
      // Fall-through to clear cache tables, since registry information is
      // usually the base for other data that is cached (e.g. SimpleTests).
    case 'cache':
      // Don't clear cache_form - in-progress form submissions may break.
      // Ordered so clearing the page cache will always be the last action.
      // @see drupal_flush_all_caches()
      $core = array('cache', 'cache_bootstrap', 'cache_filter', 'cache_page');
      $cache_tables = array_merge(module_invoke_all('flush_caches'), $core);
      foreach ($cache_tables as $table) {
        cache_clear_all('*', $table, TRUE);
      }
      break;

    case 'assets':
      // Change query-strings on css/js files to enforce reload for all users.
      _drupal_flush_css_js();

      drupal_clear_css_cache();
      drupal_clear_js_cache();

      // Clear the page cache, since cached HTML pages might link to old CSS and
      // JS aggregates.
      cache_clear_all('*', 'cache_page', TRUE);
      break;

    case 'theme':
      system_rebuild_theme_data();
      drupal_theme_rebuild();
      break;
  }
}
