<?php

/**
 * @file
 * Dashboard model functions
 */

/**
 * Aggregate total number of page per day between two dates for the given statements
 *
 * Output example:
 *  array(
 *    array('day' => '2015-01-01', 'value' => 4),
 *    array('day' => '2015-01-02', 'value' => 123),
 *    array('day' => '2015-01-03', 'value' => 0),
 *    array('day' => '2015-01-04', 'value' => 455),
 *    array('day' => '2015-01-05', 'value' => 0)
 *  )
 *
 * @param DateTime $start_date
 * @param DateTime $end_date
 * @param Statements
 *
 * @return array
*/
function opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_day_between($start_date, $end_date, $statements) {
  $formatted_data = array();
  $statements_per_day = opigno_lrs_stats_dashboard_empty_values_per_unit_between($start_date, $end_date, 'day', 'Y-m-d');

  //Aggregate viewed statements per day (override)
  $statements_per_day = array_replace($statements_per_day, opigno_lrs_stats_aggregate_statements_by_day($statements));

  foreach($statements_per_day as $day => $day_statements)
    $formatted_data[] = array('day' => $day, 'value' => count($day_statements));

  return $formatted_data;
}

/**
 * Aggregate total number of page per month between two dates for the given statements
 *
 * Output example:
 *  array(
 *    array('month' => '2011-01', 'value' => 0),
 *    array('month' => '2011-02', 'value' => 45),
 *    array('month' => '2011-03', 'value' => 56),
 *    array('month' => '2011-04', 'value' => 67),
 *    array('month' => '2011-05', 'value' => 66)
 *  )
 *
 * @param DateTime $start_date
 * @param DateTime $end_date
 * @param Statements
 *
 * @return array
 *
 */
function opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_month_between($start_date, $end_date, $statements) {
  $formatted_data = array();
  $statements_per_month = opigno_lrs_stats_dashboard_empty_values_per_unit_between($start_date, $end_date, 'month', 'Y-m');

  //Aggregate viewed statements per day (override)
  $statements_per_month = array_replace($statements_per_month, opigno_lrs_stats_aggregate_statements_by_month($statements));

  foreach($statements_per_month as $month => $month_statements)
    $formatted_data[] = array('month' => $month, 'value' => count($month_statements));

  return $formatted_data;
}

/**
 * Aggregate total number of page per year between two dates for the given statements
 *
 * Output example:
 *  array(
 *    array('year' => '2011', 'value' => 0),
 *    array('year' => '2012', 'value' => 45),
 *    array('year' => '2013', 'value' => 56),
 *    array('year' => '2014', 'value' => 67),
 *    array('year' => '2015', 'value' => 66)
 *  )
 *
 * @param DateTime $start_date
 * @param DateTime $end_date
 * @param Statements
 *
 * @return array
 *
*/
function opigno_lrs_stats_dashboard_current_week_total_number_of_page_view_per_year_between($start_date, $end_date, $statements) {
  $formatted_data = array();
  $statements_per_month = opigno_lrs_stats_dashboard_empty_values_per_unit_between($start_date, $end_date, 'year', 'Y');

  //Aggregate viewed statements per day (override)
  $statements_per_month = array_replace($statements_per_month, opigno_lrs_stats_aggregate_statements_by_year($statements));

  foreach($statements_per_month as $day => $month_statements)
    $formatted_data[] = array('year' => $day, 'value' => count($month_statements));

  return $formatted_data;
}

/**
 * Generates empty time entries for morris.js
 *
 * Output example for days:
 *  array(
 *   '2015-01-01' => 0,
 *   '2015-01-02' => 0,
 *   '2015-01-03' => 0,
 *   '2015-01-04' => 0,
 *   '2015-01-05' => 0
 *  )
 *
 * Output example for months:
 *  array(
 *   '2015-01' => 0,
 *   '2015-02' => 0,
 *   '2015-03' => 0,
 *   '2015-04' => 0,
 *   '2015-05' => 0
 *  )
 *
 * Output example for years:
 *  array(
 *   '2015' => 0,
 *   '2016' => 0,
 *   '2017' => 0
 *  )
 *
 * @param DateTime $start_date
 * @param DateTime $end_date
 * @param string $unit
 * @param string $format
 *
 * @return array
 *
*/
function opigno_lrs_stats_dashboard_empty_values_per_unit_between($start_date, $end_date, $unit, $format) {
  $empty_values_per_unit = array();
  $current_date = $start_date;

  while($current_date <= $end_date)
  {
    $empty_values_per_unit[$current_date->format($format)] = array();
    $current_date->modify("+1 {$unit}");
  }

  return $empty_values_per_unit;
}

/**
 * Retrieve top ten viewed pages from tincan statements
 *
 * Output example:
 *  array(
 *    array('title' => 'Course 3', 'href' => 'http://opigno.com/course3', 'view_count' => 67, 'user_view_count' => 12),
 *    array('title' => 'Course 2', 'href' => 'http://opigno.com/course2', 'view_count' => 34, 'user_view_count' => 2)
 *    array('title' => 'Course 4', 'href' => 'http://opigno.com/course4', 'view_count' => 22, 'user_view_count' => 4)
 *   )
 *
 * @return array
*/
function opigno_lrs_stats_dashboard_top_ten_viewed_pages() {
  $viewed_pages = array();
  $statements_per_page = opigno_lrs_stats_aggregate_viewed_statements_by_page(opigno_lrs_stats_all_viewed_statements());

  foreach($statements_per_page as $page_statements)
  {
    $user_statements = array();
    foreach($page_statements as $statement) $user_statements[$statement->actor->name] = TRUE;
    $viewed_pages[] = array('title' => $page_statements[0]->object->definition->name->{'en-US'}, 'href' => $page_statements[0]->object->id, 'view_count' => count($page_statements), 'user_view_count' => count($user_statements));
  }

  usort($viewed_pages, function($viewed_page_a, $viewed_page_b){
    return $viewed_page_a['view_count'] < $viewed_page_b['view_count'];
  });

  $top_ten_viewed_pages = array_slice($viewed_pages, 0, 10);

  return $top_ten_viewed_pages;
}

/**
 * Retrieve most active users (max 10) from tincan statements
 *
 * Output example:
 *  array(
 *     array('url' => 'user/toto4', 'name' => 'toto4', 'statement_count' => 123),
 *     array('url' => 'user/toto2', 'name' => 'toto2', 'statement_count' => 34),
 *     array('url' => 'user/toto1', 'name' => 'toto1', 'statement_count' => 23)
 *  )
 *
 * @return array
*/
function opigno_lrs_stats_dashboard_present_most_active_users() {
  $users = array();
  $statements_per_user = opigno_lrs_stats_aggregate_statements_by_user(opigno_lrs_stats_all_statements());

  foreach($statements_per_user as $user_statements)
    $users[] = array('url' => 'users/' . $user_statements[0]->actor->name, 'username' => $user_statements[0]->actor->name, 'statement_count' => count($user_statements));

  usort($users, function($user_a, $user_b){
    return $user_a['statement_count'] < $user_b['statement_count'];
  });

  $most_active_users = array_slice($users, 0, 10);

  return $most_active_users;
}
















