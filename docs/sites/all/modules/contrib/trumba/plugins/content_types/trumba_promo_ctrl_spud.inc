<?php
/**
 * @file
 * Configuration and content type for a main calendar Trumba spud.
 */

/**
 * Plugin description.
 */
$plugin = array(
  'title' => t('Trumba Promotional or Control Calendar Spud'),
  'description' => t('Provides a promotional or control type of Trumba Spud.
  Examples of these are the Date Finder, Filter by Category and Day Summary
  spuds.'),
  'single' => TRUE,
  'render callback' => 'trumba_promo_ctrl_spud_render',
  'edit form' => 'trumba_promo_ctrl_spud_edit_form',
  'no title override' => TRUE,
  'category' => t('Calendar'),
  'defaults' => array(
    'trumba_promo_ctrl_type' => '',
    'trumba_promo_ctrl_config' => '',
    'trumba_promo_ctrl_webname' => '',
    'trumba_promo_ctrl_url' => '',
  ),
  'all contexts' => TRUE,
);

/**
 * Ctools edit form.
 */
function trumba_promo_ctrl_spud_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // @todo: make the types configurable
  // @todo: Is there a search type of filter name or config?
  $types = array(
    'upcoming' => 'upcoming',
    'datefinder' => 'datefinder',
    'daysummary' => 'daysummary',
    'filter' => 'filter',
  );

  $form['trumba_promo_ctrl_type'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Spud Type'),
    '#description' => t('Select the type of spud this should be.'),
    '#default_value' => $conf['trumba_promo_ctrl_type'],
    '#empty_option' => t('- Select -'),
    '#options' => $types,
  );

  $form['trumba_promo_ctrl_config'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Spud Configuration'),
    '#description' => t('If the spud type requires configuration text enter it here.'),
    '#default_value' => $conf['trumba_promo_ctrl_config'],
  );

  // Collect the webname, used to identify the organization/account that this
  // spud belong to. Set to the default frm the admin settings to start with.
  $default_webname = variable_get('trumba_webname', '');
  $form['trumba_promo_ctrl_webname'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Web Name'),
    '#description' => t('This is the unique identifier for your calendar account on Trumba.'),
    '#default_value' => ($conf['trumba_promo_ctrl_webname']) ? $conf['trumba_promo_ctrl_webname'] : $default_webname,
  );

  $form['trumba_promo_ctrl_url'] = array(
    '#type' => 'textfield',
    '#required' => FALSE,
    '#title' => t('Calendar URL'),
    '#description' => t('<strong>Only necessary if this spud will NOT be on the
same page as the main calendar spud!</strong> Enter the full path URL for this
website where this calendar will be placed (e.g.: https://ucdavis.edu/calendar)'),
    '#default_value' => $conf['trumba_main_cal_url'],
  );

  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function trumba_promo_ctrl_spud_edit_form_submit($form, &$form_state) {
  $fields = array('trumba_promo_ctrl_type',
    'trumba_promo_ctrl_config',
    'trumba_promo_ctrl_webname',
    'trumba_promo_ctrl_url',
  );
  foreach ($fields as $key) {
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
function trumba_promo_ctrl_spud_render($subtype, $conf, $args, $context) {
  $spud_id = drupal_html_id($subtype);

  // If the webname is empty set it to the default so it won't cause a problem
  // with page loads and executing javascript.
  if (empty($conf['trumba_promo_ctrl_webname'])) {
    $webname = variable_get('trumba_webname', '');
    $conf['trumba_promo_ctrl_webname'] = $webname;
  }

  $params = array(
    'webName' => $conf['trumba_promo_ctrl_webname'],
    'spudType' => $conf['trumba_promo_ctrl_type'],
    'spudConfig' => $conf['trumba_promo_ctrl_config'],
    'teaserBase' => $conf['trumba_promo_ctrl_url'],
    'spudId' => $spud_id,
  );

  return _trumba_spud_embed($spud_id, $params);
}
