<?php
/**
 * @file
 * opigno_features_homebox.features.inc
 */

/**
 * Implements hook_views_api().
 */
function opigno_features_homebox_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_homebox().
 */
function opigno_features_homebox_homebox() {
  $homeboxes = array();

  $homeboxes['dashboard'] = array(
    'regions' => 3,
    'cache' => 0,
    'color' => 0,
    'colors' => array(
      0 => '#E4F0F8',
      1 => '#E4F0F8',
      2 => '#E4F0F8',
      3 => '#E4F0F8',
      4 => '#E4F0F8',
      5 => '#E4F0F8',
    ),
    'blocks' => array(
      'privatemsg_privatemsg-new' => array(
        'module' => 'privatemsg',
        'delta' => 'privatemsg-new',
        'region' => 2,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -19,
      ),
      'forum_active' => array(
        'module' => 'forum',
        'delta' => 'active',
        'region' => 2,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -21,
      ),
      'poll_recent' => array(
        'module' => 'poll',
        'delta' => 'recent',
        'region' => 1,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -20,
      ),
      'views_calendar-block_1' => array(
        'module' => 'views',
        'delta' => 'calendar-block_1',
        'region' => 1,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -23,
      ),
      'views_dashboard_new_content-block' => array(
        'module' => 'views',
        'delta' => 'dashboard_new_content-block',
        'region' => 2,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -22,
      ),
      'views_Notifications-block_1' => array(
        'module' => 'views',
        'delta' => 'Notifications-block_1',
        'region' => 1,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -21,
      ),
      'user_online' => array(
        'module' => 'user',
        'delta' => 'online',
        'region' => 2,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -23,
      ),
      'user_new' => array(
        'module' => 'user',
        'delta' => 'new',
        'region' => 1,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -22,
      ),
      'views_active_trainings-block_1' => array(
        'module' => 'views',
        'delta' => 'active_trainings-block_1',
        'region' => 3,
        'movable' => 1,
        'status' => 1,
        'open' => 1,
        'closable' => 1,
        'title' => '',
        'weight' => -20,
      ),
    ),
    'widths' => array(),
    'title' => 'Dashboard',
    'path' => 'dashboard',
    'menu' => 0,
    'enabled' => 1,
    'auto_save' => 1,
    'full' => 0,
    'roles' => array(
      0 => 'anonymous user',
      1 => 'authenticated user',
    ),
  );


  return $homeboxes;
}
