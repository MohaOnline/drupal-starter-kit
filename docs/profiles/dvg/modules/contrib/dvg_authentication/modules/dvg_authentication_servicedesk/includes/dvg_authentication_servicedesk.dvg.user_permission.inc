<?php

/**
 * @file
 * Provides permissions for dvg_default_permissions.
 */

/**
 * Implements hook_dvg_default_permissions().
 *
 * {@inheritdoc}
 */
function dvg_authentication_servicedesk_dvg_default_permissions() {
  $permissions = [];

  $permissions['authentication servicedesk'] = [
    'name' => 'authentication servicedesk',
    'roles' => [
      'administrator' => 'administrator',
      'servicedesk' => 'servicedesk',
    ],
    'module' => 'dvg_authentication_servicedesk',
  ];

  $permissions['assign servicedesk role'] = [
    'name' => 'Assign servicedesk role',
    'roles' => [
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ],
    'module' => 'role_delegation',
  ];

  return $permissions;
}

/**
 * Implements hook_dvg_default_roles().
 *
 * {@inheritdoc}
 */
function dvg_authentication_servicedesk_dvg_default_roles() {
  $roles['servicedesk'] = [
    'name' => 'servicedesk',
  ];

  return $roles;
}
