<?php

/**
 * @file
 * Plugin to provide access control based on WxT theme.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Current WxT theme"),
  'description' => t('Control access by checking which WxT theme is in use.'),
  'callback' => 'wetkit_wetboew_theme_access_check',
  'default' => array('theme' => variable_get('wetkit_wetboew_theme', 'theme-wet-boew')),
  'settings form' => 'wetkit_wetboew_theme_access_settings',
  'summary' => 'wetkit_wetboew_theme_access_summary',
);

/**
 * Settings form for the 'by theme' access plugin
 */
function wetkit_wetboew_theme_access_settings($form, &$form_state, $conf) {
  $form['settings']['theme'] = array(
    '#type' => 'select',
    '#options' => _wetkit_wetboew_options(),
    '#title' => t('WxT Themes'),
    '#default_value' => $conf['theme'],
    '#description' => t('This will only be accessed if the current WxT theme is the selected theme.'),
  );
  return $form;
}

/**
 * Check for access.
 */
function wetkit_wetboew_theme_access_check($conf, $context) {
  $theme = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');

  return $conf['theme'] == $theme;
}

/**
 * Provide a summary description based upon the checked roles.
 */
function wetkit_wetboew_theme_access_summary($conf, $context) {
  if (!isset($conf['theme'])) {
    return t('Error, unset WxT theme');
  }
  $theme = variable_get('wetkit_wetboew_theme', 'theme-wet-boew');

  return t('Current WxT theme is "@theme"', array('@theme' => $theme));
}
