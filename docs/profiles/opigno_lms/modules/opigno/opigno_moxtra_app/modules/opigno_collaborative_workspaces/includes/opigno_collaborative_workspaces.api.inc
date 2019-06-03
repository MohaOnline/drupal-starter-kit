<?php

/////////////////////
// GET URIs
/////////////////////////////

function opigno_collaborative_workspaces_get_create_collaborative_workspace_uri($uid)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/me/binders?access_token='. $accessToken;
}

function opigno_collaborative_workspaces_get_update_collaborative_workspace_uri($uid, $collab_workspace_id)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $collab_workspace_id .'?access_token='. $accessToken;
}

function opigno_collaborative_workspaces_get_add_users_uri($uid, $collab_workspace_id)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $collab_workspace_id .'/addteamuser?access_token='. $accessToken;
}

function opigno_collaborative_workspaces_get_invite_users_uri($uid, $collab_workspace_id)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $collab_workspace_id .'/inviteuser?access_token='. $accessToken;
}

function opigno_collaborative_workspaces_get_accept_invite($uid, $collab_workspace_id)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $collab_workspace_id .'/acceptinvite?access_token='. $accessToken;
}

function opigno_collaborative_workspaces_get_remove_user_uri($uid, $collab_workspace_id)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $collab_workspace_id .'/removeuser?access_token='. $accessToken;
}

function opigno_collaborative_workspaces_get_send_message_uri($uid, $collab_workspace_id)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $collab_workspace_id .'/comments?access_token='. $accessToken;
}

function opigno_collaborative_workspaces_get_delete_collaborative_workspace_uri($uid, $collab_workspace_id)
{
  $accessToken = opigno_moxtra_app_api_opigno_get_access_token($uid);
  return opigno_moxtra_app_api_get_gateway_uri() .'/v1/'. $collab_workspace_id. '?access_token='. $accessToken;
}



/////////////////////
// GET / POST CONTENTS
////////////////////////////////

function opigno_collaborative_workspaces_create_collaborative_workspace($uid, $collab_workspace_name, $auto_handle_response = true)
{
  $url = opigno_collaborative_workspaces_get_create_collaborative_workspace_uri($uid);
  $data = array(
    'name' => $collab_workspace_name,
    'restricted' => true,
    'conversation' => true
  );

  $response = opigno_moxtra_app_api_post_json($url, json_encode($data), 'application/json');
  if ($auto_handle_response)
    return opigno_collaborative_workspaces_handle_response_errors($response, __FUNCTION__);
  else
    return $response;
}

function opigno_collaborative_workspaces_update_collaborative_workspace($uid, $collab_workspace_id, $new_collab_workspace_name, $auto_handle_response = true)
{
  $url = opigno_collaborative_workspaces_get_update_collaborative_workspace_uri($uid, $collab_workspace_id);
  $data = array(
    'name' => $new_collab_workspace_name
  );

  $response = opigno_moxtra_app_api_post_json($url, json_encode($data), 'application/json');
  if ($auto_handle_response)
    return opigno_collaborative_workspaces_handle_response_errors($response, __FUNCTION__);
  else
    return $response;
}

function opigno_collaborative_workspaces_add_users($collab_workspace_uid, $collab_workspace_id, $users_ids, $auto_handle_response = true)
{
  global $user;

  $url = opigno_collaborative_workspaces_get_add_users_uri($collab_workspace_uid, $collab_workspace_id);
  $usersJsonReady = array();
  foreach($users_ids as $user_id) {
    $usersJsonReady[] = array('user' => array('unique_id' => $user_id));
  }

  $data = array(
    'users' => $usersJsonReady,
    'suppress_feed' => true
  );

  $response = opigno_moxtra_app_api_post_json($url, json_encode($data), 'application/json');

  // Workaround. If the response is "all invitees are already members" (409), change the response to "Success"
  if ($response->http_code == 409)
    $response->http_code = 200;

  if ($auto_handle_response)
    $response = opigno_collaborative_workspaces_handle_response_errors($response, __FUNCTION__);

  // If everything is okay, add the informal texts in the workspace
  if ($response->http_code == 200)
    foreach($users_ids as $user_id)
      opigno_collaborative_workspaces_send_message(
        $collab_workspace_id,
        $user->name .t(' invited '). user_load($user_id)->name .t(' to join this conversation.'),
        $collab_workspace_uid
      );

  return $response;
}

function opigno_collaborative_workspaces_remove_user($collab_workspace_uid, $collab_workspace_id, $uid_to_remove, $auto_handle_response = true)
{
  global $user;

  $url = opigno_collaborative_workspaces_get_remove_user_uri($collab_workspace_uid, $collab_workspace_id);
  $data = array(
    'unique_id' => $uid_to_remove,
    'suppress_feed' => true
  );

  $response = opigno_moxtra_app_api_post_json($url, json_encode($data), 'application/json');

  // Workaround. If the response is "User not found in member list." (404), change the response to "Success"
  if ($response->http_code == 404 && $response->message == "User not found in member list.")
    $response->http_code = 200;

  if ($auto_handle_response)
    $response = opigno_collaborative_workspaces_handle_response_errors($response, __FUNCTION__);


  if ($response->http_code == 200)
    opigno_collaborative_workspaces_send_message(
      $collab_workspace_id,
      $user->name .t(' removed '). user_load($uid_to_remove)->name .t(' from this conversation.'),
      $collab_workspace_uid
    );

  return $response;
}

function opigno_collaborative_workspaces_delete_collaborative_workspace($collab_workspace_uid, $collab_workspace_id, $auto_handle_response = true)
{
  $url = opigno_collaborative_workspaces_get_delete_collaborative_workspace_uri($collab_workspace_uid, $collab_workspace_id);

  $response = opigno_moxtra_app_api_delete_json($url);
  if ($auto_handle_response)
    return opigno_collaborative_workspaces_handle_response_errors($response, __FUNCTION__);
  else
    return $response;
}

function opigno_collaborative_workspaces_send_message($collab_workspace_id, $message, $uid, $auto_handle_response = true)
{
  $url = opigno_collaborative_workspaces_get_send_message_uri($uid, $collab_workspace_id);
  $data = array(
    'text' => $message
  );

  $response = opigno_moxtra_app_api_post_json($url, json_encode($data), 'application/json');

  if ($auto_handle_response)
    return opigno_collaborative_workspaces_handle_response_errors($response, __FUNCTION__);
  else
    return $response;
}



///////////////////
// HANDLE FUNCTIONS
////////////////////////////////////

function opigno_collaborative_workspaces_handle_response_errors($response, $function_name)
{
  if ($response == NULL || $response->http_code != 200)
  {
    watchdog(
      'Collaborative workspace',
      $function_name .': Error while contacting the Moxtra server. <br />$response: <pre>'. print_r($response, TRUE) .'</pre>',
      array(),
      WATCHDOG_ERROR
    );
    if ($response->message == 'cann\'t expel owner')
      form_set_error('', t('You cannot remove the owner of the collaborative workspace.'));
    else
      form_set_error('', t('Error while contacting the Moxtra server. Try again or contact the administrator.'));
    return $response;
  }

  return $response;
}
