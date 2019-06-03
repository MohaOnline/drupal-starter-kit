<?php

/**
 * @file
 * Implements functions to filter statements
 */

//Time related filter functions

/**
 * Filter statements that happened between two dates
 *
 * @param array $statements
 *
 * @return array
 */
function opigno_lrs_stats_statements_date_between($statements, $first_date, $last_date) {
  $filtered_statements = array();
  foreach($statements as $statement)
    if($statement->timestamp >= $first_date && $statement->timestamp <= $last_date)
      $filtered_statements[] = $statement;

  return $filtered_statements;
}

/**
 * Filter statements that happened this week
 *
 * @param array $statements
 *
 * @return array
 */
function opigno_lrs_stats_filter_current_week_statements($statements) {
  return opigno_lrs_stats_statements_date_between(
    $statements,
    opigno_lrs_stats_start_of_current_week(),
    opigno_lrs_stats_end_of_current_week()
  );
}

/**
 * Filter statements that happened this month
 *
 * @param array $statements
 *
 * @return array
 */
function opigno_lrs_stats_filter_current_month_statements($statements) {
  return opigno_lrs_stats_statements_date_between(
    $statements,
    opigno_lrs_stats_start_of_current_month(),
    opigno_lrs_stats_end_of_current_month()
  );
}

/**
 * Filter statements that happened this year
 *
 * @param array $statements
 *
 * @return array
 */
function opigno_lrs_stats_filter_current_year_statements($statements) {
  return opigno_lrs_stats_statements_date_between(
    $statements,
    opigno_lrs_stats_start_of_current_year(),
    opigno_lrs_stats_end_of_current_year()
  );
}

/**
 * Filter finished statements that have a score (completion)
 *
 * @param array $finished_statements
 *
 * @return array
 */
function opigno_lrs_stats_filter_finished_statements_with_score($finished_statements) {
  return array_filter($finished_statements, 'opigno_lrs_stats_filter_finished_statement_with_score');
}

/**
 * Filter finished statement that has a score (completion)
 *
 * @param array $finished_statements
 *
 * @return TRUE|FALSE
 */
function opigno_lrs_stats_filter_finished_statement_with_score($statement) {
  return (isset($statement->result))? $statement->result->completion : false;
}