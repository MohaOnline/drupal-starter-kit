<?php
/**
 * @file
 * dvg_media_file_types.dvg.user_permission.inc
 */

/**
 * Implements hook_dvg_default_permissions().
 */
function dvg_media_file_types_dvg_default_permissions() {
  $permissions = array();

  // Exported permission: 'access media browser'.
  $permissions['access media browser'] = array(
    'name' => 'access media browser',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'media',
  );

  // Exported permission: 'administer file types'.
  $permissions['administer file types'] = array(
    'name' => 'administer file types',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'administer files'.
  $permissions['administer files'] = array(
    'name' => 'administer files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'administer manualcrop settings'.
  $permissions['administer manualcrop settings'] = array(
    'name' => 'administer manualcrop settings',
    'roles' => array(),
    'module' => 'manualcrop',
  );

  // Exported permission: 'administer media browser'.
  $permissions['administer media browser'] = array(
    'name' => 'administer media browser',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'media',
  );

  // Exported permission: 'administer media wysiwyg view mode'.
  $permissions['administer media wysiwyg view mode'] = array(
    'name' => 'administer media wysiwyg view mode',
    'roles' => array(),
    'module' => 'media_wysiwyg_view_mode',
  );

  // Exported permission: 'bypass file access'.
  $permissions['bypass file access'] = array(
    'name' => 'bypass file access',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'create files'.
  $permissions['create files'] = array(
    'name' => 'create files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete any dvg_document files'.
  $permissions['delete any dvg_document files'] = array(
    'name' => 'delete any dvg_document files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete any dvg_image files'.
  $permissions['delete any dvg_image files'] = array(
    'name' => 'delete any dvg_image files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete any pdf files'.
  $permissions['delete any pdf files'] = array(
    'name' => 'delete any pdf files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete own dvg_document files'.
  $permissions['delete own dvg_document files'] = array(
    'name' => 'delete own dvg_document files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete own dvg_image files'.
  $permissions['delete own dvg_image files'] = array(
    'name' => 'delete own dvg_image files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'delete own pdf files'.
  $permissions['delete own pdf files'] = array(
    'name' => 'delete own pdf files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download any dvg_document files'.
  $permissions['download any dvg_document files'] = array(
    'name' => 'download any dvg_document files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download any dvg_image files'.
  $permissions['download any dvg_image files'] = array(
    'name' => 'download any dvg_image files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download any pdf files'.
  $permissions['download any pdf files'] = array(
    'name' => 'download any pdf files',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
      'editor' => 'editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download own dvg_document files'.
  $permissions['download own dvg_document files'] = array(
    'name' => 'download own dvg_document files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download own dvg_image files'.
  $permissions['download own dvg_image files'] = array(
    'name' => 'download own dvg_image files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'download own pdf files'.
  $permissions['download own pdf files'] = array(
    'name' => 'download own pdf files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit any dvg_document files'.
  $permissions['edit any dvg_document files'] = array(
    'name' => 'edit any dvg_document files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit any dvg_image files'.
  $permissions['edit any dvg_image files'] = array(
    'name' => 'edit any dvg_image files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit any pdf files'.
  $permissions['edit any pdf files'] = array(
    'name' => 'edit any pdf files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit own dvg_document files'.
  $permissions['edit own dvg_document files'] = array(
    'name' => 'edit own dvg_document files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit own dvg_image files'.
  $permissions['edit own dvg_image files'] = array(
    'name' => 'edit own dvg_image files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'edit own pdf files'.
  $permissions['edit own pdf files'] = array(
    'name' => 'edit own pdf files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'use manualcrop'.
  $permissions['use manualcrop'] = array(
    'name' => 'use manualcrop',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'manualcrop',
  );

  // Exported permission: 'view files'.
  $permissions['view files'] = array(
    'name' => 'view files',
    'roles' => array(
      'administrator' => 'administrator',
      'editor' => 'editor',
      'super editor' => 'super editor',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'view own files'.
  $permissions['view own files'] = array(
    'name' => 'view own files',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'view own private files'.
  $permissions['view own private files'] = array(
    'name' => 'view own private files',
    'roles' => array(
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'file_entity',
  );

  // Exported permission: 'view private files'.
  $permissions['view private files'] = array(
    'name' => 'view private files',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'file_entity',
  );

  return $permissions;
}
