<?php
/**
 * @file
 * Configuration and content type for a main calendar Trumba spud.
 */

/**
 * Plugin description.
 */
$plugin = array(
  'title' => t('Trumba Open Spud'),
  'description' => t('Provides an arbitrary type of Trumba Spud.
  Useful if the type you have is not in he pre-defined promo and control spuds.'),
  'single' => TRUE,
  'render callback' => 'trumba_open_spud_render',
  'edit form' => 'trumba_open_spud_edit_form',
  'no title override' => TRUE,
  'category' => t('Calendar'),
  'defaults' => array(
    'trumba_open_type' => '',
    'trumba_open_config' => '',
    'trumba_open_webname' => '',
    'trumba_open_url' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Ctools edit form.
 */
function trumba_open_spud_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $form['trumba_open_type'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Spud Type'),
    '#description' => t('Enter the name for the type of spud this should be.'),
    '#default_value' => $conf['trumba_open_type'],
  );

  $form['trumba_open_config'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Spud Configuration'),
    '#description' => t('If the spud type requires configuration text enter it here.'),
    '#default_value' => $conf['trumba_open_config'],
  );

  // Collect the webname, used to identify the organization/account that this
  // spud belong to. Set to the default frm the admin settings to start with.
  $default_webname = variable_get('trumba_webname', '');
  $form['trumba_open_webname'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Web Name'),
    '#description' => t('This is the unique identifier for your calendar account on Trumba.'),
    '#default_value' => ($conf['trumba_open_webname']) ? $conf['trumba_open_webname'] : $default_webname,
  );

  $form['trumba_open_url'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Calendar URL'),
    '#description' => t('<strong>Only necessary if this spud will NOT be on the
same page as the main calendar spud!</strong> Enter the full path URL for this
website where this calendar will be placed (e.g.: https://ucdavis.edu/calendar)'),
    '#default_value' => $conf['trumba_open_url'],
  );

  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function trumba_open_spud_edit_form_submit($form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

/**
 * Render callback.
 *
 * @param string $subtype
 *   The trumba subtype.
 * @param array $conf
 *   Saved configuration settings.
 * @param array $args
 *   Arguments.
 * @param string $context
 *   Context.
 */
function trumba_open_spud_render($subtype, $conf, $args, $context) {
  $spud_id = drupal_html_id($subtype);

  // If the webname is empty set it to the default so it won't cause a problem
  // with page loads and executing javascript.
  if (empty($conf['trumba_open_webname'])) {
    $webname = variable_get('trumba_webname', '');
    $conf['trumba_open_webname'] = $webname;
  }

  $params = array(
    'webName' => $conf['trumba_open_webname'],
    'spudType' => $conf['trumba_open_type'],
    'spudConfig' => $conf['trumba_open_config'],
    'teaserBase' => $conf['trumba_open_url'],
    'spudId' => $spud_id,
  );

  return _trumba_spud_embed($spud_id, $params);
}
