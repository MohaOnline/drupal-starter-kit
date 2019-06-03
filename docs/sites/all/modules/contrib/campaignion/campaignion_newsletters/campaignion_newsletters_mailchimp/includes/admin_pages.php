<?php

/**
 * @file
 * Define the configuration form for this module's newsletter providers.
 */

/**
 * Implementation of campaignion_newsletters_mailchimp_form_campaignion_newsletters_admin_settings_alter().
 */
function _campaignion_newsletters_mailchimp_form_campaignion_newsletters_admin_settings_alter(&$form, &$form_state) {
  $library = libraries_detect('dotmailer-api-php');
  if (empty($library['installed'])) {
    drupal_set_message($library['error message'], 'error', FALSE);
  }

  $form['mailchimp'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mailchimp'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['mailchimp']['api_keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('API-Keys'),
    '#prefix' => '<div id="mailchimp-api-keys-wrapper">',
    '#suffix' => '</div>',
  );
  $fs = &$form['mailchimp']['api_keys'];
  $keys = variable_get('mailchimp_api_keys', array());
  if (empty($form_state['mailchimp_new_keys'])) {
    $form_state['mailchimp_new_keys'] = count($keys) ? 0 : 1;
  }

  $machine_name = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#machine_name' => array(
      'exists' => 'campaignion_newsletters_mailchimp_get_key',
    ),
    '#maxlength' => 31,
  );
  foreach ($keys as $name => $key) {
    $fs[$name]['name'] = array(
      '#default_value' => $name,
      '#disabled' => TRUE,
    ) + $machine_name;
    $fs[$name]['key'] = array(
      '#type' => 'textfield',
      '#default_value' => $key,
      '#title' => t('API-Key'),
    );
  }
  if (!empty($form_state['mailchimp_new_keys'])) {
    for ($i = 1; $i <= $form_state['mailchimp_new_keys']; $i++) {
      $name = 'new_' . $i;
      $fs[$name]['name'] = array(
        '#default_value' => '',
        '#required' => FALSE,
      ) + $machine_name;
      $fs[$name]['key'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#title' => t('API-Key'),
      );
    }
  }
  $fs['add_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another key'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'campaignion_newsletters_mailchimp_admin_ajax',
      'wrapper' => 'mailchimp-api-keys-wrapper',
    ),
    '#submit' => array('campaignion_newsletters_mailchimp_admin_add_more_submit'),
  );

  array_unshift(
    $form['#submit'],
    'campaignion_newsletters_mailchimp_admin_submit'
  );
  array_unshift(
    $form['#validate'],
    'campaignion_newsletters_mailchimp_admin_validate'
  );
}

/**
 * Ajax callback for the add-more keys button.
 */
function campaignion_newsletters_mailchimp_admin_ajax($form, &$form_state) {
  return $form['mailchimp']['api_keys'];
}

/**
 * Submit callback for the add-more keys button.
 */
function campaignion_newsletters_mailchimp_admin_add_more_submit($form, &$form_state) {
  $form_state['mailchimp_new_keys']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate callback for the admin form.
 */
function campaignion_newsletters_mailchimp_admin_validate($form, &$form_state) {
  $keys = &$form_state['values']['mailchimp']['api_keys'];
  foreach ($keys as $key => $data) {
    if (empty($data['name']) && empty($data['key'])) {
      continue;
    }
    if (empty($data['name'])) {
      form_set_error('mailchimp][api_keys][' . $key . '][name', t('The API key has to have a unique name.'));
    }
  }
}

/**
 * Submit callback for the admin form.
 */
function campaignion_newsletters_mailchimp_admin_submit($form, &$form_state) {
  $keys = array();
  foreach ($form_state['values']['mailchimp']['api_keys'] as $data) {
    if (!empty($data['key'])) {
      $keys[$data['name']] = $data['key'];
    }
  }
  variable_set('mailchimp_api_keys', $keys);

  // Hide our values from the general submit handler.
  unset($form_state['values']['mailchimp']);
}
