<?php

/**
 * @file
 * Hook implementation for the variable module.
 */

/**
 * Implements hook_variable_info().
 */
function campaignion_protest_target_variable_info() {
  $info['campaignion_protest_target_contact_type'] = [
    'title' => t('RedHen contact type used as protest targets.'),
    'description' => t('This contact type is used when importing new protest target contacts.'),
    'type' => 'select',
    'options' => redhen_contact_type_options_list(),
  ];
  return $info;
}
