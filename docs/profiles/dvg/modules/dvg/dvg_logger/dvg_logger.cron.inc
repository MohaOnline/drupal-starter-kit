<?php

/**
 * @file
 * This is the cron file for dvg logger.
 */

/**
 * Implements hook_cron().
 */
function dvg_logger_cron() {
  _dvg_logger_send_email();
  _dvg_logger_cleanup();
}

/**
 * Send an email.
 */
function _dvg_logger_send_email() {

  $last_run = variable_get('dvg_logger_attempts_email_last_run', 0);
  $period = variable_get('dvg_logger_attempts_digest', '');
  $today = strtotime('today 00:00:00');
  $recipients = _dvg_logger_get_configured_recipient_emails();

  if ($last_run < $today) {
    switch ($period) {
      case 'daily':
        // Yesterday 00:00:00 till 23:59:59.
        $start = strtotime('yesterday 00:00:00');
        $end = strtotime('yesterday 23:59:59');
        break;

      case 'weekly':
        $first_day_of_week = strtotime('this week', REQUEST_TIME);
        if ($last_run < $first_day_of_week) {
          // Previous week from day 1 00:00:00 till day 7 23:59:59.
          $start = strtotime('previous week 00:00:00');
          $end = strtotime('previous week +6days 23:59:59');
        }
        break;

      case 'monthly':
        $first_day_of_month = strtotime('first day of this month', REQUEST_TIME);
        if ($last_run < $first_day_of_month) {
          // Previous month from day 1 00:00:00 till the last day of the month 23:59:59.
          $start = strtotime('first day of previous month 00:00:00');
          $end = strtotime('last day of previous month 23:59:59');
        }
        break;
    }

    if (isset($start) && isset($end)) {
      if ($period && !empty($recipients)) {

        $types = _dvg_logger_get_types();
        $types_filter_default = array(
          'failed_login_attempts' => 'failed_login_attempts',
          'testdata_stuf_bg_attempts' => 'testdata_stuf_bg_attempts',
        );
        $filter_types = variable_get('dvg_logger_email_types_filter', $types_filter_default);

        if (!empty($types)) {
          foreach ($types as $type) {
            if (in_array($type, $filter_types)) {
              dvg_logger_send_email($type->type, $recipients, $start, $end);
              _dvg_logger_register_log('dvg_logger_mail', format_string('E-mailed @period log of type @type', array('@period' => $period, '@type' => $type)));
            }
          }
          // Update the last cron run timestamp.
          variable_set('dvg_logger_attempts_email_last_run', $today);
        }
      }
    }
  }
}

/**
 * Send email with this function.
 *
 * @param string $type
 *   Different types of dvg_loggers.
 * @param array $recipients
 *   The email recipients.
 * @param string $start
 *   Start time.
 * @param string $end
 *   End time.
 */
function dvg_logger_send_email($type, $recipients, $start, $end) {
  global $language;

  $result = db_select('dvg_logger', 'dl')
    ->fields('dl', array('message', 'hostname', 'timestamp'))
    ->condition('timestamp', array($start, $end), 'BETWEEN')
    ->condition('type', $type)
    ->execute();

  if ($result->rowCount()) {
    // Create CSV-attachment.
    $out_filename = 'temporary://' . $type . md5(REQUEST_TIME);
    $out = fopen($out_filename, 'w');
    foreach ($result as $row) {
      $data = (array) $row;
      $data['timestamp'] = date('Y-m-d H:m:s', $data['timestamp']);
      fputcsv($out, $data);
    }
    fclose($out);

    $attachment = array(
      'filecontent' => file_get_contents($out_filename),
      'filename' => $type . '.csv',
      'filemime' => 'text/csv',
    );

    foreach ($recipients as $recipient) {
      $params = array(
        'headers' => array('Content-Type' => 'text/html'),
        'key' => $type,
        'to' => $recipient,
        'attachments' => array($attachment),
      );

      drupal_mail('dvg_logger', $type, $recipient, $language, $params);
    }
  }
}

/**
 * Cleanup encrypted logs after a configurable amount of time.
 */
function _dvg_logger_cleanup() {
  $last_run = variable_get('dvg_logger_attempts_cleanup_last_run', 0);
  $today = strtotime('today 00:00:00');

  if ($last_run < $today) {
    $days = variable_get('dvg_logger_cleanup_limit_days', '31');
    $seconds = $days * 24 * 60 * 60;
    $timestamp = time() - $seconds;
    $count = db_delete('dvg_logger')
      ->condition('encrypted', 1)
      ->condition('timestamp', $timestamp, '<')
      ->execute();
    if ($count > 0) {
      _dvg_logger_register_log('dvg_logger_cleanup', t('Removed @num_items items from this log after @num_days days.', array('@num_items' => $count, '@num_days' => $days)));
    }
    // Update the last cron run timestamp.
    variable_set('dvg_logger_attempts_cleanup_last_run', $today);
  }
}
