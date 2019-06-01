<?php
/**
 * @file
 * Stub file for bootstrap_form_element_label().
 */

/**
 * Returns HTML for a form element label and required marker.
 *
 * Form element labels include the #title and a #required marker. The label is
 * associated with the element itself by the element #id. Labels may appear
 * before or after elements, depending on theme_form_element() and
 * #title_display.
 *
 * This function will not be called for elements with no labels, depending on
 * #title_display. For elements that have an empty #title and are not required,
 * this function will output no label (''). For required elements that have an
 * empty #title, this will output the required marker alone within the label.
 * The label will use the #id to associate the marker with the field that is
 * required. That is especially important for screenreader users to know
 * which field is required.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #required, #title, #id, #value, #description.
 *
 * @return string
 *   The constructed HTML.
 *
 * @see theme_form_element_label()
 *
 * @ingroup theme_functions
 */
function wetkit_bootstrap_form_element_label(&$variables) {
  $element = $variables['element'];

  // Extract variables.
  $output = '';

  $title = !empty($element['#title']) ? filter_xss_admin($element['#title']) : '';

  // Only show the required marker if there is an actual title to display.
  if ($title && $required = !empty($element['#required']) ? theme('form_required_marker', array('element' => $element)) : '') {
    $title .= ' ' . $required;
  }

  $display = isset($element['#title_display']) ? $element['#title_display'] : 'before';
  $type = !empty($element['#type']) ? $element['#type'] : FALSE;
  $checkbox = $type && $type === 'checkbox';
  $radio = $type && $type === 'radio';

  // Immediately return if the element is not a checkbox or radio and there is
  // no label to be rendered.
  if (!$checkbox && !$radio && ($display === 'none' || !$title)) {
    return '';
  }

  // Retrieve the label attributes array.
  $attributes = &_bootstrap_get_attributes($element, 'label_attributes');

  // Add Bootstrap label class.
  $attributes['class'][] = 'control-label';

  // Add the necessary 'for' attribute if the element ID exists.
  if (!empty($element['#id'])) {
    $attributes['for'] = $element['#id'];
  }

  // Checkboxes and radios must construct the label differently.
  if ($checkbox || $radio) {
    if ($display === 'before') {
      $output .= $title;
    }
    elseif ($display === 'none' || $display === 'invisible') {
      $output .= '<span class="element-invisible">' . $title . '</span>';
    }
    // Inject the rendered checkbox or radio element inside the label.
    if (!empty($element['#children'])) {
      $output .= $element['#children'];
    }
    if ($display === 'after') {
      $output .= $title;
    }
  }
  // Otherwise, just render the title as the label.
  else {
    // Show label only to screen readers to avoid disruption in visual flows.
    if ($display === 'invisible') {
      $attributes['class'][] = 'element-invisible';
    }
    $output .= $title;
  }

  // Accessibility fix
  // See: https://www.drupal.org/node/2279111
  // and https://www.drupal.org/node/504962
  // - Removes 'for' attribute from known drupal-specific un-labelable elements.
  // - Adds IDs to labels for aria-labelledby usage.
  $label_type = 'label';
  if (!empty($element['#id'])) {
    if ($type == 'radios' || $type == 'checkboxes') {
      // label this element as a composite form
      $variables['#composite'] = TRUE;
      $label_type = 'legend';
      $attributes['class'][] = 'composite-form-label';
      unset($attributes['for']);
      $legend_label = $element['#id'] . '-legend-label';
    }
    // labelable element: add an id to allow the use of aria-labelledby
    $attributes['id'] = $element['#id'] . '-label';
  }

  if ($type == 'checkboxes' || $type == 'radios') {
    return '<fieldset class="no-show"><' . $label_type . drupal_attributes($attributes) . '><span id="'. $legend_label . '" class="field-name">' . $output . "</span></$label_type>\n";
  }
  elseif ($type == 'checkbox' || $type == 'radio') {
    return "<$label_type" . drupal_attributes($attributes) . '>' . $output . "</$label_type>\n";
  }
  else {
    return "<$label_type" . drupal_attributes($attributes) . '><span class="field-name">' . $output . "</span>" . "</$label_type>\n";
  }
}
