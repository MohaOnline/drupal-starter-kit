<?php

/**
 * Implements hook_drush_command().
 */
function dvg_roles_and_permissions_drush_command() {
  $items['dvg-permissions-status'] = array(
    'callback' => 'dvg_roles_and_permissions_drush_dvg_permissions_status',
    'description' => 'Shows the status of the DVG roles and permissions.',
    'arguments' => array(
    ),
    'examples' => array(
      '$ drush dvg-permissions-status' => 'See status of DVG roles and permissions.',
    ),
  );
  $items['dvg-permissions-update'] = array(
    'callback' => 'dvg_roles_and_permissions_drush_dvg_permissions_update',
    'description' => 'Updates the incorrect DVG roles and permissions.',
    'arguments' => array(
    ),
    'examples' => array(
      '$ drush dvg-permissions-update' => 'Update incorrect DVG roles and permissions.',
    ),
  );
  return $items;
}

/**
 * Drush command callback for 'drush dvg-permissions-status'.
 */
function dvg_roles_and_permissions_drush_dvg_permissions_status() {
  $incorrect_roles = _dvg_roles_and_permissions_missing_user_roles();
  $incorrect_permissions = _dvg_roles_and_permissions_incorrect_user_role_permissions();
  if (empty($incorrect_permissions) && empty($incorrect_roles)) {
    drush_print(dt('All roles and permissions are up to date'));

  }
  else {
    $role_names = _dvg_roles_and_permissions_user_role_names();

    foreach ($incorrect_roles as $role) {
      drush_print(dt('Role (@role) does not exist.', array('@role' => $role)));
    }
    foreach ($role_names as $rid => $role) {
      if (isset($incorrect_permissions[$rid])) {
        foreach ($incorrect_permissions[$rid] as $permission => $status) {
          if ($status == DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_OFF) {
            $status_name = dt('off');
          }
          elseif ($status == DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_ON) {
            $status_name = dt('on');
          }
          else {
            $status_name = dt('unknown');
          }
          drush_print(dt('Permission (@permission) for role (@role) should be @status.', array(
            '@permission' => $permission,
            '@role' => $role,
            '@status' => $status_name,
          )));
        }
      }
    }
  }
}

/**
 * Drush command callback for 'drush dvg-permissions-update'.
 */
function dvg_roles_and_permissions_drush_dvg_permissions_update() {
  $incorrect_roles = _dvg_roles_and_permissions_missing_user_roles();
  $incorrect_permissions = _dvg_roles_and_permissions_incorrect_user_role_permissions();
  if (empty($incorrect_permissions) && empty($incorrect_roles)) {
    drush_print(dt('All roles and permissions are up to date'));

  }
  else {
    $role_names = _dvg_roles_and_permissions_user_role_names();
    dvg_roles_and_permissions_set_roles();
    foreach ($incorrect_roles as $role) {
      drush_print(dt('Role (@role) created', array('@role' => $role)));
    }
    foreach ($role_names as $rid => $role) {
      if (isset($incorrect_permissions[$rid])) {
        foreach ($incorrect_permissions[$rid] as $permission => $status) {
          if ($status == DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_OFF) {
            user_role_revoke_permissions($rid, array($permission));
            $status = dt('off');
          }
          elseif ($status == DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_ON) {
            user_role_grant_permissions($rid, array($permission));
            $status = dt('on');
          }
          drush_print(dt('Permission (@permission) for role (@role) set to @status.', array(
            '@permission' => $permission,
            '@role' => $role,
            '@status' => $status
          )));
        }
      }
    }
  }
}

