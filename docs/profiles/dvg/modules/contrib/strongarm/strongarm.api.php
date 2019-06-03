<?php

/**
 * @file
 * Strongarm API functions.
 */

/**
 * Alter the value of variable when it is being exported.
 *
 * @param mixed $value
 *   The value being exported.
 * @param string $name
 *   The name of the variable being exported
 */
function hook_strongarm_export_value_alter(&$value, $name) {
  if ('user_admin_role' == $name) {
    $role = user_role_load($value);
    $value = $role->name;
  }
}

/**
 * Alter the value of variable when it is being imported.
 *
 * @param mixed $value
 *   The value being exported.
 * @param string $name
 *   The name of the variable being exported
 */
function hook_strongarm_import_value_alter(&$value, $name) {
  if ('user_admin_role' == $name) {
    $role = user_role_load_by_name($value);
    $value = NULL;
    if ($role) {
      $value = $role->rid;
    }
  }
}
