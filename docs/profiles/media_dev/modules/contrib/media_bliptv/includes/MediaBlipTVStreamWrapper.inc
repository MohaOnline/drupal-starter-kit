<?php

/**
 *  @file
 *  Create a BlipTV Stream Wrapper class for the Media/Resource module.
 */

/**
 *  Create an instance like this:
 *  $bliptv = new ResourceBlipTVStreamWrapper('bliptv://?v=[video-code]');
 */
class MediaBlipTVStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://blip.tv/';

  function getTarget($f) {
    return FALSE;
  }

  static function getMimeType($uri, $mapping = NULL) {
    return 'video/bliptv';
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    //http://blip.tv/players/episode/AYLA3U8C?skin=rss
    $video = media_blip_get_rss('http://blip.tv/players/episode/' . check_plain($parts['v']) . '?skin=rss');
    return 'http://a.images.blip.tv/' . $video['thumbnail_src'];
  }

  function getLocalThumbnailPath() {
    //http://a.images.blip.tv/RoosterTeeth-RedVsBlueReachMiniSeriesEpisode2998.jpg"
    $parts = $this->get_parameters();
    $local_path = 'public://media-bliptv/' . check_plain($parts['v']);
    
    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      @copy($this->getOriginalThumbnailPath(), $local_path);
    }
    return $local_path;
  }
}
