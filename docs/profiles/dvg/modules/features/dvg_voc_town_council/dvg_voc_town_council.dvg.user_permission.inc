<?php
/**
 * @file
 * dvg_voc_town_council.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_voc_town_council_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'add terms in town_council'.
  $permissions['add terms in town_council'] = array(
    'name' => 'add terms in town_council',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'delete terms in town_council'.
  $permissions['delete terms in town_council'] = array(
    'name' => 'delete terms in town_council',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in town_council'.
  $permissions['edit terms in town_council'] = array(
    'name' => 'edit terms in town_council',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'view term page in town_council'.
  $permissions['view term page in town_council'] = array(
    'name' => 'view term page in town_council',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  return $permissions;
}
