<?php

/**
 * @file
 * Functionality related to the user settings.
 */

/**
 * User settings form.
 */
function exif_custom_settings_form_user($form, $form_state) {
  $mappings = _exif_custom_get_maps();
  $options = array();
  $options['none'] = '';
  if (!empty($mappings)) {
    while ($result = $mappings->fetchObject()) {
      $options[$result->mid] = $result->name;
    }
    $form['user-default'] = array(
      '#type' => 'select',
      '#title' => 'Personal default mapping',
      '#default_value' => exif_custom_get_user_default(),
      '#options' => $options,
    );
    $form['actions']['submit'] = array(
      '#value' => 'Save',
      '#type' => 'submit',
    );
    $form['#submit'][] = 'exif_custom_settings_form_user_submit';
  }
  else {
    drupal_set_message(t('No mappings have been created yet.'));
  }
  return $form;
}

/**
 * FormAPI submission callback for exif_custom_settings_form_user().
 */
function exif_custom_settings_form_user_submit($form, $form_state) {
  global $user;
  $mid = 0;
  if (isset($form_state['values']['user-default'])) {
    $mid = (int) $form_state['values']['user-default'];
  }
  db_merge('exif_custom_users')
    ->key(array(
      'uid' => $user->uid,
    ))
    ->fields(array(
      'mid' => $mid,
    ))
    ->execute();
}
