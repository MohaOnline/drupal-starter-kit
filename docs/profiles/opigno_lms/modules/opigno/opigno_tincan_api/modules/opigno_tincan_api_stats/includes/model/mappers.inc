<?php

/**
* Map finished statement scores
*
* @param array $finished_statements
*
* @return array
*/
function opigno_lrs_stats_map_finished_statement_scores($finished_statements) {
  return array_map('opigno_lrs_stats_map_finished_statement_score', $finished_statements);
}

/**
 * Map finished statement score
 *
 * @param array $finished_statements
 *
 * @return double
 */
function opigno_lrs_stats_map_finished_statement_score($finished_statements) {
  return $finished_statements->result->score->scaled;
}