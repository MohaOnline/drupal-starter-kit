<?php

/**
 * @file
 * Administration functions for the dvg_roles_and_permissions module.
 */


define('DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_OFF', 0);
define('DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_ON', 1);

/**
 * Helper function that returns all default user roles.
 */
function _dvg_roles_and_permissions_user_roles() {
  $roles = array();
  foreach (module_implements('dvg_default_roles') as $module) {
    $module_roles = module_invoke($module, 'dvg_default_roles');
    foreach ($module_roles as $key => $value) {
      if (!isset($roles[$key])) {
        $roles[$key] = $value;
      }
    }
  }
  foreach (module_implements('dvg_default_roles_alter') as $module) {
    $function = $module . '_dvg_default_roles_alter';
    // will call all modules implementing hook_hook_name
    // and can pass each argument as reference determined
    // by the function declaration
    $function($roles);
  }
  // TODO: Sort by weight
  return $roles;
}

/**
 * Helper function that returns all default user role names.
 */
function _dvg_roles_and_permissions_user_role_names() {
  $dvg_user_roles = _dvg_roles_and_permissions_user_roles();
  $role_names = array();
  foreach (user_roles() as $rid => $name) {
    if (isset($dvg_user_roles[$name])) {
      $role_names[$rid] = $name;
    }
  }
  return $role_names;
}

/**
 * Helper function that returns all default permissions.
 */
function _dvg_roles_and_permissions_user_role_permissions($module = NULL) {
  $user_role_permissions = array();
  if (isset($module)) {
    $modules = array($module);
  }
  else {
    $modules = module_implements('dvg_default_permissions');
  }
  foreach ($modules as $implementing_module) {
    $module_user_role_permissions = module_invoke($implementing_module, 'dvg_default_permissions');
    foreach ($module_user_role_permissions as $permission_key => $permission_value) {
      if ($permission_value['module'] == 'taxonomy') {
        _user_features_change_term_permission($permission_key, 'machine_name');
      }
      if (!isset($user_role_permissions[$permission_value['module']])) {
        $user_role_permissions[$permission_value['module']] = array();
      }
      if (!isset($user_role_permissions[$permission_value['module']][$permission_key])) {
        $user_role_permissions[$permission_value['module']][$permission_key] = $permission_value;
      }
      else {
        foreach ($permission_value['roles'] as $role_key => $role_value) {
          $user_role_permissions[$permission_value['module']][$permission_key]['roles'][$role_key] = $role_value;
        }
      }
    }
  }
  $role_names = _dvg_roles_and_permissions_user_role_names();
  foreach (module_implements('dvg_default_permissions_alter') as $module) {
    $function = $module . '_dvg_default_permissions_alter';
    // will call all modules implementing hook_hook_name
    // and can pass each argument as reference determined
    // by the function declaration
    $function($user_role_permissions, $role_names);
  }
  return $user_role_permissions;
}

/**
 * Menu callback: administer permissions.
 *
 * @ingroup forms
 * @see theme_dvg_roles_and_permissions_admin_permissions()
 */
function dvg_roles_and_permissions_admin_permissions($form, $form_state) {
  foreach (_dvg_roles_and_permissions_missing_user_roles() as $role) {
    drupal_set_message(t('Role (@role) does not exist.', array('@role' => $role)), 'error');
  }
  // Retrieve role names for columns.
  $role_names = _dvg_roles_and_permissions_user_role_names();
  $user_role_permissions = _dvg_roles_and_permissions_user_role_permissions();
  // Fetch permissions for all roles or the one selected role.
  $role_permissions = user_role_permissions($role_names);
  // Store $role_names for use when saving the data.
  $form['role_names'] = array(
    '#type' => 'value',
    '#value' => $role_names,
  );
  // Render role/permission overview:
  $options = array();
  $module_info = system_get_info('module');
  $hide_descriptions = system_admin_compact_mode();

  // Get a list of all the modules implementing a hook_permission() and sort by
  // display name.
  $modules = array();
  foreach (module_implements('permission') as $module) {
    $modules[$module] = $module_info[$module]['name'];
  }
  asort($modules);
  foreach ($modules as $module => $display_name) {
    if (isset($user_role_permissions[$module])) {
      $permissions = module_invoke($module, 'permission');
      foreach ($user_role_permissions[$module] as $user_role_perm => $user_role_perm_item) {
        if (!isset($permissions[$user_role_perm])) {
          drupal_set_message(t('Permission (@permission) is not a module (@module) permission.', array('@permission' => $user_role_perm, '@module' => $module)), 'error');
        }
      }

      if ($permissions) {
        $form['permission'][] = array(
          '#markup' => $module_info[$module]['name'],
          '#id' => $module,
        );
        foreach ($permissions as $perm => $perm_item) {
          if (isset($user_role_permissions[$module][$perm])) {
            // Fill in default values for the permission.
            $perm_item += array(
              'description' => '',
              'restrict access' => FALSE,
              'warning' => !empty($perm_item['restrict access']) ? t('Warning: Give to trusted roles only; this permission has security implications.') : '',
            );
            $options[$perm] = '';
            $form['permission'][$perm] = array(
              '#type' => 'item',
              '#markup' => $perm_item['title'],
              '#description' => theme('dvg_roles_and_permissions_permission_description', array(
                'permission_item' => $perm_item,
                'hide' => $hide_descriptions
              )),
            );
            foreach ($role_names as $rid => $name) {
              // Builds arrays for checked boxes for each role
              if (isset($role_permissions[$rid][$perm])) {
                $current_value[$rid][$perm] = $perm;
              }
            }
          }
        }
      }
    }
  }

  // Have to build checkboxes here after checkbox arrays are built
  foreach ($role_names as $rid => $name) {
    $form['#permissions'][$rid] = array(
      '#permissions' => $options,
      '#current_values' => isset($current_value[$rid]) ? $current_value[$rid] : array(),
    );
    $form['role_names'][$rid] = array('#markup' => check_plain($name), '#tree' => TRUE);
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Update roles & permissions'));

  return $form;
}

/**
 * Returns HTML for the administer permissions page.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_dvg_roles_and_permissions_admin_permissions($variables) {
  $form = $variables['form'];
  $incorrect_permissions = _dvg_roles_and_permissions_incorrect_user_role_permissions();
  foreach (element_children($form['permission']) as $key) {
    $row = array();
    // Module name
    if (is_numeric($key)) {
      $row[] = array('data' => drupal_render($form['permission'][$key]), 'class' => array('module'), 'id' => 'module-' . $form['permission'][$key]['#id'], 'colspan' => count($form['role_names']['#value']) + 1);
    }
    else {
      // Permission row.
      $row[] = array(
        'data' => drupal_render($form['permission'][$key]),
        'class' => array('permission'),
      );
      foreach (element_children($form['#permissions']) as $rid) {
        $current_value = isset($form['#permissions'][$rid]['#current_values'][$key]) && ($form['#permissions'][$rid]['#current_values'][$key] == $key);
        if (!isset($incorrect_permissions[$rid][$key])) {
           $data = ($current_value) ? 'V' : '-';
           $classes = array('checkbox');
        }
        elseif ($incorrect_permissions[$rid][$key] == DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_OFF) {
          $data = t('Permission should be off');
          $classes = array('checkbox', 'permission-error');
        }
        elseif ($incorrect_permissions[$rid][$key] == DVG_ROLES_AND_PERMISSIONS_ERRORCODE_SHOULD_BE_ON) {
          $data = t('Permission should be on');
          $classes = array('checkbox', 'permission-error');
        }
        else {
          $data = t('Unknown error');
          $classes = array('checkbox', 'permission-error');
        }
        $row[] = array('data' => $data, 'class' => $classes);
      }
    }
    $rows[] = $row;
  }
  $header[] = (t('Permission'));
  foreach (element_children($form['role_names']) as $rid) {
    $header[] = array('data' => drupal_render($form['role_names'][$rid]), 'class' => array('checkbox'));
  }
  $output = theme('system_compact_link');
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'permissions')));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Returns HTML for an individual permission description.
 *
 * @param $variables
 *   An associative array containing:
 *   - permission_item: An associative array representing the permission whose
 *     description is being themed. Useful keys include:
 *     - description: The text of the permission description.
 *     - warning: A security-related warning message about the permission (if
 *       there is one).
 *   - hide: A boolean indicating whether or not the permission description was
 *     requested to be hidden rather than shown.
 *
 * @ingroup themeable
 */
function theme_dvg_roles_and_permissions_permission_description($variables) {
  if (!$variables['hide']) {
    $description = array();
    $permission_item = $variables['permission_item'];
    if (!empty($permission_item['description'])) {
      $description[] = $permission_item['description'];
    }
    if (!empty($permission_item['warning'])) {
      $description[] = '<em class="permission-warning">' . $permission_item['warning'] . '</em>';
    }
    if (!empty($description)) {
      return implode(' ', $description);
    }
  }
}

/**
 * Save permissions selected on the administer permissions page.
 *
 * @see dvg_roles_and_permissions_admin_permissions()
 */
function dvg_roles_and_permissions_admin_permissions_submit($form, &$form_state) {

  dvg_roles_and_permissions_set_roles();
  dvg_roles_and_permissions_set_permissions();

  drupal_set_message(t('The changes have been saved.'));

  // Clear the cached pages and blocks.
  cache_clear_all();
}