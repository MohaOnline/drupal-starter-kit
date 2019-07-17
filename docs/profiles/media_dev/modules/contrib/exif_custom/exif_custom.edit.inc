<?php

/**
 * @file
 *
 */

/**
 * Form API callback to edit a mapping.
 */
function exif_custom_map_edit_form() {
  // Let's get the mid.
  $mid = arg(5);
  // Load mappings.
  $sql = 'SELECT * FROM {exif_custom_mapped_fields} WHERE mid = :mid';
  $results = db_query($sql, array(
    ':mid' => $mid,
  ));
  // Load fields possible to map to.
  $fields = field_info_instances('file', 'image');
  $select_fields = array();
  // Allow mapping to title field.
  $select_fields['filename'] = 'File name';
  // Allow no field to be selected.
  $select_fields['none'] = 'none';
  foreach ($fields as $field => $field_data) {
    $select_fields[$field] = $field_data['label'];
  }
  $form['mappings-table-start'] = array(
    '#markup' => '<table><thead><tr><td><strong>EXIF field</strong></td><td><strong>Example</strong></td><td><strong>Mapped to</strong></td></tr></thead>',
  );
  while ($result = $results->fetchObject()) {
    $form[$result->exif_field] = array(
      '#markup' => $result->exif_field,
      '#prefix' => '<tr><td>',
      '#suffix' => '</td>',
    );
    $form['example:' . $result->exif_field] = array(
      '#markup' => $result->exif_example,
      '#prefix' => '<td>',
      '#suffix' => '</td>',
    );
    $form['img_field:' . $result->exif_field] = array(
      '#type' => 'select',
      '#default_value' => $result->img_field,
      '#options' => $select_fields,
      '#prefix' => '<td>',
      '#suffix' => '</td></tr>',
    );
  }
  $form['mappings-table-end'] = array(
    '#markup' => '</table>',
  );
  $form['delete'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Delete',
    '#options' => array(
      'delete' => t('Delete this mapping'),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  $form['#validate'][] = 'exif_custom_map_edit_validate';
  $form['#submit'][] = 'exif_custom_map_edit_submit';
  return $form;
}

/**
 * Validation callback for exif_custom_map_edit_form().
 */
function exif_custom_map_edit_validate($form, &$form_state) {
  // @todo Check for duplicates.
}

/**
 * Submission callback for exif_custom_map_edit_form().
 */
function exif_custom_map_edit_submit($form, $form_state) {
  $mid = arg(5);
  $test = (string) $form_state['values']['delete']['delete'];
  if ($test == 'delete') {
    db_query('DELETE FROM {exif_custom_mapped_fields} WHERE mid = :mid', array(
      ':mid' => $mid,
    ));
    db_query('DELETE FROM {exif_custom_maps} WHERE mid = :mid', array(
      ':mid' => $mid,
    ));
    db_query('DELETE FROM {exif_custom_users} WHERE mid = :mid', array(
      ':mid' => $mid,
    ));
    drupal_set_message(t('The mapping has been deleted.'));
    drupal_goto('admin/config/media/exif_custom');
    return;
  }

  foreach ($form_state['values'] as $field => $value) {
    if (substr($field, 0, 10) != 'img_field:') {
      break;
    }
    $field = substr($field, 10);
    db_query("UPDATE {exif_custom_mapped_fields}
      SET img_field = :img_field
      WHERE mid = :mid AND exif_field = :exif_field", array(
        ':img_field' => $value,
        ':mid' => $mid,
        ':exif_field' => $field,
      ));
  }

  drupal_set_message(t('The mapping has been updated.'));

  // Redirect to the mappings list page.
  drupal_goto('admin/config/media/exif_custom');
}
