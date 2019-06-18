<?php
/**
 * @file
 * Hooks provided by the Survey Builder module.
 */

/**
 * Alter survey after it's loaded
 *
 * Modules may implement this hook to alter survey forms after they are loaded.
 *
 * @param array $form
 *   The survey form to be rendered
 * @param object $survey
 *   The loaded survey entity object
 */
function hook_survey_builder_survey_load_alter(array &$form, object &$survey) {
  $form['test'] = array('#markup' => '<p>Hello World</p>');
}

/**
 * Alter survey before it's created in the form_builder cache
 *
 * Modules may implement this hook to alter survey forms after they are created
 * for the first time in the form_builder cache.
 *
 * @param array $form
 *   The submitted new survey form
 * @param array $form_state
 *   The submitted form's state
 * @param object $survey
 *   The loaded survey entity object
 */
function hook_survey_builder_survey_create_alter(array &$form, array $form_state, object $survey) {
  $form['test'] = array('#markup' => '<p>Hello World</p>');
}

/**
 * Alter survey before it's saved
 *
 * Modules may implement this hook to alter surveys before they are saved.
 *
 * @param array $form
 *   The submitted form
 * @param array $form_state
 *   The submitted form's state
 * @param object $survey
 *   The survey entity object before it's saved
 */
function hook_survey_builder_survey_save_alter(array $form, array $form_state, object &$survey) {
  $survey->title = t('Save Example');
}

/**
 * A survey was saved
 *
 * @param object $survey
 *   The survey entity that was saved
 */
function hook_survey_builder_survey_saved($survey) {
  drupal_set_message(t('Saved survey called @title',
    array('@title' => $survey->title)));
}

/**
 * A survey response is being validated
 *
 * @param object $survey
 *   The survey entity of the saved survey response was saved
 * @param array $values
 *   The survey questions values
 * @param array $form_state
 *   The submitted form state
 */
function hook_survey_builder_survey_response_validate($survey, $values, $form_state) {
  drupal_set_message(t('Saved response validated successfully'));
  return true;
}

/**
 * Alter survey response before it's saved
 *
 * Modules may implement this hook to alter survey responses before they are saved.
 *
 * @param object $survey_response
 *   The survey_response entity object before it's saved
 * @param object $survey
 *   The survey entity object
 * @param array $values
 *   The survey questions values
 * @param array $form_state
 *   The submitted form state
 */
function hook_survey_builder_survey_response_save_alter(object &$survey_response, object &$survey, array &$values, array &$form_state) {
  $survey_response->uid = 1;
}

/**
 * A survey response was saved
 *
 * @param object $survey_response
 *   The survey response entity that was saved
 * @param object $survey
 *   The survey entity of the saved survey response was saved
 * @param array $values
 *   The survey questions values
 * @param array $form_state
 *   The submitted form state
 */
function hook_survey_builder_survey_response_saved($survey_response, $survey, $values, $form_state) {
  drupal_set_message(t('Saved response saved'));
}

/**
 * Alter questions before it's saved
 *
 * Modules may implement this hook to alter questions before they are saved.
 *
 * @param object $question
 *   The question entity object before it's saved
 * @param array $form
 *   The submitted form
 */
function hook_survey_builder_question_save_alter(object &$question) {
  $question->label = t('Question Example');
}

/**
 * A question was saved
 *
 * @param object $question
 *   The question entity that was saved
 */
function hook_survey_builder_question_saved($question) {
  drupal_set_message(t('Saved question called @label',
    array('@label' => $question->label)));
}

/**
 * Alter survey question response before it's saved
 *
 * Modules may implement this hook to alter survey question responses before they are saved.
 *
 * @param object $question_response
 *   The question_response entity object before it's saved
 */
function hook_survey_builder_question_response_save_alter(object &$question_response) {
  $question_response->uid = 1;
}

/**
 * A question response was saved
 *
 * @param object $question_response
 *   The question response entity that was saved
 */
function hook_survey_builder_question_response_saved($question_response) {
  drupal_set_message(t('Question response saved'));
}
