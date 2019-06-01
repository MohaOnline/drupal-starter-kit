<?php
/**
 * File for the ShareaholicContentManager class.
 *
 * @package shareaholic
 */

/**
 * An interface to the Shareaholic Content Manager API's
 *
 * @package shareaholic
 */
class ShareaholicContentManager {


  /**
   * Wrapper for the Shareaholic Content Manager Single Domain worker API
   *
   * @param string $domain
   */
  public static function single_domain_worker($domain = NULL) {
    if ($domain == NULL) {
      $domain = $GLOBALS['base_url'];
    }

    if ($domain != NULL) {
      $single_domain_job_url = ShareaholicUtilities::CM_API_URL . '/jobs/single_domain';
      $data = '{"args":["' . $domain . '", {"force": true}]}';
      $options = array(
        'method' => 'POST',
        'data' => $data,
        'headers' => array('Content-Type' => 'application/json'),
      );
      $response = drupal_http_request($single_domain_job_url, $options);
    }
  }

  /**
   * Wrapper for the Shareaholic Content Manager Single Page worker API
   *
   * @param Object $node The content that was created/updated/deleted
   */
  public static function single_page_worker($node) {
    $page_link = url('node/'. $node->nid, array('absolute' => TRUE));

    if(isset($page_link)) {
      $single_page_job_url = ShareaholicUtilities::CM_API_URL . '/jobs/uber_single_page';
      $data = '{"args":["' . $page_link . '", {"force": true}]}';
      $options = array(
        'method' => 'POST',
        'data' => $data,
        'headers' => array('Content-Type' => 'application/json'),
      );
      $response = drupal_http_request($single_page_job_url, $options);
    }
  }

}