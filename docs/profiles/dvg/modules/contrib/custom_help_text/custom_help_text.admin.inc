<?php

/**
 * @file
 * Custom Help Text settings file.
 */

/**
 * Settings form.
 */
function custom_help_text_settings_form($form, &$form_state) {
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Help block options',
    '#description' => t('Change the behavior of every block with a Custom Help Text or System Help text.')
  );

  $form['fieldset']['custom_help_text_fieldset'] = array(
    '#type' => 'checkbox',
    '#title' => 'Wrap every help block with a fieldset',
    '#default_value' => variable_get('custom_help_text_fieldset', FALSE),
  );

  $form['fieldset']['custom_help_text_collapsible'] = array(
    '#type' => 'checkbox',
    '#title' => 'Make the help fieldset collapsible',
    '#default_value' => variable_get('custom_help_text_collapsible', FALSE),
    '#states' => array(
      'enabled' => array(
        ':input[name="custom_help_text_fieldset"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['fieldset']['custom_help_text_collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => 'Make the help fieldset collapsed by default',
    '#default_value' => variable_get('custom_help_text_collapsed', FALSE),
    '#states' => array(
      'enabled' => array(
        ':input[name="custom_help_text_fieldset"]' => array('checked' => TRUE),
        ':input[name="custom_help_text_collapsible"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['fieldset']['custom_help_text_title'] = array(
    '#type' => 'textfield',
    '#title' => 'Title',
    '#description' => t('Set the legend of the help message fieldset.'),
    '#states' => array(
      'enabled' => array(
        ':input[name="custom_help_text_fieldset"]' => array('checked' => TRUE),
      ),
      'required' => array(
        ':input[name="custom_help_text_collapsible"]' => array('checked' => TRUE),
      )
    ),
    '#default_value' => variable_get('custom_help_text_title', ''),
    '#element_validate' => array('custom_help_text_title_validation'),
  );

  return system_settings_form($form);
}

/**
 * Validation of the custom help text settings.
 */
function custom_help_text_title_validation($elements, &$form_state) {
  $values = $form_state['values'];

  if (!empty($values['custom_help_text_collapsible']) && empty($values['custom_help_text_title'])) {
    form_error($elements, t('!name field is required.', array('!name' => $elements['#title'])));
  }
}

/**
 * Restore form.
 */
function custom_help_text_restore_form($form, &$form_state) {
  $form['description'] = array(
    '#markup' => t('<h2>Restore view permissions</h2><p>Use this action with care, this can\'t be undone! After importing or updating multiple help text the \'view custom help text\' permission may not be set for certain user roles. Use this action to collect all user roles selected in the custom help texts and enable the permission for them.</p>'),
  );
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Restore \'view custom help text\' permission'),
  );

  $form['#submit'][] = 'custom_help_text_restore_form_submit';

  return $form;
}

/**
 * Submission callback from custom_help_text_restore_form().
 */
function custom_help_text_restore_form_submit($form, &$form_state) {
  $roles = array();

  $result = db_query("SELECT name, options FROM {custom_help_text}");

  foreach ($result as $record) {

    $options = unserialize($record->options);

    if (isset($options['roles'])) {
      foreach ($options['roles'] as $role_name) {
        $role = user_role_load_by_name($role_name);
        if ($role) {
          user_role_grant_permissions($role->rid, array('view custom help text'));
          $roles[$role->name] = $role->name;
        }
      }
    }
  }

  if (!empty($roles)) {
    drupal_set_message(t('The following roles can view a custom help text: @roles', array('@roles' => implode(', ', $roles))), 'warning');
  }
}
