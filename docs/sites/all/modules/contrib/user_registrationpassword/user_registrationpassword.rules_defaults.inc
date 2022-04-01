<?php

/**
 * @file
 * Rules example reaction rule for the user_registrationpassword module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function user_registrationpassword_default_rules_configuration() {
  $configs = array();
  $rule = '{ "rules_user_registrationpassword_activated_example" : {
    "LABEL" : "An example rule for the activated user rule.",
    "PLUGIN" : "reaction rule",
    "ACTIVE" : false,
    "OWNER" : "rules",
    "REQUIRES" : [ "rules", "user_registrationpassword" ],
    "ON" : { "user_registrationpassword_activated" : [] },
    "DO" : [ { "drupal_message" : { "message" : "Your account is now activate." } } ]
  }
}';

  $configs['rules_user_registrationpassword_activated_example'] = rules_import($rule);
  return $configs;
}
