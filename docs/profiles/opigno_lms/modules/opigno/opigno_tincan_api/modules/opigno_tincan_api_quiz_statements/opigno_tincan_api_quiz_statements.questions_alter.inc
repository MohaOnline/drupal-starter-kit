<?php

function opigno_tincan_api_quiz_statements_tincan_question_statement_multichoice_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  $user_answers = _opigno_tincan_api_get_multichoice_user_answers($question->nid, $question->vid, $answers[0]['result_id']);
  $response = implode('[,]', $user_answers);

  _opigno_tincan_api_set_result(
    $statement,
    $answers[0]['score'],
    $question->max_score,
    $question->correct,
    $response
  );


  /******
   ** Set the interaction type properties
   ***/

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$choice);

  $choices = array();
  $correct = array();
  foreach ($question->alternatives as $alternative) {
    $choices[] = array(
      'id' => $alternative['id'],
      'description' => array('en-US' => $alternative['answer']['value'])
    );

    if ($alternative['score_if_chosen'] > 0)
      $correct[] = $alternative['id'];
  }
  $objectDefinition->setChoices($choices);
  $objectDefinition->setCorrectResponsesPattern(array(implode('[,]', $correct)));
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_long_answer_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  $answer = long_answer_get_answer($question->nid, $question->vid, $question->answers[0]['result_id']);
  _opigno_tincan_api_set_result(
    $statement,
    $answer['score'],
    $answer['rel_max_score'],
    null,
    $answer['answer']
  );


  /******
   ** Set the interaction type properties
   ***/

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$long_fill_in);
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_matching_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  $response_map = array();
  foreach ($answers[0]['answer'] as $answer_question_id => $answer_response_id)
    $response_map[] = $answer_question_id . '[.]' . $answer_response_id;

  _opigno_tincan_api_set_result(
    $statement,
    $answers[0]['score'],
    $question->max_score,
    $question->correct,
    implode('[,]', $response_map)
  );


  /******
   ** Set the interaction type properties
   ***/

  $match_source = array();
  $match_target = array();
  $correct_response_pattern = array();

  foreach ($question->match as $possibility) {
    $match_source[] = array(
      'id' => $possibility['match_id'],
      'description' => array('en-US' => $possibility['question'])
    );
    $match_target[] = array(
      'id' => $possibility['match_id'],
      'description' => array('en-US' => $possibility['answer'])
    );
    $correct_response_pattern[] = $possibility['match_id'] . '[.]' . $possibility['match_id'];
  }

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$matching);

  $objectDefinition->setCorrectResponsesPattern(implode('[,]', $correct_response_pattern));
  $objectDefinition->setSource($match_source);
  $objectDefinition->setTarget($match_target);
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_quiz_ddlines_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  _opigno_tincan_api_set_result(
    $statement,
    $answers[0]['score'],
    $question->max_score,
    $question->correct
  );


  /******
   ** Set the interaction type properties
   ***/

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$other);
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_quiz_directions_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  _opigno_tincan_api_set_result(
    $statement,
    null,
    null,
    $question->correct
  );


  // TODO: Change the verb here and remove the InteractionType content
  /******
   ** Set the interaction type properties
   ***/

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$other);
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_h5p_content_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  _opigno_tincan_api_set_result(
    $statement,
    $answers[0]['score'],
    $question->max_score,
    $question->correct
  );


  /******
   ** Set the interaction type properties
   ***/

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$other);
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_scale_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  _opigno_tincan_api_set_result(
    $statement,
    null,
    null,
    $question->correct,
    $answers[0]['answer']
  );


  /******
   ** Set the interaction type properties
   ***/

  $choices = array();
  foreach ($question as $key => $value) {
    if (is_numeric($key) && !empty($value) && isset($value->id) && isset($value->answer)) {
      $choices[] = array(
        'id' => $value->id,
        'description' => array('en-US' => $value->answer)
      );
    }
  }

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$choice);

  $objectDefinition->setChoices($choices);
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_truefalse_alter(TinCan\Statement &$statement, $question, $answers)
{
  /******
   ** Set the result
   ***/

  _opigno_tincan_api_set_result(
    $statement,
    $answers[0]['score'],
    $question->max_score,
    $question->correct,
    ($answers[0]['answer'] ? true : false)
  );


  /******
   ** Set the interaction type properties
   ***/

  $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
  $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$true_false);

  $objectDefinition->setCorrectResponsesPattern(array(($question->correct_answer ? true : false)));
}


function opigno_tincan_api_quiz_statements_tincan_question_statement_short_answer_alter(TinCan\Statement &$statement, $question, $answers)
{
  // If the question is manually validated, the variables are not in the same place
  if ($question->correct_answer_evaluation == 3) {
    /******
     ** Set the result
     ***/

    $answer = short_answer_get_answer($question->nid, $question->vid, $answers[0]['result_id']);
    _opigno_tincan_api_set_result(
      $statement,
      $answer['score'],
      $answer['rel_max_score'],
      null,
      $answer['answer']
    );


    /******
     ** Set the interaction type properties
     ***/

    $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
    $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$fill_in);
  } // Goes here if the question is automatically validated
  else {
    /******
     ** Set the result
     ***/

    _opigno_tincan_api_set_result(
      $statement,
      $answers[0]['score'],
      $question->max_score,
      $question->correct,
      $answers[0]['answer']
    );


    /******
     ** Set the interaction type properties
     ***/

    $objectDefinition = _opigno_tincan_api_get_object_definition($statement);
    $objectDefinition->setInteractionType(OpignoTincanApiTinCanInteractionTypes::$fill_in);

    $objectDefinition->setCorrectResponsesPattern(array($question->correct_answer));
  }
}


function _opigno_tincan_api_get_multichoice_user_answers($question_nid, $question_vid, $rid)
{
  return db_query('
    SELECT quam.answer_id
    FROM {quiz_multichoice_user_answer_multi} quam
    INNER JOIN {quiz_multichoice_user_answers} qua ON qua.id = quam.user_answer_id
    WHERE qua.question_nid = :qnid
    AND qua.question_vid = :qvid
    AND qua.result_id = :rid
  ', array(
    ':qnid' => $question_nid,
    ':qvid' => $question_vid,
    ':rid' => $rid
  ))->fetchCol();
}


/**
 * @param \TinCan\Statement $statement
 * @return \Tincan\ActivityDefinition
 */
function _opigno_tincan_api_get_object_definition(TinCan\Statement $statement)
{
  if (!$statement->getObject())
    $statement->setObject(array());

  if (!$statement->getObject()->getDefinition())
    $statement->getObject()->setDefinition(array());


  return $statement->getObject()->getDefinition();
}
