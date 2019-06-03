<?php
/**
 * @file
 * The Provision_Service_platform_composer_git class.
 */

class Provision_Service_platform_composer_git extends Provision_Service {
  public $service = 'platform_composer_git';

  static function subscribe_platform($context) {
    $context->setProperty('composer_git_project_url');
    $context->setProperty('composer_git_path');
    $context->setProperty('composer_git_version');
  }

}
