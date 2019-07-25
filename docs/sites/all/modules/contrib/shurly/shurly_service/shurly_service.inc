<?php

/**
 * @file
 *  Link general search functionalities to services module.
 */

/**
 * Callback for shurly/api/shorten
 */
function shurly_service_shorten() {
  $defaults = array(
    'format' => 'json',
    // 'domain' => NULL,
    'longUrl' => '',
    'shortUrl' => NULL,
    'apiKey' => NULL,
    'func' => 'urlData', // function name for padded JSON
  );
  $input = $_GET + $defaults;

  module_load_include('inc', 'shurly_service', 'shurly_api_keys');
  $uid = isset($input['apiKey']) ? shurly_get_uid($input['apiKey']) : NULL;
  $account = $uid ? user_load($uid) : NULL;
  $access = user_access('Create short URLs', $account);

  if ($access) {
    // If the user doesn't have access to request a custom short URL from the
    // service, reset it to NULL.
    if (!user_access('Request custom short URL', $account)) {
      $input['shortUrl'] = NULL;
    }
    $data = shurly_shorten($input['longUrl'], $input['shortUrl'], $account);
  }
  else {
    $data = array(
      'success' => FALSE,
      'error' => t('Invalid API key'),
    );
  }
  shurly_service_output($data, $input);
}

/**
 * Callback for shurly/api/shorten
 */
function shurly_service_expand() {
  $defaults = array(
    'format' => 'json',
    // 'domain' => NULL,
    //'longUrl' => '',
    'shortUrl' => '',
    // 'login' => NULL,
    'apiKey' => NULL,
    'func' => 'urlData', // function name for padded JSON
  );
  $input = $_GET + $defaults;

  module_load_include('inc', 'shurly_service', 'shurly_api_keys');
  $uid = isset($input['apiKey']) ? shurly_get_uid($input['apiKey']) : NULL;
  $account = $uid ? user_load($uid) : NULL;
  $access = user_access('Expand short URLs', $account);

  if ($access) {
    $path = array_pop(explode('/', parse_url($input['shortUrl'], PHP_URL_PATH))); // only works with clean URLs
    $data = shurly_expand($path, $account);
  }
  else {
    $data = array(
      'success' => FALSE,
      'error' => t('Invalid API key'),
    );
  }

  shurly_service_output($data, $input);

}

/**
 * Output data in a given format
 */
function shurly_service_output($data, $input) {
  $format = $input['format'] ? $input['format'] : 'json';
  if (function_exists('shurly_service_output_' . $format)) {
    call_user_func('shurly_service_output_' . $format, $data, $input);
  }
  else {
    print t('invalid format request');
  }
}

/**
 * Output JSON data
 */
function shurly_service_output_json($data) {
  drupal_json_output($data);
}

/**
 * Output JSON data
 */
function shurly_service_output_jsonp($data, $input) {
  $func = $input['func'] ? $input['func'] : 'urlData';
  // send the header
  drupal_json_output();
  print $func . '(' . drupal_json_encode($data) . ');';
}

/**
 * Output txt data
 *  Just outputs the shortUrl attribute... won't show errors
 */
function shurly_service_output_txt($data) {
  // sniff the URL and figure out what to output
  switch (arg(2)) {
    case 'expand':
      $key = 'longUrl';
      break;

    case 'shorten':
    default:
      $key = 'shortUrl';
  }
  if ($data['success']) {
    print $data[$key];
  }
  else {
    print t("ERROR: !error", array('!error' => $data['error']));
  }
}

/**
 * Output PHP serialized data
 */
function shurly_service_output_php($data) {
  print serialize($data);
}

/**
 * Output xml data
 */
function shurly_service_output_xml($data) {
  $xml = simplexml_load_string("<?xml version='1.0' encoding='utf-8'?><data />");
  if (is_array($data)) {
    foreach ($data as $key => $value) {
      $value = htmlentities($value);
      $xml->addChild($key, $value);
    }
  }
  $output = $xml->asXML();
  header('Connection: close');
  header('Content-Length: ' . strlen($output));
  header('Content-Type: text/xml');
  header('Date: ' . date('r'));
  print $output;
}
