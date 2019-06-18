<?php
/**
 * @file
 * Classes and related functions for survey and survey response entity types
 */

/**
 * Main class for survey entities.
 */
class SurveyEntity extends SurveyBuilderEntity {
  protected $typeName = 'survey';
}

/**
 * Controller class for survey entities
 */
class SurveyEntityController extends EntityAPIControllerExportable {

  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    // Process fields
    foreach ($entity->fields as $key => &$field) {
      survey_builder_load_process_field($field);
    }

    // Render full survey or question text only, depending on view mode
    switch($view_mode) {
      case 'summary':
        $content['questions'] = array(
          '#theme' => 'item_list',
          '#items' => $this->getQuestionsText($entity->fields),
        );
        break;

    }

    return parent::buildContent($entity, $view_mode, $langcode, $content);
  }

  private function getQuestionsText($fields) {
    $questions = array();
    foreach ($fields as $key => &$field) {
      if ($key[0] == '#') {
        continue;
      }
      elseif (isset($field['#type']) && $field['#type'] == 'fieldset') {
        $questions += $this->getQuestionsText($field);
      }
      elseif (!empty($field['#title'])) {
        $questions[] = $field['#title'];
      }
    }
    return $questions;
  }

}

/*
 * The entity form will be defined by the ENTITY_TYPE_form() so that it can be
 * retrieved using entity_ui_get_form() invoked by
 * EntityDefaultUIController::hook_menu(). This let us to be not worried about
 * all the field attach stuff
 * ref: https://drupal.org/node/1021576
 */

/**
 * UI controller.
 */
class SurveyUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ?
      $this->entityInfo['admin ui']['menu wildcard'] :
      '%' . $this->entityType;

    $items[$this->path]['description'] = 'Add edit and update surveys.';

    // View/complete surveys
    $items['survey/' . $wildcard] = array(
      'title callback'   => 'survey_page_title',
      'title arguments'  => array(1),
      'page callback'    => 'survey_page_view',
      'page arguments'   => array(1),
      'access callback'  => 'survey_access',
      'access arguments' => array('view', 1),
      'type'             => MENU_CALLBACK,
    );

    // Default menu item options
    $defaults = array(
      'type' => MENU_CALLBACK,
      'file' => 'survey_builder.admin.inc',
      'file path' => drupal_get_path('module', 'survey_builder') . '/includes',
      'page arguments' => array(1),
      'access arguments' => array('survey create'),
    );

    // Edit survey
    $items['survey/%/edit'] = array(
      'title callback' => 'survey_builder_create_title',
      'title arguments' => array(1),
      'page callback' => 'survey_builder_create',
    ) + $defaults;

    // Save survey
    $items['survey/%/save'] = array(
      'title' => 'Save Survey',
      'page callback' => 'survey_builder_save',
    ) + $defaults;

    // Delete survey
    $items['survey/%/delete'] = array(
      'title' => 'Delete Survey',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('survey_builder_delete', 1),
      'access arguments' => array('survey delete'),
    ) + $defaults;

    // Merge multiple existing surveys into new survey
    $items['surveys/merge'] = array(
      'title' => 'Merge Surveys',
      'page callback' => 'survey_builder_merge',
      'page arguments' => array(),
    ) + $defaults;

    return $items;
  }
}

/**
 * Delete a single survey entity from its ID
 *
 * @param $pid
 *   The entity ID.
 */
function survey_delete($pid) {
  return entity_delete('survey', $pid);
}

/**
 * Rebuild form builder cache
 */
function survey_builder_rebuild_cache() {

  $result = db_select('form_builder_cache', 'c')
    ->fields('c', array('sid', 'form_id', 'data'))
    ->condition('type', 'survey')
    ->execute();

  while($row = $result->fetchAssoc()) {

    // Load form array
    $form = unserialize($row['data']);

    if (is_array($form)) {
      // Update fields in form array from master questions
      foreach ($form as $key => &$field) {
        survey_builder_rebuild_cache_field($field);
      }
    }

    // Serialize form array and update DB
    db_update('form_builder_cache')
      ->fields(array(
        'data' => serialize($form)
      ))
      ->condition('sid', $row['sid'])
      ->condition('type', 'survey')
      ->condition('form_id', $row['form_id'])
      ->execute();

  }

}

/**
 * Rebuild a form builder cache field
 */
function survey_builder_rebuild_cache_field(&$field) {

  static $elements = array('#question_id', '#title', '#key', '#form_builder', '#weight', '#required', '#master');
  static $questions = array();

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.question');

  if (isset($field['#type']) && $field['#type'] == 'fieldset') {
    // Recursively process any fields within the fieldset
    foreach ($field as $key => &$value) {
      if ($key[0] != '#') {
        survey_builder_rebuild_cache_field($value);
      }
    }
  }
  else if (isset($field['#question_id'])) {
    $id = $field['#question_id'];

    if (!isset($questions[$id]->field)) {
      $questions[$id] = question_load($id);
      if (!isset($questions[$id]->field)) {
        watchdog('survey_builder', 'Unable to load question: %id', array('%id' => $id), WATCHDOG_ERROR);
        return;
      }
    }

    // Update field from master question
    if (is_array($field)) {
      foreach($questions[$id]->field as $key => &$value) {
        if (!in_array($key, $elements)) {
          $field[$key] = $value;
        }
      }
    }
  }

}

/**
 * Main class for survey response entities.
 */
class SurveyResponseEntity extends SurveyBuilderEntity {
  protected $typeName = 'survey_response';
}

/**
 * Metadata class for survey response entities
 */
class SurveyResponseEntityMetadata extends SurveyBuilderEntityMetadata {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties =& $info[$this->type]['properties'];
    $properties['survey_id']['label'] = t('Survey');
    $properties['survey_id']['type'] = 'survey';
    $properties['uid']['label'] = t('User');
    $properties['uid']['type'] = 'user';
    return $info;
  }

}

/**
 * Load a single survey response entity from its ID
 *
 * @param $pid
 *   The entity ID.
 */
function survey_response_load($pid) {
  $entities = entity_load('survey_response', array($pid));
  if (count($entities)) {
    return array_shift($entities);
  }
  return FALSE;
}

/**
 * Save survey response
 */
function survey_response_save($survey_id, $values, $form_state, $uid = FALSE, $skip_access = FALSE) {

  global $user;

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.question');

  $survey = survey_load($survey_id);
  if (!$survey) {
    watchdog('survey_builder', 'Invalid survey: %id', array('%id' => $survey_id), WATCHDOG_ERROR);
    return FALSE;
  }

  $response = new SurveyResponseEntity();
  $response->survey_id = $survey_id;
  $response->created = time();

  if ($uid !== FALSE && ($skip_access || user_access('survey complete as user'))) {
      $response->uid = $uid;
  } else {
      $response->uid = $user->uid;
  }

  // Allow other modules to alter survey responses
  drupal_alter('survey_builder_survey_response_save', $response, $survey, $values, $form_state);

  $response->save();

  $score = NULL;
  $success = survey_response_questions_save($survey->fields, $values, $response->pid, $score);
  if (!$success) {
    return FALSE;
  }

  if ($score !== NULL) {
    $response->score = $score;
    $response->save();
  }

  module_invoke_all('survey_builder_survey_response_saved', $response, $survey, $values, $form_state);

  return $response->pid;

}

/**
 * Survey response validate
 */
function survey_response_validate($survey_id, $values, $form_state) {

  $survey = survey_load($survey_id);
  if (!$survey) {
    watchdog('survey_builder', 'Invalid survey: %id', array('%id' => $survey_id), WATCHDOG_ERROR);
    return FALSE;
  }

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.admin');
  module_load_include('inc', 'survey_builder', 'includes/survey_builder.question');

  $validations = module_invoke_all('survey_builder_survey_response_validate', $survey, $values, $form_state);
  $validations[] = survey_response_questions_validate($survey->fields, $values);

  foreach($validations as $pass) {
      if (!$pass) {
          return false;
      }
  }

  return true;

}
