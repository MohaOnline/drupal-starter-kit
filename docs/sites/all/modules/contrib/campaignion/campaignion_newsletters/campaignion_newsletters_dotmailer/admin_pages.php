<?php

/**
 * Implementation of campaignion_newsletters_mailchimp_form_campaignion_newsletters_admin_settings_alter().
 */
function _campaignion_newsletters_dotmailer_form_campaignion_newsletters_admin_settings_alter(&$form, &$form_state) {

  $form['dotmailer'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dotmailer'),
    '#weight' => 1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  $form['dotmailer']['api_keys'] = array(
    '#type' => 'fieldset',
    '#title' => t('API-Users'),
    '#prefix' => '<div id="dotmailer-api-keys-wrapper">',
    '#suffix' => '</div>',
  );
  $fs = &$form['dotmailer']['api_keys'];
  $keys = variable_get('dotmailer_api_keys', array());
  if (empty($form_state['dotmailer_new_keys'])) {
    $form_state['dotmailer_new_keys'] = count($keys) ? 0 : 1;
  }

  $machine_name = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#machine_name' => array(
      'exists' => 'campaignion_newsletters_dotmailer_get_key',
    ),
    '#maxlength' => 31,
  );
  foreach ($keys as $name => $data) {
    $fs[$name]['name'] = array(
      '#default_value' => $name,
      '#disabled' => TRUE,
    ) + $machine_name;
    $fs[$name]['username'] = array(
      '#type' => 'textfield',
      '#default_value' => $data['username'],
      '#title' => t('API-User name'),
    );
    $fs[$name]['password'] = [
      '#type' => 'textfield',
      '#default_value' => $data['password'],
      '#title' => t('Password'),
    ];
  }
  if (!empty($form_state['dotmailer_new_keys'])) {
    for ($i = 1; $i <= $form_state['dotmailer_new_keys']; $i++) {
      $name = 'new_' . $i;
      $fs[$name]['name'] = array(
        '#default_value' => '',
        '#required' => FALSE,
      ) + $machine_name;
      $fs[$name]['username'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#title' => t('API-User name'),
      );
      $fs[$name]['password'] = [
        '#type' => 'textfield',
        '#default_value' => '',
        '#title' => t('Password'),
      ];
    }
  }
  $fs['add_more'] = array(
    '#type' => 'submit',
    '#value' => t('Add another key'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
       'callback' => 'campaignion_newsletters_dotmailer_admin_ajax',
       'wrapper' => 'dotmailer-api-keys-wrapper',
    ),
    '#submit' => array('campaignion_newsletters_dotmailer_admin_add_more_submit'),
  );

  array_unshift(
    $form['#submit'],
    'campaignion_newsletters_dotmailer_admin_submit'
  );
  array_unshift(
    $form['#validate'],
    'campaignion_newsletters_dotmailer_admin_validate'
  );
}

/**
 * Ajax callback for the add-more keys button.
 */
function campaignion_newsletters_dotmailer_admin_ajax($form, &$form_state) {
  return $form['dotmailer']['api_keys'];
}

/**
 * Submit callback for the add-more keys button.
 */
function campaignion_newsletters_dotmailer_admin_add_more_submit($form, &$form_state) {
  $form_state['dotmailer_new_keys']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Validate callback for the admin form.
 */
function campaignion_newsletters_dotmailer_admin_validate($form, &$form_state) {
  $keys = &$form_state['values']['dotmailer']['api_keys'];
  foreach ($keys as $key => $data) {
    if (empty($data['name']) && empty($data['username']) && empty($data['password'])) {
      continue;
    }
    if (empty($data['name'])) {
      form_set_error('dotmailer][api_keys][' . $key . '][name', t('The API key has to have a unique name.'));
    }
  }
}

/**
 * Submit callback for the admin form.
 */
function campaignion_newsletters_dotmailer_admin_submit($form, &$form_state) {
  $keys = array();
  foreach ($form_state['values']['dotmailer']['api_keys'] as $data) {
    if (!empty($data['username']) && !empty($data['password'])) {
      $keys[$data['name']] = [
        'username' => $data['username'],
        'password' => $data['password'],
      ];
    }
  }
  variable_set('dotmailer_api_keys', $keys);

  // Hide our values from the general submit handler.
  unset($form_state['values']['dotmailer']);
}
