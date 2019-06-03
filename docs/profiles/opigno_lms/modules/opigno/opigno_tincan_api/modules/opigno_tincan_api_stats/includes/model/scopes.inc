<?php

/**
 * @file
 * Implements functions to scope queries
 */

/**
 * Scope pipeline to retrieve only viewed statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_viewed_statements($pipeline) {
//  $verb = tincanapi_get_verb('viewed'); // TODO remove this
  $verb = OpignoTincanApiTinCanVerbs::$viewed;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb['id']
  ));
}

/**
 * Scope pipeline to retrieve only started statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_started_statements($pipeline) {
//  $verb = tincanapi_get_verb('launched'); // TODO remove this
  $verb = OpignoTincanApiTinCanVerbs::$launched;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb['id']
  ));
}

/**
 * Scope pipeline to retrieve only finished statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_finished_statements($pipeline) {
//  $verb = tincanapi_get_verb('completed'); // TODO remove this
  $verb = OpignoTincanApiTinCanVerbs::$completed;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb['id']
  ));
}

/**
 * Scope pipeline to retrieve only passed statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_passed_statements($pipeline) {
  $verb = OpignoTincanApiTinCanVerbs::$passed;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb['id']
  ));
}

/**
 * Scope pipeline to retrieve only passed statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_failed_statements($pipeline) {
  $verb = OpignoTincanApiTinCanVerbs::$failed;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb['id']
  ));
}

/**
 * Scope pipeline to retrieve only answered statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_answered_statements($pipeline) {
//  $verb = tincanapi_get_verb('answered'); // TODO remove this
  $verb = OpignoTincanApiTinCanVerbs::$answered;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb['id']
  ));
}

/**
 * Scope pipeline to retrieve only attempted statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_attempted_statements($pipeline) {
//  $verb = tincanapi_get_verb('attempted'); // TODO remove this
  $verb = OpignoTincanApiTinCanVerbs::$attempted;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb['id']
  ));
}

/**
 * Scope pipeline to retrieve only scored statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_scored_statements($pipeline) {
//  $verb = tincanapi_get_verb('scored'); // TODO remove this
  $verb = OpignoTincanApiTinCanVerbs::$scored;
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.verb.id' => $verb
  ));
}

/**
 * Scope pipeline to unwind statement context
 *
 * Note:
 *  - Unwind duplicate statements
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_unwind_context($pipeline) {
  return opigno_lrs_stats_query_unwind($pipeline, '$statement.context.contextActivities.grouping');
}

/**
 * Scope pipeline to retrieve only statements with course context
 *
 * Note: "opigno_lrs_stats_query_scope_unwind_context" must be called first on pipeline
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_course_context($pipeline) {
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.context.contextActivities.grouping.definition.type' => OpignoTincanApiTinCanActivityDefinitionTypes::$course
  ));
}

/**
 * Scope pipeline to retrieve only statements related to a quiz
 *
 * Note: "opigno_lrs_stats_query_scope_unwind_context" must be called first on pipeline
 *
 * @param $pipeline
 *   A structured pipeline array.
 *
 * @return array
 *   A structured pipeline array.
 */
function opigno_lrs_stats_query_scope_quiz_object($pipeline) {
  return opigno_lrs_stats_query_match($pipeline, array(
    'statement.object.definition.type' => OpignoTincanApiTinCanActivityDefinitionTypes::$lesson
  ));
}