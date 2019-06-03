<?php

/**
 * @file
 * Hooks provided by the DVG Global API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Add options to testdata form.
 *
 * Modules may implement this hook to add items to the testdata form.
 * 
 * @return array
 *  An associative array containing fields to be added to the testdata form.
 *
 */
function hook_dvg_global_testdata() {

  $form = array();
  $form['stuf_bg'] = array(
    '#type' => 'fieldset',
    '#title' => t('StUF-BG'),
  );
  $form['stuf_bg']['dvg_stuf_bg_debug_bsn'] = array(
    '#type' => 'textfield',
    '#title' => t('Debug BSN'),
    '#default_value' => variable_get('dvg_stuf_bg_debug_bsn', ''),
    '#description' => t('Debug BSN used when debug mode is on.'),
  );
  return $form;
}

/**
 * @} End of "addtogroup hooks".
 */
