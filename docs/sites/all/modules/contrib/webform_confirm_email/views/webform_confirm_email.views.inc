<?php
/**
 * @file
 * Views hooks implemented for the Webform Confirm Email Address module.
 */

/**
 * Implements hook_views_data_alter().
 */
function webform_confirm_email_views_data_alter(&$data) {
  // confirmed
  $data['webform_submissions']['confirmed'] = array(
    'title' => t('Confirmed'),
    'help'  => t('Whether or not the submission has been confirmed.'),
    'field' => array(
      'handler'        => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'label'   => t('Confirmed'),
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
