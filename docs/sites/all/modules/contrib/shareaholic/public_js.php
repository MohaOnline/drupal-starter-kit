<?php
/**
 * Holds the ShareaholicPublicJS class.
 *
 * @package shareaholic
 */

/**
 * This class gets the necessary components ready
 * for rendering the shareaholic js code for the template
 *
 * @package shareaholic
 */
class ShareaholicPublicJS {

  /**
   * Get _SHR_SETTINGS config for shareaholic js
   */
  public static function get_base_settings() {
    $base_settings = array();

    $disable_share_counts_api = ShareaholicUtilities::get_option('disable_internal_share_counts_api');
    $share_counts_connect_check = ShareaholicUtilities::get_option('share_counts_connect_check');

    if (isset($disable_share_counts_api)) {
      if (isset($share_counts_connect_check) && $share_counts_connect_check == 'SUCCESS' && $disable_share_counts_api != 'on') {
        $base_settings['endpoints'] = array(
          'share_counts_url' => url('shareaholic/api/share_counts/v1', array('absolute' => TRUE))
        );
      }
    }

    return $base_settings;
  }

  public static function get_overrides() {
    $output = '';

    if (ShareaholicUtilities::get_env() === 'staging') {
      $output = "data-shr-environment='stage' data-shr-assetbase='//cdn-staging-shareaholic.s3.amazonaws.com/v2/'";
    }

    return $output;
  }

}
