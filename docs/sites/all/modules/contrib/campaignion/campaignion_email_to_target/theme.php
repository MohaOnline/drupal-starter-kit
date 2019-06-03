<?php

/**
 * Returns HTML for the webform component if targets and messages are displayed.
 * @param array $variables
 *   - element: the placeholder element with the following special keys:
 *     - #title: The component name.
 *
 * @ingroup themeable
 */
function theme_campaignion_email_to_target_selector_component(&$variables) {
  $element = &$variables['element'];
  $children = element_children($element, TRUE);
  $output = '';
  foreach ($children as $key) {
    $output .= drupal_render($element[$key]);
  }
  return $output;
}

/**
 * Returns HTML for the form_builder placeholder.
 *
 * @param array $variables
 *   An associative array containing:
 *   - element: the placeholder element with the following special keys:
 *     - #title: The component name.
 *
 * @ingroup themeable
 */
function theme_campaignion_email_to_target_selector_placeholder(&$variables) {
  $element = &$variables['element'];

  $render = array(
    '#theme_wrappers' => array('fieldset'),
    '#title' => $element['#title'],
    '#description' => t('This will show the target selector and allows users to edit their messages.'),
  );

  return render($render);
}

/**
 * Preprocess callback.
 */
function template_preprocess_campaignion_email_to_target_results_table_entry(&$variables) {
  $e = $variables['element'];
  $t = 'campaignion_email_to_target_mail';
  $variables['message'] = theme([$t, $t . '_' . $e['#component']['nid']], [
    'message' => $e['#data']['message'],
  ]);
}
