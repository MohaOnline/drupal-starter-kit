<?php

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Alter create/edit form to
 * - Hide "session_key" field
 * - Register new submit handlers to create or edit a meeting on moxtra
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup forms
 */
function opigno_live_meetings_form_node_form_alter(&$form, &$form_state, $form_id) {
  $content_type = $form['#node']->type;
  if($content_type === 'live_meeting')
  {
    if(isset($form_state['node']->nid))
    {
      //update
      $form['#submit'][] = 'opigno_live_meetings_form_update_live_meeting_submit';
      $form['meeting_dates']['#access'] = FALSE;
    }
    else
    {
      //create
      $form['#submit'][] = 'opigno_live_meetings_form_create_live_meeting_submit';
      $form['opigno_calendar_date']['#access'] = FALSE;
    }

    $form['schedule_binder_id']['#access'] = FALSE;
    $form['session_key']['#access'] = FALSE;
    $form['actions']['preview']['#access'] = FALSE;

    //validate
    $form['#validate'][] = 'opigno_live_meetings_form_live_meeting_validate';

    // If the course ID is set (should always be!), show the classes that the user can access and
    //   that have this course as child
    if (isset($form['og_group_ref']['und'][0]['default']['#default_value'][0]))
    {
      $course_id = $form['og_group_ref']['und'][0]['default']['#default_value'][0];
      $classes_ids = opigno_live_meetings_db_get_classes_nids_from_course($course_id);
      foreach($form['field_class_ref']['und'][0]['default']['#options'] as $op_index => $option)
        if ($op_index != '_none' && !in_array($op_index, $classes_ids))
          unset($form['field_class_ref']['und'][0]['default']['#options'][$op_index]);
    }
  }
}

/**
 * Form validate handler for the live meeting node form (same validate handler for add and update form).
 *
 * @param array $form
 * @param array $form_state
 */
function opigno_live_meetings_form_live_meeting_validate($form, &$form_state)
{
  // Verify if the schedule date is later than now
  $raw_start_date = $form_state['values']['meeting_dates']['und'][0]['value'];
  $raw_end_date = $form_state['values']['meeting_dates']['und'][0]['value2'];

  $start_date = DateTime::createFromFormat('Y-m-d H:i:s', $raw_start_date);
  $end_date = DateTime::createFromFormat('Y-m-d H:i:s', $raw_end_date);
  $now_date = new DateTime();

  if ($start_date < $now_date || $end_date < $now_date)
    form_set_error('meeting_dates', t('The schedule date should be later than the current date'));
}

/**
 * Form submit handler for the moxtra video conference node add form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup forms
 */
function opigno_live_meetings_form_create_live_meeting_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $title = $form_values['title'];

  //Create new moxtra video conference node for all the dates and schedule the meeting
  foreach($form_values['meeting_dates'][LANGUAGE_NONE] as $index => $meeting_date)
  {
    //Drupal add "add_more" key in array for whatever reason => skip string keys
    if(!is_numeric($index)) continue;

    //Generate random unique_id (used to have access permissions on the meeting)
    $unique_id = $form['#node']->uid;

    $start_time = opigno_live_meetings_form_format_date_value_to_iso8601($meeting_date['value'], $meeting_date['timezone']);
    if ($meeting_date['value'] <= $meeting_date['value2'])
      $end_time = '';
    else
      $end_time = opigno_live_meetings_form_format_date_value_to_iso8601($meeting_date['value2'], $meeting_date['timezone']);

    $response = opigno_live_meetings_api_create_live_meeting($unique_id, $title, $start_time, $end_time);
    $session_key = $response->data->session_key;
    $schedule_binder_id = $response->data->schedule_binder_id;

    if($index == 0)
    {
      //Use current form to create the first node with first date
      $form_state['values']['opigno_calendar_date'][LANGUAGE_NONE][0] = $meeting_date;
      $form_state['values']['schedule_binder_id'][LANGUAGE_NONE][0]['value'] = $schedule_binder_id;
      $form_state['values']['session_key'][LANGUAGE_NONE][0]['value'] = $session_key;
    }
    else
    {
      //Create new nodes for all following dates
      $values = array(
        'title' => $form_values['title'],
        'type' => 'live_meeting',
        'uid' => $unique_id,
        'status' => 1,
        'comment' => 1,
        'promote' => 0,
      );
      $entity = entity_create('node', $values);
      $wrapper = entity_metadata_wrapper('node', $entity);
      $wrapper->unique_id->set($unique_id);
      $wrapper->schedule_binder_id->set($schedule_binder_id);
      $wrapper->session_key->set($session_key);
      $wrapper->opigno_calendar_date->set($meeting_date);
      $wrapper->save();
    }
  }
}

/**
 * Format date value to ISO-8601 format "YYYY-MM-DDThh:mm:ssZ
 *
 * @param string $value
 * @return string
 */
function opigno_live_meetings_form_format_date_value_to_iso8601($value, $timezone_string)
{
  return DateTime::createFromFormat('Y-m-d H:i:s', $value, new DateTimeZone($timezone_string))
      ->setTimezone(new DateTimeZone('UTC'))
      ->format('Y-m-d\TH:i:s\Z');
}



/**
 * Form submit handler for the moxtra video conference node edit form.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup forms
 */
function opigno_live_meetings_form_update_live_meeting_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $node = $form_state['node'];
  $unique_id = $form_state['node']->uid;
  $session_key = $form_values['session_key'][LANGUAGE_NONE][0]['value'];

  //Diff
  $old_title = $node->title;
  $old_opigno_calendar_date = $node->opigno_calendar_date[LANGUAGE_NONE][0];

  $new_title = $form_values['title'];
  $new_opigno_calendar_date = $form_values['opigno_calendar_date'][LANGUAGE_NONE][0];

  $title_has_changed = $new_title != $old_title;
  $opigno_calendar_date_start_has_changed = $new_opigno_calendar_date['value'] != $old_opigno_calendar_date['value'];
  $opigno_calendar_date_end_has_changed = $new_opigno_calendar_date['value2'] != $old_opigno_calendar_date['value2'];

  if($title_has_changed || $opigno_calendar_date_start_has_changed || $opigno_calendar_date_end_has_changed)
  {
    $starts = opigno_live_meetings_form_format_date_value_to_iso8601($new_opigno_calendar_date['value'], $new_opigno_calendar_date['timezone']);
    if ($new_opigno_calendar_date['value'] == $new_opigno_calendar_date['value2'])
      $ends = '';
    else
      $ends = opigno_live_meetings_form_format_date_value_to_iso8601($new_opigno_calendar_date['value2'], $new_opigno_calendar_date['timezone']);

    //Send request to update modification on moxtra
    opigno_live_meetings_api_update_live_meeting($unique_id, $session_key, $new_title, $starts, $ends);
  }
}