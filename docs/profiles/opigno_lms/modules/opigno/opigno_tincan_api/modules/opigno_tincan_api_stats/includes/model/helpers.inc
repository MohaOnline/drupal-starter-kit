<?php

/**
 * @file
 * Implements helper functions
 */

//Time utility functions

/**
 * Get start of current week
 *
 * @return DateTime
 */
function opigno_lrs_stats_start_of_current_week() {
  $date = new DateTime();
  return $date->modify('midnight this week');
}

/**
 * Get end of current week
 *
 * @return DateTime
 */
function opigno_lrs_stats_end_of_current_week() {
  $date = new DateTime();
  return $date->modify('midnight this week +7 days');
}

/**
 * Get start of current month
 *
 * @return DateTime
 */
function opigno_lrs_stats_start_of_current_month() {
  $date = new DateTime();
  return $date->modify('midnight first day of this month');
}

/**
 * Get end of current month
 *
 * @return DateTime
 */
function opigno_lrs_stats_end_of_current_month() {
  $date = new DateTime();
  return $date->modify('midnight last day of this month +1 day');
}

/**
 * Get start of current year
 *
 * @return DateTime
 */
function opigno_lrs_stats_start_of_current_year() {
  $date = new DateTime();
  return $date->modify('midnight January 1st');
}

/**
 * Get end of current year
 *
 * @return DateTime
 */
function opigno_lrs_stats_end_of_current_year() {
  $date = new DateTime();
  return $date->modify('midnight December 31st +1 day');
}

//Other functions

/**
 * Retrieve first statement timestamp from LRS
 *
 * @return DateTime
 */
function opigno_lrs_stats_first_statement_timestamp() {
  $statements = opigno_lrs_stats_all_statements();
  $first_statement = (object)array('timestamp' => new DateTime()); //Fake statement of today
  foreach($statements as $statement)
    if($statement->timestamp < $first_statement->timestamp)
      $first_statement = $statement;

  return $first_statement->timestamp;
}

/**
 * Retrieve drupal node id associated with the specified object id (url)
 *
 * @param int $object_id
 *
 * @return StdClass
 */
function opigno_lrs_stats_get_nid_by_object_id($object_id) {
  $path = explode(base_path(), $object_id);
  $path = $path[1];
  $parts = explode('/', drupal_get_normal_path($path));
  return end($parts);
}