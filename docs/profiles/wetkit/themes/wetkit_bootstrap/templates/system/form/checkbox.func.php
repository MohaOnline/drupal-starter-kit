<?php
/**
 * @file
 * Stub file for wetkit_bootstrap_checkbox().
 */

/**
 * Returns HTML for a checkbox form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #id, #name, #attributes, #checked, #return_value.
 *
 * @ingroup themeable
 */
function wetkit_bootstrap_checkbox($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'checkbox';

  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  // Add aria-labelledby for better accessibility
  if (!empty($variables['element']['#parents'])) {
    $element['#attributes']['aria-labelledby'] = $element['#attributes']['id'] . "-label";
  }
  // Unchecked checkbox has #value of integer 0.
  if (!empty($element['#checked'])) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-checkbox'));

  return '<input' . drupal_attributes($element['#attributes']) . ' />';
}