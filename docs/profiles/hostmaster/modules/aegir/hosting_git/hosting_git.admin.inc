<?php

/**
 * @file
 * Admin UI code for Git support
 */

/**
 * General settings form.
 */
function hosting_git_settings_form() {
  $form = array();
  $form['deploy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Deploy from Git'),
  );
  $form['deploy']['hosting_git_allow_deploy_site'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow deploying sites from git repositories.'),
    '#default_value' => variable_get('hosting_git_allow_deploy_site', TRUE),
  );
  $form['deploy']['hosting_git_allow_deploy_platform'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow deploying platforms from git repositories.'),
    '#default_value' => variable_get('hosting_git_allow_deploy_platform', TRUE),
  );
  return system_settings_form($form);
}
