<?php
/**
 * @file
 * Survey Builder interface code
 */

/**
 * Create/edit survey page title callback
 */
function survey_builder_create_title($form_id) {
  if ($form_id != SURVEY_BUILDER_NEW) {
    return t('Edit Survey');
  }
  return t('Add Survey');
}

/**
 * Create/edit survey page callback
 */
function survey_builder_create($form_id) {

  drupal_add_css(drupal_get_path('module', 'survey_builder') . '/theme/css/survey.css', array('group' => CSS_DEFAULT));

  $options = array('type' => 'file', 'scope' => 'footer');
  drupal_add_js(drupal_get_path('module', 'survey_builder') . '/theme/js/jquery.tools.min.js', $options);
  drupal_add_js(drupal_get_path('module', 'survey_builder') . '/theme/js/survey.js', $options);

  module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');

  return array(
    '#type'       => 'container',
    '#attributes' => array(),
    'clone'       => array(
      '#markup' => '<a href="#" class="button survey-builder-clone-button">Clone</a>',
    ),
    'survey'      => form_builder_interface('survey', $form_id),
  );

}

/**
 * Survey save callback
 */
function survey_builder_save($form_id) {

  module_load_include('inc', 'form_builder', 'includes/form_builder.api');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');

  $form = form_builder_cache_load('survey', $form_id);
  form_builder_save_form($form, 'survey', $form_id);

  drupal_goto('surveys');

}

/**
 * Prepare a field from a form builder form array for saving
 */
function survey_builder_save_process_field(&$field) {

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.question');

  if (!is_array($field)) {
    return;
  }
  else {
    if ($field['#type'] == 'fieldset') {
      // Recursively process any fields within the fieldset
      foreach ($field as $key => &$value) {
        if ($key[0] != '#') {
          survey_builder_save_process_field($value);
        }
      }
    }
    else {
      $master         = FALSE;
      $question_field = $field;

      // These values will be set
      $elements = array('#question_id', '#title', '#key', '#form_builder', '#weight', '#required', '#master');

      if (!isset($field['#question_id'])) {

        // New question. Save it and store the new question's ID
        $id = question_save($field['#title'], $field);
        if ($id === FALSE) {
          watchdog('survey_builder', 'Unable to save new question: %field',
            array('%field' => print_r($field, TRUE)), WATCHDOG_ERROR);
          return;
        }

        $master = TRUE;

      }
      else {

        $id = $field['#question_id'];

        $question = question_load($id);
        if ($question === FALSE) {
          watchdog('survey_builder', 'Unable to load existing question: %field',
            array('%field' => print_r($field, TRUE)), WATCHDOG_ERROR);
          return;
        }

        if (isset($field['#master']) && $field['#master']) {
          $master = TRUE;
          unset($field['#master']);

          $question->label = $field['#title'];
          $question->field = $field;
          $question->save();
        }
        else {
          $question_field = $question->field;
        }

      }

      $field = array(
        '#question_id'  => $id,
        '#title'        => $field['#title'],
        '#key'          => $field['#key'],
        '#form_builder' => array(
          'element_id'   => $field['#form_builder']['element_id'],
          'element_type' => 'question',
          'configurable' => TRUE,
          'removable'    => TRUE,
          'parent_id'    => 0,
        ),
        '#weight'       => (isset($field['#weight'])) ? $field['#weight'] : 0,
        '#required'     => (isset($field['#required'])) ? $field['#required'] : 0,
        '#master'       => $master,
      );

      // Duplicate all other values not set
      if (is_array($field)) {
        foreach ($question_field as $key => $value) {
          if (!in_array($key, $elements)) {
            $field[$key] = $value;
          }
        }
      }

    }
  }

}

/**
 * View completed survey page callback
 */
function survey_builder_response_view($response_id) {

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.survey');

  $response = survey_response_load($response_id);
  if (!$response) {
    return MENU_NOT_FOUND;
  }

  $survey = survey_load($response->survey_id);
  if (!$survey) {
    return MENU_NOT_FOUND;
  }

  $header = array(
    t('Question'),
    t('Response')
  );

  $rows = survey_builder_response_process_fields($survey->fields, $response->pid);

  return theme('table', array('header' => $header, 'rows' => $rows));

}

function survey_builder_response_process_fields($fields, $survey_response_id) {

  $rows = array();

  // Sort the fields based on weight
  uasort($fields, 'element_sort');

  foreach ($fields as $key => &$field) {

    if ($key[0] == '#') {
      continue;
    }
    elseif (isset($field['#type']) && $field['#type'] == 'fieldset') {
      // Recursively process any fields within the fieldset
      $rows = array_merge($rows, survey_builder_response_process_fields($field, $survey_response_id));
    }
    else {

      survey_builder_load_process_field($field, $survey_response_id);

      $value = '';

      if (isset($field['#default_value'])) {
        if (is_array($field['#default_value'])) {
          $values = array();
          foreach ($field['#default_value'] as $item) {
            if (!empty($field['#options'][$item])) {
              $values[] = $field['#options'][$item];
            }
          }
          $len = count($values);
          if ($len > 1) {
            $value = implode(', ', $values);
          }
          elseif ($len === 1) {
            $value = $values[0];
          }
          else {
            $value = '';
          }
        }
        elseif (isset($field['#options'])) {
          $value = $field['#options'][$field['#default_value']];
        }
        else {
          $value = $field['#default_value'];
        }
      }

      $value_cell = strlen($value)? check_plain($value) : '&lowast;';

      $rows[] = array(check_plain($field['#title']), $value_cell);

    }

  }

  return $rows;

}

/**
 * Survey delete page callback
 */
function survey_builder_delete($form, &$form_state, $form_id) {

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.survey');

  $survey = survey_load($form_id);
  if (!$survey) {
    return MENU_NOT_FOUND;
  }

  $form['pid'] = array('#type' => 'value', '#value' => $survey->pid);

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $survey->title)),
    'surveys',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );

}

/**
 * Delete survey
 */
function survey_builder_delete_submit($form, &$form_state) {

  if ($form_state['values']['confirm']) {

    $survey = survey_load($form_state['values']['pid']);

    if (survey_delete($survey->pid) === FALSE) {
      watchdog('survey_builder', 'Failed to delete %title.', array('%title' => $survey->title));
      drupal_set_message(t('Unable to delete %title', array('%title' => $survey->title)), 'error');
    }
    else {
      watchdog('survey_builder', 'Deleted %title.', array('%title' => $survey->title));
      drupal_set_message(t('%title has been deleted.', array('%title' => $survey->title)));
    }

  }

  $form_state['redirect'] = 'surveys';

}

/**
 * Theme function for questions palette view
 */
function survey_builder_question_palette_render($vars) {

  static $url = FALSE;
  static $dest = FALSE;

  if ($url === FALSE) {
    $item = menu_get_item();
    if (isset($item['page_arguments'][0]) && is_string($item['page_arguments'][0])) {
      $url = sprintf('admin/build/form-builder/add/survey/%s/question', $item['page_arguments'][0]);
    }
  }

  if ($dest === FALSE) {
    $dest = sprintf('survey/%s/edit', arg(1));
  }

  return l(
    $vars['row']->pid . ': ' . $vars['row']->survey_question_label,
    $url,
    array(
      'query' => array(
        'destination' => $dest,
        'question_id' => $vars['row']->pid,
      ),
    )
  );
}

/**
 * Configuration form for the "scores" property.
 */
function survey_builder_property_scores_form(&$form_state, $form_type, $element) {
  $form = array();

  $form['scores'] = array(
    '#form_builder'     => array('property_group' => 'scores'),
    '#title'            => t('Scores'),
    '#type'             => 'options',
    '#default_value'    => $element['#default_value'],
    '#options'          => isset($element['#scores']['options']) ? $element['#scores']['options'] : array(),
    '#required'         => TRUE,
    '#multiple'         => isset($element['#multiple']) ? $element['#multiple'] : FALSE,
    '#multiple_toggle'  => isset($element['#multiple_toggle']) ? $element['#multiple_toggle'] : FALSE,
    '#optgroups'        => $element['#type'] == 'select' ? TRUE : FALSE,
    '#limit'            => 100,
    '#key_type'         => isset($element['#key_type']) ? $element['#key_type'] : 'mixed',
    '#key_type_toggle'  => isset($element['#key_type_toggle']) ? $element['#key_type_toggle'] : TRUE,
    '#key_type_toggled' => isset($element['#key_type_toggled']) ? $element['#key_type_toggled'] : FALSE,
  );

  // Remove the default value field, since it's handled by the options field.
  $form['default_value'] = array();
  return $form;
}

/**
 * Configuration form for the "title" property.
 */
function survey_builder_property_title_form(&$form_state, $form_type, $element) {
  $form = array();

  $form['title'] = array(
    '#title'         => t('Title'),
    '#type'          => 'textfield',
    '#default_value' => $element['#title'],
    '#required'      => TRUE,
    '#weight'        => -10,
    '#maxlength'     => 4096,
  );

  return $form;
}

/**
 * Create new survey (i.e. clear form builder cache and set title)
 */
function survey_builder_new_form($form, &$form_state) {

  $form = array();

  $form['survey_title'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Title'),
    '#size'     => 50,
    '#required' => TRUE,
  );

  $form['new'] = array(
    '#type'  => 'submit',
    '#value' => t('Create New Survey'),
  );

  return $form;

}

function survey_builder_new_form_submit($form, &$form_state) {

  module_load_include('inc', 'form_builder', 'includes/form_builder.admin');
  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');

  $form = array(
    '#survey_title' => $form_state['values']['survey_title'],
  );


  //passing query params to modules altering surveys through $form_state array.
  $params     = drupal_get_query_parameters();
  $form_state['survey_params'] = $params;
  // Allow other modules to alter surveys
  drupal_alter('survey_builder_survey_create', $form, $form_state, $survey);

  // Save form to cache
  form_builder_cache_save('survey', SURVEY_BUILDER_NEW, $form);

  // Go to survey edit form
  drupal_goto('survey/' . SURVEY_BUILDER_NEW . '/edit',
    array(
      'query' => $params + array('start' => 0)
    )
  );

}

/**
 * Clone existing survey
 */
function survey_builder_clone_form($form, &$form_state, $form_id) {

  $form = array();

  $form['survey_title'] = array(
    '#type'     => 'textfield',
    '#title'    => t('Title'),
    '#size'     => 50,
    '#required' => TRUE,
  );

  $form['survey_id'] = array(
    '#type'  => 'hidden',
    '#value' => $form_id
  );

  $form['clone'] = array(
    '#type'  => 'submit',
    '#value' => t('Clone Survey'),
  );

  return $form;

}

function survey_builder_clone_form_submit($form, &$form_state) {

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.survey');

  $pid    = $form_state['values']['survey_id'];
  $survey = survey_load($pid);
  if (!$survey) {
    drupal_set_message(t('Unable to clone existing survey'), 'error');
    watchdog('survey_builder', 'Unable to clone existing survey: !pid', array('!form_id' => $pid));
    return FALSE;
  }

  $title = $form_state['values']['survey_title'];

  $clone       = new SurveyEntity(array('title' => $title, 'name_raw' => $title));
  $clone->type = 'survey';

  // Process fields (set all as non-master)
  $clone->fields = $survey->fields;
  foreach ($clone->fields as $key => &$field) {
    survey_builder_clone_process_field($field);
  }


  //passing query params to modules altering surveys through $form_state array
  $params     = drupal_get_query_parameters();
  $form_state['survey_params'] = $params;

  // Allow other modules to alter surveys
  drupal_alter('survey_builder_survey_save', $form, $form_state, $clone);

  // Save survey
  $status = entity_save('survey', $clone);
  if ($status != SAVED_NEW && $status != SAVED_UPDATED) {
    drupal_set_message(t('Unable to clone existing survey'), 'error');
    watchdog('survey_builder', 'Unable to save cloned survey from: !pid', array('!pid' => $pid));
    return FALSE;
  }

  // Save form to cache
  form_builder_cache_save('survey', $clone->pid, $clone->fields);

  // Purge form builder cache
  survey_builder_rebuild_cache();

  // Inform user of successful cloning
  drupal_set_message(t('Successfully cloned <em>%old_title</em> as <em>%new_title</em>',
    array('%old_title' => $survey->title, '%new_title' => $clone->title)));

  // Go to survey edit form
  drupal_goto('survey/' . $clone->pid . '/edit');

}

/**
 * Prepare a field from an existing survey form array for saving as in a clone
 */
function survey_builder_clone_process_field(&$field) {

  module_load_include('inc', 'survey_builder', 'includes/survey_builder.question');

  if (!is_array($field)) {
    return;
  }
  else if ($field['#type'] == 'fieldset') {
    // Recursively process any fields within the fieldset
    foreach ($field as $key => &$value) {
      if ($key[0] != '#') {
        survey_builder_clone_process_field($value);
      }
    }
  }
  else {
    $field['#master'] = FALSE;
  }

}

/**
 * Merge surveys page callback
 */
function survey_builder_merge() {

  module_load_include('inc', 'form_builder', 'includes/form_builder.cache');
  module_load_include('inc', 'survey_builder', 'includes/survey_builder.survey');

  $params = drupal_get_query_parameters();
  if (empty($params['surveys']) || !is_array($params['surveys'])) {
    watchdog('survey_builder', 'No survey PIDs supplied for merge');
    drupal_set_message(t('No surveys supplied for merge'), 'error');
    return array();
  }

  // Load surveys
  $pids    = array_map('intval', $params['surveys']);
  $surveys = survey_load_multiple($pids);

  // Check all surveys loaded
  if (count($surveys) != count($pids)) {
    watchdog('survey_builder', 'Unable to load all surveys for merge: !pids',
      array('!pids' => implode(', ', array_diff($pids, array_keys($surveys)))));
    drupal_set_message(t('Unable to load all surveys for merge'), 'error');
    return array();
  }

  $time   = time();
  $weight = 0;
  $fields = array();

  // Merge fields from all surveys
  foreach ($surveys as $survey) {
    foreach ($survey->fields as $field) {

      // Get unique key for this field
      do {
        $key = 'new_' . $time++;
      } while (isset($fields[$key]));

      // Update field key, set as non-master and set weight
      $field['#key']                        = $key;
      $field['#form_builder']['element_id'] = $key;
      $field['#master']                     = FALSE;
      $field['#weight']                     = $weight++;

      // Add field to merged fields
      $fields[$key] = $field;

    }
  }

  $title = '';
  // Get survey title
  if (empty($params['title'])) {
    $title = t('Merged Survey');
  }
  else {
    $title = $params['title'];
  }

  // Create new survey
  $survey         = new SurveyEntity(array('title' => $title, 'name_raw' => $title));
  $survey->type   = 'survey';
  $survey->fields = $fields;

  // Allow other modules to alter surveys
  $form       = array();
  //passing query params to modules altering surveys through $form_state array
  $form_state = array('survey_params' => $params);
  drupal_alter('survey_builder_survey_save', $form, $form_state, $survey);

  // Save survey
  $status = entity_save('survey', $survey);
  if ($status != SAVED_NEW && $status != SAVED_UPDATED) {
    watchdog('survey_builder', 'Unable to save merged survey from: !pids',
      array('!pids' => implode(', ', $pids)));
    drupal_set_message(t('Unable to save merged survey'), 'error');
    return array();
  }

  // Save form to cache
  form_builder_cache_save('survey', $survey->pid, $survey->fields);

  // Purge form builder cache
  survey_builder_rebuild_cache();

  // Set destination
  $query = array();
  if (isset($_GET['destination'])) {
    $query['destination'] = $_GET['destination'];
    unset($_GET['destination']);
  }

  // Go to survey edit form
  drupal_goto('survey/' . $survey->pid . '/edit', array('query' => $query));

}

/**
 * Generates the survey editing form.
 * This callback is invoked by entity_ui_get_form() (see SurveyUIController
 * comment)
 */
function survey_form($form, &$form_state, $survey, $op = 'edit') {
  $form = survey_builder_new_form($form, $form_state);

  if ($op == 'add') {
    $survey = entity_get_controller('survey')->create(array('type' => 'survey'));
    $form['new']['#weight'] = 40;
  }

  $form_state['survey'] = $survey;
  field_attach_form('survey', $survey, $form, $form_state);

  if ($op == 'edit') {
    $form['survey_title']['#default_value'] = $survey->title;

    unset($form['new']);

    $form['actions']           = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type'   => 'submit',
      '#value'  => t('Save survey'),
      '#weight' => 40,
    );
  }
  return $form;
}

/**
 * Form API submit callback for the admin UI create survey form.
 */
function survey_form_submit(&$form, &$form_state) {

  // Add default values to form_state
  $title = $form_state['values']['survey_title'];
  $form_state['values'] += array(
      'type' => 'survey',
      'title' => $title,
      'name_raw' => $title,
      'fields' => array(),
  );
  $form_state['values'] += $form_state['survey']->getInitialValues($form_state['values']);

  // Get survey entity and save
  $survey = entity_ui_form_submit_build_entity($form, $form_state);
  $survey->save();

  // Redirect to form_builder survey editing interface
  drupal_goto('survey/' . $survey->pid . '/edit');
}
