<?php

/**
 * @file
 * All theme functions and preprocessors go here, typically.
 */

/**
 * Theme callback: Masked Input Definitions settings
 * @see masked_input_settings()
 */
function theme_masked_input_settings_definitions($variables) {
  $form = $variables['form'];

  // Hard-code default mask definitions because they come with the plugin.
  $rows = _masked_input_default_definitions();

  $header = array(
    t('Character'),
    t('Regular expression'),
    t('Description'),
  );

  foreach (element_children($form) as $key) {
    $delete_key = $key . '_delete';
    $column = array();
    $column[] = array('data' => drupal_render($form[$key]['character']));
    $column[] = array('data' => drupal_render($form[$key]['regex']));
    $column[] = array('data' => drupal_render($form[$key]['description']));
    $rows[] = $column;
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= drupal_render_children($form);

  return $output;
}
