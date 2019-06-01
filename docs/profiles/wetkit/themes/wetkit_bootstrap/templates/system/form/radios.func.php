<?php
/**
 * @file
 * Stub file for wetkit_bootstrap_radios().
 */

/**
 * Returns HTML for a radios form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #id, #name, #attributes, #checked, #return_value.
 *
 * @ingroup themeable
 */
function wetkit_bootstrap_radios($variables) {
  $element = $variables['element'];

  if ($element['#required']) {
    // Add required attribute to radio elements
    $element['#children'] = str_replace('<input', '<input required="required"', $element['#children']);
  }

  $element['#children'] = '<div role="radiogroup" aria-labelledby="' . $element['#id'] . '-legend-label">' . $element['#children'] . '</div>';

  return !empty($element['#children']) ? $element['#children'] : '' . '</fieldset>';
}
