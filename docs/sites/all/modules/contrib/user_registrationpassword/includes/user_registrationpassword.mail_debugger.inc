<?php

/**
 * @file
 * Mail debugger callbacks.
 */

/**
 * Callback for the elements on the form to send system mails to users.
 *
 * @param array $default_values
 *   The default values.
 *
 * @return bool
 *   An array of mail_debugger pages.
 */
function _user_registrationpassword_mail_debugger_form(array $default_values = array()) {
  if (!user_access('use mail debugger user')) {
    return FALSE;
  }

  // Add user section of the form.
  return array(
    'mail' => array(
      '#type' => 'textfield',
      '#title' => t('E-mail address'),
      '#default_value' => $default_values['mail'],
      '#autocomplete_path' => 'admin/config/development/mail_debugger/user_autocomplete_mail',
    ),
    'type' => array(
      '#type' => 'select',
      '#title' => t('Message'),
      '#options' => array(
        'register' => t('Welcome (no approval required, password is set).'),
        'status_activated' => t('Account activated.'),
      ),
      '#default_value' => $default_values['type'],
    ),
  );
}

/**
 * Validate if the email address is known, and do a suggestion.
 *
 * @param array $values
 *   The values.
 * @param array $elements
 *   The elements.
 * @param array $form_state
 *   The form state.
 */
function _user_registrationpassword_mail_debugger_form_validate(array $values, array $elements, array &$form_state) {
  // Test for valid e-mail address.
  if (!valid_email_address($values['mail'])) {
    form_set_error($elements['mail'], t('Enter a valid e-mail address.'));
    return;
  }

  // Try to load the user by mail.
  $user = user_load_by_mail($values['mail']);

  // No user or anonymous?
  if (empty($user->uid)) {
    form_set_error($elements['mail'], t('This e-mail address does not match any user.'));
    return;
  }
}

/**
 * Send the system mail to a user.
 *
 * @param array $values
 *   The values.
 * @param array $form_state
 *   The form state.
 */
function _user_registrationpassword_mail_debugger_form_submit(array $values, array &$form_state) {
  // Load the user account by mail.
  $account = user_load_by_mail($values['mail']);
  // Load user language.
  $language = user_preferred_language($account);

  // Send the mail.
  $params['account'] = $account;
  $result = drupal_mail('user_registrationpassword', $values['type'], $account->mail, $language, $params);

  // Notify about the result.
  if ($result) {
    drupal_set_message(t('Message sent'));
  }
  else {
    drupal_set_message(t('Message sent with errors. Check the error log.'), 'warning');
  }
}
