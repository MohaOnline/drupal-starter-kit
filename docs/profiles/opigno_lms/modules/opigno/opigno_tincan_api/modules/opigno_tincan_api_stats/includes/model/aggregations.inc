<?php

/**
 * @file
 * Implements functions to aggregate statements
 */

/**
 * Aggregate statements by day
 *
 * Output example:
 *  array(
 *     '2015-01-01' => statements,
 *     '2015-01-05' => statements,
 *     '2015-01-07' => statements
 *  )
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_statements_by_day($statements) {
  $statements_per_day = array();
  foreach($statements as $statement)
    $statements_per_day[$statement->timestamp->format('Y-m-d')][] = $statement;

  return $statements_per_day;
}

/**
 * Aggregate statements by month
 *
 * Output example:
 *  array(
 *     '2015-01' => statements,
 *     '2015-02' => statements,
 *     '2015-03' => statements
 *  )
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_statements_by_month($statements) {
  $statements_per_month = array();
  foreach($statements as $statement)
    $statements_per_month[$statement->timestamp->format('Y-m')][] = $statement;

  return $statements_per_month;
}

/**
 * Aggregate statements by year
 *
 * Output example:
 *  array(
 *     '2009' => statements,
 *     '2010' => statements,
 *     '2011' => statements
 *  )
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_statements_by_year($statements) {
  $statements_per_year = array();
  foreach($statements as $statement)
    $statements_per_year[$statement->timestamp->format('Y')][] = $statement;

  return $statements_per_year;
}

//Aggregate functions

/**
 * Aggregate statements by page
 *
 * Output example:
 *  array(
 *     'http://opigno.org/node/50' => statements,
 *     'http://opigno.org/node/51' => statements,
 *     'http://opigno.org/node/55' => statements
 *  )
 *
 * @param $statements
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_viewed_statements_by_page($statements) {
  $statements_per_page = array();
  foreach($statements as $statement) $statements_per_page[$statement->object->id][] = $statement;
  return $statements_per_page;
}

/**
 * Aggregate statements by user
 *
 * Output example:
 *  array(
 *     'toto1' => statements,
 *     'toto2' => statements,
 *     'toto3' => statements
 *  )
 *
 * @param $statements
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_statements_by_user($statements) {
  $statements_per_user = array();
  foreach($statements as $statement) $statements_per_user[$statement->actor->name][] = $statement;
  return $statements_per_user;
}

/**
 * Aggregate statements by object
 *
 * Output example:
 *  array(
 *     'http://opigno.org/node/100' => statements,
 *     'http://opigno.org/node/101' => statements,
 *     'http://opigno.org/node/102' => statements
 *  )
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_statements_by_object($statements) {
  $statements_per_object = array();
  foreach($statements as $statement) $statements_per_object[$statement->object->id][] = $statement;
  return $statements_per_object;
}

/**
 * Aggregate statements by context
 *
 * Note:
 *  - First you need to unwind context using "opigno_lrs_stats_query_scope_unwind_context" on the pipeline
 *
 * Output example:
 *  array(
 *     'http://opigno.org/node/104' => statements,
 *     'http://opigno.org/node/106' => statements,
 *     'http://opigno.org/node/109' => statements
 *  )
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_statements_by_context($statements) {
  $statements_per_context = array();
  foreach($statements as $statement) $statements_per_context[$statement->context->contextActivities->grouping->id][] = $statement;
  return $statements_per_context;
}

/**
 * Aggregate statements by verb
 *
 * Output example:
 *  array(
 *     'http://opigno.org/node/104' => statements,
 *     'http://opigno.org/node/106' => statements,
 *     'http://opigno.org/node/109' => statements
 *  )
 *
 * @return array
 */
function opigno_lrs_stats_aggregate_statements_by_verb($statements) {
  $statements_per_verb = array();
  foreach($statements as $statement) $statements_per_verb[$statement->verb->id][] = $statement;
  return $statements_per_verb;
}

