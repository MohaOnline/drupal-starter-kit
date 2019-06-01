<?php
/**
 * @file
 * shared_content_example_destination.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function shared_content_example_destination_user_default_roles() {
  $roles = array();

  // Exported role: content deployment user.
  $roles['content deployment user'] = array(
    'name' => 'content deployment user',
    'weight' => 3,
  );

  return $roles;
}
