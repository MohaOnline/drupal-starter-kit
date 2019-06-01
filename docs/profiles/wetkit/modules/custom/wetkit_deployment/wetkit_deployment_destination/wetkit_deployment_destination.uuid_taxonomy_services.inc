<?php
/**
 * @file
 * wetkit_deployment_destination.uuid_taxonomy_services.inc
 */


/**
 * Implements hook_services_resources().
 */
function wetkit_deployment_destination_services_resources() {
  return array(
    'taxonomy_term' => array(
      'actions' => array(
        'reorder_terms' => array(
          'help' => t('Reorders a set of taxonomy terms.'),
          'callback' => '_wetkit_deployment_destination_reorder_terms_callback',
          'args' => array(
            array(
              'name' => 'terms',
              'optional' => FALSE,
              'source' => 'data',
              'type' => 'array',
              'description' => t("An array of UUID-based term objects to reorder. The simplest way to generate this is by calling uuid_taxonomy_services_get_tree()."),
            ),
          ),
          'access arguments' => array('administer taxonomy'),
        ),
      ),
    ),
  );
}

/**
 * Services callback to reorder a set of taxonomy terms.
 *
 * @return array
 *   An array of term UUIDs which were actually updated as a result of the
 *   reordering.
 *
 * @see uuid_taxonomy_services_reorder_terms()
 */
function _wetkit_deployment_destination_reorder_terms_callback($terms) {
  // Convert each term to an object (since it may be an array when it comes in
  // via the Services POST data) and call the actual function to reorder the
  // terms.
  foreach ($terms as &$term) {
    $term = (object) $term;
  }
  return _wetkit_deployment_destination_reorder_terms($terms);
}

/**
 * Reorders a set of taxonomy terms.
 *
 * @param array $terms
 *   An array of term objects as returned by uuid_taxonomy_services_get_tree().
 *   Each object should have the following properties:
 *   - uuid: The UUID of the term.
 *   - weight: The new desired weight of the term.
 *   - parent_uuids: (optional) An array of UUIDs for the desired parents of
 *     this term. Similar to the behavior of taxonomy_term_save(), an array
 *     with a single element whose value is '0' will result in the term being
 *     saved with no parents. If not provided, the term's parents will not be
 *     updated.
 *
 * @return array
 *   An array of term UUIDs which were actually updated as a result of the
 *   reordering.
 *
 * @see uuid_taxonomy_services_get_tree()
 */
function _wetkit_deployment_destination_reorder_terms($terms) {
  $affected_uuids = array();

  // Validate the array of terms, and compile a list of term IDs corresponding
  // to the UUIDs referred to within it.
  $uuids = array();
  foreach ($terms as $term) {
    if (!isset($term->uuid) || !isset($term->weight)) {
      return services_error(t('Some of the provided terms were missing required properties.'), 406);
    }
    $uuids[$term->uuid] = $term->uuid;
    if (!empty($term->parent_uuids)) {
      foreach ($term->parent_uuids as $uuid) {
        $uuids[$uuid] = $uuid;
      }
    }
  }
  $ids = entity_get_id_by_uuid('taxonomy_term', $uuids);

  // Update the weight and parents for each term.
  foreach ($terms as $term) {
    // If we don't have a local term with this UUID, skip it.
    if (!isset($ids[$term->uuid])) {
      continue;
    }

    $tid = $ids[$term->uuid];

    // If parents were provided, modify those in the database.
    $parents_changed = FALSE;
    if (isset($term->parent_uuids)) {
      // Get the original set of parents so we can see if anything changed.
      $original_parents = db_query('SELECT parent FROM {taxonomy_term_hierarchy} WHERE tid = :tid', array(':tid' => $tid))->fetchCol();
      $new_parents = array();

      // Delete existing parents.
      db_delete('taxonomy_term_hierarchy')
        ->condition('tid', $tid)
        ->execute();

      // Insert the new parents.
      $has_parents = FALSE;
      $query = db_insert('taxonomy_term_hierarchy')
        ->fields(array('tid', 'parent'));
      foreach ($term->parent_uuids as $uuid) {
        if (isset($ids[$uuid])) {
          $parent_tid = $ids[$uuid];
          $query->values(array(
            'tid' => $tid,
            'parent' => $parent_tid,
          ));
          $has_parents = TRUE;
          $new_parents[$parent_tid] = $parent_tid;
        }
      }
      // If there were no valid parents, save '0' so that the term is correctly
      // recorded as not having any parents.
      if (!$has_parents) {
        $query->values(array(
          'tid' => $tid,
          'parent' => 0,
        ));
        $new_parents[0] = 0;
      }
      $query->execute();

      // Determine if the set of parents changed.
      if (array_diff($original_parents, $new_parents) || array_diff($new_parents, $original_parents)) {
        $parents_changed = TRUE;
      }
    }

    // Modify the weights in the database.
    $weights_changed = (bool) db_update('taxonomy_term_data')
      ->fields(array('weight' => $term->weight))
      ->condition('tid', $tid)
      ->execute();

    // If this term was actually updated, record that in the array that will be
    // returned.
    if ($parents_changed || $weights_changed) {
      $affected_uuids[$term->uuid] = $term->uuid;
    }
  }

  return $affected_uuids;
}
