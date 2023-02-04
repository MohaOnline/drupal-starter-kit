<?php

/**
 * @file
 * This file provides metadata to the Views 2 API for views integration.
 */

/**
 * Implements hook_views_data_alter().
 */
function quiz_views_data_alter(&$data) {
  // Join the quiz properties table to Quiz nodes.
  $data['quiz_node_properties']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'INNER',
    ),
    'node_revision' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'INNER',
    ),
  );

  $data['quiz_node_results']['table']['join'] = array(
    // This is vid because vid is always more specific.
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'quiz_node_properties' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['quiz_node_relationship']['table']['join'] = array(
    // This is vid because vid is always more specific.
    'node' => array(
      'left_field' => 'vid',
      'field' => 'parent_vid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'parent_vid',
    ),
    'quiz_node_properties' => array(
      'left_field' => 'vid',
      'field' => 'parent_vid',
      'type' => 'INNER',
    ),
  );

  $data['quiz_node_results_answers']['table']['join'] = array(
    'quiz_node_properties' => array(
      'left_table' => 'quiz_node_relationship',
      'left_field' => 'child_nid',
      'field' => 'question_nid',
      'extra' => 'quiz_node_results_answers.result_id = quiz_node_results.result_id',
    ),
  );

  // Filter for the node being a quiz question.
  $data['node']['quiz_question_type'] = array(
    'title' => t('Question type'),
    'help' => t('Filter by quiz question type.'),
    'real field' => 'type',
    'filter' => array(
      'label' => 'Quiz question type',
      'handler' => 'quiz_views_handler_filter_quiz_question_type',
    ),
  );

  // Filter for the node being a quiz question.
  $data['node']['quiz_question'] = array(
    'title' => t('Quiz question'),
    'help' => t('Filter by quiz question.'),
    'real field' => 'type',
    'filter' => array(
      'label' => 'Quiz question',
      'handler' => 'quiz_views_handler_filter_quiz_question',
    ),
  );

  $data['quiz_node_results']['answers'] = array(
    'title' => t('Answers'),
    'help' => t('When rendered, this field will be replaced by all the answers.'),
    'field' => array(
      'title' => 'Answers',
      'handler' => 'views_handler_field_custom',
    ),
  );

  $data['quiz_node_results']['answer'] = array(
    'title' => t('Answer'),
    'help' => t('When rendered, this field will be replaced by a requested answer.'),
    'field' => array(
      'title' => 'Answer',
      'handler' => 'quiz_views_handler_field_quiz_question_result_answer',
    ),
  );
}

