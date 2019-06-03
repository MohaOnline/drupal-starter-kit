<?php
/**
 * @file
 * dvg_wysiwyg.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_wysiwyg_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer ckeditor'.
  $permissions['administer ckeditor'] = array(
    'name' => 'administer ckeditor',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ckeditor',
  );

  // Exported permission: 'administer filters'.
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'customize ckeditor'.
  $permissions['customize ckeditor'] = array(
    'name' => 'customize ckeditor',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ckeditor',
  );

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format email_html'.
  $permissions['use text format email_html'] = array(
    'name' => 'use text format email_html',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'administer token insert settings'.
  $permissions['administer token insert settings'] = array(
    'name' => 'administer token insert settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'token_insert',
  );

  // Exported permission: 'use token insert'.
  $permissions['use token insert'] = array(
    'name' => 'use token insert',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
      'editor' => 'editor',
    ),
    'module' => 'token_insert',
  );

  // Exported permission: 'use media wysiwyg'.
  $permissions['use media wysiwyg'] = array(
    'name' => 'use media wysiwyg',
    'roles' => array(
      'administrator' => 'administrator',
      'super editor' => 'super editor',
      'editor' => 'editor',
    ),
    'module' => 'media_wysiwyg',
  );

  return $permissions;
}
