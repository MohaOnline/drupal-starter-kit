<?php

/**
 * @file
 *   The hosting feature definition for hosting_git.
 */

/**
 * Register a hosting feature with Aegir.
 *
 * This will be used to generate the 'admin/hosting' page.
 *
 * @return
 *  associative array indexed by feature key.
 */
function hosting_git_hosting_feature() {
  $features['git'] = array(
    // title to display in form
    'title' => t('Git integration'),
    // description
    'description' => t('Enables storing git information. Dependency for other git modules.'),
    // initial status ( HOSTING_FEATURE_DISABLED, HOSTING_FEATURE_ENABLED, HOSTING_FEATURE_REQUIRED )
    'status' => HOSTING_FEATURE_DISABLED,
    // module to enable/disable alongside feature
    'module' => 'hosting_git',
    // associate with a specific node type.
    //  'node' => 'nodetype',
    // which group to display in ( null , experimental )
    'group' => 'advanced',
  );

  $features['git_pull'] = array(
    'title' => t('Git Pull'),
    'description' => t('Pull code to the server manually or automatically.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_git_pull',
    'group' => 'advanced',
    'role_permissions' => array(
      'aegir platform manager' => array(
        'create platform git-pull task',
      ),
      'aegir account manager' => array(
        'create git-pull task',
      ),
      'aegir client' => array(
        'create git-pull task',
      ),
    )
  );

  $features['git_checkout'] = array(
    'title' => t('Git Checkout'),
    'description' => t('Change or create new branches.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_git_checkout',
    'group' => 'advanced',
    'role_permissions' => array(
      'aegir platform manager' => array(
        'create platform git-checkout task',
      ),
      'aegir account manager' => array(
        'create site git-checkout task',
      ),
      'aegir client' => array(
        'create site git-checkout task',
      ),
    )
  );

  $features['git_tag'] = array(
    'title' => t('Git Tag'),
    'description' => t('Create new git tags.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_git_tag',
    'group' => 'advanced',
    'role_permissions' => array(
      'aegir platform manager' => array(
        'create platform git-tag task',
      ),
      'aegir account manager' => array(
        'create site git-tag task',
      ),
      'aegir client' => array(
        'create site git-tag task',
      ),
    )
  );

  $features['git_commit'] = array(
    'title' => t('Git Commit'),
    'description' => t('Commit files to git repositories.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_git_commit',
    'group' => 'advanced',
    'role_permissions' => array(
      'aegir platform manager' => array(
        'create platform git-commit task',
      ),
      'aegir account manager' => array(
        'create site git-commit task',
      ),
      'aegir client' => array(
        'create site git-commit task',
      ),
    )
  );

  return $features;
}
