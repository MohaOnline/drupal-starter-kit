<?php
/**
 * @file Register the Apache HTTPS Hosting feature.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_apache_https_hosting_feature() {
  $modules = system_rebuild_module_data();
  $features['apache_https'] = array(
    'title' => t('Apache HTTPS support'),
    'description' => $modules['hosting_apache_https']->info['description'],
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_apache_https',
    'group' => 'experimental',
  );
  return $features;
}
