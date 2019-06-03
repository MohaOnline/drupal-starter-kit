<?php
/**
 * @file
 * pushtape_admin.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function pushtape_admin_user_default_roles() {
  $roles = array();

  // Exported role: artist.
  $roles['artist'] = array(
    'name' => 'artist',
    'weight' => 3,
  );

  return $roles;
}
