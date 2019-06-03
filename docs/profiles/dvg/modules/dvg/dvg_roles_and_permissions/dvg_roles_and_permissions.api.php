<?php

/**
 * @file
 * Hooks provided by the Roles And Permissions API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Set dvg default permissions.
 *
 * Modules may implement this hook to add dvg default permissions.
 * 
 * @return array
 *  An associative array containing permissions and the user roles assigned to the permission.
 *
 */
function hook_dvg_default_permissions() {
  $permissions = array();

  $permissions['access domain settings form'] = array(
    'name' => 'access domain settings form',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'domain_settings',
  );
  return $permissions;
}

/**
 * Alter the dvg default permissions.
 *
 * Modules may implement this hook to alter the dvg default permissions.
 *
 * @param &$user_role_permissions
 *  By reference. The set of dvg default permissions that is currently set.
 * @param $role_names
 *  The set of dvg default user role names that is currently set.
 * 
 * @see hook_dvg_default_permissions()
 */
function hook_dvg_default_permissions_alter(&$user_role_permissions, $role_names) {
  // Disable enabled basic create/edit/delete permissions for all roles and enable it on assigned domains only
  foreach ($user_role_permissions as $module => $permissions) {
    foreach ($permissions as $name => $permission) {
      $matches = array();
      if (preg_match('/^(create|edit|delete) (.*?) content/', $name, $matches)) {
        $op = ($matches[1] == 'edit') ? 'update' : $matches[1];
        $type_info = explode(' ', $matches[2], 2);

        foreach ($role_names as $rid => $role_name) {
          if (!in_array($role_name, array('anonymous user', 'authenticated user', 'administrator'))) {
            // We don't support the 'own' permission on Domains.
            if (count($type_info) == 2 && $type_info[0] == 'own') {
              unset($user_role_permissions[$module][$name]['roles'][$role_name]);
              continue;
            }

            // Grant the node type and operation specific permission.
            $type = end($type_info);

            // Skip these content types.
            if (in_array($type, variable_get('dvg_domains_type_blacklist', array('domain')))) {
              continue;
            }

            // Set or unset the Domain and default node permissions.
            if (isset($user_role_permissions[$module][$name]['roles'][$role_name])) {
              unset($user_role_permissions[$module][$name]['roles'][$role_name]);
              $domain_permission = "$op $type content on assigned domains";
              if (!isset($user_role_permissions['domain'])) {
                $user_role_permissions['domain'] = array();
              }
              if (!isset($user_role_permissions['domain'][$domain_permission])) {
                $user_role_permissions['domain'][$domain_permission] = array('roles' => array());
              }
              $user_role_permissions['domain'][$domain_permission]['roles'][$role_name] = $role_name;

            }
          }
        }
      }
    }
  }
}

/**
 * Set dvg default roles.
 *
 * Modules may implement this hook to add dvg default roles.
 *
 * @return array
 *  An associative array containing roles.
 *
 */
function hook_dvg_default_roles() {
  $roles = array();

  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 10,
  );
  return $roles;
}

/**
 * Alter the dvg default roles.
 *
 * Modules may implement this hook to alter the dvg default roles.
 *
 * @param &$roles
 *   By reference. The set of default roles that is currently set.
 *
 * @see hook_dvg_default_roles()
 */
function hook_dvg_default_roles_alter(&$roles) {
}


/**
 * Alter the dvg basic webform components settings.
 *
 * Modules may implement this hook to alter the basic webform components settings.
 *
 * @param &$basic_webform_components_settings
 *   By reference. The set of basic webform components settings that is currently set.
 */
function hook_dvg_basic_webform_components_settings_alter(&$basic_webform_components_settings) {
  unset($basic_webform_components_settings['preview']);
  unset($basic_webform_components_settings['preview|preview']);
}

/**
 * Alter the dvg basic webform configuration settings.
 *
 * Modules may implement this hook to alter the basic webform configuration settings.
 *
 * @param &$basic_webform_configuration_settings
 *   By reference. The set of basic webform configuration settings that is currently set.
 */
function hook_dvg_basic_webform_configuration_settings_alter(&$basic_webform_configuration_settings) {
  unset($basic_webform_configuration_settings['validation|message']);
}

/**
 * @} End of "addtogroup hooks".
 */
