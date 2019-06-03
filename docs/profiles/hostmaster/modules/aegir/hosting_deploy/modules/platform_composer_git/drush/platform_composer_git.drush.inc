<?php

/**
 * @file
 * Provision/Drush hooks for composer commands.
 */

/**
 * Implements drush_HOOK_COMMAND_validate().
 *
 * This needs to be called in the validate step so that it runs before the
 * Makefile check and the check for a Drupal installation. Those operations,
 * very inconveniently, run in the same function.
 */
function drush_platform_composer_git_provision_verify_validate() {
  if (d()->type == 'platform' && d()->composer_git_project_url) {
    $platform = new Provision_ComposerGitCreateProject();
    return $platform->validateProvisionVerify();
  }
}

 /**
 * Implements drush_HOOK_post_COMMAND() for provision-delete command.
 *
 * If repository path is different from root, delete it too.
 */
function drush_platform_composer_git_post_provision_delete() {
  if (d()->type =='platform') {
    $platform = new Provision_ComposerGitCreateProject();
    return $platform->postProvisionDelete();
  }
}

/**
 * Implements hook_composer_git_install_path().
 */
function platform_composer_git_composer_git_install_path() {
  return d()->composer_git_path;
}

/**
 * Implements hook_composer_git_install_paths_alter().
 */
function platform_composer_git_composer_git_install_paths_alter(&$paths) {
  // Provide a path for hosting_git.module
  $paths[] = d()->repo_path;
}

/**
 * Register our directory as a place to find provision classes.
 */
function platform_composer_git_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_init().
 */
function platform_composer_git_drush_init() {
  platform_composer_git_register_autoload();
}

/**
 *  Implements hook_provision_services().
 */
function platform_composer_git_provision_services() {
  platform_composer_git_register_autoload();
  return array('platform_composer_git' => NULL);
}
