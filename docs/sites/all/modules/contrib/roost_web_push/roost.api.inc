<?php

/**
 * @file
 * Roost API calls.
 */

/**
 * Core request to Roost API.
 */
function roost_remote_request($remote_data) {
  $auth_creds = '';
  if (!empty($remote_data['appkey'])) {
    $auth_creds = 'Basic ' . base64_encode($remote_data['appkey'] . ':' . $remote_data['appsecret']);
  }
  $remote_url = 'https://go.goroost.com/api/' . $remote_data['remoteAction'];

  $headers = array(
    'Authorization'  => $auth_creds,
    'Accept'       => 'application/json',
    'Content-Type'   => 'application/json',
    'Content-Length' => strlen($remote_data['remoteContent']),
  );

  $remote_payload = array(
    'method'    => $remote_data['method'],
    'headers'   => $headers,
    'data'      => $remote_data['remoteContent'],
  );
  $response = drupal_http_request($remote_url, $remote_payload);
  return $response;
}

/**
 * Roost login request.
 */
function roost_login($roost_user, $roost_pass) {
  $remote_content = array(
    'username' => $roost_user,
    'password' => $roost_pass,
  );
  $remote_data = array(
    'method' => 'POST',
    'remoteAction' => 'accounts/details',
    'appkey' => $roost_user,
    'appsecret' => $roost_pass,
    'remoteContent' => json_encode($remote_content),
  );
  return roost_remote_request($remote_data);
}

/**
 * Roost app stats request.
 */
function roost_get_stats($roost_app_key, $roost_app_secret) {
  $remote_data = array(
    'method' => 'POST',
    'remoteAction' => 'stats/app',
    'appkey' => $roost_app_key,
    'appsecret' => $roost_app_secret,
    'remoteContent' => '',
  );
  return roost_remote_request($remote_data);
}

/**
 * Roost app settings request.
 */
function roost_get_remote_settings($roost_app_key, $roost_app_secret) {
  $remote_data = array(
    'method' => 'POST',
    'remoteAction' => 'app',
    'appkey' => $roost_app_key,
    'appsecret' => $roost_app_secret,
    'remoteContent' => '',
  );
  return roost_remote_request($remote_data);
}

/**
 * Roost send notifications.
 */
function roost_send_notification($alert, $url, $image_url, $roost_app_key, $roost_app_secret) {
  $remote_content = array(
    'alert' => $alert,
  );
  if ($url) {
    $remote_content['url'] = $url;
  }
  if ($image_url) {
    $remote_content['imageURL'] = $image_url;
  }
  $remote_data = array(
    'method' => 'POST',
    'remoteAction' => 'push',
    'appkey' => $roost_app_key,
    'appsecret' => $roost_app_secret,
    'remoteContent' => json_encode($remote_content),
  );
  return roost_remote_request($remote_data);
}
