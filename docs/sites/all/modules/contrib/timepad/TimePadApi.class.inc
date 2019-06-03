<?php

/**
 * @file
 */

/**
 * Class TimePadApi.
 */
class TimePadApi {
  /**
   * URL for API
   */
  const URL = 'https://api.timepad.ru/';

  /**
   * Auth URL for API
   */
  const AUTH_URL = 'https://api.timepad.ru/oauth/authorize';

  /**
   * HTTP methods
   */
  const METHOD_GET = 'GET';
  const METHOD_POST = 'POST';

  /**
   * Curl instance
   */
  protected $curl;

  /**
   * Authorize token, for protected API methods.
   */
  protected $token;

  /**
   * Class constructor.
   */
  public function __construct() {
    $this->token = variable_get('timepad_access_token', '');
  }

  /**
   * Class destructor.
   */
  public function __destruct() {
    curl_close($this->curl);
  }

  /**
   * Generate url for request.
   *
   * @param string $path
   * @param string $base
   *
   * @return string
   */
  protected function getUrl($path, $base = '') {
    $base = $base ? $base : self::URL;
    return $base . $path;
  }

  /**
   * Get Events List.
   *
   * @param array $params
   *
   * @return array
   *
   * @access public
   */
  public function eventsGetList($params = array()) {
    $url = $this->getUrl('v1/events');

    $result = $this->curlRequest(
      $url,
      self::METHOD_GET,
      $params ? http_build_query($params) : NULL
    );

    if (!isset($result['total'])) {
      $result = array(
        'total' => 0,
        'values' => array(),
      );
    }

    return $result;
  }

  /**
   * Create Event.
   *
   * @param array $params
   *
   * @return array
   *
   * @access public
   */
  public function eventsCreate($params) {
    $url = $this->getUrl('v1/events');

    return $this->curlRequest(
      $url,
      self::METHOD_POST,
      json_encode($params),
      array(
        'Authorization: Bearer ' . $this->token,
      )
    );
  }

  /**
   * Edit Event.
   *
   * @param int $event_id
   * @param array $params
   *
   * @return array
   *
   * @access public
   */
  public function eventsEdit($event_id, $params) {
    $url = $this->getUrl('v1/events/' . $event_id);

    return $this->curlRequest(
      $url,
      self::METHOD_POST,
      json_encode($params),
      array(
        'Authorization: Bearer ' . $this->token,
      )
    );
  }

  /**
   * Execution of the request.
   *
   * @param string $url
   * @param string $method
   * @param string $parameters
   * @param array $headers
   * @param integer $timeout
   *
   * @return array
   *
   * @access protected
   *
   * @throws \Exception
   */
  protected function curlRequest($url, $method = 'GET', $parameters = '', $headers = array(), $timeout = 30) {
    if ($method == self::METHOD_GET && $parameters) {
      $url .= "?$parameters";
    }

    // Get curl handler or initiate it.
    if (!$this->curl) {
      $this->curl = curl_init();
    }

    // Set general arguments.
    curl_setopt($this->curl, CURLOPT_URL, $url);
    curl_setopt($this->curl, CURLOPT_FAILONERROR, FALSE);
    curl_setopt($this->curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_setopt($this->curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_setopt($this->curl, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($this->curl, CURLOPT_TIMEOUT, $timeout);
    curl_setopt($this->curl, CURLOPT_HEADER, FALSE);

    // Reset some arguments, in order to avoid use some from previous request.
    curl_setopt($this->curl, CURLOPT_POST, FALSE);

    curl_setopt($this->curl, CURLOPT_HTTPHEADER, $headers);

    if ($method == self::METHOD_POST && $parameters) {
      curl_setopt($this->curl, CURLOPT_POST, TRUE);

      // Encode parameters if them already not encoded in json.
      if (!is_string($parameters)) {
        $parameters = http_build_query($parameters);
      }

      curl_setopt($this->curl, CURLOPT_POSTFIELDS, $parameters);
    }

    $response = curl_exec($this->curl);
    $statusCode = curl_getinfo($this->curl, CURLINFO_HTTP_CODE);

    $errno = curl_errno($this->curl);
    $error = curl_error($this->curl);

    if ($errno) {
      throw new Exception($error, $errno);
    }

    $result = drupal_json_decode($response);

    if ($statusCode >= 400) {
      if (!empty($result['response_status']['errors'])) {
        foreach ($result['response_status']['errors'] as $error) {
          $message = '';
          if (!empty($error['error_code'])) {
            $message .= 'Error ' . $error['error_code'] . ': ';
          }
          if (!empty($error['message'])) {
            $message .= $error['message'] . ' ';
          }
          if (!empty($error['field_name'])) {
            $message .= $error['field_name'];
          }
          if (!empty($message)) {
            drupal_set_message($message, 'error');
          }
        }
      }
      throw new Exception($result['response_status']['message'], $statusCode);
    }

    return !empty($result) ? $result : array();
  }
}
