<?php

/**
 * @file
 * Defines page callbacks for DvG Authentication.
 */

use Drupal\dvg_authentication\AuthenticationManager;

/**
 * Page callback to provide a list of all available login types.
 *
 * Includes a link to each individual login callback.
 *
 * @todo: This is a temporary page for testing and development.
 */
function dvg_authentication_login_page() {
  drupal_page_is_cacheable(FALSE);
  // Show a selection of the currently available Authentication methods.
  $authentication_manager = AuthenticationManager::getInstance();
  $providers = $authentication_manager->getAuthenticationProviders();
  $login_options = [];
  // Grab the selection options for every enabled level.
  foreach ($providers as $provider_id => $provider) {
    foreach ($provider->getEnabledLevels() as $level_key => $level) {
      $login_part = $authentication_manager->buildLoginSelection([$provider_id => ['level' => $level_key]]);
      if (empty($login_options)) {
        $login_options = $login_part;
      }
      else {
        $login_options['#children'][] = reset($login_part['#children']);
      }
    }
  }
  $build['external_login_options'] = $login_options;
  return $build;
}

/**
 * Process the variables for the login options selection screen.
 *
 * @param array $variables
 *   An associative array containing:
 *    - $title: Title to show above the selection list
 *    - $description: Additional description to show above the selection.
 *    - $attributes: HTML attributes. Usually renders classes.
 *
 * @see dvg-authentication-login-options.tpl.php
 */
function template_preprocess_authentication_login_options(array &$variables) {
  if (!empty($variables['element']['#title'])) {
    $variables['title'] = trim(check_plain($variables['element']['#title']));
  }
  $variables['login_options'] = $variables['element']['#children'];
  $variables['attributes'] = '';
  if (!empty($variables['element']['#attributes'])) {
    $variables['attributes'] = drupal_attributes($variables['element']['#attributes']);
  }
}

/**
 * Process variables for dvg-authentication-login-button.tpl.php.
 *
 * @param array $variables
 *   An associative array containing:
 *   - $login-button.
 *
 * @see dvg-authentication-login-button.tpl.php
 */
function template_preprocess_authentication_login_button(array &$variables) {
  $variables['link'] = $variables['element']['#link'];

  if (!empty($variables['element']['#title'])) {
    $variables['title'] = $variables['element']['#title'];
  }
  if (!empty($variables['element']['#description'])) {
    $variables['description'] = $variables['element']['#description'];
  }
  if (!empty($variables['element']['#logo'])) {
    $variables['logo'] = $variables['element']['#logo'];
  }
  if (!empty($variables['element']['#level_indicator'])) {
    $variables['level_indicator'] = $variables['element']['#level_indicator'];
  }
  $attributes = [];
  if (!empty($variables['element']['#attributes'])) {
    $attributes += $variables['element']['#attributes'];
  }
  $attributes['class'][] = 'dvgauth__item';
  $variables['button_attributes'] = drupal_attributes($attributes);
}

/**
 * Process variables for dvg-authentication-login-button.tpl.php.
 *
 * @param array $variables
 *   Theming variables.
 */
function template_preprocess_authentication_svg_logo(array &$variables) {
  if (isset($variables['element']['#path'])) {
    $variables['path'] = $variables['element']['#path'];
  }

  $variables['attributes'] = '';
  $attributes = [
    'title' => $variables['element']['#title'],
    'alt' => $variables['element']['#alt'],
  ];
  if (!empty($variables['element']['#attributes'])) {
    $attributes += $variables['element']['#attributes'];
  }
  $variables['attributes'] = drupal_attributes($attributes);
}
