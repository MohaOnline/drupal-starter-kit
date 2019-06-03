<?php

/**
 * @file
 * Common pages for the module.
 */

/**
 * Menu callback; provides managed module interface.
 *
 * @ingroup forms
 *
 * @see system_modules()
 */
function module_permissions_ui_form_managed_modules($form, $form_state = array()) {
  // Get current list of modules.
  $files = system_rebuild_module_data();

  // Remove hidden modules from display list.
  $visible_files = $files;
  $exclude_core = variable_get('module_permissions_ui_exclude_core', 1);

  foreach ($visible_files as $filename => $file) {
    if (!empty($file->info['hidden'])) {
      unset($visible_files[$filename]);
    }
    // // Remove core modules from display list.
    if ($exclude_core == TRUE && $file->info['package'] == 'Core') {
      unset($visible_files[$filename]);
    }
  }

  // If list is empty, return and set a message.
  if (empty($visible_files)) {
    drupal_set_message(t('Managed module list is empty.'), 'warning');

    return $form;
  }

  uasort($visible_files, 'module_permissions_sort_modules_by_info_name');

  // Remove module permissions from list.
  unset($visible_files['module_permissions']);

  // Remove module permissions ui from list.
  unset($visible_files['module_permissions_ui']);

  // Create module listing.
  $managed_modules = module_permissions_load_managed_modules();
  $form['modules'] = array('#tree' => TRUE);

  // Used when displaying modules that are required by the installation profile.
  require_once DRUPAL_ROOT . '/includes/install.inc';
  $distribution_name = check_plain(drupal_install_profile_distribution_name());

  // Iterate through each of the modules and add to form.
  foreach ($visible_files as $filename => $module) {
    // Extra module information.
    $extra = array();
    $extra['requires'] = array();
    $extra['enabled'] = (bool) $module->status;

    // Set extra information while module is required.
    if (!empty($module->info['required'])) {
      $extra['disabled'] = TRUE;
      $extra['required_by'][] = $distribution_name . (!empty($module->info['explanation']) ? ' (' . $module->info['explanation'] . ')' : '');
    }

    // If this module requires other modules, add them to the array.
    foreach ($module->requires as $requires => $v) {
      if (!isset($files[$requires])) {
        $extra['requires'][$requires] = t('@module (<span class="admin-missing">missing</span>)', array('@module' => drupal_ucfirst($requires)));
        $extra['disabled'] = TRUE;
      }
      // Only display visible modules.
      elseif (isset($visible_files[$requires])) {
        $requires_name = $files[$requires]->info['name'];
        // Disable this module if it is incompatible with the
        // dependency's version.
        if ($incompatible_version = drupal_check_incompatibility($v, str_replace(DRUPAL_CORE_COMPATIBILITY . '-', '', $files[$requires]->info['version']))) {
          $extra['requires'][$requires] = t('@module (<span class="admin-missing">incompatible with</span> version @version)', array(
            '@module' => $requires_name . $incompatible_version,
            '@version' => $files[$requires]->info['version'],
          ));
          $extra['disabled'] = TRUE;
        }
        // Disable this module if the dependency is incompatible with this
        // version of Drupal core.
        elseif ($files[$requires]->info['core'] != DRUPAL_CORE_COMPATIBILITY) {
          $extra['requires'][$requires] = t('@module (<span class="admin-missing">incompatible with</span> this version of Drupal core)', array(
            '@module' => $requires_name,
          ));
          $extra['disabled'] = TRUE;
        }
        elseif ($files[$requires]->status) {
          $extra['requires'][$requires] = t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => $requires_name));
        }
        else {
          $extra['requires'][$requires] = t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => $requires_name));
        }
      }
    }

    // Build table headers.
    $form['modules'][$module->info['package']][$filename] = array(
      '#tree' => TRUE,
      'granted' => array(
        '#type' => 'checkbox',
        '#title' => t('Granted'),
        '#default_value' => isset($managed_modules[$filename]) ? 1 : 0,
      ),
      'name' => array(
        '#markup' => $module->info['name'],
      ),
      'status' => array(
        '#markup' => $extra['enabled'] === TRUE ? t('Enabled') : t('Disabled'),
      ),
      'description' => array(
        '#markup' => t('!description', array('!description' => $module->info['description'])),
      ),
      'dependency' => array(
        '#markup' => implode(',', $extra['requires']),
      ),
    );
  }

  // Add basic information to the fieldsets.
  foreach (element_children($form['modules']) as $package) {
    $form['modules'][$package] += array(
      '#type' => 'fieldset',
      '#title' => t('!package', array('!package' => $package)),
      '#collapsible' => TRUE,
      '#theme' => 'module_permissions_ui_managed_modules_list',
      '#header' => array(
        array(
          'data' => t('Granted'),
          'class' => array('checkbox'),
        ),
        t('Name'),
        t('Status'),
        t('Description'),
        t('Dependency'),
      ),
      // Ensure that the "Core" package fieldset comes first.
      '#weight' => $package == 'Core' ? -10 : NULL,
    );
  }

  // Lastly, sort all fieldsets by title.
  uasort($form['modules'], 'element_sort_by_title');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['#submit'][] = 'module_permissions_ui_form_managed_modules_submit';

  return $form;
}

/**
 * Form submission handler for module_permissions_ui_form_managed_modules().
 */
function module_permissions_ui_form_managed_modules_submit($form, &$form_state) {
  $managed_modules = array();

  if (!empty($form_state['values']['modules'])) {
    foreach ($form_state['values']['modules'] as $modules) {
      foreach ($modules as $name => $values) {
        $modules = $values['granted'];
        if ($modules) {
          $managed_modules[] = $name;
        }
      }
    }
  }

  variable_set('module_permissions_managed_modules', $managed_modules);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Menu callback; provides protected module interface.
 *
 * @ingroup forms
 *
 * @see system_modules()
 */
function module_permissions_ui_form_protected_modules($form, $form_state = array()) {
  // Get current list of modules.
  $files = system_rebuild_module_data();

  // Remove hidden modules from display list.
  $visible_files = $files;

  // Unset hidden module.
  foreach ($visible_files as $filename => $file) {
    if (!empty($file->info['hidden'])) {
      unset($visible_files[$filename]);
    }
  }

  // If list is empty, return and set a message.
  if (empty($visible_files)) {
    drupal_set_message(t('Protected module list is empty.'), 'warning');

    return $form;
  }

  uasort($visible_files, 'module_permissions_sort_modules_by_info_name');

  // Remove module permissions from list.
  unset($visible_files['module_permissions']);

  // Remove module permissions ui from list.
  unset($visible_files['module_permissions_ui']);

  // Create module listing.
  $protected_modules = module_permissions_load_protected_modules();
  $form['modules'] = array('#tree' => TRUE);

  // Used when displaying modules that are required by the installation profile.
  require_once DRUPAL_ROOT . '/includes/install.inc';
  $distribution_name = check_plain(drupal_install_profile_distribution_name());

  // Iterate through each of the modules and add to form.
  foreach ($visible_files as $filename => $module) {
    // Extra module information.
    $extra = array();
    $extra['required_by'] = array();
    $extra['enabled'] = (bool) $module->status;

    // Set extra information while module is required.
    if (!empty($module->info['required'])) {
      $extra['disabled'] = TRUE;
      $extra['required_by'][] = $distribution_name . (!empty($module->info['explanation']) ? ' (' . $module->info['explanation'] . ')' : '');
    }

    // If this module requires other modules, add them to the array.
    foreach ($module->requires as $requires => $v) {
      if (!isset($files[$requires])) {
        $extra['requires'][$requires] = t('@module (<span class="admin-missing">missing</span>)', array('@module' => drupal_ucfirst($requires)));
        $extra['disabled'] = TRUE;
      }
      // Only display visible modules.
      elseif (isset($visible_files[$requires])) {
        $requires_name = $files[$requires]->info['name'];
        // Disable this module if it is incompatible with the
        // dependency's version.
        if ($incompatible_version = drupal_check_incompatibility($v, str_replace(DRUPAL_CORE_COMPATIBILITY . '-', '', $files[$requires]->info['version']))) {
          $extra['requires'][$requires] = t('@module (<span class="admin-missing">incompatible with</span> version @version)', array(
            '@module' => $requires_name . $incompatible_version,
            '@version' => $files[$requires]->info['version'],
          ));
          $extra['disabled'] = TRUE;
        }
        // Disable this module if the dependency is incompatible with this
        // version of Drupal core.
        elseif ($files[$requires]->info['core'] != DRUPAL_CORE_COMPATIBILITY) {
          $extra['requires'][$requires] = t('@module (<span class="admin-missing">incompatible with</span> this version of Drupal core)', array(
            '@module' => $requires_name,
          ));
          $extra['disabled'] = TRUE;
        }
        elseif ($files[$requires]->status) {
          $extra['requires'][$requires] = t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => $requires_name));
        }
        else {
          $extra['requires'][$requires] = t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => $requires_name));
        }
      }
    }

    // If this module is required by other modules, list those, and then make it
    // impossible to disable this one.
    foreach ($module->required_by as $required_by => $v) {
      // Hidden modules are unset already.
      if (isset($visible_files[$required_by])) {
        if ($files[$required_by]->status == 1 && $module->status == 1) {
          $extra['required_by'][] = t('@module (<span class="admin-enabled">enabled</span>)', array('@module' => $files[$required_by]->info['name']));
          $extra['disabled'] = TRUE;
        }
        else {
          $extra['required_by'][] = t('@module (<span class="admin-disabled">disabled</span>)', array('@module' => $files[$required_by]->info['name']));
        }
      }
    }

    // Set default value.
    $default_value = 0;
    $disabled = 0;

    // Read module declaration at first.
    if (isset($module->info['required']) && $module->info['required'] === TRUE) {
      $default_value = 1;
      $disabled = 1;
    }

    // Read protected modules declaration.
    if (isset($protected_modules[$filename])) {
      $default_value = 1;
      $disabled = 0;
    }

    // Build table headers.
    $form['modules'][$module->info['package']][$filename] = array(
      '#tree' => TRUE,
      'granted' => array(
        '#type' => 'checkbox',
        '#title' => t('Granted'),
        '#default_value' => $default_value,
        '#disabled' => $disabled,

      ),
      'name' => array(
        '#markup' => $module->info['name'],
      ),
      'status' => array(
        '#markup' => $extra['enabled'] === TRUE ? t('Enabled') : t('Disabled'),
      ),
      'description' => array(
        '#markup' => t('!description', array('!description' => $module->info['description'])),
      ),
      'dependency' => array(
        '#markup' => implode(',', $extra['required_by']),
      ),
    );
  }

  // Add basic information to the fieldsets.
  foreach (element_children($form['modules']) as $package) {
    $form['modules'][$package] += array(
      '#type' => 'fieldset',
      '#title' => t('!package', array('!package' => $package)),
      '#collapsible' => TRUE,
      '#theme' => 'module_permissions_ui_managed_modules_list',
      '#header' => array(
        array(
          'data' => t('Granted'),
          'class' => array('checkbox'),
        ),
        t('Name'),
        t('Status'),
        t('Description'),
        t('Required by'),
      ),
      // Ensure that the "Core" package fieldset comes first.
      '#weight' => $package == 'Core' ? -10 : NULL,
    );
  }

  // Lastly, sort all fieldsets by title.
  uasort($form['modules'], 'element_sort_by_title');

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['#submit'][] = 'module_permissions_ui_form_protected_modules_submit';

  return $form;
}

/**
 * Form submission handler for module_permissions_ui_form_protected_modules().
 */
function module_permissions_ui_form_protected_modules_submit($form, &$form_state) {
  $protected_modules = array();

  if (!empty($form_state['values']['modules'])) {
    foreach ($form_state['values']['modules'] as $modules) {
      foreach ($modules as $name => $values) {
        $modules = $values['granted'];
        if ($modules) {
          $protected_modules[] = $name;
        }
      }
    }
  }

  variable_set('module_permissions_protected_modules', $protected_modules);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Array sorting callback; sorts modules by their name.
 */
function module_permissions_sort_modules_by_info_name($a, $b) {
  return strcasecmp($a->info['name'], $b->info['name']);
}
