<?php

/**
 * @file
 * Does some of the things.
 */

class Provision_Service_git extends Provision_Service {
  public $service = 'git';

  static function subscribe_platform($context) {
    $context->setProperty('repo_url');
    $context->setProperty('repo_path');
    $context->setProperty('deploy_from_git');
    $context->setProperty('git_ref');
  }

  static function subscribe_site($context) {
    $context->setProperty('repo_url');
    $context->setProperty('deploy_from_git');
    $context->setProperty('git_ref');
  }

}
