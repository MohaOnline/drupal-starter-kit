<?php
/**
 * @file
 *   Expose the extra tasks feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_tasks_extra_hosting_feature() {
  $features['tasks_extra'] = array(
    'title' => t('Extra tasks'),
    'description' => t("Tasks that expose additional Drush commands to Aegir's frontend."),
    'status' => HOSTING_FEATURE_ENABLED,
    'module' => 'hosting_tasks_extra',
    'group' => 'advanced',
  );
  return $features;
}

