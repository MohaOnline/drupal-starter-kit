<?php

/**
 * @file
 * Provision/Drush hooks for git commands.
 */

/**
 * Implements drush_HOOK_COMMAND_validate().
 *
 * This needs to be called in the validate step so that it runs before the
 * Makefile check and the check for a Drupal installation. Those operations,
 * very inconveniently, run in the same function.
 */
function drush_platform_git_provision_verify_validate() {
  if (d()->type == 'platform' && d()->git_repository_url) {
    $platform = new Provision_PlatformGit();
    return $platform->validateProvisionVerify();
  }
}

/**
 * Implements hook_composer_install_path().
 */
function platform_git_composer_install_path() {
  return d()->git_repository_path;
}

/**
 * Implements drush_HOOK_post_COMMAND() for provision-delete command.
 *
 * If repository path is different from root, delete it too.
 */
function drush_platform_git_post_provision_delete() {
  if (d()->type =='platform') {
    $platform = new Provision_PlatformGit();
    return $platform->postProvisionDelete();
  }
}

/**
 * Register our directory as a place to find provision classes.
 */
function platform_git_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_init().
 */
function platform_git_drush_init() {
  platform_git_register_autoload();
}

/**
 *  Implements hook_provision_services().
 */
function platform_git_provision_services() {
  platform_git_register_autoload();
  return array('platform_git' => NULL);
}
