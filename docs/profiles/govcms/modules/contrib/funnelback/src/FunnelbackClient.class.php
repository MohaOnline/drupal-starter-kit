<?php

class FunnelbackClient {

  protected $funnelnack_debug_none = 'none';
  protected $funnelnack_debug_log = 'log';
  protected $funnelnack_debug_verbose = 'verbose';

  /**
   * Make a request.
   *
   * @param string $base_url
   *   The base URL for the request.
   * @param string $api_path
   *   The api path from the base URL.
   * @param array $request_params
   *   The request parameters.
   *
   * @return object
   *   The response object.
   */
  public function request($base_url, $api_path, $request_params) {

    // Build the search URL with query params.
    $url = url($base_url . $api_path, ['query' => $request_params]);

    $url = FunnelbackQueryString::funnelbackQueryNormaliser($url);

    // Make the request.
    $response = drupal_http_request($url);

    $this->debug('Requesting url: %url. Response %response. Template %template', [
      '%url' => $url,
      '%response' => $response->code,
      '%template' => ($api_path == 's/search.json') ? t('Default') : t('Custom'),
    ]);

    return $response;
  }

  /**
   * Helper to log debug messages.
   *
   * @param string $message
   *   A message, suitable for watchdog().
   * @param array $args
   *   (optional) An array of arguments, as per watchdog().
   * @param int $log_level
   *   (optional) The watchdog() log level. Defaults to WATCHDOG_DEBUG.
   */
  public function debug($message, $args = [], $log_level = 7) {

    $debug = variable_get('funnelback_debug_mode', $this->funnelnack_debug_none);
    if ($debug == $this->funnelnack_debug_log) {
      watchdog('funnelback', $message, $args, $log_level);
    }
    elseif ($debug == $this->funnelnack_debug_verbose) {
      $string = format_string($message, $args);
      if ($log_level >= WATCHDOG_ERROR) {
        $message_level = 'error';
      }
      else {
        $message_level = 'status';
      }
      drupal_set_message($string, $message_level);
    }
  }
}
