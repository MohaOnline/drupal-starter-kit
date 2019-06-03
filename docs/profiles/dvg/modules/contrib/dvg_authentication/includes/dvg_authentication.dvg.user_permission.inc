<?php

/**
 * @file
 * Provides permissions for dvg_default_permissions.
 */

use Drupal\dvg_authentication\AuthenticationManager;

/**
 * Implements hook_dvg_default_permissions().
 *
 * {@inheritdoc}
 */
function dvg_authentication_dvg_default_permissions() {
  $permissions = [];

  $permissions['assign external user role'] = [
    'name' => 'assign external user role',
    'roles' => [
      'administrator' => 'administrator',
    ],
    'module' => 'role_delegation',
  ];

  $permissions['access authentication administration pages'] = [
    'name' => 'access authentication administration pages',
    'roles' => [
      'administrator' => 'administrator',
    ],
    'module' => 'dvg_authentication',
  ];

  $permissions['edit webform external authentication settings'] = [
    'name' => 'edit webform external authentication settings',
    'roles' => [
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ],
    'module' => 'dvg_authentication',
  ];

  $permissions['edit authentication provider user settings'] = [
    'name' => 'edit authentication provider user settings',
    'roles' => [
      'administrator' => 'administrator',
    ],
    'module' => 'dvg_authentication',
  ];

  return $permissions;
}

/**
 * Implements hook_dvg_default_roles().
 *
 * {@inheritdoc}
 */
function dvg_authentication_dvg_default_roles() {
  $role_name = AuthenticationManager::USER_ROLE;
  $roles[$role_name] = [
    'name' => $role_name,
    'weight' => 100,
  ];

  return $roles;
}
