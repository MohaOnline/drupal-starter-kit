<?php

/**
 * Displays the media_unique administration page.
 */
function media_unique_configuration($form, &$form_state) {
  $form['media']['media_unique'] = array(
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#description' => t('To use the media-unique functionality you must add the access media unique permission to the appropriate role and grant the role accordingly.  Then you can access the user interface at ') . l('media-unique/batch', 'media-unique/batch'),
  );
  $form['media']['media_unique']['media_unique_batch_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('How many records per batch do you wish to process?  Higher values improve performance but increase risk of a timeout.'),
    '#default_value' => variable_get('media_unique_batch_limit', 20),
    '#description' => t('Number of records per batch that will be processed.  So if you have 10000 files to process with a value of 20 you will have 500 batches.'),
  );
/* $form['media']['media_unique']['media_unique_file_pointer'] = array(
    '#type' => 'textfield',
    '#title' => t('File entity id of last processed.'),
    '#default_value' => variable_get('media_unique_file_pointer', 0),
    '#description' => t('Last file entity id that was processed, to reprocess all files set this to 0.'),
  );*/
  $form['media']['media_unique']['media_unique_bundle_to_process'] = array(
    '#type' => 'textfield',
    '#title' => t('Which file entity bundle would you like to process?'),
    '#default_value' => variable_get('media_unique_bundle_to_process', 'image'),
    '#description' => t('Choose the type of file you would like to process for duplicates example: "image", "video", "document", "other".'),
  );

  return system_settings_form($form);
}
