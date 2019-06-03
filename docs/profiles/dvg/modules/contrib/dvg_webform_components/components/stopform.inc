<?php

/**
 * @file
 * Name: stopform.inc.
 */

/**
 * Implements _webform_defaults_component().
 */

/**
 * Function _webform_defaults_stopform().
 */
function _webform_defaults_stopform() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'format' => NULL,
      'private' => FALSE,
      'display_on' => 'form',
    ),
  );
}

/**
 * Implements _webform_edit_component().
 */
function _webform_edit_stopform($component) {
  $form = array();
  $form['value'] = array(
    '#type' => 'text_format',
    '#title' => t('Value'),
    '#default_value' => $component['value'],
    '#description' => t('Message to be shown when this form is stopped.') . ' ' . theme('webform_token_help', array('groups' => array('node', 'submission'))),
    '#weight' => -1,
    '#format' => $component['extra']['format'],
    '#element_validate' => array('_webform_edit_stopform_validate'),
  );

  $form['display']['display_on'] = array(
    '#type' => 'select',
    '#title' => t('Display on'),
    '#default_value' => $component['extra']['display_on'],
    '#options' => array(
      'form' => t('form only'),
      'display' => t('viewed submission only'),
      'both' => t('both form and viewed submission'),
    ),
    '#weight' => 1,
    '#parents' => array('extra', 'display_on'),
  );

  drupal_set_message(t('The stopform element requires a page break after this element or the preview page to be enabled.'), 'warning');
  return $form;
}

/**
 * Element validate handler; Set the text format value.
 */
function _webform_edit_stopform_validate($form, &$form_state) {
  if (is_array($form_state['values']['value'])) {
    $form_state['values']['extra']['format'] = $form_state['values']['value']['format'];
    $form_state['values']['value'] = $form_state['values']['value']['value'];
  }
}

/**
 * Implements _webform_render_component().
 */
function _webform_render_stopform($component, $value = NULL, $filter = TRUE, $submission = NULL) {
  $element = array(
    '#type' => 'markup',
    '#title' => $filter ? NULL : $component['name'],
    '#weight' => $component['weight'],
    '#markup' => $component['value'],
    '#format' => $component['extra']['format'],
    '#theme_wrappers' => array('webform_element'),
    '#translatable' => array('title', 'markup'),
    '#access' => $component['extra']['display_on'] != 'display',
    '#webform_nid' => isset($component['nid']) ? $component['nid'] : NULL,
    '#webform_submission' => $submission,
    '#webform_format' => $component['extra']['format'],
  );

  if ($filter) {
    $element['#after_build'] = array('_webform_render_markup_after_build');
  }
  return $element;
}
