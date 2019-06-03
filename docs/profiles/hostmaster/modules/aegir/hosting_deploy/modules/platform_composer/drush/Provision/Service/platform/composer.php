<?php
/**
 * @file
 * The Provision_Service_platform_composer class.
 */

class Provision_Service_platform_composer extends Provision_Service {
  public $service = 'platform_composer';

  static function subscribe_platform($context) {
    $context->setProperty('composer_project_package');
    $context->setProperty('composer_project_path');
    $context->setProperty('composer_project_version');
  }

}
