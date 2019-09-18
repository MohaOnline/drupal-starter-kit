<?php

use Drupal\little_helpers\ArrayConfig;

/**
 * Invoked by campaignion_form_builder_form_builder_element_types().
 */
function _campaignion_form_builder_form_builder_types_opt_in() {
  $fields = [];

  $opt_in = function($component, $element) {
    $element += [
      'title' => $component['name'],
      'palette_group' => 'supporter_data',
    ];
    $element['default']['#webform_component'] = $component;
    $opt_in = campaignion_opt_in_form_builder_element_types('webform', NULL)['opt_in'];
    ArrayConfig::mergeDefaults($element, $opt_in);
    return $element;
  };

  $fields['newsletter'] = $opt_in([
    'name' => 'Email Opt-In',
    'form_key' => 'newsletter',
    'extra' => [
      'channel' => 'email',
      'fixed_channel' => TRUE,
    ],
  ], [
    'weight' => 49,
  ]);
  $fields['post_opt_in'] = $opt_in([
    'name' => 'Post Opt-In',
    'form_key' => 'post_opt_in',
    'extra' => [
      'channel' => 'post',
      'fixed_channel' => TRUE,
    ],
  ], [
    'weight' => 50,
    'unique' => TRUE,
  ]);
  $fields['phone_opt_in'] = $opt_in([
    'name' => 'Phone Opt-In',
    'form_key' => 'phone_opt_in',
    'extra' => [
      'channel' => 'phone',
      'fixed_channel' => TRUE,
    ],
  ], [
    'weight' => 51,
    'unique' => TRUE,
  ]);

  return $fields;
}
