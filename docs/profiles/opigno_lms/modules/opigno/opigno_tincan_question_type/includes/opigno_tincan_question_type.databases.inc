<?php

/**
 * @file
 * This file contains the classes corresponding to each database's table.
 *
 * It contains these classes:
 * - OpignoTincanQuestionTypePropertiesDatabase.
 * - OpignoTincanQuestionTypeAnswersDatabase.
 */

/**
 * This class provide the information about the question properties table.
 */
class OpignoTincanQuestionTypePropertiesDatabase {
  const NAME = 'opigno_tincan_question_type_question_properties';

  public static $fields = array(
    'nid' => array(
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
    ),
    'vid' => array(
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
    ),
    'fid' => array(
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
    ),
    'activity_id' => array(
      'type' => 'text',
    ),
    'launch_filename' => array(
      'type' => 'text',
    ),
  );

  public static $indexes = array(
    'question_nid' => array('nid'),
    'question_vid' => array('vid'),
  );

}


/**
 * This class provide the information about the user's answers table.
 */
class OpignoTincanQuestionTypeAnswersDatabase {
  const NAME = 'opigno_tincan_question_type_answers';

  public static $fields = array(
    'rid' => array(
      'type' => 'int',
      'unsigned' => TRUE,
      'not null' => TRUE,
    ),
    'registration' => array(
      'type' => 'text',
      'not null' => TRUE,
    ),
  );

  public static $indexes = array(
    'result_id' => array('rid'),
  );

}
