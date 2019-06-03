<?php
/**
 * @file
 * dvg_digid.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function dvg_digid_user_default_roles() {
  $roles = array();

  // Exported role: digid.
  $roles['digid'] = array(
    'name' => 'digid',
    'weight' => 4,
  );

  return $roles;
}
