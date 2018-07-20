<?php

/**
 * @file
 * Plugin to provide a baidu geocoder. Currently supports Baidu Geocoder V2.
 */

/**
 * Plugins are described by creating a $plugin array which will be used by the
 * system that includes this file. See the Geocoder Plugins API.
 */
$plugin = array(
  'title' => t("Baidu Geocoder"),
  'description' => t('Geocodes via baidu geocoder'),
  'callback' => 'baidu_map_geocoder_baidu',
  // Supported field types.
  'field_types' => array(
    'text',
    'text_long',
    'addressfield',
    'location',
    'text_with_summary',
    'computed',
    'taxonomy_term_reference',
  ),
  'field_callback' => 'baidu_map_geocoder_baidu_field',
  'settings_callback' => 'baidu_map_geocoder_baidu_form',
  'terms_of_service' => 'http://developer.baidu.com/map/question.htm#qa0013',
);

/**
 * Helper function for the field callback to geocode an address.
 *
 * @param string $address
 *   The full address to be sent to Baidu Map API for geocoding.
 * @param array $options
 *   Array of additional options to handle results from the geocoder.
 *
 * @return object
 *   FALSE is returned if the geocoding operation fails. Otherwise, it returns
 *   a geoPHP Point object based on the geo-coordinates returned by Baidu Map
 *   Geocoder API.
 */
function baidu_map_geocoder_baidu($address, $options = array()) {
  try {
    // The Baidu Map API key is required for Geocoding with Baidu.
    $api_key = variable_get('baidu_map_api_key');
    if (empty($api_key)) {
      throw new Exception(t('Geocoding with the Baidu Geocoder is currently impossible, because <strong>no Baidu Map API Key could be found</strong>. Please configure first a valid <em>Baidu Map API key</em> in the <a href="@bm_admin">Baidu Map settings page</a>, then try submitting the geocoding request again.', array('@bm_admin' => url('admin/config/services/baidu_map'))));
    }

    // Prevent from sending too many requests at a time if there are multiple
    // addresses to be geocoded.
    $delay_trigger = &drupal_static(__FUNCTION__);
    $delay = variable_get('baidu_map_geofield_geocoder_delay', 0);
    if ($delay > 0 && $delay_trigger) {
      usleep($delay * 1000);
    }

    // Load GeoPHP libraries from the geophp module.
    geophp_load();

    // Prepare address to be queried.
    $query = array(
      'address' => $address,
      'ak' => $api_key,
      'output' => 'json',
    );

    // Send query to be geocoded from baidu geocoder V2 API URL.
    $url = url("http://api.map.baidu.com/geocoder/v2/", array('query' => $query));
    $result = drupal_http_request($url);
    $delay_trigger = TRUE;

    if (isset($result->error)) {
      $args = array(
        '@code' => $result->code,
        '@error' => $result->error,
      );
      $msg = t('HTTP request to Baidu Map API failed.\nCode: @code\nError: @error', $args);
      throw new Exception($msg);
    }

    $data = json_decode($result->data);

    // Check the returned status code if there is any.
    if (empty($data) OR !isset($data->status) OR $data->status !== 0) {
      if (!isset($data->status)) {
        // No status returned at all: raise a critical error.
        $msg = t("Sorry, a critical error was returned by Baidu Geocoder for the requested query and no geocoded information could be saved.");
      }
      else {
        // Throw an exception with the error code reported by Baidu Map API.
        $msg = t("Sorry, a critical error was returned by Baidu Geocoder for the requested query and no geocoded information could be saved. Error code: <strong>@status</strong>.<br/> Please check on <a href='@link_status' target='_blank'>Baidu Geocoder API reference website</a> for more information on this error code.", array('@status' => $data->status, '@link_status' => 'http://developer.baidu.com/map/webservice-geocoding.htm#.E6.8E.A5.E5.8F.A3.E7.A4.BA.E4.BE.8A'));
      }
      throw new Exception($msg);
    }

    // Check returned result.
    if (empty($data->result) OR empty($data->result->location)) {
      // The request was successfully sent and data received but no matching
      // result was found by the Baidu geocoder.
      drupal_set_message(t('Baidu Geocoder returned successfully, but failed to find the corresponding geo-coordinates for the address provided.<br/>No geocoded information could be saved because no coordinates were found for the address: <em>@address</em>.<br/>Please check the address provided and try again.', array('@address' => $address)), 'error');
      return FALSE;
    }
    // Validate result based on precision.
    elseif (!empty($options['reject_precise']) && empty($data->result->precise)) {
      // Matching result was found by the baidu geocoder.
      drupal_set_message(t('The returned Geocoded information was not <em>precise</em> for the address: @address.<br/>Please check the address provided and try again.', array('@address' => $address)), 'error');
      return FALSE;
    }

    // Return successful result received from baidu geocoder request.
    return new Point($data->result->location->lng, $data->result->location->lat);

  }
  catch (Exception $e) {
    // Capture any exception raised to be recorded in watchdog.
    watchdog_exception('geocoder', $e, $e->getMessage());
    // Let the user know about this error by displaying an error message.
    drupal_set_message(t("An error in the request was reported by Baidu when returning from geocoding operation.<br/>Please check the <a href='@link_logs'>recent log entries</a> for the error code recorded with the exception raised.<br/><strong>Geocoding with Baidu API was not successful.</strong>", array('@link_logs' => url('admin/reports/dblog'))), 'error');

    return FALSE;
  }
}

/**
 * Baidu geocoder plugin defined field callback.
 */
function baidu_map_geocoder_baidu_field($field, $field_item, $options = array()) {
  if ($field['type'] == 'text' || $field['type'] == 'text_long' || $field['type'] == 'text_with_summary' || $field['type'] == 'computed') {
    // Process any configured text field though the baidu geocoder.
    return baidu_map_geocoder_baidu($field_item['value'], $options);
  }
  if ($field['type'] == 'addressfield') {
    // The address field would need to be provided in a specific order.
    // So geocoder_widget_parse_addressfield can't be directly used.
    $address = baidu_map_geofield_geocoder_widget_parse_addressfield_china($field_item);
    return baidu_map_geocoder_baidu($address, $options);
  }
  if ($field['type'] == 'location') {
    // Process Location fields with the standard geocoder function.
    $address = geocoder_widget_parse_locationfield($field_item);
    return baidu_map_geocoder_baidu($address, $options);
  }
  if ($field['type'] == 'taxonomy_term_reference') {
    // Process taxonomy term name to be passed to baidu geocoder.
    $term = taxonomy_term_load($field_item['tid']);
    return baidu_map_geocoder_baidu($term->name, $options);
  }
}

/**
 * Baidu geocoder plugin defined field settings form callback.
 */
function baidu_map_geocoder_baidu_form($default_values = array()) {
  $form = array();

  // Allow results to be filtered depending on returned geocoded precision.
  $form['reject_precise'] = array(
    '#type' => 'checkbox',
    '#title' => t('Only keep precise results'),
    '#default_value' => isset($default_values['reject_precise']) ? $default_values['reject_precise'] : FALSE,
    '#description' => t('Often an ambiguous address (such as "Nanjing Road, Shanghai") can return imprecise geocoded information. Check this option to reject any results that would not be precise.'),
  );

  return $form;
}
