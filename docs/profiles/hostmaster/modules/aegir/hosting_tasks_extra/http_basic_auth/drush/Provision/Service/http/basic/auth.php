<?php

/**
 * The http_basic_auth service base class.
 */
class Provision_Service_http_basic_auth extends Provision_Service {
  public $service = 'http_basic_auth';

  /**
   * Add the needed properties to the site context.
   */
  static function subscribe_site($context) {
    $context->setProperty('http_basic_auth_username');
    $context->setProperty('http_basic_auth_password');
    $context->setProperty('http_basic_auth_message');
    $context->setProperty('http_basic_auth_whitelist');
  }
}

