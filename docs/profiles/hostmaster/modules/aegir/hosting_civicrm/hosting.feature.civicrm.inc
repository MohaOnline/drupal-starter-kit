<?php
/**
 * @file
 *   Expose the CiviCRM feature to hostmaster.
 */

/**
 * Implementation of hook_hosting_feature().
 */
function hosting_civicrm_hosting_feature() {
  $features['civicrm'] = array(
    'title' => t('CiviCRM'),
    'description' => t('Enables installation and maintenance of CiviCRM on Aegir-managed sites.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_civicrm',
    'group' => 'experimental'
    );
  return $features;
}
