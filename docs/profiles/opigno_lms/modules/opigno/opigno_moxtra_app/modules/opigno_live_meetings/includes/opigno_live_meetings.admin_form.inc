<?php

function opigno_live_meetings_form_opigno_moxtra_app_moxtra_configuration_form_validate(&$form, &$form_state)
{
  if($form_state['triggering_element']['#name'] == 'add_user' && !empty($form_state['values']['admin_users_add']))
  {
    $user_to_add = user_load_by_name($form_state['values']['admin_users_add']);
    if ($user_to_add == false) {
      form_set_error('', t('The user does not exists'));
      return;
    }

    $exists = db_query('SELECT 1 FROM opigno_live_meetings_users_can_start_meetings WHERE user_id = :uid',
      array(':uid' => $user_to_add->uid))
      ->fetchField();

    if ($exists) {
      form_set_error('', t('This user can already create/start live meetings'));
      return;
    }

    $response = opigno_moxtra_app_api_opigno_allow_user_meetings(array(
      'uid' => $user_to_add->uid,
      'name' => $user_to_add->name,
      'timezone' => $user_to_add->timezone
    ));

    if (isset($response->opigno_error_message)) {
      form_set_error('', $response->opigno_error_message);
      return;
    }

    db_insert('opigno_live_meetings_users_can_start_meetings')
      ->fields(array('user_id' => $user_to_add->uid))->execute();
  }


  else if (strpos($form_state['triggering_element']['#name'], 'revoke_') === 0)
  {
    $user_to_remove = user_load(substr($form_state['triggering_element']['#name'], 7));

    $response = opigno_moxtra_app_api_opigno_deny_user_meetings(array(
      'uid' => $user_to_remove->uid,
      'name' => $user_to_remove->name,
      'timezone' => $user_to_remove->timezone
    ));

    if (isset($response->opigno_error_message)) {
      form_set_error('', $response->opigno_error_message);
      return;
    }

    db_delete('opigno_live_meetings_users_can_start_meetings')
      ->condition('user_id', $user_to_remove->uid, '=')->execute();
  }
}
