<?php

/**
 * @file
 * Admin page callbacks.
 */


/**
 * Admin settings form
 */
function multiselect_settings() {
  $form = array();
  
  $form['multiselect_widths'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of Select Boxes (in pixels)'),
    '#description' => t('By default, the boxes are each 250px wide.'),
    '#default_value' => variable_get('multiselect_widths'),
    '#size' => 3,
    '#field_suffix' => 'px',
    '#required' => TRUE
  );

  $form = system_settings_form($form);
  return $form;
}