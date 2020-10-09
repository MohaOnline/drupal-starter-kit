<?php

/**
 * @file
 * Plugin definition for synonyms friendly select behavior.
 */

$plugin = array(
  'title' => t('Select'),
  'description' => t('Synonyms friendly select'),
  'settings form callback' => 'synonyms_behavior_select_settings_form',
  'interface' => 'SelectSynonymsBehavior',
);

/**
 * Settings form for select behavior.
 */
function synonyms_behavior_select_settings_form($form, &$form_state, $settings) {
  static $is_first_time = TRUE;

  $element = array();

  $element['wording'] = array(
    '#type' => 'textfield',
    '#title' => t('Select wording'),
    '#default_value' => isset($settings['wording']) ? $settings['wording'] : '@synonym',
    '#description' => t('Specify with what wording the synonyms should be placed in the select form element. You may use: <ul><li><em>@synonym</em> to denote value of the synonym</li><li><em>@entity</em> to denote entity name</li><li><em>@field_name</em> to denote lowercase label of the field from where the synonym originates</li></ul>'),
    '#required' => TRUE,
  );

  if (!$is_first_time) {
    // Remove the description, if the element is created more than once on the
    // same form. Otherwise the whole form looks too clumsy.
    unset($element['wording']['#description']);
  }

  $is_first_time = FALSE;

  return $element;
}

