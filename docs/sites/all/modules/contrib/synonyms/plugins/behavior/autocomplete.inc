<?php

/**
 * @file
 * Plugin definition for autocomplete synonyms behavior.
 */

$plugin = array(
  'title' => t('Autocomplete'),
  'description' => t('Synonyms friendly autocomplete'),
  'settings form callback' => 'synonyms_behavior_autocomplete_settings_form',
  'interface' => 'AutocompleteSynonymsBehavior',
);

/**
 * Settings form for autocomplete behavior.
 */
function synonyms_behavior_autocomplete_settings_form($form, &$form_state, $settings) {
  static $is_first_time = TRUE;

  $element = array();

  $element['wording'] = array(
    '#type' => 'textfield',
    '#title' => t('Autocomplete wording'),
    '#default_value' => isset($settings['wording']) ? $settings['wording'] : '@synonym is a synonym of @entity',
    '#description' => t('Specify with what wording the synonyms should be suggested in the autocomplete feature. You may use: <ul><li><em>@synonym</em> to denote value of the synonym</li><li><em>@entity</em> to denote entity name</li><li><em>@field_name</em> to denote lowercase label of the field from where the synonym originates</li><li><em>@bundle</em> to denote bundle name of the suggested entity</li></ul>'),
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

