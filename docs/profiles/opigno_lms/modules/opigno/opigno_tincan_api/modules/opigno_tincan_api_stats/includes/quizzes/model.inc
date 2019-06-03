<?php

/**
 * @file
 * Quizzes presentation functions
 */

/**
 * Present quizzes statistics
 *
 * Output example:
 *  array(
 *    'http://opigno.com/node/14' => array(
 *       'time_taken' => 34,
 *       'number_of_users' => 15,
 *       'average_score' => 36
 *    ),
 *    'http://opigno.com/node/17' => array(
 *       'time_taken' => 14,
 *       'number_of_users' => 133,
 *       'average_score' => 23
 *    )
 *
 * @return array
 */
function opigno_lrs_stats_quizzes_quizzes_statistics() {
  $statements_aggregated_by_quizzes = opigno_lrs_stats_aggregate_statements_by_object(
    opigno_lrs_stats_all_statements_with_quiz_object()
  );

  //Fill with all course context with default values
  $quizzes_statistics = array_fill_keys(array_keys($statements_aggregated_by_quizzes), array(
    'number_of_views' => 0,
    'number_of_attempts' => 0,
    'number_of_users' => 0,
    'average_score' => 0
  ));

  $statements_aggregated_by_quizzes_and_verb = array_map('opigno_lrs_stats_aggregate_statements_by_verb', $statements_aggregated_by_quizzes);
  $statements_aggregated_by_quizzes_and_user = array_map('opigno_lrs_stats_aggregate_statements_by_user', $statements_aggregated_by_quizzes);

  foreach($quizzes_statistics as $quiz_id => &$quiz_statistics)
  {
    //Get quiz title in first statement
    //Try to use node title first, then if node has been removed, use title from statement
    $quiz_nid = opigno_lrs_stats_get_nid_by_object_id($quiz_id);
    $quiz = node_load($quiz_nid);
    $quiz_statistics['title'] = $quiz? $quiz->title : $statements_aggregated_by_quizzes[$quiz_id][0]->object->definition->name->{'en-US'};

    $total_score = 0;
    $number_of_score = 0;

    //Finished statements (can be completed or not, if not then score is in a scored statement)
//    $tincan_verb=tincanapi_get_verb('completed'); // TODO: Remove this
    $tincan_verb = OpignoTincanApiTinCanVerbs::$completed;
    $finished_verb_id = $tincan_verb['id'];
    $finished_statements = @$statements_aggregated_by_quizzes_and_verb[$quiz_id][$finished_verb_id];

    if(isset($finished_statements))
    {
      $finished_statements_with_score = opigno_lrs_stats_filter_finished_statements_with_score($finished_statements);
      $total_score += array_sum(opigno_lrs_stats_map_finished_statement_scores($finished_statements_with_score));
      $number_of_score += count($finished_statements_with_score);
    }
    $quiz_statistics['number_of_users'] = count($statements_aggregated_by_quizzes_and_user[$quiz_id]);

    //Scored statements (always has a score)
//    $tincan_verb=tincanapi_get_verb('scored'); // TODO: Remove this
    $tincan_verb = OpignoTincanApiTinCanVerbs::$scored;
    $scored_verb_id = $tincan_verb['id'];
    $scored_statements = @$statements_aggregated_by_quizzes_and_verb[$quiz_id][$scored_verb_id];
    if(isset($scored_statements))
    {
      $total_score += array_sum(opigno_lrs_stats_map_finished_statement_scores($scored_statements));
      $number_of_score += count($scored_statements);
    }
    $quiz_statistics['number_of_attempts'] = $number_of_score;
    $quiz_statistics['average_score'] = ($number_of_score > 0)? round(($total_score / $number_of_score) * 100, 0) : 0;

//    $tincan_verb=tincanapi_get_verb('viewed'); // TODO: Remove this
    $tincan_verb = OpignoTincanApiTinCanVerbs::$viewed;
    $viewed_verb_id = $tincan_verb['id'];
    $quiz_statistics['number_of_views'] = count($statements_aggregated_by_quizzes_and_verb[$quiz_id][$viewed_verb_id]);
  }

  return $quizzes_statistics;
}