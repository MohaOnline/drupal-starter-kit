<?php
/**
 * Created by PhpStorm.
 * User: amermod
 * Date: 22/10/2015
 * Time: 14:44
 */

function opigno_live_meetings_score_page($node)
{
  return drupal_get_form('opigno_live_meetings_score_form',$node);
}

function opigno_live_meetings_score_form($form, &$form_state, $node)
{
  $live_meeting_info = opigno_live_meetings_api_get_live_meeting_info($node->uid, $node->session_key[LANGUAGE_NONE][0]['value']);

  if ($live_meeting_info->data->status != 'SESSION_ENDED')
  {
    drupal_set_message(t('The meeting has to be ended in order to save the presences.'), 'error');
    return $form;
  }

  $participants_ids = array_map(function($participant) {
    return $participant->unique_id;
  }, $live_meeting_info->data->participants);

  $users = opigno_get_users_in_group($node->og_group_ref[LANGUAGE_NONE][0]['target_id']);

  $defaultValues = opigno_live_meetings_score_get_default_value($node->nid, $users, $participants_ids);



  $form['submit_scores'] = array(
    '#type' => 'fieldset',
    '#title' => t('Participants for') .' '. $node->title,
    '#collapsible' => FALSE,
    '#collapsed' => TRUE,
    '#prefix' => '<table><thead><tr><th>' . t("Name") . '</th><th>' . t("Attendance") . '</th><th>' . t("Score") . '</th></tr></thead><tbody>',
    '#suffix' => '</tbody></table>',
  );


  foreach ($users as $user)
  {
    $form['submit_scores']['name_' . $user->uid] = array(
      '#type' => 'item',
      '#title' => $user->name,
      '#prefix' => '<tr><td>',
      '#suffix' => '</td>',
    );
    $form['submit_scores']['status_' . $user->uid] = array(
      '#type' => 'select',
      '#options' => array(
        0 => t('Absent'),
        1 => t('Attended'),
      ),
      '#default_value' => $defaultValues[$user->uid]['status'],
      '#prefix' => '<td>',
      '#suffix' => '</td>',
    );
    $form['submit_scores']['score_' . $user->uid] = array(
      '#type' => 'textfield',
      '#size' => '10',
      '#required' => FALSE,
      '#prefix' => '<td>',
      '#suffix' => '</td></tr>',
      '#default_value' => $defaultValues[$user->uid]['score'],
    );
  }


  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save attendances'),
    '#submit' => array('opigno_live_meetings_score_form_submit'),
  );

  return $form;
}

function opigno_live_meetings_score_form_validate($form, &$form_state)
{
  foreach($form_state['values'] as $key => $value)
  {
    if (strpos($key, 'score_') === 0)
    {
      if ((!is_numeric($value)) || ($value < 0) || ($value > 100))
        form_set_error($key, t('Score must be integer (0-100)'));
    }
  }
}

function opigno_live_meetings_score_form_submit($form, &$form_state)
{
  foreach($form_state['values'] as $key => $score) {
    if (strpos($key, 'score_') === 0) {
      $uid = explode('score_', $key)[1];
      opigno_live_meetings_score_form_set_value($form_state['build_info']['args'][0]->nid, $uid, $score, $form_state['values']['status_' . $uid]);
      opigno_live_meetings_int_finished($form_state['build_info']['args'][0]->nid, $score, $form_state['values']['status_' . $uid], $uid);
    }
  }
  drupal_set_message(t('Presences saved successfully'), 'status');
}

function opigno_live_meetings_score_form_set_value($live_meeting_nid, $uid, $score, $status) {
  db_merge('opigno_live_meetings_results')
    ->key(array('live_meeting_nid' => $live_meeting_nid, 'uid' => $uid))
    ->fields(array(
      'live_meeting_nid' => $live_meeting_nid,
      'score' => $score,
      'status' => $status,
      'uid' => $uid,
    ))
    ->execute();
}

function opigno_live_meetings_score_get_default_value($live_meeting_nid, $users, $participantsIds) {

  $rowsQuery = db_select('opigno_live_meetings_results', 'm')
    ->fields('m')
    ->condition('live_meeting_nid', $live_meeting_nid, '=')
    ->execute();

  $defaultValues = array();

  foreach($users as $user)
  {
    $defaultValues[$user->uid]['score'] = 100;
    $defaultValues[$user->uid]['status'] = (in_array($user->uid, $participantsIds) ? 1 : 0);
  }

  if ($rowsQuery->rowCount() > 0)
  {
    while($row = $rowsQuery->fetchAssoc())
    {
      $defaultValues[ $row['uid'] ]['score'] = $row['score'];
      $defaultValues[ $row['uid'] ]['status'] = $row['status'];
    }
  }

  return $defaultValues;
}

function opigno_live_meetings_score_get_db_values($live_meeting_nid, $uid)
{
  $rowsQuery = db_select('opigno_live_meetings_results', 'm')
    ->fields('m')
    ->condition('live_meeting_nid', $live_meeting_nid, '=')
    ->condition('uid', $uid, '=')
    ->execute();

  if ($rowsQuery->rowCount() > 0)
    while($row = $rowsQuery->fetchAssoc())
      return ($row);

  return null;
}

function opigno_live_meetings_score_is_registered_db($live_meeting_nid)
{
  $nbrEntries = db_select('opigno_live_meetings_results', 'm')
    ->fields('m')
    ->condition('live_meeting_nid', $live_meeting_nid, '=')
    ->execute()->rowCount();
  return ($nbrEntries > 0 ? true : false);
}

function opigno_live_meetings_get_participants_uid_from_meeting_info($live_meeting_info)
{
  return array_map(function($participant) {
    return $participant->unique_id;
  }, $live_meeting_info->data->participants);
}


function opigno_live_meetings_int_finished($live_meeting_nid, $score, $status, $taker = NULL)
{
  if (module_exists('rules')) {
    $taker = user_load($taker);
    $live_meeting = node_load($live_meeting_nid);
    $author = user_load($live_meeting->uid);
    if ((int)$status == 1)
    {
      $hook_name = 'opigno_meeting_finished';
      foreach (module_implements($hook_name) as $module) {
        $function = $module.'_'.$hook_name;
        $function($taker, $live_meeting_nid, $score);
      }

      foreach($live_meeting->og_group_ref[LANGUAGE_NONE] as $index => $value)
      {
        if (opigno_quiz_app_user_passed($value['target_id'], $taker->uid))
        {
          rules_invoke_event('opigno_quiz_app_rules_quiz_passed', $taker, $author, $live_meeting);
          drupal_get_messages();
        }
      }
    }
  }
}
