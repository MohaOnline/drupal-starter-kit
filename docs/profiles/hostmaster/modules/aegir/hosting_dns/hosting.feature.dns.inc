<?php
/**
 * @file
 * Expose the dns feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_dns_hosting_feature() {
  $features['dns'] = array(
    'title' => t('DNS support'),
    'description' => t('Manage DNS records for your hosted sites. WARNING: This module requires additional server configuration, please refer to the <a href="@readme">README file</a>.', array('@readme' => url('http://cgit.drupalcode.org/hosting_dns/tree/README.md'))),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_dns',
    'group' => 'experimental',
    'role_permissions' => array(
      'aegir administrator' => array(
        'create dns-deploy task',
      ),
    ),
  );
  return $features;
}
