<?php
/**
 * @file
 *   Expose the fix-permissions feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_fix_permissions_hosting_feature() {
  $features['fix_permissions'] = array(
    'title' => t('Fix Permissions'),
    'description' => t("Task that fixes file permissions of Drupal platforms and sites."),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'fix_permissions',
    'group' => 'experimental',
    'enable' => 'hosting_fix_permission_feature_enable_callback',
  );
  return $features;
}

