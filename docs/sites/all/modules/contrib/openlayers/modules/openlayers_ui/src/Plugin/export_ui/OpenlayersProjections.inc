<?php

/**
 * @file
 * CTools Export UI plugin definition for projections.
 */

use Drupal\openlayers_ui\UI\OpenlayersProjections;

/**
 * Projection base config form handler.
 */
function openlayers_projection_form_start($form, &$form_state) {
  $class = new OpenlayersProjections();
  $class->init($form_state['plugin']);
  $class->edit_form($form, $form_state);

  $form['definition'] = array(
    '#type' => 'textarea',
    '#title' => t('Projection definition'),
    '#rows' => '10',
    '#default_value' => isset($form_state['item']->definition) ? $form_state['item']->definition : '',
  );

  return $form;
}

/**
 * Projection base config form validation handler.
 */
function openlayers_projection_form_start_validate($form, &$form_state) {
  $class = new OpenlayersProjections();
  $class->init($form_state['plugin']);
  $class->edit_form_validate($form, $form_state);
}

/**
 * Projection base config form submit handler.
 */
function openlayers_projection_form_start_submit($form, &$form_state) {
  $class = new OpenlayersProjections();
  $class->init($form_state['plugin']);
  $class->edit_form_submit($form, $form_state);
}
