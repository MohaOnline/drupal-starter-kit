<?php

class AppointmentsGPlanClient extends AppointmentsClientApi {

  protected $location_id;
  protected $products;
  protected $api_method= 'soap';

  const APPOINTMENT_STATUS_BOOKED                      =  0;
  const APPOINTMENT_STATUS_DATE_TIME_PAST              =  1;
  const APPOINTMENT_STATUS_INVALID_PRODUCT             =  2;
  const APPOINTMENT_STATUS_INVALID_LOCATION            =  3;
  const APPOINTMENT_STATUS_INVALID_CLIENT_NAME         =  4;
  const APPOINTMENT_STATUS_NO_PLANNER_OBJECT_AVAILABLE =  5;
  const APPOINTMENT_STATUS_PLANNER_OBJECT_OVERLAP      =  6;
  const APPOINTMENT_STATUS_INVALID_PRODUCT_NUMBER      =  7;
  const APPOINTMENT_STATUS_INVALID_PRODUCT_COMBINATION = 11;

  function __construct() {
    $this->url = variable_get('dvg_appointments_gplan_url', '');
    if (empty($this->url)) {
      throw new Exception('No WSDL configured.');
    }

    $this->location_id = variable_get('dvg_appointments_gplan_location_id');
    if (empty($this->location_id)) {
      throw new Exception('No location id configured.');
    }

    $options = variable_get('dvg_appointments_soap_options', array());
    // This triggers Exceptions by soap client.
    $options['exceptions'] = TRUE;

    drupal_alter('dvg_appointments_soap_options', $options);

    try {
      $this->soap_client = new SoapClient($this->url, $options);
    }
    catch (SoapFault $soapFault) {
      throw new Exception('Appointments WSDL is not available.');
    }
  }

  public static function config_form() {
    $form = array();

    $form['dvg_appointments_gplan_url'] = array(
      '#title' => t('WSDL url'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dvg_appointments_gplan_url', ''),
      '#required' => TRUE,
    );
    $form['dvg_appointments_gplan_location_id'] = array(
      '#title' => t('Location ID'),
      '#type' => 'textfield',
      '#default_value' => variable_get('dvg_appointments_gplan_location_id', ''),
      '#required' => TRUE,
    );
    $form['dvg_appointments_get_productinfo_from_webservice'] = array(
      '#title' => t('Get product info from Gplan webservice'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('dvg_appointments_get_productinfo_from_webservice', ''),
      '#required' => FALSE,
    );

    return $form;
  }

  public function get_available_products() {
    if ($this->products) {
      return $this->products;
    }

    $this->products = array();

    if ($available_products = $this->call('getGovAvailableProducts')) {
      foreach ($available_products->products as $product) {
        $this->products[$product->productId] = array(
          'id' => $product->productId,
          'name' => $product->productDesc,
          'duration' => 0,
        );
      }

      foreach ($this->products as $key => $product) {
        $details = $this->call('getGovProductDetails', array('productID' => $key));
        if (!empty($details->out->appDuration)) {
          $this->products[$key]['duration'] = $details->out->appDuration;
        }
      }
    }

    uasort($this->products, array($this, 'sort_available_products'));

    return $this->products;
  }

  public function get_product_details($product_id) {
    $productDetails = $this->call('getGovProductDetails', array(
      'productID' => $product_id
    ), TRUE);

    if(isset($productDetails->out)){
      return base64_decode($productDetails->out->requisites);
    }
    return '';
  }

  protected function get_products_duration($product_link_id) {
    if (!is_array($product_link_id)) {
      $product_link_id  = array($product_link_id);
    }
    $products = $this->get_available_products();
    $total_appointLength = 0;
    foreach ($product_link_id as $product_id) {
      $total_appointLength += $products[$product_id]['duration'];
    }

    return $total_appointLength;
  }

  public function get_available_appointment_days($product_id) {
    $available_days = array();
    if (!is_array($product_id)) {
      $product_id = array($product_id);
    }

    $product_ids = implode(',', $product_id);
    $available_appointment_days = $this->call('getGovAvailableDays', array(
      'locationID' => $this->location_id,
      'productID' => $product_ids,
      'startDate' => dvg_appointments_format_date(REQUEST_TIME, 'custom', 'Y-m-d'),
      'endDate' => dvg_appointments_format_date(REQUEST_TIME + (variable_get('dvg_appointments__end_date', 8) * 86400), 'custom', 'Y-m-d'),
      'appDuration' => $this->get_products_duration($product_id),
    ));

    if (!empty($available_appointment_days->dates)) {
      $available_days = array_merge($available_days, (array)$available_appointment_days->dates);
    }

    return $available_days;
  }

  public function get_available_appointment_times($product_id, $date) {
    $available_times = array();
    if (!is_array($product_id)) {
      $product_id = array($product_id);
    }
    $product_ids = implode(',', $product_id);
    $available_appointment_times = $this->call('getGovAvailableTimesPerDay', array(
      'locationID' => $this->location_id,
      'productID' => $product_ids,
      'date' => $date,
      'appDuration' => $this->get_products_duration($product_id),
    ));

    if (!empty($available_appointment_times->times)) {
      $available_times = array_merge($available_times, (array)$available_appointment_times->times);
    }

    return $available_times;
  }

  public function get_customer_appointments($vars) {
    $get_customer_appointment = $this->call('getGovAppointmentDetails', array(
      'appID' => $vars['appointment_id'],
    ), (isset($vars['reset']) ? $vars['reset'] : FALSE));
    if (isset($get_customer_appointment->appointment->productID) && strtolower($get_customer_appointment->appointment->clientMail) == strtolower($vars['email'])) {
      $products = $this->get_available_products();
      $return = array(
        'appointmentId' => $vars['appointment_id'],
        'appointmentDate' => $get_customer_appointment->appointment->appStartTime,
        'appointmentTime' => $get_customer_appointment->appointment->appStartTime,
        'appointmentLength' => $products[$get_customer_appointment->appointment->productID]['duration'],
        'productName' => $products[$get_customer_appointment->appointment->productID]['name'],
      );
      return $return;
    }

    return FALSE;
  }

  public function delete_appointment(DvgAppointment $appointment) {
    $success = TRUE;
    foreach ($appointment->getRemoteAppointmentIds() as $remote_id) {
      $delete_appointment = $this->call('deleteGovAppointment', array(
        'appID' => $remote_id,
      ), TRUE);
      $success = $success && $delete_appointment->updateStatus == 0;
    }

    return $success;
  }

  public function book_appointment($appointment_data, $book_data) {
    $base_info = array(
      'locationID' => $this->location_id,
      'clientInitials' => (isset($book_data['clientInitials'])) ? $book_data['clientInitials'] : '',
      'clientLastName' => (isset($book_data['clientLastName'])) ? $book_data['clientLastName'] : '',
      'clientAddress' => (isset($book_data['clientAddress'])) ? $book_data['clientAddress'] : '',
      'clientCity' => (isset($book_data['clientCity'])) ? $book_data['clientCity'] : '',
      'clientPostalCode' => (isset($book_data['clientPostalCode'])) ? $book_data['clientPostalCode'] : '',
      'clientCountry' => (isset($book_data['clientCountry'])) ? $book_data['clientCountry'] : '',
      'clientTel' => (isset($book_data['clientTel'])) ? $book_data['clientTel'] : '',
      'clientMail' => (isset($book_data['clientMail'])) ? strtolower($book_data['clientMail']) : '',
      'clientDateOfBirth' => (isset($book_data['clientDateOfBirth'])) ? $book_data['clientDateOfBirth'] : '',
      'clientSex' => (isset($book_data['clientSex'])) ? $book_data['clientSex'] : '',
      'isClientVerified' => 'true',
    );

    $appointment = new DvgAppointment();
    $appointment->setStart(DateTime::createFromFormat('U', $appointment_data['datetime']));

    $start_time = $appointment_data['datetime'];
    foreach ($appointment_data['products'] as $product) {
      for ($p = 1; $p <= $product['count']; $p++) {
        $duration = $product['duration'] * 60;
        $start_date = dvg_appointments_format_date($start_time, 'custom', 'c');
        $end_date = dvg_appointments_format_date($start_time + $duration, 'custom', 'c');

        $data = array(
          'productID' => ($product['id']) ? $product['id'] : '',
          'appStartTime' => $start_date,
          'appEndTime' => $end_date,
        ) + $base_info;

        $book_appointment = $this->call('bookGovAppointment', array(
          'appDetail' => $data,
        ), TRUE);

        if (empty($book_appointment->appID) || $book_appointment->updateStatus != self::APPOINTMENT_STATUS_BOOKED) {
          $date_time_error = in_array($book_appointment->updateStatus, array(
            self::APPOINTMENT_STATUS_NO_PLANNER_OBJECT_AVAILABLE,
            self::APPOINTMENT_STATUS_DATE_TIME_PAST
          ));
          $error_code = ($date_time_error) ? DVG_APPOINTMENTS_ERRORCODE_DATE_TIME : DVG_APPOINTMENTS_ERRORCODE_UNKNOWN;
          watchdog('dvg_appointments', 'Booking failed with code @code', array('@code' => $book_appointment->updateStatus), WATCHDOG_ERROR);

          // Delete all previous remote appointments.
          foreach ($appointment->getRemoteAppointmentIds() as $remote_id) {
            $this->delete_appointment($remote_id);
          }

          throw new Exception('Booking failed. No appointment id.', $error_code);
        }
        else {
          $appointment->addProduct($product['id'], 1, $duration);
          $appointment->addRemoteAppointmentId($book_appointment->appID);
        }
        $start_time += $duration;
      }
    }

    if (isset($end_date)) {
      $appointment->setEnd(DateTime::createFromFormat('U', $start_time));
    }

    return $appointment;
  }

  public function get_dates_times($product_ids, $durations, $persons) {
    $dates_all = array();
    foreach ($product_ids as $product_link_id) {
      foreach ($this->get_available_appointment_days($product_link_id) as $date) {
        $time = $this->get_available_appointment_times($product_link_id, $date);
        if (!isset($dates_all[$product_link_id])) {
          $dates_all[$product_link_id] = array();
          $dates_all[$product_link_id]['dates'] = array();
          $dates_all[$product_link_id]['product_id'] = $product_link_id;
          $dates_all[$product_link_id]['duration'] = $durations[$product_link_id];
        }
        $date .= 'T00:00:00.000Z';
        if (!isset($dates_all[$product_link_id]['dates'][$date])) {
          $dates_all[$product_link_id]['dates'][$date] = is_array($time) ? $time : array($time);
        }
      }
    }

    $right_dates_time = $this->_appointments_magic($dates_all, $persons);

    return $right_dates_time;
  }

  /**
   * Magic function that check all correct time for appointments.
   */
  protected function _appointments_magic($dates_all, $persons = array()) {
    $available_dates = $this->check_dates($dates_all);

    $good_times = array();
    $dates = array_shift($dates_all);
    foreach ($available_dates as $date) {
      foreach ($dates['dates'][$date] as $time) {
        $time = strtotime($time);
        $person_is_ok = array();
        $appointments = array();
        for ($i = 0; $i < $persons[$dates['product_id']]; $i++) {
          // Why was ($i+1) ? Deleted because 9;00 was not visible
          $search_time = $time + $dates['duration'] * 60 * $i;
          foreach ($dates['dates'][$date] as $_time) {
            if ($search_time == strtotime($_time)) {
              $person_is_ok[$i] = TRUE;
              $appointments[$_time] = array(
                'product_id' => $dates['product_id'],
                'duration' => $dates['duration'],
                'date' => $date,
                'time' => $_time,
              );
            }
          }
        }

        // Is this ok? Then check for person 1
        $is_ok = TRUE;
        for ($p = 0; $p < $persons[$dates['product_id']]; $p++) {
          if (!isset($person_is_ok[$p])) {
            $is_ok = FALSE;
          }
        }

        if ($is_ok) {
          $good_times[] = array_values($appointments);
        }
      }
    }

    foreach ($dates_all as $dates) {
      $good_times_temp = $good_times;
      $good_times = array();
      foreach ($available_dates as $date) {
        foreach ($dates['dates'][$date] as $time) {
          foreach ($good_times_temp as $appointments) {
            $last_appointment = end($appointments);

            if ($date == $last_appointment['date'] && (strtotime($time) == strtotime($last_appointment['time']) + $last_appointment['duration'] * 60)) {
              $person_is_ok = array();
              for ($i = 1; $i <= $persons[$dates['product_id']]; $i++) {
                // This part should check the next moment that is available. The for-loop should start.
                $search_time = strtotime($last_appointment['time']) + $last_appointment['duration'] * 60 * ($i);
                foreach ($dates['dates'][$date] as $_time) {
                  if ($search_time == strtotime($_time)) {
                    $person_is_ok[$i] = TRUE;
                    $appointments[] = array(
                      'product_id' => $dates['product_id'],
                      'duration' => $dates['duration'],
                      'date' => $date,
                      'time' => $_time,
                    );
                  }
                }
              }

              // Is this OK? Then start with 1 because that should be added ($i) to the time in the above loop to find the next moment.
              $is_ok = TRUE;
              for ($o = 1; $o <= $persons[$dates['product_id']]; $o++) {
                if (!isset($person_is_ok[$o])) {
                  $is_ok = FALSE;
                }
              }

              if ($is_ok) {
                $good_times[] = $appointments;
              }
            }
          }
        }
      }
    }

    // Returns all good times that need to be selectable.
    return $good_times;
  }

  /**
   * Helper function which check the same date for appointments.
   */
  protected function check_dates($dates_all) {
    $right_dates = array();

    $first_dates = array_shift($dates_all);
    if (isset($first_dates['dates'])) {
      foreach ($first_dates['dates'] as $date => $time) {
        $right_dates[] = $date;
      }

      foreach ($dates_all as $product) {
        $right_dates_temp = $right_dates;
        $right_dates = array();
        foreach ($product['dates'] as $date => $time) {
          if (in_array($date, $right_dates_temp)) {
            $right_dates[] = $date;
          }
        }
      }
    }

    return $right_dates;
  }

  public static function get_book_fields() {
    return array(
      'clientInitials' => 'Initials',
      'clientLastName' => 'Name',
      'clientSex' => 'Sex',
      'clientAddress' => 'Address',
      'clientPostalCode' => 'PostalCode',
      'clientCity' => 'City',
      'clientCountry' => 'Country',
      'clientTel' => 'Telephone',
      'clientMail' => 'Email',
      'clientDateOfBirth' => 'Birthdate',
    );
  }

  protected function get_cache_expire() {
    return array(
      'getGovAvailableTimesPerDay' => 10,
      'getGovAvailableProducts' => 60 * 60 * 24,
      'getGovProductDetails' => 60 * 60 * 24,
    );
  }

}
