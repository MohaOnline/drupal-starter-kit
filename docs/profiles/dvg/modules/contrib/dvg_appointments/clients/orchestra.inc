<?php

class AppointmentsOrchestraClient extends AppointmentsClientApi {
  protected $timezone = 'UTC';
  protected $api_method = 'rest';
  protected $max_persons = 5;
  protected $branch_id;

  function __construct() {
    $this->url = variable_get('dvg_appointments_orchestra_url', '');
    if (empty($this->url)) {
      throw new Exception('No REST url configured.');
    }
    $this->branch_id = variable_get('dvg_appointments_orchestra_branch_id');
    if (empty($this->branch_id)) {
      throw new Exception('No branch id configured.');
    }
  }

  protected function hash_external_id($email, $birth_date) {
    return md5(strtolower($email) . strtolower($birth_date));
  }

  public static function config_form() {
    $form = array();

    $form['dvg_appointments_orchestra_url'] = array(
      '#title' => t('REST url'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dvg_appointments_orchestra_url', ''),
      '#required' => TRUE,
    );
    $form['dvg_appointments_orchestra_branch_id'] = array(
      '#title' => t('Branch ID'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dvg_appointments_orchestra_branch_id', ''),
      '#required' => TRUE,
    );
    return $form;
  }

  public function get_available_products() {
    $products = array();

    $available_products = $this->call('getAvailableProducts', array(
      'rest_url' => '/calendar-backend/public/api/v1/branches/' . $this->branch_id . '/services',
      'rest_method' => 'GET',
    ));

    foreach ($available_products->serviceList as $product) {
      $max_persons = $product->additionalCustomerDuration == 0 ? 1 : $this->get_max_persons();
      $products[$product->publicId] = array(
        'id' => $product->publicId ,
        'name' => $product->name,
        'duration' => $product->duration,
        'additional_customer_duration' => $product->additionalCustomerDuration,
        'max_persons' => $max_persons,
      );
    }

    uasort($products, array($this, 'sort_available_products'));

    return $products;
  }

  protected function get_available_appointment_days($product_id, $count) {
    $rest_url_parameters['numberOfCustomers'] = $count;
    $available_appointment_days = $this->call('getAvailableAppointmentDays', array(
      'rest_url' => '/calendar-backend/public/api/v1/branches/' . $this->branch_id . '/services/' . $product_id . '/dates',
      'rest_method' => 'GET',
      'rest_url_parameters' => $rest_url_parameters,
    ));
    foreach ($available_appointment_days->dates as &$date) {
      $date .= 'Z';
    }

    return $available_appointment_days->dates;
  }

  protected function get_available_appointment_times($product_id, $count, $date) {
    $rest_url_parameters['numberOfCustomers'] = $count;
    $date = substr($date, 0, strpos($date, 'T'));
    $available_appointment_times = $this->call('getAvailableAppointmentTimes', array(
      'rest_url' => '/calendar-backend/public/api/v1/branches/' . $this->branch_id . '/services/' . $product_id . '/dates/' . $date . '/times',
      'rest_method' => 'GET',
      'rest_url_parameters' => $rest_url_parameters,
    ));
    foreach ($available_appointment_times->times as &$time) {
      $time .= ':00.000Z';
    }
    return $available_appointment_times->times;
  }

  /**
   * Function to retrieve all times by date.
   *
   * @param array $product_ids
   *   Array of product id's.
   * @param array $durations
   *   Durations of an appointment.
   * @param array $persons
   *   Number of persons.
   * @param string $date
   *   the date. (2017-02-28T00:00:00.000Z).
   *
   * @return array
   *   array of all the times of that day.
   */
  public function get_times_by_date($product_ids, $durations, $persons, $date) {
    $dates_all = array();
    foreach ($product_ids as $product_link_id) {
        $date = substr($date, 0, strpos($date, 'T')) . 'T00:00:00';
        $time = $this->get_available_appointment_times($product_link_id, $persons[$product_link_id], $date);
        if (!isset($dates_all[$product_link_id])) {
          $dates_all[$product_link_id] = array();
          $dates_all[$product_link_id]['dates'] = array();
          $dates_all[$product_link_id]['product_id'] = $product_link_id;
          $dates_all[$product_link_id]['duration'] = $durations[$product_link_id];
        }
        if (!isset($dates_all[$product_link_id]['dates'][$date])) {
          $dates_all[$product_link_id]['dates'][$date] = is_array($time) ? $time : array($time);
        }
    }

    $right_dates_time = $this->_check_consecutive_appointments($dates_all);
    return $right_dates_time;
  }

  public function get_customer_appointments($vars) {
    $get_customer_appointment = $this->call('getGovAppointmentDetails', array(
      'rest_url' => '/calendar-backend/public/api/v1/appointments/' . $vars['appointment_id'],
      'rest_method' => 'GET',
    ), (isset($vars['reset']) ? $vars['reset'] : FALSE));
    if (isset($get_customer_appointment->appointment->services[0]->publicId) && strtolower($get_customer_appointment->appointment->customers[0]->email) == strtolower($vars['email'])) {
      $products = $this->get_available_products();
      $return = array(
        'appointmentId' => $vars['appointment_id'],
        'appointmentDate' => $get_customer_appointment->appointment->start,
        'appointmentTime' => $get_customer_appointment->appointment->start,
        'appointmentLength' => $products[$get_customer_appointment->appointment->services[0]->publicId]['duration'],
        'productName' => $products[$get_customer_appointment->appointment->services[0]->publicId]['name'],
      );
      return $return;
    }

    return FALSE;
  }

  public function delete_appointment(DvgAppointment $appointment) {
    $success = TRUE;
    foreach ($appointment->getRemoteAppointmentIds() as $remote_id) {
      $delete_appointment = $this->call('deleteAppointment', array(
        'rest_url' => '/calendar-backend/public/api/v1/appointments/' . $remote_id,
        'rest_method' => 'DELETE',
      ));
      if (isset($delete_appointment->msg)) {
        watchdog('dvg_appointments', 'Delete appointment failed with code @code', array('@code' => $delete_appointment->msg), WATCHDOG_ERROR);
      }
      $success = $success && !isset($delete_appointment->msg);
    }

    return $success;
  }

  public function book_appointment($appointment_data, $book_data) {
    if (isset($book_data['email'])) {
      $book_data['externalId'] = $this->hash_external_id($book_data['email'], (isset($book_data['dateOfBirth'])) ? $book_data['dateOfBirth'] : '');
    }
    else {
      watchdog('dvg_appointments', 'Booking failed: invalid externalId: @book_data', array('@book_data' => var_export($book_data, 1)), WATCHDOG_ERROR);
      throw new Exception('Booking failed. No email found.', DVG_APPOINTMENTS_ERRORCODE_UNKNOWN);
    }
    $base_info = array(
      'firstName' => (isset($book_data['firstName'])) ? $book_data['firstName'] : '',
      'lastName' => (isset($book_data['lastName'])) ? $book_data['lastName'] : '',
      'phone' => (isset($book_data['phone'])) ? $book_data['phone'] : '',
      'email' => (isset($book_data['email'])) ? strtolower($book_data['email']) : '',
      'dateOfBirth' => (isset($book_data['dateOfBirth'])) ? $book_data['dateOfBirth'] : '',
      'externalId' => (isset($book_data['externalId'])) ? $book_data['externalId'] : '',
    );

    $appointment = new DvgAppointment();
    $appointment->setStart(DateTime::createFromFormat('U', $appointment_data['datetime']));
    $start_time = $appointment_data['datetime'];
    foreach ($appointment_data['products'] as $product) {
      $duration = $product['duration'] * 60 + ($product['additional_customer_duration'] * ($product['count'] - 1) * 60);
      $end_date = dvg_appointments_format_date($start_time + $duration, 'custom', 'c');
      $customers = array();
      for ($i = 0; $i < $product['count']; $i++) {
        $customers[] = $base_info;
      }
      $notes = format_plural(
        $product['count'],
        'Booked through the Internet for one person.',
        'Booked through the Internet for @count persons.'
      );

      $date = dvg_appointments_format_date($start_time, 'custom', 'Y-m-d');
      $time = dvg_appointments_format_date($start_time, 'custom', 'H:i');
      $book_appointment = $this->call('bookGovAppointment', array(
        'rest_url' => '/calendar-backend/public/api/v2/branches/' . $this->branch_id . '/services/' . $product['id'] . '/dates/' . $date . '/times/' . $time . '/book',
        'rest_method' => 'POST',
        'title' => 'Online booking',
        'notes' => $notes,
        'customers' => $customers,
      ), TRUE);

      if (isset($book_appointment->code) && isset($book_appointment->error)) {

        watchdog('dvg_appointments', 'Booking failed: "@error" (@code). @message', array(
          '@code' => $book_appointment->code,
          '@error' => $book_appointment->error,
          '@message' => $book_appointment->msg,
        ), WATCHDOG_ERROR);
        foreach ($appointment->getRemoteAppointmentIds() as $remote_id) {
          $this->delete_appointment($remote_id);
        }
        if (strpos($book_appointment->msg, 'No resource is available for the selected date and time') !== FALSE) {
          throw new Exception($book_appointment->msg, DVG_APPOINTMENTS_ERRORCODE_DATE_TIME);
        }
        throw new Exception($book_appointment->msg, DVG_APPOINTMENTS_ERRORCODE_UNKNOWN);
      }
      else {
        $appointment->addProduct($product['id'], $product['count'], $product['duration'] * 60, $product['additional_customer_duration'] * 60);
        $appointment->addRemoteAppointmentId($book_appointment->publicId);
      }

      $start_time += $duration;
    }

    if (isset($end_date)) {
      $appointment->setEnd(DateTime::createFromFormat('U', $start_time));
    }

    return $appointment;
  }

  public function get_dates_times($product_ids, $durations, $persons) {
    $dates_all = array();
    foreach ($product_ids as $product_link_id) {
      foreach ($this->get_available_appointment_days($product_link_id, $persons[$product_link_id]) as $date) {
        $date = str_replace('12:00:00.000Z', '00:00:00.000Z', $date);
        $time = $this->get_available_appointment_times($product_link_id, $persons[$product_link_id], $date);
        if (!isset($dates_all[$product_link_id])) {
          $dates_all[$product_link_id] = array();
          $dates_all[$product_link_id]['dates'] = array();
          $dates_all[$product_link_id]['product_id'] = $product_link_id;
          $dates_all[$product_link_id]['duration'] = $durations[$product_link_id];
        }
        if (!isset($dates_all[$product_link_id]['dates'][$date])) {
          $dates_all[$product_link_id]['dates'][$date] = is_array($time) ? $time : array($time);
        }
      }
    }

    $right_dates_time = $this->_check_consecutive_appointments($dates_all);
    return $right_dates_time;
  }

  /**
   * Magic function that check all correct time for appointments.
   */
  protected function _check_consecutive_appointments($dates_all) {

    // First get all possible starting times of the first product
    $starting_times = array();
    $first_dates = array_shift($dates_all);
    foreach ($first_dates['dates'] as $date => $times) {
      foreach ($times as $time) {
        $starting_times[] = array(
          array(
            'duration' => $first_dates['duration'],
            'date' => $date,
            'time' => $time,
          ),
        );
      }
    }

    // Check for each additional product if a timeslot is available at the end of the previous product.
    foreach ($dates_all as $dates) {
      $new_starting_times = array();
      foreach ($starting_times as $date) {
        foreach ($date as $starting_time) {

          // Time to check initial appointment time with duration of the previous product(s).
          $search_time = strtotime($starting_time['time']) + $starting_time['duration'] * 60;

          // If the product can be booked after the previous product add the duration, so the next product can be checked after this product.
          foreach ($dates['dates'][$starting_time['date']] as $time) {
            if ($search_time == strtotime($time)) {
              $starting_time['duration'] = $starting_time['duration'] + $dates['duration'];
              $new_starting_times[] = array($starting_time);
            }
          }
        }
      }

      $starting_times = $new_starting_times;
    }

    return $starting_times;
  }

  public static function get_book_fields() {
    return array(
      'firstName' => 'First Name',
      'lastName' => 'Last Name',
      'phone' => 'Telephone',
      'email' => 'Email',
      'dateOfBirth' => 'Birthdate',
    );
  }

  protected function get_cache_expire() {
    return array(
      'getAvailableAppointmentTimes' => 10,
      'getAvailableProducts' => 60 * 60 * 24,
    );
  }
}
