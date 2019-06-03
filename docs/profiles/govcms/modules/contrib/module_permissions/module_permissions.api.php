<?php

/**
 * @file
 * Hooks provided by the module_permissions API.
 */

/**
 * Allows modules to deny or provide access for a user to modules permissions.
 *
 * Modules implementing this hook can return FALSE to provide a blanket
 * prevention for the user to perform the requested operation on the specified
 * page. If no modules implementing this hook return FALSE but at least one
 * returns TRUE, then the operation will be allowed, even for a user without
 * role based permission to perform the operation.
 *
 * If no modules return FALSE but none return TRUE either, normal permission
 * based checking will apply.
 *
 * $op - system_modules or user_admin_permissions.
 * $account - The user account whose access should be determined.
 *
 * @see module_permissions_menu_alter()
 */
function hook_module_permissions_access($op, $account) {
  // Placeholder.
}

/**
 * Allows modules to deny or provide a restrict status check callback.
 *
 * @see module_permissions_form_alter()
 */
function hook_module_permissions_restrict($op, $account) {
  // Placeholder.
}
