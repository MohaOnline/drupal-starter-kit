<?php
/**
 * @file
 * dvg_ct_plan.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_plan_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'add terms in spatial_plan'.
  $permissions['add terms in spatial_plan'] = array(
    'name' => 'add terms in spatial_plan',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'create plan content'.
  $permissions['create plan content'] = array(
    'name' => 'create plan content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any plan content'.
  $permissions['delete any plan content'] = array(
    'name' => 'delete any plan content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own plan content'.
  $permissions['delete own plan content'] = array(
    'name' => 'delete own plan content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in spatial_plan'.
  $permissions['delete terms in spatial_plan'] = array(
    'name' => 'delete terms in spatial_plan',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit any plan content'.
  $permissions['edit any plan content'] = array(
    'name' => 'edit any plan content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own plan content'.
  $permissions['edit own plan content'] = array(
    'name' => 'edit own plan content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in spatial_plan'.
  $permissions['edit terms in spatial_plan'] = array(
    'name' => 'edit terms in spatial_plan',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'view term page in spatial_plan'.
  $permissions['view term page in spatial_plan'] = array(
    'name' => 'view term page in spatial_plan',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  return $permissions;
}
