<?php

/**
 * @file The hosting feature definition for hosting_platform_git.
 */

/**
 * Implements hook_hosting_feature().
 */
function hosting_platform_git_hosting_feature() {
  $features['platform_git'] = array(
    'title' => t('Git Deploy (without Composer)'),
    'description' => t('Deploy platforms using Git, that are not managed by Composer.'),
    'status' => HOSTING_FEATURE_DISABLED,
    'module' => 'hosting_platform_git',
    'group' => 'optional',
  );

  return $features;
}
