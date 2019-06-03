<?php

/**
 * @file
 * Module Permissions admin paths.
 */

/**
 * Menu callback; provides managed module config interface.
 *
 * @ingroup forms
 */
function module_permissions_ui_config($form, $form_state = array()) {
  $form['module_permissions_ui_exclude_core'] = array(
    '#type' => 'radios',
    '#title' => t('Exclude core modules'),
    '#default_value' => variable_get('module_permissions_ui_exclude_core', 1),
    '#options' => array(0 => 'No', 1 => 'Yes'),
    '#description' => t('Exclude core modules from managed modules list. Warning: for security reasons, it is advisable to always exclude core modules from the  managed module list.'),
  );
  return system_settings_form($form);
}
