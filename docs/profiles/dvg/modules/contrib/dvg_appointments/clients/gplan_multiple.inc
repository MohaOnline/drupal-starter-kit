<?php

class AppointmentsGPlanMultipleClient extends AppointmentsClientApi {

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

  /**
   * @inheritdoc
   */
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
        // Check if locations are there.
        if ($this->get_gov_locations()) {
          // Get the products that are available per location.
          if ($products = $this->call('getGovLocationsForProduct', ['productID' => $key])) {
            $this->products[$key]['locations'] = $products->location;
          }
        }
      }
    }

    uasort($this->products, array($this, 'sort_available_products'));

    return $this->products;
  }

  /**
   * @inheritdoc
   */
  public function get_filtered_available_products() {
    $products = $this->get_available_products();
    $has_locations = !empty($this->get_gov_locations());

    foreach ($products as $key => $product) {
      // Remove product if there is no location.
      if ($has_locations && empty($products[$key]['locations'])) {
        unset($products[$key]);
      }
    }

    return $products;
  }

  /**
   * @inheritdoc
   */
  public function get_gov_locations() {
    if ($this->locations) {
      return $this->locations;
    }

    $this->locations = array();

    // Get all the locations.
    if ($locations = $this->call('getGovLocations')) {
      if (isset($locations->locationIDs) && is_array($locations->locationIDs)) {
        foreach ($locations->locationIDs as $location_id) {
          $this->locations[$location_id] = [
            'id' => $location_id,
          ];
          // Get the locations detail information.
          if ($details = $this->call('getGovLocationDetails', array('locationID' => $location_id))) {
            $this->locations[$location_id]['detail'] = $details->locaties;
          }
        }
      }
    }

    return $this->locations;
  }

  /**
   * @inheritdoc
   *
   * @todo needs refactoring.
   */
  public function get_filtered_locations($products = FALSE) {
    $locations = $this->get_gov_locations();

    // Filter locations by products.
    if ($products) {
      $filter_products = [];
      // Loop all products.
      $product_list = $this->get_filtered_available_products();
      foreach ($product_list as $value => $product) {
        // Loop through given products.
        foreach ($products as $product_id) {
          if ($value == $product_id['id']) {
            $product_locations = array();
            foreach ($product_list[$value]['locations'] as $key => $location) {
              $product_locations[$key] = $location->locationID;
            }
            $filter_products[$value] = $product_locations;
          }
        }
      }
      // Loop all filtered products.
      foreach ($filter_products as $key => $product_location) {
        // Loop all locations.
        foreach ($locations as $key2 => $location) {
          // Check if location id matches product location id.
          if (in_array($location['id'], $product_location)) {
            $locations[$key2]['available_products'][$key] = $key;
          }
          else {
            $locations[$key2]['unavailable_products'][$key] = $key;
          }
        }
      }
    }

    return $locations;
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

  protected function get_products_duration($product_link_id, $durations, $persons) {
    if (!is_array($product_link_id)) {
      $product_link_id  = array($product_link_id);
    }

    $total_appointLength = 0;
    foreach ($product_link_id as $product_id) {
      $total_appointLength += $durations[$product_id] * $persons[$product_id];
    }

    return $total_appointLength;
  }

  public function get_dates_times($product_ids, $durations, $persons) {
    $right_dates_time = array();
    foreach ($this->get_available_appointment_days($product_ids, $durations, $persons) as $date) {
      $times = $this->get_available_appointment_times($product_ids, $durations, $persons, $date);
      $date .= 'T00:00:00.000Z';
      foreach ($times as $time) {
        $right_dates_time[] = array(
          array(
            'date' => $date,
            'time' => $time,
          ),
        );
      }
    }

    return $right_dates_time;
  }

  protected function get_available_appointment_days($product_ids, $durations, $persons) {
    $available_days = array();
    if (!is_array($product_ids)) {
      $product_ids = array($product_ids);
    }

    $full_product_ids = array();
    foreach ($product_ids as $product_id) {
      $expanded_ids = array_fill(0, $persons[$product_id], $product_id);
      $full_product_ids = array_merge($full_product_ids, $expanded_ids);
    }

    $available_appointment_days = $this->call('getGovAvailableDays', array(
      'locationID' => $this->location_id,
      'productID' => implode(',', $full_product_ids),
      'startDate' => dvg_appointments_format_date(REQUEST_TIME, 'custom', 'Y-m-d'),
      'endDate' => dvg_appointments_format_date(REQUEST_TIME + (variable_get('dvg_appointments__end_date', 8) * 86400), 'custom', 'Y-m-d'),
      'appDuration' => $this->get_products_duration($product_ids, $durations, $persons),
    ));

    if (!empty($available_appointment_days->dates)) {
      $available_days = array_merge($available_days, (array)$available_appointment_days->dates);
    }

    return $available_days;
  }

  protected function get_available_appointment_times($product_ids, $durations, $persons, $date) {
    $available_times = array();

    if (!is_array($product_ids)) {
      $product_ids = array($product_ids);
    }

    $full_product_ids = array();
    foreach ($product_ids as $product_id) {
      $expanded_ids = array_fill(0, $persons[$product_id], $product_id);
      $full_product_ids = array_merge($full_product_ids, $expanded_ids);
    }

    $available_appointment_times = $this->call('getGovAvailableTimesPerDay', array(
      'locationID' => $this->location_id,
      'productID' => implode(',', $full_product_ids),
      'date' => $date,
      'appDuration' => $this->get_products_duration($product_ids, $durations, $persons),
    ));

    if (!empty($available_appointment_times->times)) {
      $available_times = array_merge($available_times, (array)$available_appointment_times->times);
    }

    return $available_times;
  }

  protected function get_appointment($appointment_id) {
    $appointment = $this->call('getGovAppointmentDetails', array(
      'appID' => $appointment_id
    ), TRUE);

    if (isset($appointment->appointment)) {
      return $appointment->appointment;
    }
  }

  public function get_customer_appointments($vars) {
    $get_customer_appointment = $this->call('getGovAppointmentDetails', array(
      'appID' => $vars['appointment_id'],
    ), (isset($vars['reset']) ? $vars['reset'] : FALSE));
    if (isset($get_customer_appointment->appointment->productID) && strtolower($get_customer_appointment->appointment->clientMail) == strtolower($vars['email'])) {
      $products = $this->get_available_products();
      $appointmentLength = 0;
      $productNames = array();
      $productIDs = explode(',', $get_customer_appointment->appointment->productID);
      if (is_array($productIDs)) {
        foreach ($productIDs as $key => $productID) {
          $appointmentLength += $products[$productID]['duration'];
          $productNames[] = $products[$productID]['name'];
        }
        $productNameLabel = implode(', ', $productNames);
        $return = array(
          'appointmentId' => $vars['appointment_id'],
          'appointmentDate' => $get_customer_appointment->appointment->appStartTime,
          'appointmentTime' => $get_customer_appointment->appointment->appStartTime,
          'appointmentLength' => $appointmentLength,
          'productName' => $productNameLabel,
        );
        return $return;
      }
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
    $appointment_duration = 0;
    $product_ids = array();
    $start_date = dvg_appointments_format_date($appointment_data['datetime'], 'custom', 'c');
    // Calculate the end datetime by adding the cumulative duration in seconds.
    foreach ($appointment_data['products'] as $product) {
      // A product that has been ordered N times needs to be in the product
      // list N times.
      $expanded_ids = array_fill(0, $product['count'], $product['id']);
      $product_ids = array_merge($product_ids, $expanded_ids);
      $appointment_duration += $product['duration'] * $product['count'] * 60;
    }
    $end_date = dvg_appointments_format_date($appointment_data['datetime'] + $appointment_duration, 'custom', 'c');

    $data = array(
      'productID' => implode(',', $product_ids),
      'appStartTime' => $start_date,
      'appEndTime' => $end_date,
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

    $book_appointment = $this->call('bookGovAppointment', array(
      'appDetail' => $data,
    ), TRUE);

    if (empty($book_appointment->appID) || $book_appointment->updateStatus != self::APPOINTMENT_STATUS_BOOKED) {
      $date_time_error = in_array($book_appointment->updateStatus, array(self::APPOINTMENT_STATUS_NO_PLANNER_OBJECT_AVAILABLE, self::APPOINTMENT_STATUS_DATE_TIME_PAST));
      $error_code = ($date_time_error) ? DVG_APPOINTMENTS_ERRORCODE_DATE_TIME : DVG_APPOINTMENTS_ERRORCODE_UNKNOWN;
      watchdog('dvg_appointments', 'Booking failed with code @code', array('@code' => $book_appointment->updateStatus), WATCHDOG_ERROR);
      throw new Exception('Booking failed. No appointment id.', $error_code);
    }

    $app = $this->get_appointment($book_appointment->appID);

    $appointment = new DvgAppointment();
    $format = 'Y-m-d?H:i:s';
    $appointment->setStart(DateTime::createFromFormat($format, $app->appStartTime));
    $appointment->setEnd(DateTime::createFromFormat($format, $app->appEndTime));
    foreach ($appointment_data['products'] as $product) {
      $appointment->addProduct($product['id'], $product['count'], $product['duration'] * 60);
    }

    $appointment->addRemoteAppointmentId($book_appointment->appID);

    return $appointment;
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
      'getGovAvailableDays' => 60,
      'getGovAvailableTimesPerDay' => 10,
      'getGovAvailableProducts' => 60 * 60 * 24,
      'getGovProductDetails' => 60 * 60 * 24,
      'getGovAvailableProductsByProduct' => 60 * 15,
    );
  }

  public function get_qrcode($remote_appointment_id) {
    $qrcodetext = $this->call('GetAppointmentQRCodeText', array(
      'appID' => $remote_appointment_id,
    ), TRUE);
    return $qrcodetext;
  }

}
