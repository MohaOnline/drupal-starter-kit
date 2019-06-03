<?php

/**
 * @file
 * Implements functions to progressively build a pipeline for mongoDB aggregate function
 */

/**
 * Create a valid empty pipeline
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_new_pipeline() {
  return array(
    array(
      '$match' => new stdClass()
    )
  );
}

/**
 * Set pipeline to retrieve only statements which match provided predicates
 *
 * @param array $pipeline
 *   A structured pipeline array.
 *
 * @param array $predicates
 *   A structured $predicates array.
 *
 * @return array
 *   A structured pipeline array.
 *
 */
function opigno_lrs_stats_query_match($pipeline, $predicates) {
  $pipeline[]['$match'] = $predicates;
  return $pipeline;
}

/**
 * Set pipeline to retrieve only a fixed number of statements
 *
 * @param array $pipeline
 *   A structured pipeline array.
 *
 * @param int $limit
 *
 * @return array
 *   A structured pipeline array.
 *
 */
function opigno_lrs_stats_query_limit($pipeline, $limit) {
  $pipeline[]['$limit'] = $limit;
  return $pipeline;
}

/**
 * Unwind array by path
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @param $path
 *
 * @return array
 */
function opigno_lrs_stats_query_unwind($pipeline, $path) {
  $pipeline[]['$unwind'] = $path;
  return $pipeline;
}


