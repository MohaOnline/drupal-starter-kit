<?php
/**
 * @file
 * Register this module as a Hosting feature.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_https_hosting_feature() {
  $features['https'] = array(
    'title' => t('HTTPS support'),
    'description' => t('Provides a service to expose a site encrypted by HTTPS certificates.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_https',
    'group' => 'optional',
    'role_permissions' => array(
      'aegir client' => array(
        'manage site HTTPS settings'
      ),
    ),
    'enable' => 'hosting_https_feature_enable_callback',
  );
  return $features;
}
