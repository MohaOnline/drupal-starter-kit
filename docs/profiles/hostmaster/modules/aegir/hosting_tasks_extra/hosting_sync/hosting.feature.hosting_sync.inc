<?php
/**
 * @file
 *   Expose the hosting sync feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_sync_hosting_feature() {
  $features['sync'] = array(
    'title' => t('Sync task'),
    'description' => t("Adds task for synchronizing data between sites."),
    'status' => HOSTING_FEATURE_ENABLED,
    'module' => 'hosting_sync',
    'group' => 'advanced',
  );
  return $features;
}

