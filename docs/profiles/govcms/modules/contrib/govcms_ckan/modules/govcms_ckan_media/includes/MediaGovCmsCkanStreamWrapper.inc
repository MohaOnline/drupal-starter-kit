<?php
/**
 * @file
 * Extends the MediaReadOnlyStreamWrapper class to handle CKAN datasets.
 */

/**
 * MediaGovCmsCkanStreamWrapper class.
 *
 *  Create an instance like this:
 *  $dataset = new MediaYouTubeStreamWrapper('ckan://dataset');
 */
class MediaGovCmsCkanStreamWrapper extends MediaReadOnlyStreamWrapper {

  /**
   * MediaGovCmsCkanStreamWrapper constructor.
   */
  public function __construct() {
    $this->base_url = variable_get('govcms_ckan_endpoint_url', '');
  }

  /**
   * Define the mime type.
   */
  public static function getMimeType($uri, $mapping = NULL) {
    return 'other/ckan';
  }

  /**
   * Get the thumbnail for a graph.
   */
  public function getThumbnailPath() {
    return drupal_get_path('module', 'govcms_ckan_media') . '/images/thumbnail.png';
  }

  /**
   * Returns a web accessible URL for the resource.
   *
   * @return string
   *   Returns a string containing a web accessible URL for the resource.
   */
  public function getExternalUrl() {
    return $this->base_url . '/dataset/' . $this->parseIdFromUri('package') . '/resource/' . $this->parseIdFromUri('resource');
  }

  /**
   * Extract the CKAN resource ID or instance ID from a Drupal URI.
   *
   * @param string $extract
   *   What to extract from the uri.
   *   Options are: 'package', 'resource' or 'instance'.
   *
   * @return string/int
   *   The package, resource or instance ID depending on $extract arg.
   */
  public function parseIdFromUri($extract = 'resource') {
    $target = file_uri_target($this->uri);
    $params = explode('/', $target, 4);
    switch ($extract) {
      case 'package':
        return $params[1];

      case 'resource':
        return $params[2];

      case 'instance':
        return (int) $params[3];
    }
    return NULL;
  }

}
