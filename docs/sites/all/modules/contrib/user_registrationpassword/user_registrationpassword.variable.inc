<?php

/**
 * @file
 * Definition of variables for Variable API module.
 */

/**
 * Implements hook_variable_info().
 *
 * @see variable.module
 */
function user_registrationpassword_variable_info($options) {
  $variables['user_registrationpassword_register_[mail_part]'] = array(
    'type' => 'user_registrationpassword_mail',
    'title' => t('Welcome, no approval, password is already set', array(), $options),
    'description' => t('Customize welcome e-mail messages sent to new members when password is already set.', array(), $options),
    'group' => 'user_mails',
  );

  return $variables;
}

/**
 * Implements hook_variable_type_info().
 */
function user_registrationpassword_variable_type_info() {
  $types['user_registrationpassword_mail'] = array(
    'title' => t('User mail text (User registration password)'),
    'type' => 'mail_text',
    'repeat' => array(
      'default callback' => 'user_registrationpassword_variable_mail_default',
    ),
  );

  return $types;
}
