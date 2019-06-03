<?php

/**
 * @file
 * Expose the self_signed feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 *
 * Register the self_signed hosting feature with Aegir, initially this feature
 * will be disabled.
 */
function hosting_self_signed_hosting_feature() {
  $modules = system_rebuild_module_data();
  $features['self_signed'] = array(
    'title' => t('Self-signed service'),
    'description' => $modules['hosting_self_signed']->info['description'],
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_self_signed',
    'group' => 'experimental'
    );
  return $features;
}
