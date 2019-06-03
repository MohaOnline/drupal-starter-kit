<?php


/**
 * Post request with values and decode json response
 *
 * @param string $uri
 * @param string $data_string
 *
 * @return StdClass
 *
 */
function opigno_moxtra_app_api_post_json($uri, $data_string, $custom_content_type = NULL)
{
  $channel = curl_init();
  curl_setopt($channel, CURLOPT_URL, $uri);
  curl_setopt($channel, CURLOPT_POST, 1);
  curl_setopt($channel, CURLOPT_POSTFIELDS, $data_string);
  curl_setopt($channel, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($channel, CURLOPT_SSL_VERIFYPEER, false);
  if($custom_content_type) curl_setopt($channel, CURLOPT_HTTPHEADER, array('Content-Type: ' . $custom_content_type));
  $response = curl_exec($channel);
  $http_code = curl_getinfo($channel, CURLINFO_HTTP_CODE);

  if ($response === false)
    watchdog('opigno_moxtra_app', 'Curl error in '. __FUNCTION__ .':<br />' .
      'Sent this url: '. $uri .'<br />' .
      'Error: <pre>'. curl_error($channel) .'</pre><br />' .
      'Data sent:<pre>'. $data_string .'</pre>', array(), WATCHDOG_ERROR);
  else {
    // Add the http response code in the $response object
    $responseDecoded = json_decode($response);
    $response = (object) array_merge(array('http_code' => $http_code), (empty($responseDecoded) ? array() : (array) $responseDecoded));
  }

  curl_close($channel);
  return $response;
}

/**
 * Request http Delete
 *
 * @param string $uri
 * @param string $data_string
 *
 * @return StdClass
 *
 */
function opigno_moxtra_app_api_delete_json($uri, $custom_content_type = NULL)
{
  $channel = curl_init();
  curl_setopt($channel, CURLOPT_URL, $uri);
  curl_setopt($channel, CURLOPT_CUSTOMREQUEST, "DELETE");
  curl_setopt($channel, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($channel, CURLOPT_SSL_VERIFYPEER, false);
  if($custom_content_type) curl_setopt($channel, CURLOPT_HTTPHEADER, array('Content-Type: ' . $custom_content_type));
  $response = curl_exec($channel);
  $http_code = curl_getinfo($channel, CURLINFO_HTTP_CODE);

  if ($response === false)
    watchdog('opigno_moxtra_app', 'Curl error in '. __FUNCTION__ .':<br />' .
      'Sent this url: '. $uri .'<br />' .
      'Error: <pre>'. curl_error($channel) .'</pre>', array(), WATCHDOG_ERROR);
  else {
    // Add the http response code in the $response object
    $responseDecoded = json_decode($response);
    $response = (object) array_merge(array('http_code' => $http_code), (empty($responseDecoded) ? array() : (array) $responseDecoded));
  }

  curl_close($channel);
  return $response;
}

/**
 * Get request and decode json response
 *
 * @param string $uri
 *
 * @return StdClass
 *
 */
function opigno_moxtra_app_api_get_json($uri)
{
  $channel = curl_init();
  curl_setopt($channel, CURLOPT_URL, $uri);
  curl_setopt($channel, CURLOPT_RETURNTRANSFER, true);
  curl_setopt($channel, CURLOPT_SSL_VERIFYPEER, false);
  $response = curl_exec($channel);
  $http_code = curl_getinfo($channel, CURLINFO_HTTP_CODE);

  if ($response === false)
    watchdog('opigno_moxtra_app', 'Curl error in '. __FUNCTION__ .':<br />' .
      'Sent this url: '. $uri .'<br />' .
      'Error: <pre>'. curl_error($channel) .'</pre>', array(), WATCHDOG_ERROR);
  else {
    // Add the http response code in the $response object
    $responseDecoded = json_decode($response);
    $response = (object) array_merge(array('http_code' => $http_code), (empty($responseDecoded) ? array() : (array) $responseDecoded));
  }

  curl_close($channel);
  return $response;
}
