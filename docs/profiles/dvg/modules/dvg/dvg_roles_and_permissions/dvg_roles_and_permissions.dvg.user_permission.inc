<?php
/**
 * @file
 * dvg_roles_and_permissions.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_roles().
 */
function dvg_roles_and_permissions_dvg_default_roles() {
  $roles = array();

  $roles['anonymous user'] = array(
    'name' => 'anonymous user',
    'weight' => 0,
  );
  $roles['authenticated user'] = array(
    'name' => 'authenticated user',
    'weight' => 1,
  );
  $roles['administrator'] = array(
    'name' => 'administrator',
    'weight' => 10,
  );
  $roles['editor'] = array(
    'name' => 'editor',
    'weight' => 20,
  );
  $roles['super editor'] = array(
    'name' => 'super editor',
    'weight' => 30,
  );

  return $roles;
}

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_roles_and_permissions_dvg_default_permissions() {
  $permissions = array();

  $permissions['administer dvg_roles_and_permissions'] = array(
    'name' => 'administer dvg_roles_and_permissions',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'dvg_roles_and_permissions',
  );

  $permissions['edit basic webform components settings'] = array(
    'name' => 'edit basic webform components settings',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_roles_and_permissions',
  );

  $permissions['edit basic webform configuration settings'] = array(
    'name' => 'edit basic webform configuration settings',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_roles_and_permissions',
  );

  $permissions['delete terms in related_pages_title'] = array(
    'name' => 'delete terms in related_pages_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  $permissions['edit terms in related_pages_title'] = array(
    'name' => 'edit terms in related_pages_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy',
  );

  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'system',
  );

  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'node',
  );

  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'contextual',
  );

  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'user',
  );

  $permissions['add terms in related_pages_title'] = array(
    'name' => 'add terms in related_pages_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'taxonomy_access_fix',
  );

  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'block',
  );

  $permissions['administer content types'] = array(
    'name' => 'administer content types',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  $permissions['administer empty page callbacks'] = array(
    'name' => 'administer empty page callbacks',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'empty_page',
  );

  $permissions['administer functional content'] = array(
    'name' => 'administer functional content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'functional_content',
  );

  $permissions['administer functional content settings'] = array(
    'name' => 'administer functional content settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'functional_content',
  );

  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'menu',
  );

  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  $permissions['administer redirects'] = array(
    'name' => 'administer redirects',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'redirect',
  );

  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'user',
  );

  $permissions['assign all roles'] = array(
    'name' => 'assign all roles',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'role_delegation',
  );

  $permissions['assign editor role'] = array(
    'name' => 'assign editor role',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'role_delegation',
  );

  $permissions['assign super editor role'] = array(
    'name' => 'assign super editor role',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'role_delegation',
  );

  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'path',
  );

  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  $permissions['dvg administer menu-footer-menu'] = array(
    'name' => 'dvg administer menu-footer-menu',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_ct_menu_page',
  );

  $permissions['dvg administer menu-social-media-menu'] = array(
    'name' => 'dvg administer menu-social-media-menu',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_ct_menu_page',
  );

  $permissions['dvg_global administer functional content'] = array(
    'name' => 'dvg_global administer functional content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'dvg_global',
  );

  $permissions['dvg_global administer texts'] = array(
    'name' => 'dvg_global administer texts',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  $permissions['schedule (un)publishing of nodes'] = array(
    'name' => 'schedule (un)publishing of nodes',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'scheduler',
  );

  $permissions['use media browser library'] = array(
    'name' => 'use media browser library',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_media_file_types',
  );

  $permissions['use media browser my library'] = array(
    'name' => 'use media browser my library',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_media_file_types',
  );

  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'node',
  );

  $permissions['view term page in related_pages_title'] = array(
    'name' => 'view term page in related_pages_title',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'dvg_global',
  );

  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'system',
  );

  $permissions['access administration menu'] = array(
    'name' => 'access administration menu',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'admin_menu',
  );

  $permissions['manage xmlsitemap'] = array(
    'name' => 'Manage xmlsitemap',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'xmlsitemap',
  );

  $permissions['administer xmlsitemap content settings'] = array(
    'name' => 'Administer xmlsitemap content settings',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'xmlsitemap',
  );

  $permissions['administer xmlsitemap'] = array(
    'name' => 'Administer xmlsitemap',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'xmlsitemap',
  );

  return $permissions;
}
