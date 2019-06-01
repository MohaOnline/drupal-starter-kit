<?php

/**
 * A class of static helper functions
 *
 */

module_load_include('php', 'shareaholic', 'lib/social-share-counts/drupal_http');
module_load_include('php', 'shareaholic', 'lib/social-share-counts/seq_share_count');

class ShareaholicUtilities {
  const MODULE_VERSION = '7.x-3.31';
  const URL = 'https://www.shareaholic.com';
  const API_URL = 'https://web.shareaholic.com';
  const CM_API_URL = 'https://cm-web.shareaholic.com';
  
  /**
   * Returns whether the user has accepted our terms of service.
   * If the user has accepted, return true otherwise return NULL
   *
   * @return mixed (true or NULL)
   */
  public static function has_accepted_terms_of_service() {
    return variable_get('shareaholic_has_accepted_tos');
  }

  /**
   * Accepts the terms of service by setting the variable to true
   */
  public static function accept_terms_of_service() {
    variable_set('shareaholic_has_accepted_tos', true);
    ShareaholicUtilities::log_event('AcceptedToS');
  }

  /**
   * Returns the defaults for shareaholic settings
   *
   * @return array
   */
  private static function defaults() {
    return array(
      'disable_internal_share_counts_api' => 'on',
      'api_key' => '',
      'verification_key' => '',
    );
  }

  /**
   * Just a wrapper around variable_get to
   * get the shareaholic settings. If the settings
   * have not been set it will return an array of defaults.
   *
   * @return array
   */
  public static function get_settings() {
    return variable_get('shareaholic_settings', self::defaults());
  }

  /**
   * Wrapper for wordpress's get_option: for Drupal
   *
   * @param string $option
   *
   * @return mixed
   */
  public static function get_option($option) {
    $settings = self::get_settings();
    return (isset($settings[$option]) ? $settings[$option] : array());
  }

  /**
   * Update multiple keys of the settings object
   * Works like the Wordpress function for Shareaholic
   *
   * @param  array $array an array of options to update
   * @return bool
   */
  public static function update_options($array) {
    $old_settings = self::get_settings();
    $new_settings = self::array_merge_recursive_distinct($old_settings, $array);
    variable_set('shareaholic_settings', $new_settings);
  }

  /**
   * Deletes the settings option
   */
  public static function destroy_settings() {
    // Delete cloud site id
    ShareaholicUtilities::delete_api_key();
    // Delete local Drupal site id
    variable_del('shareaholic_settings');
  }

  /**
   * Set the settings option
   */
  public static function set_settings($settings) {
    variable_set('shareaholic_settings', $settings);
  }


  /**
   * Returns the site's url stripped of protocol.
   *
   * @return string
   */
  public static function site_url() {
    return preg_replace('/https?:\/\//', '', $GLOBALS['base_url']);
  }

  /**
   * Returns the site's name
   *
   * @return string
   */
  public static function site_name() {
    return variable_get('site_name', $GLOBALS['base_url']);
  }

  /**
   * Returns the site's primary locale / language
   *
   * @return string
   */
  public static function site_language() {
    $language_id_map = array(
      "ar" => 1, // Arabic
      "bg" => 2, // Bulgarian
      "zh-hans" => 3, // Chinese (Simplified)
      "zh-hant" => 4, // Chinese (Traditional)
      "hr" => 5, // Croatian
      "cs" => 6, // Czech
      "da" => 7, // Danish
      "nl" => 8, // Netherlands
      "en" => 9, // English
      "et" => 10, // Estonian
      "fi" => 11, // Finnish
      "fr" => 12,  // French
      "de" => 13,  // German
      "el" => 14,  // Greek
      "he" => 15,  // Hebrew
      "hu" => 16,  // Hungarian
      "id" => 17,  // Indonesian
      "it" => 18,  // Italian
      "ja" => 19,  // Japanese
      "ko" => 20,  // Korean
      "lv" => 21,  // Lativan
      "lt" => 22,  // Lithuanian
      "nn" => 23,  // Norwegian
      "pl" => 24,  // Poland
      "pt-pt" => 25, // Portuguese
      "ro" => 26,    // Romanian
      "ru" => 27,    // Russian
      "sr" => 28,    // Serbian
      "sk" => 29,    // Slovak
      "sl" => 30,    // Slovenian
      "es" => 31,    // Spanish
      "sv" => 32,    // Swedish
      "th" => 33,    // Thai
      "tr" => 34,    // Turkish
      "uk" => 35,    // Ukrainian
      "vi" => 36,    // Vietnamese
    );
    $language = $GLOBALS['language']->language;
    return isset($language_id_map[$language]) ? $language_id_map[$language] : NULL;
  }

  /**
   * Returns the api key or creates a new one.
   *
   * It first checks the database. If the key is not
   * found (or is an empty string or empty array or
   * anything that evaluates to false) then we will
   * attempt to make a new one by POSTing to the
   * anonymous configuration endpoint
   *
   * @return string
   */
  public static function get_or_create_api_key() {

    $api_key = self::get_option('api_key');
    // ensure api key set is atleast 30 characters, if not, retry to set new api key
    if ($api_key && (strlen($api_key) > 30)) {
      return $api_key;
    }

    // destroy the shareaholic settings except certain flags
    $old_settings = self::get_settings();
    self::destroy_settings();
    // restore any old settings that should be preserved between resets
    if (isset($old_settings['share_counts_connect_check'])) {
      self::update_options(array(
        'share_counts_connect_check' => $old_settings['share_counts_connect_check'],
      ));
    }

    $verification_key = md5(mt_rand());
    $page_types = self::page_types();
    $turned_on_recommendations_locations = self::get_default_rec_on_locations();
    $turned_off_recommendations_locations = self::get_default_rec_off_locations();
    $turned_on_share_buttons_locations = self::get_default_sb_on_locations();
    $turned_off_share_buttons_locations = self::get_default_sb_off_locations();

    $share_buttons_attributes = array_merge($turned_on_share_buttons_locations, $turned_off_share_buttons_locations);
    $recommendations_attributes = array_merge($turned_on_recommendations_locations, $turned_off_recommendations_locations);
    $post_data = array(
      'configuration_publisher' => array(
        'verification_key' => $verification_key,
        'site_name' => self::site_name(),
        'domain' => self::site_url(),
        'platform_id' => '2',
        'language_id' => self::site_language(),
        'shortener' => 'shrlc',
        'recommendations_attributes' => array(
          'locations_attributes' => $recommendations_attributes
        ),
        'share_buttons_attributes' => array(
          'locations_attributes' => $share_buttons_attributes
       )
      )
    );

    $response = drupal_http_request(self::API_URL . '/publisher_tools/anonymous', array(
      'method' => 'POST',
      'headers' => array(
        'Content-Type' => 'application/json'
      ),
      'data' => json_encode($post_data)
    ));
    
    if(self::has_bad_response($response, 'FailedToCreateApiKey', true)) {
      return NULL;
    }
    $response = (array) $response;
    $json_response = json_decode($response['data'], true);
    self::update_options(array(
      'version' => self::get_version(),
      'api_key' => $json_response['api_key'],
      'verification_key' => $verification_key,
      'location_name_ids' => $json_response['location_name_ids']
    ));

    if (isset($json_response['location_name_ids']) && is_array($json_response['location_name_ids']) && isset($json_response['location_name_ids']['recommendations']) && isset($json_response['location_name_ids']['share_buttons'])) {
      self::set_default_location_settings($json_response['location_name_ids']);
      ShareaholicContentManager::single_domain_worker();
    } else {
      ShareaholicUtilities::log_event('FailedToCreateApiKey', array('reason' => 'no location name ids the response was: ' . $response['data']));
    }
  }

  /**
   * Get share buttons locations that should be turned on by default
   *
   * @return {Array}
   */
  public static function get_default_sb_on_locations() {
    $page_types = self::page_types();
    $turned_on_share_buttons_locations = array();

    foreach($page_types as $key => $page_type) {
      $page_type_name = $page_type->type;

      $turned_on_share_buttons_locations[] = array(
        'name' => $page_type_name . '_below_content'
      );
    }

    return $turned_on_share_buttons_locations;
  }

  /**
   * Get share buttons locations that should be turned off by default
   *
   * @return {Array}
   */
  public static function get_default_sb_off_locations() {
    $page_types = self::page_types();
    $turned_off_share_buttons_locations = array();

    foreach($page_types as $key => $page_type) {
      $page_type_name = $page_type->type;

      $turned_off_share_buttons_locations[] = array(
        'name' => $page_type_name . '_above_content'
      );
    }

    return $turned_off_share_buttons_locations;
  }


  /**
   * Get recommendations locations that should be turned on by default
   *
   * @return {Array}
   */
  public static function get_default_rec_on_locations() {
    $page_types = self::page_types();
    $turned_on_recommendations_locations = array();

    foreach($page_types as $key => $page_type) {
      $page_type_name = $page_type->type;
      if($page_type_name === 'article' || $page_type_name === 'page') {
        $turned_on_recommendations_locations[] = array(
          'name' => $page_type_name . '_below_content'
        );
      }
    }

    return $turned_on_recommendations_locations;
  }

  /**
   * Get recommendations locations that should be turned off by default
   *
   * @return {Array}
   */
  public static function get_default_rec_off_locations() {
    $page_types = self::page_types();
    $turned_off_recommendations_locations = array();

    foreach($page_types as $key => $page_type) {
      $page_type_name = $page_type->type;
      if($page_type_name !== 'article' && $page_type_name !== 'page') {
        $turned_off_recommendations_locations[] = array(
          'name' => $page_type_name . '_below_content'
        );
      }
    }

    return $turned_off_recommendations_locations;
  }

  /**
   * Given an object, set the default on/off locations
   * for share buttons and recommendations
   *
   */
  public static function set_default_location_settings($location_name_ids) {
    $turned_on_share_buttons_locations = self::get_default_sb_on_locations();
    $turned_off_share_buttons_locations = self::get_default_sb_off_locations();

    $turned_on_recommendations_locations = self::get_default_rec_on_locations();
    $turned_off_recommendations_locations = self::get_default_rec_off_locations();

    $turned_on_share_buttons_keys = array();
    foreach($turned_on_share_buttons_locations as $loc) {
      $turned_on_share_buttons_keys[] = $loc['name'];
    }

    $turned_on_recommendations_keys = array();
    foreach($turned_on_recommendations_locations as $loc) {
      $turned_on_recommendations_keys[] = $loc['name'];
    }

    $turned_off_share_buttons_keys = array();
    foreach($turned_off_share_buttons_locations as $loc) {
      $turned_off_share_buttons_keys[] = $loc['name'];
    }

    $turned_off_recommendations_keys = array();
    foreach($turned_off_recommendations_locations as $loc) {
      $turned_off_recommendations_keys[] = $loc['name'];
    }

    $turn_on = array(
      'share_buttons' => self::associative_array_slice($location_name_ids['share_buttons'], $turned_on_share_buttons_keys),
      'recommendations' => self::associative_array_slice($location_name_ids['recommendations'], $turned_on_recommendations_keys)
    );

    $turn_off = array(
      'share_buttons' => self::associative_array_slice($location_name_ids['share_buttons'], $turned_off_share_buttons_keys),
      'recommendations' => self::associative_array_slice($location_name_ids['recommendations'], $turned_off_recommendations_keys)
    );

    ShareaholicUtilities::turn_on_locations($turn_on, $turn_off);
  }


  /**
   * Restore the plugin settings
   *
   */
  public static function reset_settings() {
    $settings = self::get_settings();
    $api_key = self::get_option('api_key');

    $response = drupal_http_request(self::API_URL . '/publisher_tools/'  . $api_key .  '/reset/', array(
      'method' => 'POST',
      'headers' => array('Content-Type' => 'application/json'),
      'data' => json_encode($settings)
    ));

    // set the location on/off back to their defaults
    if (isset($settings['location_name_ids']) && is_array($settings['location_name_ids'])) {
      self::set_default_location_settings($settings['location_name_ids']);
    }
  }
  
  /**
   * Deletes the api key
   *
   */
   public static function delete_api_key () {
     $payload = array(
       'site_id' => self::get_option('api_key'),
       'verification_key' => self::get_option('verification_key')
     );
     
     $response = drupal_http_request(self::API_URL . '/integrations/plugin/delete', array(
       'method' => 'POST',
       'headers' => array('Content-Type' => 'application/json'),
       'data' => json_encode($payload)
     ));     
  }

  /**
   * Checks bad response and logs errors if any
   *
   * @return boolean
   */
  public static function has_bad_response($response, $type, $json_parse = FALSE) {
    if(!$response) {
      ShareaholicUtilities::log_event($type, array('reason' => 'there was no response'));
      return true;
    }
    $response = (array) $response;
    if(isset($response['error'])) {
      $error_message = print_r($response['error'], true);
      ShareaholicUtilities::log_event($type, array('reason' => 'there was an error: ' . $error_message));
      return true;
    }
    if(!preg_match('/20*/', $response['code'])) {
      ShareaholicUtilities::log_event($type, array('reason' => 'the server responded with code ' . $response['code']));
      return true;
    }
    if($json_parse && json_decode($response['data']) === NULL) {
      ShareaholicUtilities::log_event($type, array('reason' => 'could not parse JSON. The response was: ' . $response['data']));
      return true;
    }
    return false;
  }

  /**
   * Log the errors in the database if debug flag is set to true
   *
   */
  public static function log($message) {
    if(defined('SHAREAHOLIC_DEBUG') && SHAREAHOLIC_DEBUG) {
      watchdog('Shareaholic', print_r($message, true));
    }
  }

  /**
   * Direct copy of the wordpress util function
   * If the two arrays have the same key, the value from array2 overrides
   * the value on array1
   *
   * @param  array $array1
   * @param  array $array2
   * @return array
   */
  public static function array_merge_recursive_distinct ( array &$array1, array &$array2 )
  {
    $merged = $array1;

    foreach ( $array2 as $key => &$value )
    {
      if ( is_array ( $value ) && isset ( $merged [$key] ) && is_array ( $merged [$key] ) )
      {
        if (empty($value)) {
          $merged[$key] = array();
        } else {
          $merged [$key] = self::array_merge_recursive_distinct ( $merged [$key], $value );
        }
      }
      else
      {
        $merged [$key] = $value;
      }
    }

    return $merged;
  }

  /**
   * Returns the appropriate asset path for something from our
   * rails app based on environment.
   *
   * @param string $asset
   * @return string
   */
  public static function asset_url($asset) {
    $env = self::get_env();
    if ($env === 'development') {
      return 'http://spreadaholic.com:8080/' . $asset;
    } elseif ($env === 'staging') {
      return '//d2062rwknz205x.cloudfront.net/' . $asset;
    } else {
      return '//cdn.shareaholic.net/' . $asset;
    }
  }

  /**
   * Returns the appropriate environment based on URL constant
   *
   * @return string
   */
  public static function get_env() {
    if (preg_match('/spreadaholic/', self::URL)) {
      return 'development';
    } elseif (preg_match('/stageaholic/', self::URL)) {
      return 'staging';
    } else {
      return 'production';
    }
  }

  /**
   * Check if the installation has accepted ToS and we created an apikey
   *
   */
  public static function has_tos_and_apikey() {
    return (ShareaholicUtilities::has_accepted_terms_of_service() &&
              ShareaholicUtilities::get_option('api_key'));
  }

  /**
   * Gets the current version of this module
   */
  public static function get_version() {
    return self::MODULE_VERSION;
  }

  /**
   * Sets the current version of this module in the database
   */
  public static function set_version($version) {
    self::update_options(array('version' => $version));
  }

  /**
   * Checks if the current page is an admin page
   * @return mixed: returns 1 if matched, 0 if no match, false if error occurs
   */
  public static function is_admin_page() {
    return path_is_admin(current_path());
  }

  /**
   * Checks if the current page is the settings page
   * @return Boolean (actually 1, 0, or FALSE)
   */
  public static function is_shareaholic_settings_page() {
    return preg_match('/admin\/config\/shareaholic\//', request_uri());
  }

  /**
   * Cleans the string to be HTML safe
   *
   * Given a string, it will:
   * - Encode the string
   * - Trim the string
   * - lower case the string
   *
   * @return String the cleaned string
   */
  public static function clean_string($word) {
    return trim(trim(strtolower(trim(htmlspecialchars(htmlspecialchars_decode($word), ENT_QUOTES))), ","));
  }

  /**
   * Give back only the request keys from an array. The first
   * argument is the array to be sliced, and after that it can
   * either be a variable-length list of keys or one array of keys.
   *
   * @param  array $array
   * @param  Mixed ... can be either one array or many keys
   * @return array
   */
  public static function associative_array_slice($array) {
    $keys = array_slice(func_get_args(), 1);
    if (func_num_args() == 2 && is_array($keys[0])) {
      $keys = $keys[0];
    }

    $result = array();

    foreach($keys as $key) {
      $result[$key] = $array[$key];
    }

    return $result;
  }


  /**
   * Passed an array of location names mapped to ids per app.
   *
   * @param array $array
   */
  public static function turn_on_locations($array, $turn_off_array = array()) {
    if (is_array($array)) {
      foreach($array as $app => $ids) {
        if (is_array($ids)) {
          foreach($ids as $name => $id) {
            self::update_options(array(
              $app => array($name => 'on')
            ));
          }
        }
      }
    }

    if (is_array($turn_off_array)) {
      foreach($turn_off_array as $app => $ids) {
        if (is_array($ids)) {
          foreach($ids as $name => $id) {
            self::update_options(array(
              $app => array($name => 'off')
            ));
          }
        }
      }
    }
  }

  /**
   * Get all the available page types
   * Insert the teaser mode as a page type
   *
   * @return Array list of page types
   */
  public static function page_types() {
    $page_types = node_type_get_types();
    $teaser = new stdClass;
    $teaser->name = 'Teaser';
    $teaser->type = 'teaser';
    $page_types['shareaholic_custom_type'] = $teaser;
    return $page_types;
  }


  /**
   * Checks whether the api key has been verified
   * using the rails endpoint. Once the key has
   * been verified, we store that away so that we
   * don't have to check again.
   *
   * @return bool
   */
  public static function api_key_verified() {
    $settings = self::get_settings();
    if (isset($settings['api_key_verified']) && $settings['api_key_verified']) {
      return true;
    }

    $api_key = $settings['api_key'];
    if (!$api_key) {
      return false;
    }

    $response = drupal_http_request(self::API_URL . '/publisher_tools/' . $api_key . '/verified', array('method' => 'GET'));
    if(self::has_bad_response($response, 'FailedApiKeyVerified')) {
      return false;
    }
    $response = (array) $response;

    $result = $response['data'];

    if ($result == 'true') {
      ShareaholicUtilities::update_options(array(
        'api_key_verified' => true
      ));
      return true;
    }
    return false;
  }
  

  /**
   * Clears Facebook Open Graph cache for provided node
   *
   * @param Object $node
   */
  public static function clear_fb_opengraph($node) {
    if ($node->status !== NODE_PUBLISHED) return;

    $page_link = url('node/'. $node->nid, array('absolute' => TRUE));
    if(isset($page_link)) {
      $fb_graph_url = "https://graph.facebook.com/?id=". urlencode($page_link) ."&scrape=true";
      $options = array(
        'method' => 'POST',
        'timeout' => 5,
        );
      $result = drupal_http_request($fb_graph_url, $options);
    }
  }


  /**
   * This is a wrapper for the Event API
   *
   * @param string $event_name    the name of the event
   * @param array  $extra_params  any extra data points to be included
   */
  public static function log_event($event_name = 'Default', $extra_params = false) {

    $event_metadata = array(
  	  'plugin_version' => self::get_version(),
  	  'api_key' => self::get_option('api_key'),
  	  'domain' => $GLOBALS['base_url'],
  	  'language' => $GLOBALS['language']->language,
  	  'stats' => self::get_stats(),
      'diagnostics' => array (
  		  'php_version' => phpversion(),
  		  'drupal_version' => self::get_drupal_version(),
  		  'theme' => variable_get('theme_default', $GLOBALS['theme']),
  		  'active_plugins' => module_list(),
  	  ),
  	  'features' => array (
  		  'share_buttons' => self::get_option('share_buttons'),
  		  'recommendations' => self::get_option('recommendations'),
  	  )
    );

    if ($extra_params) {
  	  $event_metadata = array_merge($event_metadata, $extra_params);
    }

  	$event_api_url = self::API_URL . '/api/events';
  	$event_params = array('name' => "Drupal:".$event_name, 'data' => json_encode($event_metadata) );
  	$options = array(
  	  'method' => 'POST',
  	  'headers' => array('Content-Type' => 'application/json'),
  	  'body' => $event_params,
  	);
    ShareaholicHttp::send($event_api_url, $options, true);
  }


  /**
   * Get the total number of comments for this site
   *
   * @return integer The total number of comments
   */
  public static function total_comments() {
    if (!db_table_exists('comment')) {
      return array();
    }
    return db_query("SELECT count(cid) FROM {comment}")->fetchField();
  }

  /**
   * Get the stats for this website
   * Stats include: total number of pages by type, total comments, total users
   *
   * @return array an associative array of stats => counts
   */
  public static function get_stats() {
    $stats = array();
    // Query the database for content types and add to stats
    $result = db_query("SELECT type, count(*) as count FROM {node} GROUP BY type");
    foreach ($result as $record) {
      $stats[$record->type . '_total'] = $record->count;
    }

    // Get the total comments
    $stats['comments_total'] = self::total_comments();
    return $stats;
  }

  /**
   * Get the drupal version via VERSION constant if it exists
   */
  public static function get_drupal_version() {
    if (defined('VERSION')) {
      return VERSION;
    }
    return '7';
  }

  /**
   * Server Connectivity check
   *
   */
  public static function connectivity_check() {
    $health_check_url = self::API_URL . "/haproxy_health_check";
    $response = ShareaholicHttp::send($health_check_url, array('method' => 'GET'), true);
    if(is_array($response) && array_key_exists('body', $response)) {
      $response_code = $response['response']['code'];
      if ($response_code == "200"){
        return "SUCCESS";
      } else {
        return "FAIL";
      }
    } else {
      return "FAIL";
    }
  }

  /**
   * Locate and require a template, and extract some variables
   * to be used in that template.
   *
   * @param string $template  the name of the template
   * @param array  $vars      any variables to be extracted into the template
   */
  public static function load_template($template, $vars = array()){
    // you cannot let locate_template to load your template
    // because WP devs made sure you can't pass
    // variables to your template :(

    $template_path = SHAREAHOLIC_DIR . '/templates/' . $template . '.php';

    // load it
    extract($vars);
    require $template_path;
  }


  /**
   * Share Counts API Connectivity check
   *
   */
   public static function share_counts_api_connectivity_check() {

    // if we already checked and it is successful, then do not call the API again
    $share_counts_connect_check = self::get_option('share_counts_connect_check');
    if (isset($share_counts_connect_check) && $share_counts_connect_check == 'SUCCESS') {
      return $share_counts_connect_check;
    }

    $services_config = ShareaholicSeqShareCount::get_services_config();
    $services = array_keys($services_config);
    $param_string = implode('&services[]=', $services);
    $share_counts_api_url = url('shareaholic/api/share_counts/v1', array('absolute' => TRUE)) . '?action=shareaholic_share_counts_api&url=https%3A%2F%2Fwww.google.com%2F&services[]=' . $param_string;
    $cache_key = 'share_counts_api_connectivity_check';
    // fetch cached response if it exists or has not expired
    $response = ShareaholicCache::get($cache_key);

    if (!$response) {
      $response = ShareaholicHttp::send($share_counts_api_url, array('method' => 'GET'), true);
    }

    $response_status = self::get_share_counts_api_status($response);
    // if this was the first time we are doing this and it failed, disable
    // the share counts API
    if (empty($share_counts_connect_check) && $response_status == 'FAIL') {
      self::update_options(array('disable_internal_share_counts_api' => 'on'));
    }

    if ($response_status == 'SUCCESS') {
      ShareaholicCache::set($cache_key, $response, SHARE_COUNTS_CHECK_CACHE_LENGTH);
    }

    self::update_options(array('share_counts_connect_check' => $response_status));
    return $response_status;
   }

  /**
   * Check the share counts API for empty response or missing services
   */
  public static function get_share_counts_api_status($response) {
    if (!$response || !isset($response['body'])) {
      return 'FAIL';
    }

    $response['body'] = json_decode($response['body'], TRUE);

    if (!is_array($response['body'])) {
      return 'FAIL';
    }

    // Did it return at least 6 services?
    $has_majority_services = count(array_keys($response['body']['data'])) >= 5 ? true : false;
    $has_important_services = true;
    // Does it have counts for fb, pinterest?
    foreach (array('facebook', 'pinterest') as $service) {
      if (!isset($response['body']['data'][$service]) || !is_numeric($response['body']['data'][$service])) {
        $has_important_services = false;
      }
    }

    if (!$has_majority_services || !$has_important_services) {
      return 'FAIL';
    }

    return 'SUCCESS';
  }


  /**
   * Return host domain of Drupal install
   *
   * @return string
   */
  public static function get_host() {
    $parse = parse_url($GLOBALS['base_url']);
    return $parse['host'];
  }


}
