<?php

// !! DO NOT MODIFY THIS FILE !! (unless you really know what you are doing)

/**
 * Flux Advanced Theme Settings
 * - Custom image uploads
 * - Color settings
 */

/**
 * Implements hook_form_system_theme_settings_alter
 */
function flux_form_system_theme_settings_alter(&$form, &$form_state) {
  // Container fieldset
  $form['cover_photo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cover Photo'),
  );
  
  // Default path for image

  $form['theme_settings']['toggle_cover_photo']['#type'] = 'checkbox';
  $form['theme_settings']['toggle_cover_photo']['#title'] = t('Cover Photo');
  $form['theme_settings']['toggle_cover_photo']['#default_value'] = theme_get_setting('toggle_cover_photo');
  

  // cover_photo settings
    $form['cover_photo'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Cover photo image settings'), 
      '#description' => t('If toggled on, the following cover photo will be displayed.'),
      '#attributes' => array('class' => array('theme-settings-bottom')),
    );
    $form['cover_photo']['default_cover_photo'] = array(
      '#type' => 'checkbox', 
      '#title' => t('Use the default cover photo'), 
      '#default_value' => theme_get_setting('default_cover_photo'), 
      '#tree' => FALSE, 
      '#description' => t('Check here if you want the theme to use the cover photo supplied with it.'),
    );
    $form['cover_photo']['settings'] = array(
      '#type' => 'container', 
      '#states' => array(
        // Hide the cover_photo settings when using the default cover_photo.
        'invisible' => array(
          'input[name="default_cover_photo"]' => array('checked' => TRUE),
        ),
      ),
    );
    $form['cover_photo']['settings']['cover_photo_path'] = array(
        '#type' => 'hidden', 
        '#title' => t('Path to custom cover photo'), 
        '#description' => t('The path to the image file on your server.'), 
        '#default_value' => theme_get_setting('cover_photo_path'),
      );      
    $form['cover_photo']['settings']['cover_photo_upload'] = array(
      '#type' => 'file', 
      '#title' => t('Upload new photo'), 
      '#maxlength' => 40,
      '#description' => t("Use this field to upload a new image. Recommended size is 1024x768 or larger."),
      '#suffix' => theme_get_setting('cover_photo_path') ? theme('image_style', array('style_name' => 'medium', 'path' => theme_get_setting('cover_photo_path'))) : null,       
    );
    
    // Attach custom submit handler to the form
    $form['#submit'][] = 'flux_theme_settings_submit';
    $form['#validate'][] = 'flux_theme_settings_validate';
}


/**
 * Submit handler
 */
function flux_theme_settings_submit($form, &$form_state) {

  $previous = theme_get_setting('cover_photo_path');
  
  // If the user uploaded a new cover_photo or favicon, save it to a permanent location
  // and use it in place of the default theme-provided file.

  if (!empty($form_state['values']['cover_photo_upload'])) {
    $file = $form_state['values']['cover_photo_upload'];
    unset($form_state['values']['cover_photo_upload']);
    $filename = file_unmanaged_copy($file->uri, NULL, FILE_EXISTS_REPLACE);
    $form_state['values']['default_cover_photo'] = 0;
    $form_state['values']['cover_photo_path'] = $filename;
    $form_state['values']['toggle_cover_photo'] = 1;
    // Remove previous file uploaded
    $current = $form_state['values']['cover_photo_path'];
    if (($previous != $current) && is_file($previous)) {
      // Delete previous file
      drupal_unlink($previous);
    }    
  }
  
  // If the user entered a path relative to the system files directory for
  // a cover_photo or favicon, store a public:// URI so the theme system can handle it.
  if (!empty($form_state['values']['cover_photo_path'])) {
    $form_state['values']['cover_photo_path'] = flux_system_theme_settings_validate_path($form_state['values']['cover_photo_path']);
  }

}

/**
 * Validate handler where we actually save the files...
 */
function flux_theme_settings_validate($form, &$form_state) {
  // Handle file uploads.
  $validators = array('file_validate_is_image' => array());

  // Check for a new uploaded logo.
  $file = file_save_upload('cover_photo_upload', $validators);
  if (isset($file)) {
    // File upload was attempted.
    if ($file) {
      // Put the temporary file in form_values so we can save it on submit.
      $form_state['values']['cover_photo_upload'] = $file;
    }
    else {
      // File upload failed.
      form_set_error('cover_photo_upload', t('The image could not be uploaded.'));
    }
  }

  $validators = array('file_validate_extensions' => array('ico png gif jpg jpeg apng svg'));

  // If the user provided a path for a logo or favicon file, make sure a file
  // exists at that path.
  if ($form_state['values']['cover_photo_path']) {
    $path = flux_system_theme_settings_validate_path($form_state['values']['cover_photo_path']);
    if (!$path) {
      form_set_error('cover_photo_path', t('The custom path is invalid.'));
    }
  }
}


/**
 * Copy of _system_theme_settings_validate_path($path)
 */
function flux_system_theme_settings_validate_path($path) {
  // Absolute local file paths are invalid.
  if (drupal_realpath($path) == $path) {
    return FALSE;
  }
  // A path relative to the Drupal root or a fully qualified URI is valid.
  if (is_file($path)) {
    return $path;
  }
  // Prepend 'public://' for relative file paths within public filesystem.
  if (file_uri_scheme($path) === FALSE) {
    $path = 'public://' . $path;
  }
  if (is_file($path)) {
    return $path;
  }
  return FALSE;
}
