<?php

namespace Drupal\campaignion_content_security_policy;

/**
 * Encapsulate Drupal-API into a mockable class for tests.
 */
class Drupal {

  /**
   * Actually send the headers.
   */
  public function addHeader($name, $content) {
    drupal_add_http_header($name, $content);
  }

}
