<?php

/**
 * @file
 * Supportive functions for the module.
 */

/**
 * Mark nodes that reference specific terms for search re-indexing.
 *
 * This is particularly useful, when the terms in question have been changed.
 *
 * @param $tids array
 *   Array of tids of the terms
 */
function synonyms_search_reindex_nodes_by_terms($tids) {
  // In order to speed up the process, we will query DB for nid's that reference
  // handled to us tids, and at the end we'll trigger their re-indexing just in
  // a single SQL query. Probably it is better to use search_touch_node(), but
  // that would imply a big amount of SQL queries on some websites.
  $found_nids = array();
  foreach (field_info_field_map() as $field_name => $v) {
    // TODO: explore possibility of using foreign keys instead of hard coding
    // fields types in here.
    if ($v['type'] == 'taxonomy_term_reference' && isset($v['bundles']['node'])) {
      $query = new EntityFieldQuery();
      $result = $query->entityCondition('entity_type', 'node')
        ->fieldCondition($field_name, 'tid', $tids, 'IN')
        ->execute();
      if (isset($result['node'])) {
        $found_nids = array_merge($found_nids, array_keys($result['node']));
      }
    }
    if ($v['type'] == 'entityreference' && isset($v['bundles']['node'])) {
      $field = field_info_field($field_name);
      if ($field['settings']['target_type'] == 'taxonomy_term') {
        $query = new EntityFieldQuery();
        $result = $query->entityCondition('entity_type', 'node')
          ->fieldCondition($field_name, 'target_id', $tids, 'IN')
          ->execute();
        if (isset($result['node'])) {
          $found_nids = array_merge($found_nids, array_keys($result['node']));
        }
      }
    }
  }
  if (!empty($found_nids)) {
    db_update('search_dataset')
      ->fields(array('reindex' => REQUEST_TIME))
      ->condition('type', 'node')
      ->condition('sid', $found_nids, 'IN')
      ->execute();
  }

  // Integration with Term Search module: reset terms index too.
  if (module_exists('term_search')) {
    db_update('search_dataset')
      ->fields(array('reindex' => REQUEST_TIME))
      ->condition('type', 'term')
      ->condition('sid', $tids)
      ->execute();
  }
}

/**
 * Mark nodes that reference terms from a vocabulary for search re-indexing.
 *
 * @param $vocabulary object
 *   Fully loaded vocabulary object
 */
function synonyms_search_reindex_nodes_by_vocabulary($vocabulary) {
  $tids = db_select('taxonomy_term_data', 't')
    ->fields('t', array('tid'))
    ->condition('vid', $vocabulary->vid)
    ->execute()
    ->fetchCol();
  if (!empty($tids)) {
    synonyms_search_reindex_nodes_by_terms($tids);
  }
}
