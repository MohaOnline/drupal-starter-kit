<?php
/**
 * @file
 * wetkit_deployment_environment.default_environment_indicator_environments.inc
 */

/**
 * Implements hook_default_environment_indicator_environment().
 */
function wetkit_deployment_environment_default_environment_indicator_environment() {
  $export = array();

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'development_environment';
  $environment->name = 'Stage Environment';
  $environment->regexurl = '';
  $environment->settings = array(
    'color' => '#176ca7',
    'text_color' => '#FFFFFF',
    'weight' => '10',
    'position' => 'top',
    'fixed' => 0,
  );
  $export['development_environment'] = $environment;

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'production_environment';
  $environment->name = 'Prod Environment';
  $environment->regexurl = 'prod';
  $environment->settings = array(
    'color' => '#A33',
    'text_color' => '#FFFFFF',
    'weight' => '9',
    'position' => 'top',
    'fixed' => 0,
  );
  $export['production_environment'] = $environment;

  $environment = new stdClass();
  $environment->disabled = FALSE; /* Edit this to true to make a default environment disabled initially */
  $environment->api_version = 1;
  $environment->machine = 'quality_assurance_environment';
  $environment->name = 'QA Environment';
  $environment->regexurl = 'qa';
  $environment->settings = array(
    'color' => '#F9AD81',
    'text_color' => '#FFFFFF',
    'weight' => '9',
    'position' => 'top',
    'fixed' => 0,
  );
  $export['quality_assurance_environment'] = $environment;

  return $export;
}
