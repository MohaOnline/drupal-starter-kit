<?php

abstract class AppointmentsClientApi {

  protected $soap_client;
  protected $timezone;
  protected $locations;
  protected $max_persons = 10;
  protected $url;

  abstract function __construct();

  protected function call($function, $args = array(), $reset = FALSE) {
    try {
      $start = microtime(TRUE);

      // Allow other modules to alter the function arguments.
      drupal_alter('dvg_appointments_call_args', $args, $function);

      ksort($args);
      $cid = 'dvg_appointments__' . $function . '__' . substr(md5(serialize($args)), 0, 8);
      if (module_exists('domain')) {
        $current_domain = domain_get_domain();
        $cid .= ':' . $current_domain['machine_name'];
      }

      $cache = cache_get($cid);
      if (!$reset && $cache) {
        // Make sure the cache isn't expired.
        $expired = REQUEST_TIME > $cache->expire;
        if (!$expired) {
          return $cache->data;
        }
      }
      else {
        // Init expired as TRUE.
        $expired = TRUE;
      }

      if ($this->api_method == 'soap') {
        $return = $this->soap_client->{$function}($args);
      }
      elseif ($this->api_method == 'rest') {
        $rest_url = $this->url . $args['rest_url'];
        $method = $args['rest_method'];
        $rest_url_parameters = (isset($args['rest_url_parameters'])) ? $args['rest_url_parameters'] : array();

        // Remove REST information from the data.
        unset(
          $args['rest_method'],
          $args['rest_url'],
          $args['rest_url_parameters']
        );

        $options = $this->get_options($args, $method);
        drupal_alter('dvg_appointments_rest_options', $options, $function, $rest_url);
        $response = drupal_http_request(url($rest_url, array('query' => $rest_url_parameters)), $options);
        $return = json_decode($response->data);
        if (isset($response->code) && isset($response->error)) {
          $return->code = $response->code;
          $return->error = $response->error;
        }
      }

      $duration = microtime(TRUE) - $start;
      if ($duration >= variable_get('dvg_appointments_api_duration_threshold', 2)) {
        watchdog('dvg_appointments', '@method duration exceeded threshold: @durations', array(
          '@method' => $function,
          '@duration' => round($duration, 2),
        ), WATCHDOG_WARNING);
      }

      // Get the expiration times of this function.
      $expires = $this->get_cache_expire();
      $function_expire = isset($expires[$function]) ? $expires[$function] : variable_get('dvg_appointments__cache_default', 3600);

      // The default function expire is FALSE. When this variable is set to
      // a expire date and the return is not empty then the cache will be reset.
      $custom_function_expire = variable_get('dvg_appointments__cache__' . $function, FALSE);

      // Set expired to false because we don't want to override
      // cache with empty values.
      if ($function == 'getAvailableProducts' && empty($return)) {
        watchdog('appointment_api', 'Empty return value: @function', array('@function' => $function), WATCHDOG_WARNING);
        $custom_function_expire = FALSE;
        $expired = FALSE;
      }

      // Store the results in cache if the cache hasn't been set.
      // When custom_function_expire is set reset the cache.
      if (!$cache || $expired || $custom_function_expire) {
        $expire = REQUEST_TIME + ($custom_function_expire ? $custom_function_expire : $function_expire);
        cache_set($cid, $return, 'cache', $expire);
      }

      return $return;
    }
    catch (SoapFault $f) {
      watchdog('appointment_api', 'Soap Fault: @error', array('@error' => var_export($f, TRUE)), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  public function get_options($args, $method) {
    $data = drupal_json_encode($args);
    $options = array(
      'headers' => array(
        'Content-Type' => 'application/json',
      ),
      'method' => $method,
    );
    // Only POST requests should have a data header (content body) set.
    if ($method == 'POST') {
      $options['data'] = $data;
    }
    return $options;
  }

  public function get_timezone() {
    return $this->timezone;
  }

  /**
   * Get the endpoint Url.
   *
   * @return mixed
   *   the url or null.
   */
  public function getUrl() {
    return $this->url;
  }

  protected function sort_available_products($a, $b) {
    return strcmp($a['name'], $b['name']);
  }

  abstract public static function get_book_fields();

  public function set_booking_value($field, $value) {
    return $value;
  }

  public function get_max_persons() {
    return $this->max_persons;
  }

  /**
   * @return array
   *   List of products available for appointments.
   */
  abstract public function get_available_products();

  /**
   * Filtered version of get_available_products().
   * If a location is used any products not available at any locations are removed.
   *
   * @return array
   *   List of products available for appointments.
   */
  public function get_filtered_available_products() {
    // Default implementation.
    return $this->get_available_products();
  }

  /**
   * @return array
   *   List of locations available for appointments.
   */
  public function get_gov_locations() {
    return array();
  }

  /**
   * Filter the available locations by the given products.
   *
   * @param $products array
   *   List of products used for filtering.
   *
   * @return array
   *   List of locations combined with available
   *   and unavailable products for each location.
   */
  public function get_filtered_locations($products) {
    return array();
  }

  /**
   * @param $location_id int
   *   ID of the location to get details about.
   *
   * @return bool|array
   *   FALSE if not available.
   *   Array with the location object.
   */
  public function get_location_details($location_id) {
    if ($this->get_gov_locations()) {
      if (isset($this->locations[$location_id])) {
        return $this->locations[$location_id];
      }
    }

    return FALSE;
  }

  public function set_location($location_id) {
    $this->location_id = $location_id;
  }

  public function get_product_details($product_id) {
    return '';
  }

  abstract public function get_customer_appointments($vars);

  abstract public function delete_appointment(DvgAppointment $appointment);

  /**
   * @param array $appointment_data
   *
   * @param array $book_data
   *
   * @return DvGAppointment
   */
  abstract public function book_appointment($appointment_data, $book_data);

  abstract public function get_dates_times($product_ids, $durations, $persons);

  /**
   * Get the dates available using the product_ids.
   *
   * @param array $product_ids
   *   Array of product id's.
   * @param int $days_limit
   *   Limit number of days.
   * @param int $start
   *   Start date.
   *
   * @deprecated Please use get_dates_by_products() instead.
   */
  public function get_dates($product_ids, $days_limit = 0, $start = 0){
    watchdog("DvG Appointments", "Your client doesn't support this functionality (function: get_dates)");
    return array();
  }

  /**
   * Get the dates available for selected products.
   *
   * @param array $products
   *   As returned from dvg_appointments_get_selected_products.
   * @param int $days_limit
   *   Limit number of days.
   * @param int $start
   *   Start date.
   *
   * @return array
   *   Returns an array of dates.
   */
  public function get_dates_by_products($products, $days_limit = 0, $start = 0) {
    watchdog("DvG Appointments", "Your client doesn't support this functionality (function: get_dates_by_products)");
    return array();
  }

  /**
   * Get the timeslots available for a given date.
   *
   * @param $product_ids
   * @param int $days_limit
   * @param int $start
   * @return array
   */
  public function get_times_by_date($product_ids, $durations, $persons, $date){
    watchdog("DvG Appointments", "Your client doesn't support this functionality (function: get_times_by_date)");
    return array();
  }

  protected function get_cache_expire() {
    return array();
  }
}
