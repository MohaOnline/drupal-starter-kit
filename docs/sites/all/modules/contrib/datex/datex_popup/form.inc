<?php

/**
 * @file
 * Alter date_popup element in various forms.
 */

/**
 * From date_popup_authored.
 */
function _datex_popup_authored_form(&$form, $form_state) {
  $el = &$form['author']['date'];

  $calendar = datex_factory(NULL, NULL, 'en');
  if (!$calendar) {
    return;
  }

  $el['#type'] = 'date_popup';

  if (!empty($el['#default_value'])) {
    $fmt = 'Y-m-d H:i:s O';
    $date_obj = date_create_from_format($fmt, $el['#default_value']);
    if ($date_obj) {
      $calendar->setTimestamp($date_obj->format('U'));
      $el['#default_value'] = $calendar->xFormat('Y-m-d H:i:s');
    }
  }

  $el['#date_year_range'] = '-50:+50';
  $el['#date_format'] = 'Y-m-d H:i:s';

  unset($el['#maxlength']);
  unset($el['#description']);

  $el['#after_build'][] = '_datex_popup_authored_element_after_build';
  $form['#submit'][] = '_datex_popup_authored_form_submit';
}

/**
 * Datexifies node authored on date form element.
 */
function datex_popup_form_node_form_alter(&$form, $form_state) {
  if (!$form['author']['#access'] || !isset($form['author']['date'])) {
    return;
  }
  _datex_popup_authored_form($form, $form_state);
}

/**
 * Implements hook_form_alter().
 */
function datex_popup_form_alter(&$form, &$form_state) {
  if (isset($form['#id']) && $form['#id'] === 'comment-form' && $form['author']['date']['#access']) {
    _datex_popup_authored_form($form, $form_state);
  }
}

/**
 * From date_popup_authored.
 */
function _datex_popup_authored_element_after_build($element, &$form_state) {
  // Add a validate handler after the one that is added by date_popup.
  $element['#element_validate'][] = '_datex_popup_authored_element_validate';
  return $element;
}

/**
 * From date_popup_authored.
 */
function _datex_popup_authored_element_validate($element, &$form_state) {
  if (date_hidden_element($element) || is_string($element['#value'])) {
    return;
  }

  // If an error occurred in the validation of the date popup field the date
  // cannot be correctly rendered as a string. In this case clear the date value
  // to avoid subsequent errors when the node is validated.
  // @see date_popup_validate()
  // @see node_validate()
  $input_exists = NULL;
  $input = drupal_array_get_nested_value($form_state['values'], $element['#parents'], $input_exists);
  $date = date_popup_input_date($element, $input);
  if (is_object($date) && !empty($date->errors)) {
    $form_state['values']['date'] = NULL;
  }
}

/**
 * From date_popup_authored.
 */
function _datex_popup_authored_form_submit($form, &$form_state) {
  // @see http://drupal.org/node/847854
  if (isset($form_state['values']['date']) && $form_state['values']['date'] instanceof DateObject) {
    $form_state['values']['date'] = $form_state['values']['date']->format('Y-m-d H:i:s O');
  }
}
