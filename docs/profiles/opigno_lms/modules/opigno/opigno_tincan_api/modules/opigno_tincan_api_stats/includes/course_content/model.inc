<?php

/**
 * @file
 * Courses model functions
 */

/**
 * Count total number of page view for all course contents
 *
 * @param string $statement_filter_function
 *  Function to use as statement filter
 *
 * @return int
 */
function opigno_lrs_stats_course_content_total_number_of_page_view($statement_filter_function = NULL)
{
  $statements = isset($statement_filter_function)? $statement_filter_function(opigno_lrs_stats_all_course_content_viewed_statements()) : opigno_lrs_stats_all_course_content_viewed_statements();
  return count($statements);
}

/**
 * Count total number of active users for all course contents
 *
 * @param string $statement_filter_function
 *  Function to use as statement filter
 *
 * @return int
 */
function opigno_lrs_stats_course_content_total_number_of_active_users($statement_filter_function = NULL)
{
  $statements = isset($statement_filter_function)? $statement_filter_function(opigno_lrs_stats_all_course_content_statements()) : opigno_lrs_stats_all_course_content_statements();
  return count(opigno_lrs_stats_aggregate_statements_by_user($statements));
}

/**
 * Count total number of activities for all course contents
 *
 * @param string $statement_filter_function
 *  Function to use as statement filter
 *
 * @return int
 */
function opigno_lrs_stats_course_content_total_number_of_activities($statement_filter_function = NULL)
{
  $statements = isset($statement_filter_function)? $statement_filter_function(opigno_lrs_stats_all_course_content_statements()) : opigno_lrs_stats_all_course_content_statements();
  return count($statements);
}

/**
 * Calculate average score for all course contents
 *
 * @param string $statement_filter_function
 *  Function to use as statement filter
 *
 * @return int
 */
function opigno_lrs_stats_course_content_avg_score($statement_filter_function = NULL)
{
  $total_score = 0;
  $passed_statements = isset($statement_filter_function)? $statement_filter_function(opigno_lrs_stats_all_course_content_passed_statements()) : opigno_lrs_stats_all_course_content_passed_statements();
  $failed_statements = isset($statement_filter_function)? $statement_filter_function(opigno_lrs_stats_all_course_content_failed_statements()) : opigno_lrs_stats_all_course_content_failed_statements();
  $finished_statements = array_merge($passed_statements, $failed_statements);
  $finished_statements_with_score = opigno_lrs_stats_filter_finished_statements_with_score($finished_statements);
  $total_score += array_sum(opigno_lrs_stats_map_finished_statement_scores($finished_statements_with_score));
  $number_of_score = count($finished_statements);
  return ($number_of_score>0)? round(($total_score / $number_of_score) * 100, 0) : 0;
}

/**
 * Present general statistics
 *
 * Output example:
 *  array(
 *    'total_number_of_page_view' => 150,
 *    'total_number_of_active_users => 34,
 *    'total_number_of_activities' => 90
 *  )
 *
 * @param $statement_filter_function
 *  Function to use as statement filter
 *
 * @return array
 */
function opigno_lrs_stats_course_content_general_statistics($statement_filter_function)
{
  return array(
    'total_number_of_page_view' => opigno_lrs_stats_course_content_total_number_of_page_view($statement_filter_function),
    'total_number_of_active_users' => opigno_lrs_stats_course_content_total_number_of_active_users($statement_filter_function),
    'total_number_of_activities' => opigno_lrs_stats_course_content_total_number_of_activities($statement_filter_function),
    'avg_score' => opigno_lrs_stats_course_content_avg_score($statement_filter_function),
  );
}

/**
 * Present course contexts statistics
 *
 * Output example:
 *  array(
 *    'http://opigno.com/node/15' => array(
 *       'number_of_visit' => 123,
 *       'number_of_users' => 15,
 *       'percentage_of_users' => 34
 *    ),
 *    'http://opigno.com/node/18' => array(
 *       'number_of_visit' => 33,
 *       'number_of_users' => 15,
 *       'percentage_of_users' => 12
 *    )
 *
 *  )
 *
 * @param $statement_filter_function
 *  Function to use as statement filter
 *
 * @return array
 */
function opigno_lrs_stats_course_content_course_contexts_statistics($statement_filter_function = NULL)
{
  $statements = function_exists($statement_filter_function) ?  $statement_filter_function(opigno_lrs_stats_all_course_content_statements()) : opigno_lrs_stats_all_course_content_statements();
  $statements_aggregated_by_courses = opigno_lrs_stats_aggregate_statements_by_context($statements);
  $course_contexts_statistics = array_fill_keys(array_keys($statements_aggregated_by_courses), array(
    'number_of_visit' => 0,
    'number_of_users' => 0,
    'percentage_of_users' => 0
  ));

  $statements_aggregated_by_courses_and_verb = array_map('opigno_lrs_stats_aggregate_statements_by_verb', $statements_aggregated_by_courses);
  $statements_aggregated_by_courses_and_user = array_map('opigno_lrs_stats_aggregate_statements_by_user', $statements_aggregated_by_courses);

  $total_number_of_users = count(opigno_lrs_stats_aggregate_statements_by_user((isset($statement_filter_function)? $statement_filter_function(opigno_lrs_stats_all_statements()) : opigno_lrs_stats_all_statements())));

  foreach($course_contexts_statistics as $course_context_id => &$course_context_statistics)
  {
    //Get course title in first statement
    //Try to use node title first, then if node has been removed, use title from statement
    $course_nid = opigno_lrs_stats_get_nid_by_object_id($course_context_id);
    $course = node_load($course_nid);
    $course_context_statistics['title'] = ($course)? $course->title : $statements_aggregated_by_courses[$course_context_id][0]->context->contextActivities->grouping->definition->name->{'en-US'};

//    $tincan_verb=tincanapi_get_verb('viewed'); // TODO: remove this
    $tincan_verb=OpignoTincanApiTinCanVerbs::$viewed;
    $viewed_verb_id = $tincan_verb['id'];
    $viewed_statements = @$statements_aggregated_by_courses_and_verb[$course_context_id][$viewed_verb_id];
    if(isset($viewed_statements)) $course_context_statistics['number_of_visit'] = count($viewed_statements);
    $course_context_statistics['number_of_users'] = count($statements_aggregated_by_courses_and_user[$course_context_id]);
    $course_context_statistics['percentage_of_users'] = ($total_number_of_users>0)? round(($course_context_statistics['number_of_users'] / $total_number_of_users) * 100, 0) : 0;
  }

  return $course_contexts_statistics;
}