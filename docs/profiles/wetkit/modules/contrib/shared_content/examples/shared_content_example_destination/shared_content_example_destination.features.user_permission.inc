<?php
/**
 * @file
 * shared_content_example_destination.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function shared_content_example_destination_user_default_permissions() {
  $permissions = array();

  // Exported permission: administer taxonomy.
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(
      'administrator' => 'administrator',
      'content deployment user' => 'content deployment user',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: bypass file access.
  $permissions['bypass file access'] = array(
    'name' => 'bypass file access',
    'roles' => array(
      'administrator' => 'administrator',
      'content deployment user' => 'content deployment user',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: bypass node access.
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(
      'administrator' => 'administrator',
      'content deployment user' => 'content deployment user',
    ),
    'module' => 'node',
  );

  return $permissions;
}
