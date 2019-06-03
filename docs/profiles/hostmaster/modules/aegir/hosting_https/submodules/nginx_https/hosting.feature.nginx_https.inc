<?php
/**
 * @file Register the NGINX HTTPS Hosting feature.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_nginx_https_hosting_feature() {
  $modules = system_rebuild_module_data();
  $features['nginx_https'] = array(
    'title' => t('Nginx HTTPS support'),
    'description' => $modules['hosting_nginx_https']->info['description'],
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_nginx_https',
    'group' => 'experimental',
  );
  return $features;
}
