<?php
/**
 * @file
 * wetkit_users.path_breadcrumbs.inc
 */

/**
 * Implements hook_path_breadcrumbs_settings_info().
 */
function wetkit_users_path_breadcrumbs_settings_info() {
  $export = array();

  $path_breadcrumb = new stdClass();
  $path_breadcrumb->api_version = 1;
  $path_breadcrumb->machine_name = 'wetkit_pm_existing_pages_user';
  $path_breadcrumb->name = 'Page Manager Existing Pages: User';
  $path_breadcrumb->path = 'user';
  $path_breadcrumb->data = array(
    'titles' => array(
      0 => 'Log in',
    ),
    'paths' => array(
      0 => '<none>',
    ),
    'home' => 1,
    'translatable' => 1,
    'arguments' => array(),
    'access' => array(),
  );
  $path_breadcrumb->weight = -96;
  $export['wetkit_pm_existing_pages_user'] = $path_breadcrumb;

  $path_breadcrumb = new stdClass();
  $path_breadcrumb->api_version = 1;
  $path_breadcrumb->machine_name = 'wetkit_pm_existing_pages_user_password';
  $path_breadcrumb->name = 'Page Manager Existing Pages: User Password';
  $path_breadcrumb->path = 'user/password';
  $path_breadcrumb->data = array(
    'titles' => array(
      0 => 'Log in',
      1 => 'Request Password',
    ),
    'paths' => array(
      0 => 'user/login',
      1 => '<none>',
    ),
    'home' => 1,
    'translatable' => 1,
    'arguments' => array(),
    'access' => array(),
  );
  $path_breadcrumb->weight = -95;
  $export['wetkit_pm_existing_pages_user_password'] = $path_breadcrumb;

  $path_breadcrumb = new stdClass();
  $path_breadcrumb->api_version = 1;
  $path_breadcrumb->machine_name = 'wetkit_pm_existing_pages_user_register';
  $path_breadcrumb->name = 'Page Manager Existing Pages: User Register';
  $path_breadcrumb->path = 'user/register';
  $path_breadcrumb->data = array(
    'titles' => array(
      0 => 'Create an Account',
    ),
    'paths' => array(
      0 => '<none>',
    ),
    'home' => 1,
    'translatable' => 1,
    'arguments' => array(),
    'access' => array(),
  );
  $path_breadcrumb->weight = 0;
  $export['wetkit_pm_existing_pages_user_register'] = $path_breadcrumb;

  return $export;
}
