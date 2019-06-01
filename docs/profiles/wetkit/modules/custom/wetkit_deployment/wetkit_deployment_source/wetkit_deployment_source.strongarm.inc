<?php
/**
 * @file
 * wetkit_deployment_source.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function wetkit_deployment_source_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'pm_existing_pages_disabled_deploy';
  $strongarm->value = FALSE;
  $export['pm_existing_pages_disabled_deploy'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workbench_moderation_default_state_deploy_content_moderated';
  $strongarm->value = 'draft';
  $export['workbench_moderation_default_state_deploy_content_moderated'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'workbench_moderation_default_state_deploy_content_simple';
  $strongarm->value = 'draft';
  $export['workbench_moderation_default_state_deploy_content_simple'] = $strongarm;

  return $export;
}
