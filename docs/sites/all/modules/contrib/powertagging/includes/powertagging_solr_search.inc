<?php

/**
 * @file
 * Extends the PowerTagger module such that works with Search API, Solr Search
 * and Facet API.
 *
 * @todo:
 * - Preflabels can change, indexed documents need to be reindexed when a tag
 *   changes.
 */

// Extend the Facet API filter if it exists.
if (module_exists('facetapi') && class_exists('FacetapiFilter')) {

  // Returns the name (label) of the facet item instead of the URI (stored in
  // the Solr index).
  class FacetapiFilterUsePreflabelInsteadOfUri extends FacetapiFilter {

    /**
     * Filters facet items.
     */
    public function execute(array $build) {
      global $language;

      $uris = array();
      foreach ($build as $facet) {
        $uris[] = $facet['#indexed_value'];
      }

      if (!empty($uris)) {
        $query = db_select('field_data_field_uri', 'u');
        $query->join('taxonomy_term_data', 't', 't.tid = u.entity_id');
        $query->fields('u', array('field_uri_value'))
          ->fields('t', array('name'))
          ->condition('u.entity_type', 'taxonomy_term')
          ->condition('u.field_uri_value', $uris);
        if (module_exists('i18n_taxonomy')) {
          $query->condition('t.language', array(LANGUAGE_NONE, $language->language));
        }
        $preflabels_by_uri = $query->execute()
          ->fetchAllKeyed();

        if (!empty($preflabels_by_uri)) {
          foreach($build as &$facet) {
            if (isset($preflabels_by_uri[$facet['#indexed_value']])) {
              $facet['#markup'] = $preflabels_by_uri[$facet['#indexed_value']];
            }
          }
        }
      }
      return $build;
    }
  }

  // Returns the name (label) of the facet item instead of the term ID
  // (stored in the Solr index).
  class FacetapiFilterUsePreflabelInsteadOfTid extends FacetapiFilter {

    /**
     * Filters facet items.
     */
    public function execute(array $build) {
      $tids = array();
      foreach ($build as $facet) {
        $tids[] = $facet['#indexed_value'];
      }

      if (!empty($tids)) {
        $preflables_by_tid = db_select('taxonomy_term_data', 't')
          ->fields('t', array('tid', 'name'))
          ->condition('t.tid', $tids, 'in')
          ->execute()
          ->fetchAllKeyed();

        if (!empty($preflables_by_tid)) {
          foreach($build as &$facet) {
            if (isset($preflables_by_tid[$facet['#indexed_value']])) {
              $facet['#markup'] = $preflables_by_tid[$facet['#indexed_value']];
            }
          }
        }
      }
      return $build;
    }
  }
}

/**
 * Implements hook_facetapi_filters().
 */
function powertagging_facetapi_filters() {
  return array(
    'use_preflabel_instead_of_uri' => array(
      'handler' => array(
        'label' => t('Use prefLabels instead of URIs in PowerTagging field facets'),
        'class' => 'FacetapiFilterUsePreflabelInsteadOfUri',
      ),
    ),
    'use_preflabel_instead_of_tid' => array(
      'handler' => array(
        'label' => t('Use prefLabels instead of term IDs in PowerTagging field facets'),
        'class' => 'FacetapiFilterUsePreflabelInsteadOfTid',
      ),
    ),
  );
}

/**
 * Returns all entity ids that are linked with the $term.
 *
 * @param object $term
 *   The term object.
 *
 * @return array
 *   A list of entity ids grouped by entity type.
 */
function powertagging_get_entity_ids($term) {
  // Check if Search API or Apache Solr is installed.
  $entity_ids = array();
  if ((module_exists('search_api') && function_exists('search_api_track_item_change'))
    || (module_exists('apachesolr') && function_exists('apachesolr_mark_entity'))) {
    // Get the PowerTagging field names.
    $field_names = powertagging_get_field_name();
    foreach ($field_names as $field_name) {
      $table = 'field_data_' . $field_name;
      $column = $field_name . '_tid';

      // Get all entities linked with the given term.
      $result = db_select($table, 't')
        ->fields('t', array('entity_type', 'entity_id'))
        ->condition($column, $term->tid)
        ->execute()
        ->fetchAll();

      if (!empty($result)) {
        foreach ($result as $row) {
          $entity_ids[$row->entity_type][] = $row->entity_id;
        }
      }
    }
  }

  return $entity_ids;
}

/**
 * Updates the solr index for the given entities.
 *
 * @param array $entity_ids
 *   A list of entity ids grouped by entity type.
 */
function powertagging_update_solr_index($entity_ids) {
  // Check if Search API is installed.
  if (module_exists('search_api') && function_exists('search_api_track_item_change')) {
    // If entities found mark it for re-indexing.
    if (!empty($entity_ids)) {
      foreach ($entity_ids as $entity_type => $ids) {
        // Re-index the entity.
        search_api_track_item_change($entity_type, $ids);
      }
    }
  }
  // Check if Apache Solr is installed.
  if (module_exists('apachesolr') && function_exists('apachesolr_mark_entity')) {
    // If entities found mark it for re-indexing.
    if (!empty($entity_ids)) {
      foreach ($entity_ids as $entity_type => $ids) {
        // Re-index the entity.
        foreach ($ids as $id) {
          apachesolr_mark_entity($entity_type, $id);
        }
      }
    }
  }
}

/**
 * Hooks needed by the module Search API with Solr Search.
 * -------------------------------------------------------
 */

/**
 * Implements hook_entity_property_info_alter().
 */
function powertagging_entity_property_info_alter(&$info) {
  $info['node']['properties']['powertagging_tags_id'] = array(
    'type' => 'list<integer>',
    'label' => t('PowerTagging Tags IDs'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_id_search_api_property_getter_callback',
  );
  $info['node']['properties']['powertagging_tags_uri'] = array(
    'type' => 'list<uri>',
    'label' => t('PowerTagging Tags URIs'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_uri_search_api_property_getter_callback',
  );
  $info['node']['properties']['powertagging_tags_preflabel'] = array(
    'type' => 'list<text>',
    'label' => t('PowerTagging Tags prefLabels'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_preflabel_search_api_property_getter_callback',
  );
  $info['node']['properties']['powertagging_tags_altlabel'] = array(
    'type' => 'list<text>',
    'label' => t('PowerTagging Tags altLabels'),
    'sanitized' => TRUE,
    'getter callback' => 'powertagging_tags_altlabel_search_api_property_getter_callback',
  );
}


/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of Ids
 */
function powertagging_tags_id_search_api_property_getter_callback($item) {
  $values = array();

  if (($field_name = powertagging_get_field_name($item->type)) == '') {
    return $values;
  }

  $field = $item->{$field_name};
  if (!empty($field) && isset($field[LANGUAGE_NONE])) {
    $tids = array();
    foreach ($field[LANGUAGE_NONE] as $tid) {
      $tids[] = $tid['tid'];
    }

    // Only deliver the tids of concepts.
    $concepts_query = new EntityFieldQuery();
    $concepts_query->entityCondition('entity_type', 'taxonomy_term')
      ->entityCondition('entity_id', $tids, 'IN')
      ->fieldCondition('field_uri', 'value', '', '!=');
    $concepts = $concepts_query->execute();
    if (isset($concepts['taxonomy_term'])) {
      $values = array_keys($concepts['taxonomy_term']);
    }
  }

  return $values;
}

/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of URIs
 */
function powertagging_tags_uri_search_api_property_getter_callback($item) {
  $tids = powertagging_tags_id_search_api_property_getter_callback($item);
  $values = array();

  if (!empty($tids)) {
    $tag_terms = taxonomy_term_load_multiple($tids);
    foreach ($tag_terms as $tag_term) {
      if (!empty($tag_term->field_uri) && isset($tag_term->field_uri[LANGUAGE_NONE])) {
        $values[] = $tag_term->field_uri[LANGUAGE_NONE][0]['value'];
      }
    }
  }

  return $values;
}

/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of term names (prefLabels)
 */
function powertagging_tags_preflabel_search_api_property_getter_callback($item) {
  $tids = powertagging_tags_id_search_api_property_getter_callback($item);
  $values = array();

  if (!empty($tids)) {
    $tag_terms = taxonomy_term_load_multiple($tids);
    foreach ($tag_terms as $tag_term) {
      $values[] = $tag_term->name;
    }
  }

  return $values;
}

/**
 * Callback function for the entity property.
 *
 * @param array $item
 *  A facet item
 *
 * @return array
 *  A list of altLabels
 */
function powertagging_tags_altlabel_search_api_property_getter_callback($item) {
  $tids = powertagging_tags_id_search_api_property_getter_callback($item);
  $values = array();

  if (!empty($tids)) {
    $tag_terms = taxonomy_term_load_multiple($tids);
    foreach ($tag_terms as $tag_term) {
      if (!empty($tag_term->field_alt_labels) && isset($tag_term->field_alt_labels[LANGUAGE_NONE])) {
        foreach ($tag_term->field_alt_labels[LANGUAGE_NONE] as $alt_label) {
          $values[] = $alt_label['value'];
        }
      }
    }
  }

  return $values;
}

/**
 * Hooks needed by the module Apache Solr Search.
 * ----------------------------------------------
 */

/**
 * Implements hook_apachesolr_field_mappings().
 */
function powertagging_apachesolr_field_mappings() {
  $mappings = array();
  $fields = field_info_fields();
  foreach ($fields as $field) {
    if ($field['type'] == 'powertagging') {
      $mappings['per-field'][$field['field_name']] = array(
        'indexing_callback' => array('powertagging_fields_indexing_callback'),
        'index_type' => 'storage',
        'facets' => TRUE,
        'multiple' => TRUE,
        'query types' => array('term'),
        'facet mincount allowed' => TRUE,
      );
    }
  }

  return $mappings;
}

/**
 * Takes the values from the field and tells the module where to put them.
 *
 * @param object $entity
 *   An object containing the entity that will get indexed.
 * @param string $field_name
 *   A string containing the name of the poolparty field.
 * @param string $index_key
 *   A string containing the solr index field name.
 * @param array $field_info
 *   Array.
 *
 * @return array
 *   An array containing all the fields that needs to be indexed.
 */
function powertagging_fields_indexing_callback($entity, $field_name, $index_key, array $field_info) {
  $fields = array();
  if (!empty($entity->{$field_name})) {
    // Get all the tagged term ids and load its data.
    $terms = $entity->{$field_name}[LANGUAGE_NONE];
    $tids = array();
    foreach ($terms as $term) {
      $tids[] = $term['tid'];
    }
    $terms = taxonomy_term_load_multiple($tids);
    foreach ($terms as $term) {
      // Set the concept prefLabel.
      $fields[] = array(
        'key' => $index_key,
        'value' => $term->name,
      );

      // Set the concept prefLabel.
      $field = array(
        'index_type' => 'text',
        'name' => 'powertagging_tags_preflabel',
        'multiple' => TRUE
      );
      $fields[] = array(
        'key' => apachesolr_index_key($field),
        'value' => $term->name,
      );

      // Set the term ID.
      $field = array(
        'index_type' => 'integer',
        'name' => 'powertagging_tags_id',
        'multiple' => TRUE
      );
      $fields[] = array(
        'key' => apachesolr_index_key($field),
        'value' => $term->tid,
      );

      // Set the concept URI.
      if (!empty($term->field_uri)) {
        $field = array(
          'index_type' => 'string',
          'name' => 'powertagging_tags_uri',
          'multiple' => TRUE
        );
        $fields[] = array(
          'key' => apachesolr_index_key($field),
          'value' => $term->field_uri[LANGUAGE_NONE][0]['value'],
        );
      }

      // Set the concept altLabels.
      if (!empty($term->field_alt_labels) && isset($term->field_alt_labels[LANGUAGE_NONE])) {
        $field = array(
          'index_type' => 'text',
          'name' => 'powertagging_tags_altlabel',
          'multiple' => TRUE
        );
        foreach ($term->field_alt_labels[LANGUAGE_NONE] as $alt_label) {
          $fields[] = array(
            'key' => apachesolr_index_key($field),
            'value' => trim($alt_label['value']),
          );
        }
      }
    }
  }

  return $fields;
}

/**
 * Implements hook_facetapi_facet_info().
 * Provides the definitions for the extra facets.
 *
 * @param array $searcher_info
 *   The definition of the searcher that facets are being collected for.
 *
 * @return array
 *   An associative array keyed by unique name of the facet.
 */
function powertagging_facetapi_facet_info(array $searcher_info) {
  $facets['powertagging_tags_id'] = array(
    'name' => 'powertagging_tags_id',
    'label' => t('PowerTagging Tags IDs'),
    'description' => t('Filter by PowerTagging Tags IDs.'),
    'field' => 'im_powertagging_tags_id',
    'field alias' => 'powertagging_tags_id',
    'field api bundles' => array('node', 'user', 'taxonomy_term'),
    'facet mincount allowed' => TRUE,
    'dependency plugins' => array('bundle', 'role'),
  );
  $facets['powertagging_tags_preflabel'] = array(
    'name' => 'powertagging_tags_preflabel',
    'label' => t('PowerTagging Tags prefLabels'),
    'description' => t('Filter by PowerTagging Tags prefLabels.'),
    'field' => 'tm_powertagging_tags_preflabel',
    'field alias' => 'powertagging_tags_preflabel',
    'field api bundles' => array('node', 'user', 'taxonomy_term'),
    'facet mincount allowed' => TRUE,
    'dependency plugins' => array('bundle', 'role'),
  );
  $facets['powertagging_tags_uri'] = array(
    'name' => 'powertagging_tags_uri',
    'label' => t('PowerTagging Tags URIs'),
    'description' => t('Filter by PowerTagging Tags URIs.'),
    'field' => 'sm_powertagging_tags_uri',
    'field alias' => 'powertagging_tags_uri',
    'field api bundles' => array('node', 'user', 'taxonomy_term'),
    'facet mincount allowed' => TRUE,
    'dependency plugins' => array('bundle', 'role'),
  );
  $facets['powertagging_tags_altlabel'] = array(
    'name' => 'powertagging_tags_altlabel',
    'label' => t('PowerTagging Tags altLabels'),
    'description' => t('Filter by PowerTagging Tags altLabels.'),
    'field' => 'tm_powertagging_tags_altlabel',
    'field alias' => 'powertagging_tags_altlabel',
    'field api bundles' => array('node', 'user', 'taxonomy_term'),
    'facet mincount allowed' => TRUE,
    'dependency plugins' => array('bundle', 'role'),
  );

  return $facets;
}
