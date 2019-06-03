<?php

/**
 * @file
 * Media skyfish browser window.
 */

include_once 'media_skyfish.admin.inc';

/**
 * Media browser plugin for displaying a specific view and display.
 */
class MediaSkyfishBrowser extends MediaBrowserPlugin {

  /**
   * Implements MediaBrowserPluginInterface::access().
   */
  public function access($account = NULL) {
    return user_access('configure own skyfish', $account) || user_access('configure skyfish', $account);
  }

  /**
   * Implements MediaBrowserPlugin::view().
   */
  public function view() {
    if (user_access('configure skyfish')) {
      $token = media_skyfish_get_api_token();
      $build['form'] = drupal_get_form('media_skyfish_add', $token);
      return $build;
    }
  }

}
