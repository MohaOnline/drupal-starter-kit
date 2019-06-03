<?php

/**
 * @file
 * Hooks provided by the DVG Appointments API.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Alter the DVG Appointment table rows.
 *
 * Modules may implement this hook to alter the DVG Appointment table rows.
 *
 */
function hook_dvg_appointments_table_rows_alter(&$rows, $appointment) {
  foreach ($rows as $key => $row) {
    if (count($row) == 2) {
      list($label, $value) = $row;
      if ($label['data'] == t('Appointment id')) {
        $rows[$key][1] = $appointment->getRemoteAppointmentIds()[0];
      }
    }
  }
}

/**
 * Alter the DVG Appointment to be cancelled.
 *
 * Modules may implement this hook to alter the DVG Appointment to be cancelled.
 *
 */
function hook_dvg_appointments_cancel_appointment_alter(&$appointment, $remote_id) {
  $result = db_query('SELECT dvg_appointments_appointments.id, nid, sid, appointment_start, appointment_end FROM {dvg_appointments_appointments}, {dvg_appointments_remote_ids}  WHERE local_id = dvg_appointments_appointments.id AND remote_id = :remote_id', array(':remote_id' => $remote_id))
    ->fetch();
  if ($result) {
    $appointment = new DvgAppointment();

    $appointment->setId($result->id);
    $appointment->setNodeId($result->nid);
    $appointment->setSubmissionId($result->sid);

    $UTC = new DateTimeZone('UTC');
    $appointment->setStart(DateTime::createFromFormat('Y-m-d?H:i:s', $result->appointment_start, $UTC));
    $appointment->setEnd(DateTime::createFromFormat('Y-m-d?H:i:s', $result->appointment_end, $UTC));
    // Add remotes.
    $remotes = db_query('SELECT remote_id FROM {dvg_appointments_remote_ids} WHERE local_id = :id', array(
      ':id' => $result->id,
    ));
    foreach ($remotes as $remote) {
      $appointment->addRemoteAppointmentId($remote->remote_id);
    }
    // Add products.
    $products = db_query('SELECT product, count, duration, additional_customer_duration FROM {dvg_appointments_products} WHERE local_id = :id', array(
      ':id' => $result->id,
    ));
    foreach ($products as $product) {
      $appointment->addProduct($product->product, $product->count, $product->duration, $product->additional_customer_duration);
    }
  }
}

/**
 * Alter the DVG Appointment to be cancelled after we've queried the remote client api.
 *
 * Modules may implement this hook to alter the DVG Appointment to be cancelled.
 *
 */
function hook_dvg_appointments_cancel_appointment_after_remote_alter(&$appointment, &$remote_appointment) {
  // Remote appointment has more data than the local appointment, so transfer the info.
  if ($remote_appointment && $appointment->getId() === 0) {
    $date_str = $remote_appointment->appointmentDate;
    $time_str = $remote_appointment->appointmentTime;
    $date = substr($date_str, 0, strpos($date_str, 'T'));
    $time = substr($time_str, strpos($time_str, 'T'));
    $new_start = new DateTime($date . $time);

    if ($new_start) {
      $new_end = clone $new_start;
      $new_end->add(new DateInterval('PT' . $remote_appointment->appointmentLength . 'M'));
      $appointment->setStart($new_start);
      $appointment->setEnd($new_end);
    }
    $appointment->addProduct($remote_appointment->productLinkId, 1, $remote_appointment->appointmentLength * 60, 0);
  }
}

/**
 * Alter the DVG Appointment products to be shown on the client form.
 *
 * Modules may implement this hook to alter the DVG Appointment products that are shown on teh client form.
 *
 */
function hook_dvg_appointments_available_products_alter(&$available_products, $element, $form_state, $form) {
  // Default filter all products starting with an A.
  $filter = variable_get('dvg_appointments_available_products_filter_default', 'A');
  // Allow form specific filtering.
  $nid = $form['#node']->nid;
  $form_specific_filter = variable_get('dvg_appointments_available_products_filter_form_' . $nid, FALSE);
  if (!empty($form_specific_filter)) {
    $filter = $form_specific_filter;
  }
  if (!empty($filter)) {
    $length = strlen($filter);
    foreach ($available_products as $key => $value) {
      if (!(substr($key, 0, $length) === $filter)) {
        unset($available_products[$key]);
      }
    }
  }
}

/**
 * Alter the options used for the SoapClient.
 * E.g. add 2 way SSL.
 *
 * @param array $options
 *   Options that are passed to the SoapClient.
 */
function hook_dvg_appointments_soap_options_alter(&$options) {
  $opts = array(
    'ssl' => array(
      'verify_peer' => FALSE,
      'verify_peer_name' => FALSE,
      'local_cert' => '/path/to/my/fancy/cert',
      'passphrase' => 'wow so_s3cRet!',
    ),
  );
  $options['local_cert'] = $opts['ssl']['local_cert'];
  $options['passphrase'] = $opts['ssl']['passphrase'];
  $options['stream_context'] = stream_context_create($opts);
}

/**
 * Alter the options used for the drupal_http_request.
 * E.g. add 2 way SSL.
 *
 * @param array  $options
 *   Options that are passed to the drupal_http_request function.
 * @param string $function
 *   Function the REST API is calling.
 * @param string $rest_url
 *   REST URL that will be called.
 */
function hook_dvg_appointments_rest_options_alter(&$options, $function, $rest_url) {
  // See available options: https://secure.php.net/manual/en/context.ssl.php
  $opts = array(
    'ssl' => array(
      'verify_peer' => FALSE,
      'verify_peer_name' => FALSE,
      'local_cert' => '/path/to/my/fancy/cert',
      'passphrase' => 'wow so_s3cRet!',
    ),
  );
  $options['context'] = stream_context_create($opts);
}

/**
 * @} End of "addtogroup hooks".
 */
