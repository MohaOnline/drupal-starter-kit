<?php

/**
 * @file The hosting feature definition for hosting_platform_composer_git.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_platform_composer_git_hosting_feature() {
  $features['platform_composer_git'] = array(
    'title' => t('Composer Deploy via Git'),
    'description' => t('Deploy Composer-based platforms from Git repositories.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_platform_composer_git',
    'group' => 'optional',
  );

  return $features;
}
