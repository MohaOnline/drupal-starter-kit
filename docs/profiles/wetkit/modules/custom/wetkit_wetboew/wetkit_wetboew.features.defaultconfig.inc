<?php
/**
 * @file
 * wetkit_wetboew.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function wetkit_wetboew_defaultconfig_features() {
  return array(
    'wetkit_wetboew' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}
