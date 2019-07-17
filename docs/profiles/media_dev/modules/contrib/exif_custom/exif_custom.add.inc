<?php

/**
 * @file
 * Form for creating a new mapping.
 */

/**
 * Initial form for creating a new mapping using an example file.
 */
function exif_custom_new_map_form() {
  $form['name'] = array(
    '#title' => t('Mapping name'),
    '#type' => 'textfield',
    '#description' => t("The name for this mapping (e.g. Jeremy's camera)."),
    '#required' => TRUE,
  );
  $form['example_file'] = array(
    '#title' => t('Example file'),
    '#type' => 'file',
    '#description' => t('A file with the EXIF fields you would like to map. Required.'),
    '#required' => FALSE,
  );
  $form['actions']['submit'] = array(
    '#value' => 'Save',
    '#type' => 'submit',
  );
  return $form;
}

/**
 * Form validation callback for exif_custom_new_map_form().
 */
function exif_custom_new_map_form_validate($form, &$form_state) {
  // Check that the name is unique.
  $sql = 'SELECT name FROM {exif_custom_maps} WHERE name = :name';
  $result = db_query($sql, array(
    ':name' => $form_state['values']['name'],
  ))->rowCount();
  if ($result > 0) {
    form_set_error('name', t('The name must be unique'));
  }

  // Verify an example image was uploaded.
  if (!isset($_FILES, $_FILES['files'], $_FILES['files']['name'], $_FILES['files']['name']['example_file'])) {
    form_set_error('example_file', t('An example file must be provided.'));
  }

  // @todo Check image is an allowed type.
}

/**
 * Form submission callback for exif_custom_new_map_form().
 */
function exif_custom_new_map_form_submit($form, &$form_state) {
  // Handle file uploading.
  $validators = array(
    'file_validate_is_image' => array(
      '',
    ),
  );

  // Save file as temporary.
  $file = file_save_upload('example_file', $validators);
  $fields = exif_custom_get_exif_fields($file->uri);

  // Add the mapping to the database.
  db_query('INSERT INTO {exif_custom_maps}(name) VALUES (:name)', array(
      ':name' => $form_state['values']['name'],
    ));

  // Retrieve the mid.
  $mid = db_query('SELECT mid FROM {exif_custom_maps} WHERE name = :name', array(
      ':name' => $form_state['values']['name'],
    ))->fetchField();

  // Save each of the field mappings.
  foreach ($fields as $key => $value) {
    if (is_array($value)) {
      $value_for_db = implode("; ", $value);
    }
    else {
      $value_for_db = $value;
    }
    db_query("INSERT INTO {exif_custom_mapped_fields}
      (mid, exif_field, exif_example, img_field)
      VALUES (:mid, :exif_field, :exif_example, :img_field)", array(
        ':mid' => $mid,
        ':exif_field' => $key,
        ':exif_example' => $value_for_db,
        ':img_field' => 'none',
      ));
  }
  drupal_set_message('New EXIF mapping has been saved: ' . $form_state['values']['name'], 'status');

  // Redirect to the edit page.
  $form_state['redirect'] = 'admin/config/media/exif_custom/map/' . $mid;
}
