<?php
/**
 * @file
 * pushtape_admin.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function pushtape_admin_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access administration pages'.
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access content'.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access content overview'.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access site in maintenance mode'.
  $permissions['access site in maintenance mode'] = array(
    'name' => 'access site in maintenance mode',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer blocks'.
  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'block',
  );

  // Exported permission: 'administer site configuration'.
  $permissions['administer site configuration'] = array(
    'name' => 'administer site configuration',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer themes'.
  $permissions['administer themes'] = array(
    'name' => 'administer themes',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'system',
  );

  // Exported permission: 'create album content'.
  $permissions['create album content'] = array(
    'name' => 'create album content',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create track content'.
  $permissions['create track content'] = array(
    'name' => 'create track content',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own album content'.
  $permissions['delete own album content'] = array(
    'name' => 'delete own album content',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own track content'.
  $permissions['delete own track content'] = array(
    'name' => 'delete own track content',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own album content'.
  $permissions['edit own album content'] = array(
    'name' => 'edit own album content',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own track content'.
  $permissions['edit own track content'] = array(
    'name' => 'edit own track content',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'node',
  );

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(),
    'module' => 'filter',
  );

  // Exported permission: 'use text format full_html'.
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(),
    'module' => 'filter',
  );

  // Exported permission: 'view own unpublished content'.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'artist' => 'artist',
    ),
    'module' => 'system',
  );

  return $permissions;
}
