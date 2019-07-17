<?php

/**
 * @file Drush integration for the boost_blast module.
 */

/**
 * Implements hook_drush_command().
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function media_unique_drush_command() {
  $items = array();

  $items['media_unique'] = array(
    'description' => "Immediately run media_unique to remove duplicate/file copies.",
    'drupal dependencies' => array('file_entity', 'media'),
    'aliases' => array('media-unique','file-entity-unique','feu'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function media_unique_drush_help($section) {
  switch ($section) {
    case 'drush:file_entity_unique':
      return dt("Process media entities to search and remove duplicate files.");
      break;
    case 'drush:media_unique':
      return dt("Process media entities to search and remove duplicate files.");
      break;
    case 'drush:feu':
      return dt("Process media entities to search and remove duplicate files.");
      break;
  }
}

/**
 * Clear the boost_blast cache
 */
function drush_media_unique() {
  if (variable_get('media_unique_', FALSE)) {
    drush_log(dt('Media Unique processing has not yet been implemented.  Nothing happened yet.'), 'success');
    drupal_set_message('media_unique', 'TODO, not yet implemented', 'notice', TRUE);
  } else {
    drush_log(dt('Media Unique processing was not executed because media_unique is currently disabled.  Edit your media_unique settings at admin/config/media/media_unique'), 'success');
  }
  return TRUE;
}


