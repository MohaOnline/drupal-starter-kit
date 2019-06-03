<?php

/**
 * @file
 * The hosting feature definition for hosting_logs_git.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return array
 *   associative array indexed by feature key.
 */
function hosting_logs_git_hosting_feature() {
  $features['logs_git'] = array(
    // Title to display in form.
    'title' => t('Git commit logs integration'),
    // Description.
    'description' => t('Provides a sites Git commit logs to hostmaster front end.'),
    // Initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // Module to enable/disable alongside feature.
    'module' => 'hosting_logs_git',
    // Associate with a specific node type.
    // 'node' => 'nodetype',
    // Which group to display in ( null , experimental )
    'group' => 'experimental',
    'role_permissions' => array(
      'aegir platform manager' => array(
        'access hosting logs git',
      ),
      'aegir account manager' => array(
        'access hosting logs git',
      ),
    ),
  );

  return $features;
}
