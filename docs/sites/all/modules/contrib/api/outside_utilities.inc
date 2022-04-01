<?php

/**
 * @file
 * Utility functions for use by other projects.
 *
 * These functions are no longer used by the API project itself, but are
 * maintained for use by other projects that depend on the API module.
 */

/**
 * Counts items by type for a branch.
 *
 * @param object $branch
 *   Object representing the branch to count.
 *
 * @return array
 *   Associative array where the keys are the type of listing ('functions',
 *   'classes', etc.) and the values are the count of how many there are in
 *   that listing for the given branch.
 */
function api_listing_counts($branch) {
  static $cached_counts = array();

  // Check the cache.
  $key = $branch->branch_name . $branch->branch_id;
  if (isset($cached_counts[$key])) {
    return $cached_counts[$key];
  }

  $return = array(
    'groups' => 0,
    'classes' => 0,
    'functions' => 0,
    'constants' => 0,
    'globals' => 0,
    'files' => 0,
    'namespaces' => 0,
    'deprecated' => 0,
    'services' => 0,
    'elements' => 0,
  );

  // These queries mirror what is done in the views used by api_page_listing().
  $query = db_select('api_documentation', 'ad')
    ->condition('branch_id', $branch->branch_id)
    ->condition('object_type', 'group');
  $query->addExpression('COUNT(*)', 'num');
  $return['groups'] = $query
    ->execute()
    ->fetchField();

  $query = db_select('api_documentation', 'ad')
    ->condition('branch_id', $branch->branch_id)
    ->condition('namespace', '', '<>')
    ->groupBy('namespace');
  $query->addExpression('COUNT(*)', 'num');
  $return['namespaces'] = $query
    ->execute()
    ->fetchField();

  $query = db_select('api_documentation', 'ad')
    ->condition('branch_id', $branch->branch_id)
    ->condition('object_type', array('class', 'interface', 'trait'))
    ->condition('class_did', 0);
  $query->addExpression('COUNT(*)', 'num');
  $return['classes'] = $query
    ->execute()
    ->fetchField();

  $query = db_select('api_reference_storage', 'ars')
    ->condition('branch_id', $branch->branch_id)
    ->condition('object_type', array('element'));
  $query->addExpression('COUNT(*)', 'num');
  $return['elements'] = $query
    ->execute()
    ->fetchField();

  $query = db_select('api_documentation', 'ad')
    ->condition('branch_id', $branch->branch_id)
    ->condition('deprecated', '', '<>');
  $query->addExpression('COUNT(*)', 'num');
  $return['deprecated'] = $query
    ->execute()
    ->fetchField();

  foreach (array('function', 'constant', 'global', 'file', 'service') as $type) {
    $query = db_select('api_documentation', 'ad')
      ->condition('branch_id', $branch->branch_id)
      ->condition('object_type', $type)
      ->condition('class_did', 0);
    $query->addExpression('COUNT(*)', 'num');
    $return[$type . 's'] = $query
      ->execute()
      ->fetchField();
  }

  $cached_counts[$key] = $return;
  return $return;
}
