<?php

/**
 *  @file
 *  Create a Archive Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $archive = new ResourceArchiveStreamWrapper('archive://?v=[video-code]');
 */
class MediaArchiveStreamWrapper extends MediaReadOnlyStreamWrapper {

  // Overrides $base_url defined in MediaReadOnlyStreamWrapper.
  protected $base_url = 'https://archive.org/details';

  protected function _parse_url($url) {
    $path = explode('://', $url);
    $parts = explode('/',  $url[1]);

    $properties = MediaInternetArchiveHandler::getVideoProperties($path[1]);
    $params['v'] = $path[1];
    $params['video_thumb'] = $properties['misc']['image'];
    $matches = explode('/', $properties['misc']['image']);
    $params['video_dir'] = $matches[3];
    $params['video_videofirst'] = $matches[5];
    $params['video_videosecond'] = $matches[6];

    return $params;
  }

  /**
   * Returns a url in the format "http://archive.org/id".
   *
   * Overrides interpolateUrl() defined in MediaReadOnlyStreamWrapper.
   * This is an exact copy of the function in MediaReadOnlyStreamWrapper,
   * here in case that example is redefined or removed.
   */
  function interpolateUrl() {
    if ($parameters = $this->get_parameters()) {
      //return $this->base_url . '?' . http_build_query($parameters);
      return $this->base_url . '/' . $parameters['v'];
    }
  }
  static function getMimeType($uri, $mapping = NULL) {
    return 'video/archive';
  }

  function getTarget($f) {
    return FALSE;
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    return check_plain($parts['video_thumb']);
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = 'public://media-archive/' . check_plain($parts['v']) . '.jpg';

    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $response = drupal_http_request($this->getOriginalThumbnailPath());
      if (!isset($response->error)) {
        file_save_data($response->data, $local_path, TRUE);
      }
      else {
        @copy($this->getOriginalThumbnailPath(), $local_path);
      }
    }
    return $local_path;
  }
}
