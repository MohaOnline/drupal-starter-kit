<?php

/**
 * @file
 * Does some of the things.
 */

class Provision_Service_logs extends Provision_Service {
  public $service = 'logs';

  static function subscribe_site($context) {
    $context->setProperty('logs_enabled');
    $context->setProperty('logs_available');
    $context->setProperty('logs_visible');
  }

}
