<?php
/**
 * @file
 */

include_once 'webform_confirm_email.inc';

module_load_include('inc', 'webform', 'includes/webform.report');
module_load_include('inc', 'webform', 'includes/webform.submissions');

/**
 * Helper function to propvide a form for filtering for (un)confirmed submissions
 */
function webform_confirm_email_confirmed_filter_form($form, &$form_state) {
  $options = array(
    WEBFORM_CONFIRM_EMAIL_FILTER_NONE        => t('All submissions'),
    WEBFORM_CONFIRM_EMAIL_FILTER_CONFIRMED   => t('Only confirmed submissions'),
    WEBFORM_CONFIRM_EMAIL_FILTER_UNCONFIRMED => t('Only unconfirmed submissions'),
  );
  $query = drupal_get_query_parameters();
  $confirmed = isset($query['confirmed']) ? $query['confirmed'] : WEBFORM_CONFIRM_EMAIL_FILTER_NONE;
  // Set the confirmed filter for queries in the same request.
  _webform_confirm_email_range_confirmed($confirmed);
  $form['confirmed']= array(
    '#type'          => 'radios',
    '#title'         => t('Filter for (un)confirmed submissions'),
    '#default_value' => $confirmed,
    '#options'       => $options,
    '#access'        => TRUE,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Filter submissions'),
  );

  $form['#submit'] = array('webform_confirm_email_confirmed_filter_form_submit');

  return $form;
}

/**
 * Submit handler for filter form for filtering for (un)confirmed submissions
 */
function webform_confirm_email_confirmed_filter_form_submit($form, &$form_state) {
  $path = current_path();
  $query_parameters = drupal_get_query_parameters();
  if (   isset($form_state['values']['confirmed']) == TRUE
      && $form_state['values']['confirmed'] != WEBFORM_CONFIRM_EMAIL_FILTER_NONE) {
    $query_parameters['confirmed'] = $form_state['values']['confirmed'];
  }
  else {
    unset($query_parameters['confirmed']);
  }

  drupal_goto($path, array('absolute' => TRUE, 'query' => $query_parameters));
}

function webform_confirm_email_results_submissions($node, $user_filter, $pager_count) {
  global $conf;

  // Load this early on so that the filter is set before executing the queries.
  $form = drupal_get_form('webform_confirm_email_confirmed_filter_form');

  $table = NULL;
  if (isset($conf['webform_table'])) {
    $table = $conf['webform_table'];
  }
  $conf['webform_table'] = TRUE;
  $element = webform_results_submissions($node, $user_filter, $pager_count);
  $conf['webform_table'] = $table;
  $element['confirmed_form'] = $form;
  return $element;
}

/**
 * Preprocess function for webform-confirm-email-results-submissions.tpl.php
 */
function template_preprocess_webform_confirm_email_results_submissions(&$vars) {
  $vars['confirmed_form'] = $vars['element']['confirmed_form'];
}

/**
 * Create a table containing all submitted values for a webform node.
 */
function webform_confirm_email_results_table($node, $pager_count = 0) {
  global $conf;

  // Load this early on so that the filter is set before executing the queries.
  $form = drupal_get_form('webform_confirm_email_confirmed_filter_form');

  $table = NULL;
  if (isset($conf['webform_table'])) {
    $table = $conf['webform_table'];
  }
  $conf['webform_table'] = TRUE;
  $output = webform_results_table($node, $pager_count);
  $conf['webform_table'] = $table;
  $output[0]['#confirmed_form'] = $form;
  return $output;
}

function theme_webform_confirm_email_results_table($variables) {
  $output = drupal_render($variables['confirmed_form']);
  $output .= theme_webform_results_table($variables);
  return $output;
}
