<?php
/**
 * @file
 * Stub file for wetkit_bootstrap_radio().
 */

/**
 * Returns HTML for a radio button form element.
 *
 * Note: The input "name" attribute needs to be sanitized before output, which
 *       is currently done by passing all attributes to drupal_attributes().
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #required, #return_value, #value, #attributes, #title,
 *     #description
 *
 * @ingroup themeable
 */
function wetkit_bootstrap_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio'));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}