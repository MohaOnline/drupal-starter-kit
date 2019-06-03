<?php
/**
 * @file
 * dvg_webform_component_collections.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function dvg_webform_component_collections_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_webform_collection';
  $strongarm->value = '0';
  $export['language_content_type_webform_collection'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_options_webform_collection';
  $strongarm->value = array();
  $export['menu_options_webform_collection'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_parent_webform_collection';
  $strongarm->value = 'main-menu:0';
  $export['menu_parent_webform_collection'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_webform_collection';
  $strongarm->value = array(
    0 => 'revision_moderation',
  );
  $export['node_options_webform_collection'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_preview_webform_collection';
  $strongarm->value = '0';
  $export['node_preview_webform_collection'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_submitted_webform_collection';
  $strongarm->value = 0;
  $export['node_submitted_webform_collection'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'webform_node_webform_collection';
  $strongarm->value = 1;
  $export['webform_node_webform_collection'] = $strongarm;

  return $export;
}
