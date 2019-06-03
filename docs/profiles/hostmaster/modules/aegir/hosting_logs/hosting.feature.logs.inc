<?php

/**
 * @file
 * The hosting feature definition for hosting_logs.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * Implements hook_hosting_feature().
 *
 * @return array
 *   associative array indexed by feature key.
 */
function hosting_logs_hosting_feature() {
  $features['logs'] = array(
    // Title to display in form.
    'title' => t('Webserver logs integration'),
    'description' => t('Provides a sites error and watchdog logs to hostmaster front end. WARNING: Exposing logs could expose sensitive information.'),
    // Initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED ).
    'status' => HOSTING_FEATURE_DISABLED,
    // Module to enable/disable alongside feature.
    'module' => 'hosting_logs',
    // Which group to display in ( null , experimental )
    'group' => 'experimental',
    'role_permissions' => array(
      'aegir platform manager' => array(
        'access hosting logs',
      ),
      'aegir account manager' => array(
        'access hosting logs',
      ),
    ),
    'enable' => 'hosting_logs_feature_enable_callback',
  );

  return $features;
}
