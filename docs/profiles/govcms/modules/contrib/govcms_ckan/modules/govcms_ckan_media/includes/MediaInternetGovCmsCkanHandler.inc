<?php
/**
 * @file
 * Extends the MediaInternetBaseHandler class to handle CKAN Datasets.
 */

/**
 * Implementation of MediaInternetBaseHandler.
 *
 * @see hook_media_internet_providers().
 */
class MediaInternetGovCmsCkanHandler extends MediaInternetBaseHandler {

  protected $baseUrl;
  protected $baseHost;
  protected $basePath;
  protected $baseUri = 'ckan://resource/';

  /**
   * MediaInternetGovCmsCkanHandler constructor.
   */
  public function __construct($embed_code) {
    parent::__construct($embed_code);
    $this->baseUrl = variable_get('govcms_ckan_endpoint_url', '');
    $this->baseHost = $this->getBaseHost();
    $this->basePath = $this->getBasePath();
  }

  /**
   * Parse a dataset url to return a normalised URI with the resource ID.
   *
   * @return string
   *   A normalised file URI. FALSE if no match.
   */
  public function parse($embed_code) {
    // @baseurl/dataset/PACKAGE_ID/resource/RESOURCE_ID/download/filename.csv
    // @baseurl/dataset/PACKAGE_ID/resource/RESOURCE_ID
    $patterns = array(
      '@' . $this->baseHost . $this->basePath . '/dataset/([^"\&\? ]+)/resource/([^"\&\? ]+)/download/(?:[^"\&\? ]+)@i',
      '@' . $this->baseHost . $this->basePath . '/dataset/([^"\&\? ]+)/resource/([^"\&\? ]+)@i',
    );

    // First check if the url contains the embed code.
    if ($this->isCkanUrl($embed_code)) {
      foreach ($patterns as $pattern) {
        preg_match($pattern, $embed_code, $matches);
        if (isset($matches[1]) && isset($matches[2])) {
          $package_id = empty($matches[1]) ? 'package' : $matches[1];
          $next_instance = $this->getNextInstanceId($package_id, $matches[2]);
          $uri = $this->baseUri . $package_id . '/' . $matches[2] . '/' . $next_instance;
          return file_stream_wrapper_uri_normalize($uri);
        }
      }
    }

    return FALSE;
  }

  /**
   * Determines if this handler should claim the item.
   *
   * @param string $embed_code
   *   A string of user-submitted embed code.
   *
   * @return bool
   *   Pass TRUE to claim the item.
   */
  public function claim($embed_code) {
    return $this->parse($embed_code) !== FALSE;
  }

  /**
   * Returns a file object which can be used for validation.
   *
   * @return StdClass
   *   A file object.
   */
  public function getFileObject() {
    $uri = $this->parse($this->embedCode);
    $file = file_uri_to_object($uri, TRUE);

    // Try to default the file name to the resource's title.
    if (empty($file->fid) && $info = $this->getResource($uri)) {
      // Some resources may not have a name so we add a fallback.
      $file->filename = !empty($info->name) ? truncate_utf8($info->name, 255) : t('Untitled dataset');
    }

    return $file;
  }

  /**
   * Returns information about the resource.
   *
   * @param string $uri
   *   The Drupal file URI for this resource.
   *
   * @return object
   *   If resource information is available, an object containing 'name', 'id',
   *   'url', and other information as specified by the CKAN standard.
   *   Otherwise, NULL.
   *
   * @throws Exception
   *   Error if unable to load the resource.
   */
  public function getResource($uri) {
    $meta = govcms_ckan_client_request_meta($this->parseIdFromUri($uri));

    if ($meta->valid === TRUE) {
      return $meta->data;
    }
    else {
      throw new Exception("Unable to retrieve resource information. (Error: {$meta->code}, {$meta->status})");
    }
  }

  /**
   * Extract the CKAN resource ID or instance ID from a Drupal URI.
   *
   * @param string $uri
   *   The drupal file uri.
   * @param string $extract
   *   What to extract from the uri.
   *   Options are 'package', 'resource' or 'instance'.
   *
   * @return string/int
   *   The resource ID or instance ID depending on $extract arg.
   */
  public function parseIdFromUri($uri, $extract = 'resource') {
    $wrapper = file_stream_wrapper_get_instance_by_uri($uri);
    return $wrapper->parseIdFromUri($extract);
  }

  /**
   * Get the next instance id for this resource.
   *
   * This allows for multiple instances of the same resource id.
   *
   * @param string $package_id
   *   The resource ID.
   * @param string $resource_id
   *   The resource ID.
   *
   * @return int
   *   The next unused instance id.
   */
  public function getNextInstanceId($package_id, $resource_id) {
    $query = new EntityFieldQuery();
    $results = $query
      ->entityCondition('entity_type', 'file')
      ->propertyCondition('uri', $this->baseUri . $package_id . '/' . $resource_id, 'STARTS_WITH')
      ->execute();

    // No instances of this resource yet.
    if (!isset($results['file'])) {
      return 0;
    }

    // Get all the instances then add 1 to the highest number.
    $instances = array();
    foreach (file_load_multiple(array_keys($results['file'])) as $file) {
      $instances[] = $this->parseIdFromUri($file->uri, 'instance');
    }
    return (max($instances) + 1);
  }

  /**
   * This simply checks if the domain from config exists in the url string.
   *
   * @param string $url
   *   The url string entered by the user.
   *
   * @return bool
   *   TRUE if match, FALSE if not.
   */
  public function isCkanUrl($url) {
    return (!empty($url) && !empty($this->baseHost) && strpos($url, $this->baseHost) !== FALSE);
  }

  /**
   * Get the base host.
   *
   * @return mixed
   *   If valid host found it is returned, otherwise FALSE.
   */
  public function getBaseHost() {
    $url_parts = parse_url($this->baseUrl);
    $port = (!empty($url_parts['port']) ? ':' . $url_parts['port'] : NULL);
    return (!empty($url_parts['host']) ? $url_parts['host'] . $port : NULL);
  }

  /**
   * Get the base path.
   *
   * @return string
   *   If valid path found it is returned, otherwise an empty string.
   */
  public function getBasePath() {
    $url_parts = parse_url($this->baseUrl);
    return (!empty($url_parts['path']) ? $url_parts['path'] : '');
  }

  /**
   * Build a resource url.
   *
   * @param string $package_id
   *   The package id for the resource.
   * @param string $resource_id
   *   The resource id for the resource.
   *
   * @return string
   *   Return a valid url for the full resource.
   */
  public function buildResourceUrl($package_id, $resource_id) {
    return $this->baseUrl . '/dataset/' . $package_id . '/resource/' . $resource_id;
  }

}
