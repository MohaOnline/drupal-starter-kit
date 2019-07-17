<?php

/**
 * @file
 * Main settings page.
 */

/**
 * General settings page for the exif_custom module.
 */
function exif_custom_settings_form() {
  $mappings = _exif_custom_get_maps();
  $options = array();
  $options['none'] = t('- No default selected -');
  if (!empty($mappings)) {
    while ($result = $mappings->fetchObject()) {
      $options[$result->mid] = $result->name;
    }
    $form['exif_custom_default'] = array(
      '#type' => 'select',
      '#title' => 'Site default mapping',
      '#default_value' => variable_get('exif_custom_default', ''),
      '#options' => $options,
      '#description' => 'Individual users may override the site default.',
    );

    $form['exif_custom_exclude_paths'] = array(
      '#type' => 'textarea',
      '#title' => t('Exclude paths'),
      '#default_value' => variable_get('exif_custom_exclude_paths', ''),
      '#description' => t('Newline separated list of file-paths that should be skipped, must be prefixed with public:// or temporary:// or private://. The directory-part of the path should have a trailing / unless you want to match a partial directory-name. You will most likely want to end the pattern with an asterisk.<br>Eg. public://some_directory/image* will match files in "some_directory" starting with "image" while public://some_directory/* will match any files in "some_directory"'),
    );
  }
  else {
    drupal_set_message(t('No mappings have been created yet.'));
  }

  $form['exif_custom_overwrite_existing'] = array(
    '#type' => 'checkbox',
    '#title' => t("Overwrite existing values"),
    '#description' => t('By default EXIF data for an image will only be imported the when the file is uploaded. By enabling this option it will be possible to have images processed for EXIF data every time they are edited; this is required for processing old images.<br />Note: enabling this option will also cause image meta data to be reset, so customizations to imported values will be lost. Be careful.'),
    '#default_value' => variable_get('exif_custom_overwrite_existing', FALSE),
  );
  $form['exif_custom_request_method'] = array(
    '#type' => 'radios',
    '#title' => t('Image request method'),
    '#options' => array(
      'legacy' => t('Use drupal_realpath() - works with HTTP Authentication, self-signed certificates, etc.'),
      'new' => t('Use file_create_url() - works with CDNs and other remote file storage systems.'),
    ),
    '#default_value' => variable_get('exif_custom_request_method', 'legacy'),
  );

  return system_settings_form($form);
}
