<?php
/**
 * @file
 * Expose a hosting feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_drush_alias_hosting_feature() {
  $features['drush_alias'] = array(
    'title' => t('Show example Drush aliases'),
    'description' => t("Adds a tab to show a copy-paste able drush alias for local development."),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_drush_alias',
    'group' => 'experimental'
  );
  return $features;
}

