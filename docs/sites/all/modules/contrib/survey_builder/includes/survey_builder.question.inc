<?php
/**
 * @file
 * Classes and related functions/hooks for question and question response entity types
 */

/**
 * Main class for question entities.
 */
class QuestionEntity extends SurveyBuilderEntity {
  protected $typeName = 'question';
}

/**
 * Load a single question entity from its ID
 *
 * @param $pid
 *   The entity ID.
 */
function question_load($pid) {
  $entities = entity_load('question', array($pid));
  if (count($entities)) {
    return array_shift($entities);
  }
  return FALSE;
}

/**
 * Load multiple question entities based on certain conditions.
 *
 * @param $pids
 *   An array of entity IDs.
 * @param $conditions
 *   An array of conditions to match against the {entity} table.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   An array of question entity objects, indexed by pid.
 */
function question_load_multiple($pids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('question', $pids, $conditions, $reset);
}

/**
 * Save a question entity and return its ID
 *
 * @param $label
 *   The question label
 *
 * @param $field
 *   The question field
 */
function question_save($label, $field, $type = 'question') {

  $question = new QuestionEntity(array(
    'label' => $label,
    'name_raw' => $label,
    'field' => $field,
  ));

  $question->type = $type;

  // Allow other modules to alter questions
  drupal_alter('survey_builder_question_save', $question);

  if (entity_save('question', $question) != SAVED_NEW) {
    return FALSE;
  }

  module_invoke_all('survey_builder_question_saved', $question);

  return $question->pid;

}

/**
 * Delete multiple question entities.
 *
 * @param $pids
 *   An array of question entity IDs.
 */
function question_delete_multiple(array $pids) {
  entity_get_controller('question')->delete($pids);
}

/**
 * Main class for question response entities.
 */
class QuestionResponseEntity extends Entity {

  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'question_response');
  }

  /**
   * Specifies the default uri, which is picked up by uri() by default.
   */
  protected function defaultURI() {
    return array('path' => 'question-response/' . $this->identifier());
  }

}

/**
 * Metadata class for question response entities
 */
class QuestionResponseEntityMetadata extends SurveyBuilderEntityMetadata {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties =& $info[$this->type]['properties'];
    $properties['question_id']['label'] = t('Question');
    $properties['question_id']['type'] = 'question';
    $properties['survey_response_id']['label'] = t('Survey Response');
    $properties['survey_response_id']['type'] = 'survey_response';
    return $info;
  }

}

/**
 * Load a single question response from a survey response ID and question key
 */
function question_response_load($survey_response_id, $question_key = false) {

  $query = new EntityFieldQuery;
  $query = $query
    ->entityCondition('entity_type', 'question_response')
    ->propertyCondition('survey_response_id', $survey_response_id, '=');

  if ($question_key !== false) {
    $query = $query->propertyCondition('question_key', $question_key, '=');
  }

  $result = $query->execute();

  if (empty($result['question_response'])) {
    return FALSE;
  }

  $entities = entity_load('question_response', array_keys($result['question_response']));

  if (count($entities) > 1) {
    return $entities;
  }
  elseif (count($entities) == 1) {
    return array_pop($entities);
  }

  return FALSE;

}

/**
 * Save a survey response's questions
 */
function survey_response_questions_save($questions, $values, $survey_response_id, &$score) {

  foreach ($questions as $key => $field) {

    if ($key[0] == '#') {
      continue;
    }
    elseif (isset($field['#type']) && $field['#type'] == 'fieldset') {

      $success = survey_response_questions_save($field, $values, $survey_response_id, $score);
      if (!$success) {
        return FALSE;
      }

    }
    else {

      if (!isset($values[$key])) {
        $values[$key] = '';
      }

      $question = array(
        'survey_response_id' => $survey_response_id,
        'question_id' => $field['#question_id'],
        'question_key' => $key,
        'value' => $values[$key],
      );

      $success = question_response_save($question, $score);
      if (!$success) {
        watchdog('survey_builder', 'Unable to save response for question: %id',
          array('%id' => $question['question_id']), WATCHDOG_ERROR);
        return FALSE;
      }

    }

  }

  return TRUE;

}

/**
 * Save a question response
 */
function question_response_save($params, &$score) {

  $question = question_load($params['question_id']);
  if (!$question) {
    watchdog('survey_builder', 'Invalid question ID when saving response: %id',
      array('%id' => $params['question_id']), WATCHDOG_ERROR);
    return FALSE;
  }

  $params['scores'] = (isset($question->field['#scores']['options'])) ? $question->field['#scores']['options'] : FALSE;
  $params['value_key'] = FALSE;

  switch ($question->field['#type']) {

    case 'checkboxes':
      $success = TRUE;
      if (!empty($params['value'])) {
        foreach ($params['value'] as $key => $val) {
          if (!empty($val)) {
            $params = array('value' => $val, 'value_key' => $key) + $params;
            if (!question_response_entity_save($params, $score)) {
              $success = FALSE;
            }
          }
        }
      }
      return $success;

    case 'radios':
    case 'select':
      if (isset($question->field['#options'][$params['value']])) {
        $params = array('value' => $question->field['#options'][$params['value']], 'value_key' => $params['value']) + $params;
        return question_response_entity_save($params, $score);
      }

    default:
      // The score for a number field is the value entered
      if ($question->field['#key'] == 'number') {
        $params = array('scores' => array($params['value'] => $params['value']), 'value_key' => $params['value']) + $params;
        return question_response_entity_save($params, $score);
      }
      // Fall through to default response save
      else if (strlen((string)$params['value']) < 1) {
        return TRUE;
      }
      return question_response_entity_save($params, $score);

  }

}

function question_response_entity_save($params, &$score) {

  $response = new QuestionResponseEntity();
  $response->survey_response_id = $params['survey_response_id'];
  $response->question_id = $params['question_id'];
  $response->question_key = $params['question_key'];
  $response->value = $params['value'];
  $response->created = time();

  if ($params['value_key'] !== FALSE) {
    $response->value_key = $params['value_key'];

    if ($params['scores'] !== FALSE && isset($params['scores'][$params['value_key']])) {
      $response->score = $params['scores'][$params['value_key']];

      if ($score === NULL) {
        $score = 0;
      }
      $score += $response->score;
    }
  }

  // Allow other modules to alter questions
  drupal_alter('survey_builder_question_response_save', $response);

  if (!$response->save()) {
    return FALSE;
  }

  module_invoke_all('survey_builder_question_response_saved', $response);

  return TRUE;

}

/**
 * Validate a survey response's questions
 */
function survey_response_questions_validate($questions, $values) {

  $success = true;

  foreach ($questions as $key => $field) {

    if ($key[0] == '#') {
      continue;

    } elseif (isset($field['#type']) && $field['#type'] == 'fieldset') {

      $ret = survey_response_questions_validate($field, $values);
      if (!$ret) {
        $success = false;
      }

    } else {

      $question = question_load($field['#question_id']);
      if (!$question) {
        watchdog('survey_builder', 'Unable to load question for validation: %id',
          array('%id' => $field['#question_id']), WATCHDOG_ERROR);
        continue;

      } else if (!isset($values[$key])) {
        $values[$key] = '';

      }

      $ret = question_field_response_validate($question->field, $values[$key]);
      if (!$ret) {
        form_set_error($key, t('The %name field is required', array('%name' => $field['#title'])));
        $success = false;
      }

    }

  }

  return $success;

}

/**
 * Validate a question response
 */
function question_field_response_validate($field, $value) {

  if (!isset($field['#required']) || !$field['#required']) {
    return TRUE;
  }

  switch ($field['#type']) {

    case 'checkboxes':
      $success = FALSE;
      foreach ($value as $key => $val) {
        if (!is_null($val)) {
          $success = TRUE;
        }
      }
      return $success;

    case 'radios':
    case 'select':
      if (isset($field['#options'][$value])) {
        $text_value = $field['#options'][$value];
        return (!empty($text_value));
      }
      // Fall through to default response validation

    default:
      return (strlen((string)$value) > 0);

  }

}

/**
 * Implements hook_field_info().
 */
function survey_builder_field_info() {
  return array(
    'question' => array(
      'label' => t('Question'),
      'description' => t('This field encapsulates another field of which there can be only one instance'),
      'instance_settings' => array('label' => '', 'data' => ''),
      'default_widget' => 'question',
      'default_formatter' => 'question',
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function survey_builder_field_widget_info() {
  return array(
    'question' => array(
      'label' => t('Question'),
      'field types' => array('question'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function survey_builder_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  $element += array(
    '#type' => 'textfield',
    '#default_value' => '',
  );

  $element['#element_validate'][] = 'survey_builder_field_widget_validate';

  return array('value' => $element);

}

/**
 * FAPI validation of an individual question element.
 */
function survey_builder_field_widget_validate($element, &$form_state) {
  // TODO
}

/**
 * Implements hook_field_widget_error().
 */
function survey_builder_field_widget_error($element, $error, $form, &$form_state) {
  form_error($element['value'], $error['message']);
}

/**
 * Implements hook_element_info()
 */
function survey_builder_element_info() {
  return array(
    'question' => array(
      '#type' => 'question',
      '#input' => TRUE,
      '#theme' => 'question',
      '#theme_wrappers' => array('form_element'),
      '#process' => array('survey_builder_element_process'),
    ),
  );
}

function survey_builder_element_process($element, $form_state, $complete_form) {
  return $element;
}
