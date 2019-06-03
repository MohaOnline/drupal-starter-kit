<?php

/**
 * Implements hook_hosting_feature().
 */
function hosting_remote_import_hosting_feature() {
  $features['remote_import'] = array(
    // title to display in form
    'title' => t('Remote import'),
    // description
    'description' => t('Allows you to import sites from remote Aegir master servers.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_remote_import',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'advanced',
  );
  return $features;
}
