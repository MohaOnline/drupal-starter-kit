<?php
/**
 * @file
 * The Provision_Service_platform_git class..
 */

class Provision_Service_platform_git extends Provision_Service {
  public $service = 'platform_git';

  static function subscribe_platform($context) {
    $context->setProperty('git_repository_url');
    $context->setProperty('git_repository_path');
    $context->setProperty('git_reference');
  }

}
