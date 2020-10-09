<?php

/**
 * @file
 * Synonyms-friendly entity look up feeds taper plugin.
 */

$plugin = array(
  'form' => 'synonyms_entity_find_feeds_tamper_form',
  'callback' => 'synonyms_entity_find_feeds_tamper_callback',
  'name' => 'Synonyms-friendly entity look up',
  'multi' => 'loop',
  'category' => 'Other',
);

/**
 * Feeds tamper settings form builder.
 */
function synonyms_entity_find_feeds_tamper_form($importer, $element_key, $settings, array &$form_state) {
  $html_id = 'synonysm-entity-find-feeds-tamper-settings';
  $form = array(
    '#prefix' => '<div id="' . $html_id . '">',
    '#suffix' => '</div>',
  );

  $entity_info = entity_get_info();

  $entity_type_options = array();
  foreach ($entity_info as $entity_type => $entity_type_info) {
    $entity_type_options[$entity_type] = $entity_type_info['label'];
  }

  $default_entity_type = isset($form_state['values']['settings']['entity_type']) ? $form_state['values']['settings']['entity_type'] : (isset($settings['entity_type']) ? $settings['entity_type'] : NULL);

  $form['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Entity type'),
    '#required' => TRUE,
    '#options' => $entity_type_options,
    '#default_value' => $default_entity_type,
    '#ajax' => array(
      'callback' => 'synonyms_entity_find_feeds_tamper_form_ajax',
      'wrapper' => $html_id,
    ),
  );

  $bundle_options = array();
  if ($default_entity_type) {
    foreach ($entity_info[$default_entity_type]['bundles'] as $bundle => $bundle_info) {
      $bundle_options[$bundle] = $bundle_info['label'];
    }
  }

  $form['bundle'] = array(
    '#type' => 'select',
    '#title' => t('Bundle'),
    '#options' => $bundle_options,
    '#default_value' => isset($settings['bundle']) ? $settings['bundle'] : NULL,
    '#access' => isset($entity_type['entity keys']['bundle']) && $entity_type['entity keys']['bundle'],
  );

  return $form;
}

/**
 * Feeds tamper callback to execute entity look up by its synonyms.
 */
function synonyms_entity_find_feeds_tamper_callback($result, $item_key, $element_key, &$field, array $settings) {
  $found = synonyms_get_entity_by_synonym($settings['entity_type'], $field, $settings['bundle']);
  if ($found) {
    $field = $found;
  }
}

/**
 * Ajax callback for form changes.
 */
function synonyms_entity_find_feeds_tamper_form_ajax(array $form, array &$form_state) {
  return isset($form['plugin']['settings']) ? $form['plugin']['settings'] : $form['settings'];
}
