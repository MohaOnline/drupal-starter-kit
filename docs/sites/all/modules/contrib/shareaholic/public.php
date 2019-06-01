<?php
/**
 * Holds the ShareaholicPublic class.
 *
 * @package shareaholic
 */

// Load the necessary libraries for Share Count API
module_load_include('php', 'shareaholic', 'lib/social-share-counts/drupal_http');
module_load_include('php', 'shareaholic', 'lib/social-share-counts/seq_share_count');
module_load_include('php', 'shareaholic', 'lib/social-share-counts/curl_multi_share_count');
module_load_include('php', 'shareaholic', 'public_js');

/**
 * This class is all about drawing the stuff in publishers'
 * templates that visitors can see.
 *
 * @package shareaholic
 */
class ShareaholicPublic {

  /**
   * Inserts the script code snippet into the head of the
   * public pages of the site if they have accepted ToS and have apikey
   */
  public static function insert_script_tag() {
    if (!ShareaholicUtilities::is_admin_page() &&
        ShareaholicUtilities::has_tos_and_apikey()) {
        $markup = self::js_snippet();
        $element = array(
          '#type' => 'markup',
          '#markup' => $markup,
        );
      drupal_add_html_head($element, 'shareaholic_script_tag');
    }
  }

  /**
   * The actual text for the JS snippet because drupal doesn't seem to be
   * able to add JS from template like Wordpress does...
   * Using heredocs for now
   *
   * @return string JS block for shareaholic code
   */
  private static function js_snippet() {
    $api_key = ShareaholicUtilities::get_option('api_key');
    $js_url = ShareaholicUtilities::asset_url('assets/pub/shareaholic.js');
    $base_settings = json_encode(ShareaholicPublicJS::get_base_settings());
    $overrides = ShareaholicPublicJS::get_overrides();

    $js_snippet = <<< DOC
<!-- Growth powered by Shareaholic - https://www.shareaholic.com -->
<link rel='preload' href='$js_url' as='script'>
<script data-cfasync='false'>
  //<![CDATA[
    _SHR_SETTINGS = $base_settings;
  //]]>
</script>
<script 
  data-cfasync='false'
  src='$js_url'
  data-shr-siteid='$api_key'
  async $overrides>
</script>
DOC;
    return $js_snippet;
  }


  /**
   * Inserts the xua-compatible header if the user has accepted
   * ToS and has API key
   */
  public static function set_xua_compatible_header() {
    if(ShareaholicUtilities::has_tos_and_apikey() &&
        !drupal_get_http_header('X-UA-Compatible')) {
      drupal_add_http_header('X-UA-Compatible', 'IE=edge');
    }
  }
  
  /**
   * Inserts dns-prefetch tags on the page
   *
   */
  
  public static function insert_dns_snippet() {
    
    $dns_snippet = <<< DOC
<link rel='dns-prefetch' href='//k4z6w9b5.stackpathcdn.com' />
<link rel='dns-prefetch' href='//cdn.shareaholic.net' />
<link rel='dns-prefetch' href='//www.shareaholic.net' />
<link rel='dns-prefetch' href='//analytics.shareaholic.com' />
<link rel='dns-prefetch' href='//recs.shareaholic.com' />
<link rel='dns-prefetch' href='//go.shareaholic.com' />
<link rel='dns-prefetch' href='//partner.shareaholic.com' />
DOC;

    $element = array(
      '#type' => 'markup',
      '#markup' => $dns_snippet
    );

    drupal_add_html_head($element, 'shareaholic_dns_snippet');
  }
    
  /**
   * Inserts the shareaholic content meta tags on the page
   * On all pages, it will insert the standard content meta tags
   * On full post pages, it will insert page specific content meta tags
   *
   */
  public static function insert_content_meta_tags($node = NULL, $view_mode = NULL, $lang_code = NULL) {
    if(isset($view_mode) && $view_mode === 'rss') {
      return;
    }
    $site_name = ShareaholicUtilities::site_name();
    $api_key = ShareaholicUtilities::get_option('api_key');
    $module_version = ShareaholicUtilities::get_version();
    
    $content_tags = "\n<!-- Shareaholic Content Tags -->";
    
    if (!empty($site_name)) {
      $content_tags .= "\n<meta name='shareaholic:site_name' content='$site_name' />";
    }
    if (empty($lang_code)) {
      if (isset($GLOBALS['language']->language)) {
        $lang_code = $GLOBALS['language']->language;
      }
      $content_tags .= "\n<meta name='shareaholic:language' content='$lang_code' />";
    } else {
      $content_tags .= "\n<meta name='shareaholic:language' content='$lang_code' />";
    }
    if (!empty($api_key)) {
      $content_tags .= "\n<meta name='shareaholic:site_id' content='$api_key' />";
    }
    if (!empty($module_version)) {
      $content_tags .= "\n<meta name='shareaholic:drupal_version' content='$module_version' />";
    }
    if(empty($view_mode) || (isset($view_mode) && ($view_mode === 'teaser' || $view_mode === 'search_result'))) {
      $content_tags .= "\n<meta name='shareaholic:article_visibility' content='private' />";
    }
    
    if(isset($node) && isset($view_mode) && $view_mode === 'full') {
      $url = $GLOBALS['base_root'] . request_uri();
      $published_time = date('c', $node->created);
      $modified_time = date('c', $node->changed);
      $author = user_load($node->uid);
      $author_name = self::get_user_name($author);
      $tags = implode(', ', self::get_keywords_for($node));
      $image_url = self::get_image_url_for($node);
      $visibility = self::get_visibility($node);
      $shareable = self::is_shareable($node);

      $content_tags .= "\n<meta name='shareaholic:url' content='$url' />";
      $content_tags .= "\n<meta name='shareaholic:article_published_time' content='$published_time' />";
      $content_tags .= "\n<meta name='shareaholic:article_modified_time' content='$modified_time' />";
      $content_tags .= "\n<meta name='shareaholic:article_author_name' content='$author_name' />";

      if(!empty($tags)) {
        $content_tags .= "\n<meta name='shareaholic:keywords' content='$tags' />";
      }
      if(!empty($image_url)) {
        $content_tags .= "\n<meta name='shareaholic:image' content='$image_url' />";
      }
      if(!empty($visibility)) {
        $content_tags .= "\n<meta name='shareaholic:article_visibility' content='$visibility' />";
      }
      if(!empty($shareable)) {
        $content_tags .= "\n<meta name='shareaholic:shareable_page' content='$shareable' />";
      }
    }
    $content_tags .= "\n<!-- Shareaholic Content Tags End -->\n";

    $element = array(
      '#type' => 'markup',
      '#markup' => $content_tags
    );

    drupal_add_html_head($element, 'shareaholic_content_meta_tags');
  }

  /**
   * Get the user's name from an account object
   * If the user has a full name, then that is returned
   * Otherwise it returns the user's username
   *
   * @return String the user name
   */
  public static function get_user_name($account) {
    $full_name = isset($account->field_fullname) ? $account->field_fullname : false;
    $full_name = isset($account->field_full_name) ? $account->field_full_name : $full_name;

    if($full_name && isset($full_name['und']['0']['value'])) {
      $full_name = $full_name['und']['0']['value'];
    } else {
      $first_name = isset($account->field_firstname) ? $account->field_firstname : false;
      $first_name = isset($account->field_first_name) ? $account->field_first_name : $first_name;

      $last_name = isset($account->field_lastname) ? $account->field_lastname : false;
      $last_name = isset($account->field_last_name) ? $account->field_last_name : $last_name;

      if(!empty($first_name) && !empty($last_name) && isset($first_name['und']['0']['value']) && isset($last_name['und']['0']['value'])) {
        $full_name = $first_name['und']['0']['value'] . ' ' . $last_name['und']['0']['value'];
      }
    }
    return (!empty($full_name)) ? $full_name : $account->name;
  }

  /**
   * Get a list of tags for a piece of content
   *
   * @return Array an array of terms or empty array
   */
  public static function get_keywords_for($node) {
    $terms = array();
    if (!db_table_exists('taxonomy_index')) {
      return $terms;
    }
    $results = db_query('SELECT tid FROM {taxonomy_index} WHERE nid = :nid', array(':nid' => $node->nid));
    foreach ($results as $result) {
      $term = taxonomy_term_load($result->tid);
      if(empty($term) || empty($term->name)) {
        continue;
      }
      array_push($terms, ShareaholicUtilities::clean_string($term->name));
      $vocabulary = taxonomy_vocabulary_load($term->vid);
      if(empty($vocabulary) || empty($vocabulary->name) || preg_match('/tags/i', $vocabulary->name)) {
        continue;
      }
      array_push($terms, ShareaholicUtilities::clean_string($vocabulary->name));
    }
    $terms = array_unique($terms);
    return $terms;
  }

  /**
   * Get image used in a piece of content
   *
   * @return mixed either returns a string or false if no image is found
   */
  public static function get_image_url_for($node) {
    if(isset($node->field_image['und']['0']['uri'])) {
      return file_create_url($node->field_image['und']['0']['uri']);
    }
    if(isset($node->field_simage['und']['0']['uri'])) {
      return file_create_url($node->field_simage['und']['0']['uri']);
    }
    if(isset($node->body) && isset($node->body['und']['0']['value'])) {
      return self::post_first_image($node->body['und']['0']['value']);
    }
  }

  /**
   * Copied straight out of the wordpress version,
   * this will grab the first image in a post.
   *
   * @return mixed either returns `false` or a string of the image src
   */
  public static function post_first_image($body) {
    preg_match_all('/<img.*?src=[\'"](.*?)[\'"].*?>/i', $body, $matches);
    if(isset($matches) && isset($matches[1][0]) ) {
      // Exclude base64 images; meta tags require full URLs 
      if (strpos($matches[1][0], 'data:') === false) {
        // file_create_url function doesn't convert paths starting with "/" so check for "/" and trim it off if present
        if (substr($matches[1][0], 0, 1) === "/") {
          $first_img = substr($matches[1][0], 1);
        } else {
          $first_img = $matches[1][0];
        }
      } 
    }
    if(empty($first_img)) { // return false if nothing there, makes life easier
      return false;
    }
    return file_create_url($first_img);
  }

  /**
   * Inserts the Shareaholic widget/apps on the page
   * By drawing the canvas on a piece of content
   *
   * @param $node The node object representing a piece of content
   * @param $view_mode The view that tells how to show the content
   * @param $lang_code The language code
   */
  public static function insert_widgets($node, $view_mode, $lang_code) {
    if($view_mode === 'rss') {
      return;
    }
    
    if(isset($node->content)) {
      self::draw_canvases($node, $view_mode);
    }
  }

  /**
   * This static function inserts the shareaholic canvas in a node
   *
   * @param  string $node The node object to insert the canvas into
   */
  public static function draw_canvases(&$node, $view_mode) {
    $settings = ShareaholicUtilities::get_settings();
    $page_type = $node->type;
    $sb_above_weight = -1000;
    $rec_above_weight = -999;
    $sb_below_weight = 1000;
    $rec_below_weight = 1001;
    if($view_mode === 'teaser') {
      $page_type = 'teaser';
    }
    foreach (array('share_buttons', 'recommendations') as $app) {
      if(isset($node->shareaholic_options["shareaholic_hide_{$app}"]) && $node->shareaholic_options["shareaholic_hide_{$app}"]) {
        continue;
      }
      $title = $node->title;
      $summary = isset($node->teaser) ? $node->teaser : '';
      $link = url('node/'. $node->nid, array('absolute' => TRUE));

      if (isset($settings[$app]["{$page_type}_above_content"]) &&
          $settings[$app]["{$page_type}_above_content"] == 'on') {
        $id = $settings['location_name_ids'][$app]["{$page_type}_above_content"];
        $id_name = $page_type.'_above_content';
        $node->content["shareaholic_{$app}_{$page_type}_above_content"] = array(
          '#markup' => self::canvas($id, $app, $id_name, $title, $link),
          '#weight' => ($app === 'share_buttons') ? $sb_above_weight : $rec_above_weight
        );
      }

      if (isset($settings[$app]["{$page_type}_below_content"]) &&
          $settings[$app]["{$page_type}_below_content"] == 'on') {   
        $id = $settings['location_name_ids'][$app]["{$page_type}_below_content"];
        $id_name = $page_type.'_below_content';
        $node->content["shareaholic_{$app}_{$page_type}_below_content"] = array(
          '#markup' => self::canvas($id, $app, $id_name, $title, $link),
          '#weight' => ($app === 'share_buttons') ? $sb_below_weight : $rec_below_weight
        );
      }
    }
  }

  /**
   * Draws an individual canvas given a specific location
   * id and app
   *
   * @param string $id  the location id for configuration
   * @param string $app the type of app
   * @param string $id_name location id name for configuration
   * @param string $title the title of URL
   * @param string $link url
   * @param string $summary summary text for URL
   */
  public static function canvas($id, $app, $id_name = NULL, $title = NULL, $link = NULL, $summary = NULL) {
    $title = trim(htmlspecialchars($title, ENT_QUOTES));
    $link = trim($link);
    $summary = trim(htmlspecialchars(strip_tags($summary), ENT_QUOTES));

    $canvas = "<div class='shareaholic-canvas'
      data-app-id='$id'
      data-app-id-name='$id_name'
      data-app='$app'
      data-title='$title'
      data-link='$link'
      data-summary='$summary'></div>";
    return trim(preg_replace('/\s+/', ' ', $canvas));
  }


  /**
   * Determines the visibility of a piece of content
   * and returns that value
   *
   * Possible values are: 'draft', 'private', and NULL
   *
   * @param Object $node The content to determine its visibility
   * @return String a string indicating its visibility
   */
  public static function get_visibility($node) {
    $visibility = NULL;
    // Check if it is a draft
    if(isset($node->status) && $node->status == 0) {
      $visibility = 'draft';
    }
    // Check if it should be excluded from recommendations
    if(isset($node->shareaholic_options) && $node->shareaholic_options['shareaholic_exclude_from_recommendations']) {
      $visibility = 'private';
    }
    // Check if a site visitor can see the content
    try {
      $anonymous_user = user_load(0);
      if ($anonymous_user && !node_access('view', $node, $anonymous_user)) {
        $visibility = 'private';
      }
    } catch (Exception $e) {
      ShareaholicUtilities::log('Error in checking node_access: ' . $e->getMessage());
    }

    return $visibility;
  }



  /**
   * Determines the shareability of a piece of content
   * and returns that value
   *
   * Possible values are: 'true', 'false', and NULL
   *
   * @param Object $node The content to determine its shareability
   * @return String a string indicating if it is shareable or not
   */
  public static function is_shareable($node) {
    $shareable = NULL;
    // Check if it is a draft
    if(isset($node->status) && $node->status == 0) {
      $shareable = 'false';
    }

    // Check if a site visitor can see the content
    try {
      $anonymous_user = user_load(0);
      if ($anonymous_user && !node_access('view', $node, $anonymous_user)) {
        $shareable = 'false';
      }
    } catch (Exception $e) {
      ShareaholicUtilities::log('Error in checking node_access: ' . $e->getMessage());
    }

    return $shareable;
  }


  /**
   * Function to handle the share count API requests
   *
   */
  public static function share_counts_api() {
    // sometimes the drupal http request function throws errors so setting handler
    set_error_handler(array('ShareaholicPublic', 'custom_error_handler'));
    $debug_mode = isset($_GET['debug']) && $_GET['debug'] === '1';
    $cache_key = 'shr_api_res-' . md5( $_SERVER['QUERY_STRING'] );
    $result = ShareaholicCache::get($cache_key);
    $has_curl_multi = self::has_curl();

    if (!$result) {
      $url = isset($_GET['url']) ? $_GET['url'] : NULL;
      $services = isset($_GET['services']) ? $_GET['services'] : NULL;
      $result = array();
      $options = array();

      if ($debug_mode && isset($_GET['timeout'])) {
        $options['timeout'] = intval($_GET['timeout']);
      }

      if(is_array($services) && count($services) > 0 && !empty($url)) {
        if ($debug_mode && isset($_GET['client'])) {
          if ($has_curl_multi && $_GET['client'] !== 'seq') {
            $shares = new ShareaholicCurlMultiShareCount($url, $services, $options);
          } else {
            $shares = new ShareaholicSeqShareCount($url, $services, $options);
          }
        } else if($has_curl_multi) {
          $shares = new ShareaholicCurlMultiShareCount($url, $services, $options);
        } else {
          $shares = new ShareaholicSeqShareCount($url, $services, $options);
        }
        $result = $shares->get_counts();

        if ($debug_mode) {
          $result['has_curl_multi'] = $has_curl_multi;
          $result['curl_type'] = get_class($shares);
          $result['raw'] = $shares->raw_response;
        }

        if (isset($result['data']) && !$debug_mode) {
          ShareaholicCache::set($cache_key, $result, SHARE_COUNTS_CHECK_CACHE_LENGTH);
        }
      }
    }

    header('Content-Type: application/json');
    echo json_encode($result);
    exit;
  }


  /**
   * Custom error handler
   *
   * @param integer $errno The error level as an integer
   * @param string $errstr The error string
   * @param string $errfile The file where the error occurred
   * @param string $errline The line number where the error occurred
   */
  public static function custom_error_handler($errno, $errstr, $errfile, $errline) {
    ShareaholicUtilities::log($errstr . ' ' . $errfile . ' ' . $errline);
  }

  /**
   * Checks to see if curl is installed
   *
   * @return bool true or false that curl is installed
   */
  public static function has_curl() {
    return function_exists('curl_version') && function_exists('curl_multi_init') && function_exists('curl_multi_add_handle') && function_exists('curl_multi_exec');
  }

  /**
   * Insert the Open Graph Tags
   */
  public static function insert_og_tags($node = false, $view_mode = '') {
    $markup = '';
    $disable_og_tags_check = ShareaholicUtilities::get_option('disable_og_tags');
    if ($disable_og_tags_check && $disable_og_tags_check == 'on') {
      return;
    }
    if ($view_mode != 'full') {
      return;
    }
    if ($node && (!isset($node->shareaholic_options["shareaholic_exclude_og_tags"]) || !$node->shareaholic_options["shareaholic_exclude_og_tags"]) ) {
      $image_url = self::get_image_url_for($node);
      if (!empty($image_url)) {
        $markup .= "\n<!-- Shareaholic Open Graph Tags -->\n";
        $markup .= "<meta property='og:image' content='" . $image_url . "' />";
        $markup .= "\n<!-- Shareaholic Open Graph Tags End -->\n";
      }
    }
    $element = array(
      '#type' => 'markup',
      '#markup' => $markup,
    );
    drupal_add_html_head($element, 'shareaholic_og_tags');
  }

}
