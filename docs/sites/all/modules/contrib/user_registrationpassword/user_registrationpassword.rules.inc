<?php

/**
 * @file
 * Rules event for the user_registrationpassword module.
 */

/**
 * Implements hook_rules_event_info().
 */
function user_registrationpassword_rules_event_info() {
  return array(
    'user_registrationpassword_activated' => array(
      'label' => t('After a user account has been activated (via user_registrationpassword)'),
      'group' => 'User Registration Password',
      'variables' => array(
        'account' => array(
          'type' => 'user',
          'label' => t("The activated user's account"),
        ),
      ),
      'group' => 'User',
      'access callback' => 'rules_user_integration_access',
    ),
  );
}
