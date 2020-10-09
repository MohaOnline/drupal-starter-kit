<?php

/**
 * @file
 * Plugin definition for synonyms search behavior.
 */

$plugin = array(
  'title' => t('Search'),
  'description' => t('Integrate synonyms with Search module'),
  'interface' => 'SearchSynonymsBehavior',
  'enabled callback' => 'synonyms_search_behavior_search_enabled',
  'disabled callback' => 'synonyms_search_behavior_search_disabled',
);

/**
 * Callback for when the behavior is enabled.
 *
 * Trigger re-indexing of all the nodes that reference terms from the vocabulary
 * where the change has taken place.
 */
function synonyms_search_behavior_search_enabled($behavior_definition, $behavior_implementation) {
  if ($behavior_implementation['entity_type'] == 'taxonomy_term') {
    module_load_include('inc', 'synonyms_search', 'synonyms_search.pages');
    synonyms_search_reindex_nodes_by_vocabulary(taxonomy_vocabulary_machine_name_load($behavior_implementation['bundle']));
  }
}

/**
 * Callback for when the behavior is disabled.
 *
 * Trigger re-indexing of all the nodes that reference terms from the vocabulary
 * where the change has taken place.
 */
function synonyms_search_behavior_search_disabled($behavior_definition, $behavior_implementation) {
  if ($behavior_implementation['entity_type'] == 'taxonomy_term') {
    module_load_include('inc', 'synonyms_search', 'synonyms_search.pages');
    synonyms_search_reindex_nodes_by_vocabulary(taxonomy_vocabulary_machine_name_load($behavior_implementation['bundle']));
  }
}
