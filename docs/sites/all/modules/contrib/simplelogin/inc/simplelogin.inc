<?php

/**
 * @file
 * The Login screens, which controls the customized background image.
 */

/**
 * Implements of background image form.
 */
function simplelogin_background_form($form, &$form_state) {
  $form['login_bgimage'] = array(
    '#type' => 'managed_file',
    '#name' => 'login_bgimage',
    '#title' => t('Image'),
    '#description' => t("Allowed extensions: gif png jpg jpeg"),
    '#default_value' => variable_get('simplelogin_fid', ''),
    '#upload_location' => 'public://simplelogin/',
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
      'file_validate_size' => array(1024 * 1024 * 1024),
      'file_validate_is_image' => array(),
    ),
    '#progress_indicator' => 'throbber',
    '#progress_message' => t('Uploading...'),
    '#theme' => 'simplelogin_imageupload',
  );
  $form['#submit'][] = 'simplelogin_background_form_submit';
  return system_settings_form($form);
}

/**
 * Implements of form submit function.
 */
function simplelogin_background_form_submit($form, &$form_state) {
  global $user;
  if (isset($form_state['values']['login_bgimage'])
    && is_numeric($form_state['values']['login_bgimage'])
    && ($form_state['values']['login_bgimage'] > 0)) {
    $file = file_load($form_state['values']['login_bgimage']);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    variable_set('simplelogin_fid', $file->fid);
    // Records that a module is using a file.
    file_usage_add($file, 'user', 'user', $user->uid);
    if ($file) {
      drupal_set_message(t('Image successfully uploaded.'), 'status');
    }
    else {
      form_set_error('image', t('Image was not uploaded.'));
    }
    unset($form_state['values']['login_bgimage']);
  }
  else {
    if (variable_get('simplelogin_fid', '') != '') {
      $file = file_load(variable_get('simplelogin_fid', ''));
      if ($file->fid) {
        // Delete the file and the usage record.
        file_delete($file, TRUE);
      }
      variable_set('simplelogin_fid', '');
    }
  }
}
