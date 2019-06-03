<?php

/**
 * @file
 * Administration back-end for block settings.
 */

/**
 * Administration form alterations for block support.
 *
 * @todo allow invert of accessible blocks.
 */
function _custom_help_text_block_admin_form_alter(&$form, &$form_state, $form_id) {

  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allowed blocks'),
    '#description' => t('Based on the settings, a custom help text can be displayed in certain blocks.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $options = array();
  $blocks = _custom_help_text_get_blocks();

  foreach ($blocks as $module_name => $module_blocks) {
    foreach ($module_blocks as $block => $delta) {
      $options[$block] = $module_name . ': ' . $delta;
    }
  }

  $values = variable_get('custom_help_text_block_access', array());
  $values = array_merge($values, array('system' . CUSTOM_HELP_TEXT_GLUE . 'help' => 'system' . CUSTOM_HELP_TEXT_GLUE . 'help'));

  $form['blocks']['custom_help_text_block_access'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Blocks'),
    '#options' => $options,
    '#default_value' => $values,
  );

  $form['blocks']['custom_help_text_block_access']['system' . CUSTOM_HELP_TEXT_GLUE . 'help'] = array(
    '#disabled' => TRUE,
  );

  array_unshift($form['#submit'], 'custom_help_text_block_admin_submit');
}

/**
 * Submit callback for admin form alter.
 */
function custom_help_text_block_admin_submit($form, &$form_state) {
  foreach ($form_state['values']['custom_help_text_block_access'] as $id => $checked) {
    if (!$checked) {
      unset($form_state['values']['custom_help_text_block_access'][$id]);
    }
  }
}
