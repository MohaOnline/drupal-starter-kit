<?php
/**
 * @file
 * Configuration and content type for a main calendar Trumba spud.
 */

/**
 * Plugin description.
 */
$plugin = array(
  'title' => t('Trumba Main Calendar Spud'),
  'description' => t('Provides a Main Calendar type of Trumba Spud. Not for use
  in sidebars or smaller regions. More suitable for use in the content area of
  a page.'),
  'single' => TRUE,
  'content_types' => array('trumba_main_calendar_spud'),
  'render callback' => 'trumba_main_calendar_spud_render',
  'edit form' => 'trumba_main_calendar_spud_edit_form',
  'no title override' => TRUE,
  'icon' => 'trumba_icon_17.png',
  'category' => t('Calendar'),
  'defaults' => array(
    'trumba_main_cal_webname' => '',
    'trumba_main_cal_url' => '',
    'trumba_main_cal_new_window' => FALSE,
  ),
  'all contexts' => TRUE,
);

/**
 * Ctools edit form.
 */
function trumba_main_calendar_spud_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  // Collect the webname, used to identify the organization/account that this
  // spud belong to. Set to the default frm the admin settings to start with.
  $default_webname = variable_get('trumba_webname', '');
  $form['trumba_main_cal_webname'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Web Name'),
    '#description' => t('This is the unique identifier for your calendar account on Trumba.'),
    '#default_value' => ($conf['trumba_main_cal_webname']) ? $conf['trumba_main_cal_webname'] : $default_webname,
  );

  $form['trumba_main_cal_url'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Calendar URL'),
    '#description' => t('Enter the full path URL for this website where this
    calendar will be placed (e.g.: https://ucdavis.edu/calendar)'),
    '#default_value' => $conf['trumba_main_cal_url'],
  );

  $form['trumba_main_cal_new_window'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open events in new window'),
    '#default_value' => $conf['trumba_main_cal_new_window'],
  );

  return $form;
}

/**
 * Ctools edit form submit handler.
 */
function trumba_main_calendar_spud_edit_form_submit($form, &$form_state) {
  $fields = array('trumba_main_cal_webname',
    'trumba_main_cal_url',
    'trumba_main_cal_new_window',
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
function trumba_main_calendar_spud_render($subtype, $conf, $args, $context) {
  $spud_id = drupal_html_id($subtype);

  // If the webname is empty set it to the default so it won't cause a problem
  // with page loads and executing javascript.
  if (empty($conf['trumba_main_cal_webname'])) {
    $webname = variable_get('trumba_webname', '');
    $conf['trumba_main_cal_webname'] = $webname;
  }

  $params = array(
    'webName' => $conf['trumba_main_cal_webname'],
    'spudType' => 'main',
    'detailBase' => $conf['trumba_main_cal_url'],
    'openInNewWindow' => $conf['trumba_main_cal_new_window'],
    'spudId' => $spud_id,
  );

  return _trumba_spud_embed($spud_id, $params);
}
