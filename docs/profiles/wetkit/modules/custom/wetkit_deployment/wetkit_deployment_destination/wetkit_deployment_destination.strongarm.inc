<?php
/**
 * @file
 * wetkit_deployment_destination.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function wetkit_deployment_destination_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uuid_redirect_external_base_url';
  $strongarm->value = 'http://normandy.dev:8080';
  $export['uuid_redirect_external_base_url'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'uuid_redirect_menu_paths';
  $strongarm->value = 'block/%bean_delta/edit
block/%bean_delta/delete
block/%bean_delta/revisions
menu/item/%menu_link/edit
menu/item/%menu_link/delete
node/%node/edit
node/%node/delete
node/%node/revisions
file/%file/edit
file/%file/delete
taxonomy/term/%taxonomy_term/edit
user/%user/edit
user/%user/delete';
  $export['uuid_redirect_menu_paths'] = $strongarm;

  return $export;
}
