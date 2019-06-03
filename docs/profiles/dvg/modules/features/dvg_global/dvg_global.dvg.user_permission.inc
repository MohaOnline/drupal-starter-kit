<?php
/**
 * @file
 * dvg_global.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_global_dvg_default_permissions() {
  $permissions = array();

  $permissions['dvg_global administer functional content'] = array(
    'name' => 'dvg_global administer functional content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'dvg_global',
  );

  $permissions['dvg_global administer texts'] = array(
    'name' => 'dvg_global administer texts',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  $permissions['dvg_global administer testdata'] = array(
    'name' => 'dvg_global administer testdata',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  return $permissions;
}
