<?php

/**
 * @file
 * Views integration of Synonyms module.
 */

/**
 * Implements hook_views_data().
 */
function synonyms_views_data() {
  $data = array();

  foreach (entity_get_info() as $entity_type => $entity_info) {
    $wrapper = entity_metadata_wrapper($entity_type);
    $property_info = $wrapper->getPropertyInfo();
    if (isset($property_info['synonyms']) && isset($entity_info['base table']) && $entity_info['base table']) {
      $data[$entity_info['base table']]['synonyms'] = array(
        'title' => t('All synonyms'),
        'help' => t('All synonyms of @entity_type', array(
          '@entity_type' => $entity_info['label'],
        )),
        'field' => array(
          'handler' => 'synonyms_views_handler_field_synonyms',
          'real field' => $entity_info['entity keys']['id'],
          'click sortable' => FALSE,
          'synonyms entity type' => $entity_type,
        ),
      );
    }
  }

  return $data;
}

/**
 * Implements hook_views_plugins_alter().
 */
function synonyms_views_plugins_alter(&$plugins) {
  if (module_exists('taxonomy')) {
    // Replace default taxonomy term argument validator with our extended
    // version, which can also handle a term synonym as an argument.
    $plugins['argument validator']['taxonomy_term']['handler'] = 'synonyms_views_plugin_argument_validate_taxonomy_term';
  }
}

/**
 * Implements hook_field_views_data_alter().
 */
function synonyms_field_views_data_alter(&$result, $field, $module) {
  switch ($field['type']) {
    case 'taxonomy_term_reference':
      // Add synonyms friendly filters.
      foreach ($field['storage']['details']['sql'] as $table) {
        $tid_column = reset($table);
        $tid_column = $tid_column['tid'];
        $table = array_keys($table);
        $table = $table[0];
        if (isset($result[$table][$tid_column]['filter'])) {
          $result[$table][$tid_column]['filter']['handler'] = 'synonyms_views_handler_filter_term_tid';
        }
      }
      break;

    case 'entityreference':
      foreach ($field['storage']['details']['sql'] as $table) {
        $target_id_column = reset($table);
        $target_id_column = $target_id_column['target_id'];
        $table = array_keys($table);
        $table = $table[0];
        if (isset($result[$table][$target_id_column]['filter'])) {
          $result[$table][$target_id_column]['filter']['handler'] = 'synonyms_views_handler_filter_entityreference_synonyms';
        }
      }
      break;
  }
}
