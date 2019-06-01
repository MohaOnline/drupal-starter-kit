<?php

/**
 * @file
 * Stub file for wetkit_bootstrap_checkboxes().
 */

/**
 * Returns HTML for a checkboxes form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #id, #name, #attributes, #checked, #return_value.
 *
 * @ingroup themeable
 */
function wetkit_bootstrap_checkboxes($variables) {
  $element = $variables['element'];

  return !empty($element['#children']) ? $element['#children'] : '' . '</fieldset>';
}
