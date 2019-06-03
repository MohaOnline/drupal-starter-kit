<?php

/**
 * DVG DataB configuration form.
 */
function dvg_datab_settings() {
  $form = array();

  $form['dvg_datab'] = array(
    '#type' => 'fieldset',
    '#title' => t('DataB settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $datab = variable_get('dvg_datab', array());

  $form['dvg_datab']['cid'] = array(
    '#type' => 'textfield',
    '#title' => t('CID'),
    '#required' => TRUE,
    '#default_value' => !empty($datab['cid']) ? $datab['cid'] : '',
  );

  $form['dvg_datab']['secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret'),
    '#required' => TRUE,
    '#default_value' => !empty($datab['secret']) ? $datab['secret'] : '',
  );

  $form['dvg_datab']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#required' => TRUE,
    '#default_value' => !empty($datab['url']) ? $datab['url'] : '',
  );

  $form['dvg_datab']['strict'] = array(
    '#type' => 'checkbox',
    '#title' => t('Strict document mode'),
    '#description' => t('Should the documents be filtered on whether the current date is between the document\'s start and end date?'),
    '#default_value' => isset($datab['strict']) ? $datab['strict'] : false,
  );

  $form['dvg_datab']['demo_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Demo mode'),
    '#default_value' => !empty($datab['demo_mode']) ? $datab['demo_mode'] : 0,
    '#description' => t('Demo mode will display a list of example files.'),
  );

  $form['dvg_datab_test'] = array(
    '#type' => 'fieldset',
    '#title' => t('Test connection'),
  );

  $form['dvg_datab_test']['dvg_datab_test_bsn'] = array(
    '#title' => t('BSN'),
    '#type' => 'textfield',
    '#default_value' => '',
    '#description' => t('For testing purposes you can use BSN %demo_bsn which has documents available.', array('%demo_bsn' => DVG_DATAB_DEMO_BSN)),
  );

  $form['dvg_datab_test']['dvg_datab_test_submit'] = array(
    '#value' => t('Test'),
    '#type' => 'submit',
    '#submit' => array('dvg_datab_test_submit'),
    '#id' => 'dvg_datab_test_submit',
  );

  $form['#validate'][] = 'dvg_datab_settings_validate_test';

  return system_settings_form($form);
}

/**
 * Validation handler for the dvg_datab settings form.
 *
 * Removes the bsn to make sure it won't be stored.
 */
function dvg_datab_settings_validate_test($form, &$form_state) {
  if (empty($form_state['clicked_button']['#id']) || $form_state['clicked_button']['#id'] != 'dvg_datab_test_submit') {
    unset($form_state['values']['dvg_datab_test_bsn']);
    unset($form_state['input']['dvg_datab_test_bsn']);
  }
}

/**
 * Tests the DataB connection by requesting the doclist for the provided bsn.
 */
function dvg_datab_test_submit($form, $form_state) {
  $bsn = $form_state['values']['dvg_datab_test_bsn'];

  $success = TRUE;
  try {
    $doclist = dvg_datab_doclist($bsn);
  }
  catch (DvgDataBException $e) {
    $success = FALSE;
    drupal_set_message($e->getMessage(), 'error');
  }

  if ($success) {
    if (empty($doclist)) {
      drupal_set_message(t('DataB settings probably correct, but no documents available'), 'warning');
    }
    else {
      drupal_set_message(t('DataB settings correct'));
    }
  }
}
