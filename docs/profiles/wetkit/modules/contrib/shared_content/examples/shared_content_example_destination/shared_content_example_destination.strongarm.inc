<?php
/**
 * @file
 * shared_content_example_destination.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function shared_content_example_destination_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uuid_redirect_external_base_url';
  $strongarm->value = 'http://source-site.example.com';
  $export['uuid_redirect_external_base_url'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uuid_redirect_menu_paths';
  $strongarm->value = 'node/%node/edit
node/%node/delete
node/%node/revisions
taxonomy/term/%taxonomy_term/edit
file/%file/edit
file/%file/delete';
  $export['uuid_redirect_menu_paths'] = $strongarm;

  return $export;
}
