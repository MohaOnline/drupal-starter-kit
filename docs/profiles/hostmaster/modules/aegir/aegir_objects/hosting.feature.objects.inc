<?php
/**
 * @file
 * Expose the Aegir Objects API feature to hostmaster.
 */

/**
 * Implements hook_hosting_feature().
 */
function aegir_objects_hosting_feature() {
  $features = array();

  $features['aegir_objects'] = array(
    'title' => t('Aegir Objects API'),
    'description' => t('API helpers to simplify interacting with Drupal 7 in an object-oriented fashion.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'aegir_objects',
    'group' => 'experimental',
  );

  return $features;
}
