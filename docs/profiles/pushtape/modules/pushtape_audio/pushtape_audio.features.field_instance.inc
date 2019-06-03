<?php
/**
 * @file
 * pushtape_audio.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function pushtape_audio_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-track-field_audio'
  $field_instances['node-track-field_audio'] = array(
    'bundle' => 'track',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'pushtape_player',
        'settings' => array(
          'text' => 'title',
        ),
        'type' => 'pushtape_player',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => -5,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_audio',
    'label' => 'Audio',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => 'audio',
      'file_extensions' => 'mp3',
      'max_filesize' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'file',
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 0,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Audio');

  return $field_instances;
}
