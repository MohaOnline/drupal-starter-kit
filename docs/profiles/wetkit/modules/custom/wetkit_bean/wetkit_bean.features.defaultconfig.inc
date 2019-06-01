<?php
/**
 * @file
 * wetkit_bean.features.defaultconfig.inc
 */

/**
 * Implements hook_defaultconfig_features().
 */
function wetkit_bean_defaultconfig_features() {
  return array(
    'wetkit_bean' => array(
      'user_default_permissions' => 'user_default_permissions',
    ),
  );
}

/**
 * Implements hook_defaultconfig_user_default_permissions().
 */
function wetkit_bean_defaultconfig_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'view any wetkit_bean bean'.
  $permissions['view any wetkit_bean bean'] = array(
    'name' => 'view any wetkit_bean bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_highlight bean'.
  $permissions['view any wetkit_highlight bean'] = array(
    'name' => 'view any wetkit_highlight bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_link bean'.
  $permissions['view any wetkit_link bean'] = array(
    'name' => 'view any wetkit_link bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_media bean'.
  $permissions['view any wetkit_media bean'] = array(
    'name' => 'view any wetkit_media bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_rate bean'.
  $permissions['view any wetkit_rate bean'] = array(
    'name' => 'view any wetkit_rate bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_search bean'.
  $permissions['view any wetkit_search bean'] = array(
    'name' => 'view any wetkit_search bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_share bean'.
  $permissions['view any wetkit_share bean'] = array(
    'name' => 'view any wetkit_share bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_slide bean'.
  $permissions['view any wetkit_slide bean'] = array(
    'name' => 'view any wetkit_slide bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_slideout bean'.
  $permissions['view any wetkit_slideout bean'] = array(
    'name' => 'view any wetkit_slideout bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view any wetkit_twitter bean'.
  $permissions['view any wetkit_twitter bean'] = array(
    'name' => 'view any wetkit_twitter bean',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  // Exported permission: 'view bean page'.
  $permissions['view bean page'] = array(
    'name' => 'view bean page',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bean',
  );

  return $permissions;
}
