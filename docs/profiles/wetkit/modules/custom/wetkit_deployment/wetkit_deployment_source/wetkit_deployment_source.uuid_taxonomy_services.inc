<?php
/**
 * @file
 * wetkit_deployment_source.uuid_taxonomy_services.inc
 */


/**
 * Implements hook_form_FORM_ID_alter().
 */
function wetkit_deployment_form_taxonomy_overview_terms_alter(&$form, &$form_state) {
  $form['#submit'][] = 'wetkit_deployment_taxonomy_overview_terms_submit';
}

/**
 * Custom submit handler for the form that reorders taxonomy terms.
 */
function wetkit_deployment_taxonomy_overview_terms_submit($form, &$form_state) {
  // The first section just deals with annoying issues specific to the taxonomy
  // overview form. It's necessary for the code to work, but not very
  // illustrative
  //
  // When the "Reset to alphabetical" button is clicked, we might be displaying
  // a confirmation form rather than actually doing the reordering. Catch that
  // case and bail out when it happens.
  if ($form_state['triggering_element']['#value'] == t('Reset to alphabetical') && $form_state['values']['reset_alphabetical'] !== TRUE) {
    return;
  }
  // Get the vocabulary ID. This can be stored in different places depending on
  // which stage of the form submission we're at.
  if (isset($form['#vocabulary'])) {
    $vid = $form['#vocabulary']->vid;
  }
  elseif (isset($form_state['values']['vid'])) {
    $vid = $form_state['values']['vid'];
  }
  // Bail out if we don't recognize this form.
  else {
    return;
  }

  // The interesting code is below. You'd probably run it only after checking
  // that the vocabulary identified by $vid is actually one you want to
  // propagate to the target site.
  // Force a static cache reset before propagating the changes, since the
  // submit handlers which ran before this one may have updated things in the
  // database without clearing static caches.
  taxonomy_terms_static_reset();
  // Now propagate the changes. Here, the Deployment endpoint you would load is
  // the one associated with the desired target site.
  try {
    $terms = _wetkit_deployment_source_uuid_get_tree($vid);
    $endpoint = deploy_endpoint_load('destination_site_endpoint');
    $client = new DeployServicesClient($endpoint);
    $client->login();
    $client->request('taxonomy_term/reorder_terms', 'POST', drupal_json_encode($terms));
    $client->logout();
  }
  catch (Exception $e) {
    watchdog_exception('wetkit_deployment', $e);
  }
}

/**
 * Creates a UUID-based tree of taxonomy terms.
 *
 * This is useful for code which wants to prepare an array of terms before
 * sending it to the Services resource defined by this module on another site.
 *
 * @return array
 *   An array of partial term objects with 'uuid', 'weight', and 'parent_uuids'
 *   properties, suitable to send to uuid_taxonomy_services_reorder_terms() via
 *   this module's Services resource.
 *
 * @see uuid_taxonomy_services_services_resources()
 * @see taxonomy_get_tree()
 */
function _wetkit_deployment_source_uuid_get_tree($vid, $parent = 0, $max_depth = NULL) {
  // Get the original tree, and compile a list of parent UUIDs corresponding to
  // the term IDs referred to within it.
  $tree = taxonomy_get_tree($vid, $parent, $max_depth);
  $parent_ids = array();
  foreach ($tree as $term) {
    if (!empty($term->parents)) {
      foreach ($term->parents as $parent_id) {
        $parent_ids[$parent_id] = $parent_id;
      }
    }
  }
  $parent_uuids = entity_get_uuid_by_id('taxonomy_term', $parent_ids);

  // Build the tree of UUID-based partial term objects.
  $uuid_tree = array();
  foreach ($tree as $term) {
    $uuid_term = new stdClass();
    $uuid_term->uuid = $term->uuid;
    $uuid_term->weight = $term->weight;
    // Transform all parent IDs into UUIDs. In case there aren't any, use a
    // single parent with a value of '0' to indicate the term should not have
    // any parents on the target site. This is similar to the behavior of
    // taxonomy_term_save().
    $uuid_term->parent_uuids = array_values(array_intersect_key($parent_uuids, array_flip($term->parents)));
    if (empty($uuid_term->parent_uuids)) {
      $uuid_term->parent_uuids = array(0);
    }
    $uuid_tree[] = $uuid_term;
  }

  return $uuid_tree;
}
