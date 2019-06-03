<?php

/**
 * @file
 * User page callbacks for Media: Skyfish.
 */

/**
 * Callback for user skyfish settings form.
 *
 * @param array $form
 *   The form being constructed.
 * @param array $form_state
 *   The form state array.
 * @param object $account
 *   The user object initialized from the menu item.
 *
 * @return array
 *   An array containing form elements
 */
function media_skyfish_user_settings_form(array $form, array $form_state, $account) {

  $token = media_skyfish_get_api_token();

  $form = [];
  $form['api'] = [
    '#type' => 'fieldset',
    '#title' => t('Skyfish login'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  ];

  if ($token) {
    $form['api']['message']['#markup'] = t('You have a valid Skyfish API token');
  }
  else {
    $form['api']['username'] = [
      '#type' => 'textfield',
      '#title' => t('Skyfish username'),
      '#required' => TRUE,
      '#description' => t('Please enter your Skyfish username, (email address)'),
    ];

    $form['api']['password'] = [
      '#type' => 'password',
      '#title' => t('Skyfish password'),
      '#required' => TRUE,
      '#description' => t('Please enter your Skyfish password'),
    ];

    $form['uid'] = [
      '#type' => 'hidden',
      '#value' => $account->uid,
    ];

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Login',
    );
  }

  return $form;
}

/**
 * Validation handler for media_skyfish_user_settings_form().
 *
 * @param array $form
 *   The form being validated.
 * @param array $form_state
 *   The form state array.
 */
function media_skyfish_user_settings_form_validate(array $form, array &$form_state) {

  $username = $form_state['values']['username'];
  $password = $form_state['values']['password'];

  if ($username && $password) {
    try {
      $form_state['values']['authenticate'] = media_skyfish_authenticate($username, $password);
    }
    catch (Exception $e) {
      form_set_error('api', t('Invalid credentials'));
    }
  }

}

/**
 * Submit handler for media_skyfish_user_settings_form().
 *
 * @param array $form
 *   The form being submitted.
 * @param array $form_state
 *   The form state array.
 */
function media_skyfish_user_settings_form_submit(array $form, array $form_state) {

  $uid = $form_state['values']['uid'];
  $authenticate = $form_state['values']['authenticate'];

  db_merge('skyfish_api_tokens')
    ->key(['uid' => $uid])
    ->fields([
      'uid' => $uid,
      'token' => $authenticate->token,
      'valid_until' => $authenticate->validUntil,
    ])
    ->execute();

  drupal_set_message(t('Login successful, token saved'));
}
