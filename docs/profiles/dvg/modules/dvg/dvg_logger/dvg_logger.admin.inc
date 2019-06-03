<?php

/**
 * @file
 * Contains admin user interface for DvG Logger.
 */

/**
 * DVG Logger configuration form.
 */
function dvg_logger_admin_form() {
  $form = array();

  $types_filter_default = array(
    'failed_login_attempts' => 'failed_login_attempts',
    'testdata_stuf_bg_attempts' => 'testdata_stuf_bg_attempts',
  );

  $types_filter = array_merge($types_filter_default, _dvg_logger_get_types());

  $form['dvg_logger_recipient_emails'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient email'),
    '#description' => t('A comma separated list of email addresses.'),
    '#default_value' => variable_get('dvg_logger_recipient_emails', ''),
    '#element_validate' => array('_dvg_logger_validate_emails'),
    '#required' => TRUE,
  );

  $form['dvg_logger_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject'),
    '#default_value' => variable_get('dvg_logger_email_subject', ''),
    '#required' => TRUE,
  );

  $form['dvg_logger_attempts_digest'] = array(
    '#type' => 'select',
    '#title' => t('Period'),
    '#description' => t('Select when to send an email containing logs of types below.'),
    '#options' => array(
      '' => t('Do not send emails'),
      'daily' => t('Daily'),
      'weekly' => t('Weekly (first day of the week)'),
      'monthly' => t('Monthly (first day of the month)'),
    ),
    '#default_value' => variable_get('dvg_logger_attempts_digest', ''),
  );
  $form['dvg_logger_email_types_filter'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#title' => t('Only email reports of these types'),
    '#options' => $types_filter,
    '#default_value' => variable_get('dvg_logger_email_types_filter', $types_filter_default),
    '#required' => FALSE,
  );

  $form['dvg_logger_cleanup_limit_days'] = array(
    '#type' => 'select',
    '#title' => t('Remove encrypted logs after:'),
    '#options' => array(
      '1' => t('One day'),
      '2' => t('Two days'),
      '3' => t('Three days'),
      '4' => t('Four days'),
      '5' => t('Five days'),
      '6' => t('Six days'),
      '7' => t('One week'),
      '14' => t('Two weeks'),
      '21' => t('Three weeks'),
      '31' => t('One month'),
      '61' => t('Two months'),
      '92' => t('Three months'),
    ),
    '#default_value' => variable_get('dvg_logger_cleanup_limit_days', '7'),
    '#required' => TRUE,
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * Validate callback that validates one or more email addresses.
 */
function _dvg_logger_validate_emails($form_element, &$form_state) {
  $value = str_replace(' ', '', $form_element['#value']);
  $emails = explode(',', $value);

  if (!empty($emails)) {
    foreach ($emails as $email) {
      if (!valid_email_address($email)) {
        form_error($form_element, t('%field contains an invalid email address: @email', array('%field' => $form_element['#title'], '@email' => $email)));
      }
    }
  }
}

/**
 * DVG Logger list logs form.
 */
function dvg_logger_list_form($form, &$form_state) {

  $header = [
    ['data' => t('Id'), 'field' => 'lid'],
    ['data' => t('Type'), 'field' => 'type'],
    ['data' => t('Message')],
    ['data' => t('Hostname'), 'field' => 'hostname'],
    ['data' => t('Timestamp'), 'field' => 'timestamp', 'sort' => 'desc'],
  ];

  $query = db_select('dvg_logger', 'dl');

  $query->fields('dl', [
    'lid',
    'type',
    'message',
    'hostname',
    'timestamp',
    'encrypted',
  ])
    ->extend('TableSort')
    ->extend('PagerDefault')
    ->orderByHeader($header)
    ->limit(25);

  if (isset($form_state['input']['filter_type']) && $form_state['input']['filter_type'] != 'all') {
    $query->condition('dl.type', $form_state['input']['filter_type'], '=');
  }

  $result = $query->execute();

  $form = [];
  $types = [];
  $types['all'] = t('All');
  $types = array_merge($types, _dvg_logger_get_types());

  $form['filters'] = [
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Filters'),
  ];
  $form['filters']['filter_type'] = [
    '#type' => 'select',
    '#options' => $types,
    '#title' => t('Type'),
  ];
  $form['filters']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );

  $rows = [];
  foreach ($result as $row) {
    $message = $row->message;
    $link_path = 'admin/reports/dvg-logger/' . $row->lid;
    if ($row->encrypted) {
      try {
        $message = decrypt($message);
      }
      catch (Exception $ignored) {
        $link = l(t('View details'), $link_path);
        watchdog('dvg_logger', 'Encountered corrupt message for log !id', array('!id' => $row->lid), WATCHDOG_ERROR, $link);
        $message = t("Couldn't decrypt message.");
      }
    }
    if (strlen($message) > 100) {
      $message = substr($message, 0, 100) . '...';
    }
    $rows[] = [
      $row->lid,
      $row->type,
      l($message, $link_path),
      $row->hostname,
      format_date($row->timestamp, 'custom', 'd-M-Y H:i:s', date_default_timezone()),
    ];
  }

  $form['table'] = [
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('Table has no rows!'),
  ];

  $form['pager'] = ['#markup' => theme('pager')];

  return $form;
}

/**
 * Submit handler.
 */
function dvg_logger_list_form_submit($form, &$form_state) {
  $form_state['filters']['type'] = $form_state['values']['filter_type'];
  $form_state['rebuild'] = TRUE;
}

/**
 * Event detail.
 *
 * @param int $lid
 *   Event id.
 *
 * @return string
 *   A build array in the
 *   format expected by drupal_render();
 */
function dvg_logger_event($lid) {
  $log = db_query('SELECT * FROM {dvg_logger} dl WHERE dl.lid = :lid', array(':lid' => $lid))->fetchObject();
  if (!$log) {
    return t('No logs found.');
  }
  $message = $log->message;
  if ($log->encrypted) {
    $message = decrypt($log->message);
  }
  $rows = array(
    array(
      array('data' => t('Type'), 'header' => TRUE),
      $log->type,
    ),
    array(
      array('data' => t('Date'), 'header' => TRUE),
      format_date($log->timestamp, 'long'),
    ),
    array(
      array('data' => t('Hostname'), 'header' => TRUE),
      check_plain($log->hostname),
    ),
    array(
      array('data' => t('Message'), 'header' => TRUE),
      $message,
    ),
  );
  $build['dvg_logger_detail'] = array(
    '#theme' => 'table',
    '#rows' => $rows,
  );

  return $build;

}
