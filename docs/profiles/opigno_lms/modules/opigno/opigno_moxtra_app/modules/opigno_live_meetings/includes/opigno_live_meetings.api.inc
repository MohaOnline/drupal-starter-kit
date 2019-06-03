<?php

/////////////
// GET ALL URIs
/////////////////////////

function opigno_live_meetings_api_get_live_meeting_info_uri($uid, $session_key)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/meets/'. $session_key .'?access_token='. $access_token;
}

function opigno_live_meetings_api_get_create_live_meeting_uri($uid)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/meets/schedule?access_token='. $access_token;
}

function opigno_live_meetings_api_get_update_live_meeting_uri($uid, $session_key)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/meets/'. $session_key .'?access_token='. $access_token;
}

function opigno_live_meetings_api_get_delete_live_meeting_uri($uid, $session_key)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/meets/'. $session_key .'?access_token='. $access_token;
}

function opigno_live_meetings_api_get_files_list_uri($uid, $mox_binder_id)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $mox_binder_id .'/files?access_token='. $access_token;
}

function opigno_live_meetings_api_get_file_info_uri($uid, $mox_binder_id, $file_id)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $mox_binder_id .'/files/'. $file_id .'/viewonlylink?access_token='. $access_token;
}

function opigno_live_meetings_api_get_recording_info_uri($uid, $mox_binder_id)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/meets/recordings/'. $mox_binder_id .'?access_token='. $access_token;
}

function opigno_live_meetings_api_get_live_meeting_status_uri($uid, $session_key)
{
  $access_token = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/meets/status/'. $session_key .'?access_token='. $access_token;
}



////////////
// ACCESS API
//////////////////////////////

function opigno_live_meetings_api_get_live_meeting_info($uid, $session_key)
{
  $uri = opigno_live_meetings_api_get_live_meeting_info_uri($uid, $session_key);
  return opigno_moxtra_app_api_get_json($uri);
}

function opigno_live_meetings_api_create_live_meeting($uid, $title, $start_timestamp, $end_timestamp)
{
  $uri = opigno_live_meetings_api_get_create_live_meeting_uri($uid);
  $data = array(
    'name' => $title,
    'starts' => $start_timestamp,
    ) +
    (empty($end_timestamp) ? array() : array('ends' => $end_timestamp));

  return opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
}

function opigno_live_meetings_api_update_live_meeting($uid, $session_key, $title, $start_timestamp, $end_timestamp)
{
  $uri = opigno_live_meetings_api_get_update_live_meeting_uri($uid, $session_key);
  $data = array(
    'name' => $title,
    'starts' => $start_timestamp,
  ) +
    (empty($end_timestamp) ? array() : array('ends' => $end_timestamp));

  return opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
}

function opigno_live_meetings_api_delete_live_meeting($uid, $session_key)
{
  $uri = opigno_live_meetings_api_get_delete_live_meeting_uri($uid, $session_key);
  return opigno_moxtra_app_api_delete_json($uri);
}

function opigno_live_meetings_api_get_live_meeting_status($uid, $session_key)
{
  $uri = opigno_live_meetings_api_get_live_meeting_status_uri($uid, $session_key);
  return opigno_moxtra_app_api_get_json($uri);
}

function opigno_live_meetings_api_get_files_list($uid, $mox_binder_id)
{
  $uri = opigno_live_meetings_api_get_files_list_uri($uid, $mox_binder_id);
  return opigno_moxtra_app_api_get_json($uri);
}

function opigno_live_meetings_api_get_file_info($uid, $mox_binder_id, $file_id)
{
  $uri = opigno_live_meetings_api_get_file_info_uri($uid, $mox_binder_id, $file_id);
  return opigno_moxtra_app_api_get_json($uri);
}

function opigno_live_meetings_api_get_recording_info($uid, $mox_binder_id)
{
  $uri = opigno_live_meetings_api_get_recording_info_uri($uid, $mox_binder_id);
  return opigno_moxtra_app_api_get_json($uri);
}
