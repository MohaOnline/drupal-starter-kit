<?php
/**
 * @file
 * dvg_ct_task.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_ct_task_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'add terms in appointment_button_title'.
  $permissions['add terms in appointment_button_title'] = array(
    'name' => 'add terms in appointment_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in owms_audience'.
  $permissions['add terms in owms_audience'] = array(
    'name' => 'add terms in owms_audience',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in owms_authority'.
  $permissions['add terms in owms_authority'] = array(
    'name' => 'add terms in owms_authority',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in owms_uniform_product_name'.
  $permissions['add terms in owms_uniform_product_name'] = array(
    'name' => 'add terms in owms_uniform_product_name',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'add terms in webform_button_title'.
  $permissions['add terms in webform_button_title'] = array(
    'name' => 'add terms in webform_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy_access_fix',
  );

  // Exported permission: 'create task content'.
  $permissions['create task content'] = array(
    'name' => 'create task content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any task content'.
  $permissions['delete any task content'] = array(
    'name' => 'delete any task content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own task content'.
  $permissions['delete own task content'] = array(
    'name' => 'delete own task content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in appointment_button_title'.
  $permissions['delete terms in appointment_button_title'] = array(
    'name' => 'delete terms in appointment_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in owms_audience'.
  $permissions['delete terms in owms_audience'] = array(
    'name' => 'delete terms in owms_audience',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in owms_authority'.
  $permissions['delete terms in owms_authority'] = array(
    'name' => 'delete terms in owms_authority',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in owms_uniform_product_name'.
  $permissions['delete terms in owms_uniform_product_name'] = array(
    'name' => 'delete terms in owms_uniform_product_name',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in webform_button_title'.
  $permissions['delete terms in webform_button_title'] = array(
    'name' => 'delete terms in webform_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'dvg_ct_task reference webforms'.
  $permissions['dvg_ct_task reference webforms'] = array(
    'name' => 'dvg_ct_task reference webforms',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_ct_task',
  );

  // Exported permission: 'edit any task content'.
  $permissions['edit any task content'] = array(
    'name' => 'edit any task content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own task content'.
  $permissions['edit own task content'] = array(
    'name' => 'edit own task content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in appointment_button_title'.
  $permissions['edit terms in appointment_button_title'] = array(
    'name' => 'edit terms in appointment_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in owms_audience'.
  $permissions['edit terms in owms_audience'] = array(
    'name' => 'edit terms in owms_audience',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in owms_authority'.
  $permissions['edit terms in owms_authority'] = array(
    'name' => 'edit terms in owms_authority',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in owms_uniform_product_name'.
  $permissions['edit terms in owms_uniform_product_name'] = array(
    'name' => 'edit terms in owms_uniform_product_name',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in webform_button_title'.
  $permissions['edit terms in webform_button_title'] = array(
    'name' => 'edit terms in webform_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'view term page in appointment_button_title'.
  $permissions['view term page in appointment_button_title'] = array(
    'name' => 'view term page in appointment_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  // Exported permission: 'view term page in owms_audience'.
  $permissions['view term page in owms_audience'] = array(
    'name' => 'view term page in owms_audience',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  // Exported permission: 'view term page in owms_authority'.
  $permissions['view term page in owms_authority'] = array(
    'name' => 'view term page in owms_authority',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  // Exported permission: 'view term page in owms_uniform_product_name'.
  $permissions['view term page in owms_uniform_product_name'] = array(
    'name' => 'view term page in owms_uniform_product_name',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  // Exported permission: 'view term page in webform_button_title'.
  $permissions['view term page in webform_button_title'] = array(
    'name' => 'view term page in webform_button_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  return $permissions;
}
