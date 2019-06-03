<?php

/******************************************
 *
 *       CONSTANTS
 *
 */

class OpignoMoxtraAppErrorsCodes {
  public static $organization_not_created = 4000;
}

class OpignoApiErrorCodes {
  public static $error_create_org_moxtra_servers = 'error_create_org_moxtra_servers';
  public static $error_update_org_moxtra_servers = 'error_update_org_moxtra_servers';
  public static $org_not_exists = 'org_not_exists';
  public static $org_not_valid = 'org_not_valid';
  public static $error_create_users_moxtra_servers = 'error_create_users_moxtra_servers';
  public static $error_create_users_opigno_servers = 'error_create_users_opigno_servers';
  public static $error_create_users_maximum_reached = 'error_create_users_maximum_reached';
  public static $error_update_users_moxtra_servers = 'error_update_users_moxtra_servers';
  public static $error_update_users_opigno_servers = 'error_update_users_opigno_servers';
  public static $error_get_users_moxtra_servers = 'error_get_users_moxtra_servers';
  public static $error_disable_user_moxtra_servers = 'error_disable_user_moxtra_servers';
  public static $error_disable_user_opigno_servers = 'error_disable_user_opigno_servers';
  public static $error_enable_user_moxtra_servers = 'error_enable_user_moxtra_servers';
  public static $error_enable_user_opigno_servers = 'error_enable_user_opigno_servers';
  public static $error_allow_user_create_meet_opigno_servers = 'error_allow_user_create_meet_opigno_servers';
  public static $error_allow_user_create_meet_maximum_reached = 'error_allow_user_create_meet_maximum_reached';
  public static $error_allow_user_create_meet_moxtra_servers = 'error_allow_user_create_meet_moxtra_servers';
  public static $error_deny_user_create_meet_opigno_servers = 'error_deny_user_create_meet_opigno_servers';
  public static $error_deny_user_create_meet_moxtra_servers = 'error_deny_user_create_meet_moxtra_servers';
  public static $error_get_token_moxtra_servers = 'error_get_token_moxtra_servers';
}





/******************************************
 *
 *       URI FUNCTIONS
 *
 */

/**
 * Get the uri to use to create an organization
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_create_organization()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/organization_create.json';
}

/**
 * Get the uri to use to update an organization
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_update_organization()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/organization_update.json';
}

/**
 * Get the uri to use to update an organization
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_get_max_users_organization()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/max_users_get.json';
}

/**
 * Get the uri to use to update an organization
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_get_max_users_meetings_organization()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/max_users_meetings_get.json';
}

/**
 * Get the uri to get the organization info
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_get_organization_info()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/organization_info_get.json';
}

/**
 * Get the uri to use to create some users
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_create_users()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/users_create.json';
}

/**
 * Get the uri to use to update some users
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_update_user()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/user_update.json';
}

/**
 * Get the uri to use to disable a user
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_disable_user()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/user_disable.json';
}

/**
 * Get the uri to use to delete a user.
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_delete_user()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/user_delete.json';
}

/**
 * Get the uri to use to enable a user
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_enable_user()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/user_enable.json';
}

/**
 * Get the uri to use to get the access token
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_get_token()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/token_get.json';
}

/**
 * Get the uri to use to enable a user
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_allow_user_meetings()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/allow_user_create_meeting.json';
}

/**
 * Get the uri to use to enable a user
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_uri_deny_user_meetings()
{
  return opigno_moxtra_app_api_opigno_get_gateway() .'/disallow_user_create_meeting.json';
}





/******************************************
 *
 *       API CALLS FUNCTIONS
 *
 */



////////////////////////
//
//   ORGANIZATIONS
///////////////////

/**
 * Create an organization
 *
 * @return StdClass
 */
function opigno_moxtra_app_api_opigno_create_organization()
{
  global $base_url;

  $users = entity_load('user');
  unset($users[0]); // Remove the anonymous user from the array

  // Format the users array
  $users_formated = array();
  foreach ($users as $user)
  {
    if ($user->status == 1)
      $users_formated[] = array(
        'uid' => $user->uid,
        'name' => $user->name,
        'timezone' => $user->timezone
      );
  }


  $uri = opigno_moxtra_app_api_opigno_get_uri_create_organization();

  $data = array(
    'site_name' => variable_get('site_name', 'Undefined name'),
    'base_url' => $base_url,
    'email' => variable_get('site_mail', 'No admin mail set'),
    'users' => $users_formated
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, array('response' => $response, 'data' => $data));

  variable_set('opigno_moxtra_app_org_id', $response->org_id);
  variable_set('opigno_moxtra_app_client_id', $response->client_id);

  drupal_flush_all_caches();

  return $response;
}

/**
 * Update an organization
 */
function opigno_moxtra_app_api_opigno_update_organization($site_name, $email)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_update_organization();

  $data = array(
    'org_id' => $org_id,
    'site_name' => $site_name,
    'email' => $email,
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');

  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Get max users info
 */
function opigno_moxtra_app_api_opigno_get_max_users_organization()
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_get_max_users_organization();

  $data = array(
    'org_id' => $org_id
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');

  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Get max users info for meetings
 */
function opigno_moxtra_app_api_opigno_get_max_users_meetings_organization()
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_get_max_users_meetings_organization();

  $data = array(
    'org_id' => $org_id
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');

  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Get organization info
 */
function opigno_moxtra_app_api_opigno_get_organization_info()
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_get_organization_info();

  $data = array(
    'org_id' => $org_id
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');

  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}



////////////////////////
//
//   USERS
///////////////////

/**
 * Create some users
 *
 * @param $users array An array of this format -> array(array('uid' => X, 'name' => Y, 'timezone' => Z), ...)
 * @return StdClass
 */
function opigno_moxtra_app_api_opigno_create_users($users)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_create_users();

  $data = array(
    'org_id' => $org_id,
    'users' => $users
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Update some users
 *
 * @param $user array An array of this format -> array(array('uid' => X, 'name' => Y, 'timezone' => Z), ...)
 * @return StdClass
 */
function opigno_moxtra_app_api_opigno_update_user($user)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_update_user();

  $data = array(
    'org_id' => $org_id,
    'user' => $user
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Disable a user
 *
 * @param $user_id
 * @return StdClass
 */
function opigno_moxtra_app_api_opigno_disable_user($user_id)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_disable_user();

  $data = array(
    'org_id' => $org_id,
    'user_id' => $user_id
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Delete a user.
 *
 * @param $user_id
 * @return StdClass
 */
function opigno_moxtra_app_api_opigno_delete_user($user_id)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_delete_user();

  $data = array(
    'org_id' => $org_id,
    'user_id' => $user_id
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Enable a user
 *
 * @param $user_id
 * @return StdClass
 */
function opigno_moxtra_app_api_opigno_enable_user($user)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_enable_user();

  $data = array(
    'org_id' => $org_id,
    'user' => $user
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Allow a user to create meetings
 *
 * @param $user array An array containing the keys 'uid', 'name', 'timezone'
 * @return array
 */
function opigno_moxtra_app_api_opigno_allow_user_meetings($user)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_allow_user_meetings();

  $data = array(
    'org_id' => $org_id,
    'user' => $user
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}

/**
 * Disallow a user to create meetings
 *
 * @param $user array An array containing the keys 'uid', 'name', 'timezone'
 * @return array
 */
function opigno_moxtra_app_api_opigno_deny_user_meetings($user)
{
  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '')
    return opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);


  $uri = opigno_moxtra_app_api_opigno_get_uri_deny_user_meetings();

  $data = array(
    'org_id' => $org_id,
    'user' => $user
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code))
    return opigno_moxtra_app_api_opigno_error($response->error_code, $response);

  return $response;
}



////////////////////////
//
//   TOKEN
///////////////////

/**
 * Get the access token for this user
 *
 * @param int|null $user_id If null, will use the current logged user
 * @param bool|TRUE $use_cache Use the cache or not
 * @return string The access token
 */
function opigno_moxtra_app_api_opigno_get_access_token($user_id = NULL, $use_cache = TRUE)
{
  if ($user_id == null) {
    global $user;
    $user_id = $user->uid;
  }


  $cache_id = __FUNCTION__.':'.$user_id;
  $cached_response = cache_get($cache_id);
  if($use_cache && $cached_response) return $cached_response->data->access_token;


  $org_id = variable_get('opigno_moxtra_app_org_id', '');
  if ($org_id == '') {
    $error = opigno_moxtra_app_api_opigno_error(OpignoMoxtraAppErrorsCodes::$organization_not_created);
    drupal_set_message($error->opigno_error_message);
    return false;
  }


  $uri = opigno_moxtra_app_api_opigno_get_uri_get_token();

  $data = array(
    'org_id' => $org_id,
    'user_id' => $user_id
  );

  $response = opigno_moxtra_app_api_post_json($uri, json_encode($data), 'application/json');
  if ($response->http_code != 200 || isset($response->error_code)) {
    $error = opigno_moxtra_app_api_opigno_error($response->error_code, $response);
    drupal_set_message($error->opigno_error_message, 'error');
    return false;
  }


  cache_set($cache_id, $response, 'cache', $response->expires_in);


  return $response->access_token;
}





/******************************************
 *
 *       HELPERS FUNCTIONS
 *
 */

/**
 * Get the gateway to use for each call to opigno API
 *
 * @return string
 */
function opigno_moxtra_app_api_opigno_get_gateway()
{
  return OPIGNO_MOXTRA_APP_OPIGNO_API_GATEWAY;
}

/**
 * The the error id and his message
 *
 * @param $error_id
 * @param string $error_message
 * @return stdClass
 */
function opigno_moxtra_app_api_opigno_error($error_id, $add_in_backtrace = '')
{
  $to_return = new stdClass();
  $to_return->opigno_error_code = $error_id;


  watchdog('opigno_moxtra_app opigno_api', 'Error while contacting opigno servers.<br />' .
    'Debug backtraces here <pre>'. print_r(debug_backtrace(DEBUG_BACKTRACE_PROVIDE_OBJECT, 3), true) .'</pre>', array(), WATCHDOG_WARNING);


  switch ($error_id) {
    case OpignoMoxtraAppErrorsCodes::$organization_not_created:
      $to_return->opigno_error_message = t('This Opigno instance is not registered in Opigno.org for using the Moxtra App. ' .
        'Use the Administration menu to manually register the app.');
      return $to_return;

    case OpignoApiErrorCodes::$error_create_org_moxtra_servers :
      $to_return->opigno_error_message = t('Error while creating the organization');
      return $to_return;

    case OpignoApiErrorCodes::$error_update_org_moxtra_servers :
      $to_return->opigno_error_message = t('There was an error while updating the organization on Moxtra servers');
      return $to_return;

    case OpignoApiErrorCodes::$org_not_exists :
      $to_return->opigno_error_message = t('This organisation does not exist in Opigno database');
      return $to_return;

    case OpignoApiErrorCodes::$org_not_valid :
      $to_return->opigno_error_message = t('Organization not valid');
      return $to_return;

    case OpignoApiErrorCodes::$error_create_users_moxtra_servers :
      $to_return->opigno_error_message = t('Error while creating the users on Moxtra servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_create_users_opigno_servers :
      $to_return->opigno_error_message = t('Error while creating the users on Opigno servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_create_users_maximum_reached :
      $to_return->opigno_error_message =
        t('You have reached your total number of authorized users for collaborative workspaces.') .' '.
        l(t('Manage your subscription to add more users.'), OPIGNO_MOXTRA_APP_OPIGNO_RENEW_GATEWAY. variable_get('opigno_moxtra_app_org_id'), array('attributes' => array('target' => '_blank')));
      return $to_return;

    case OpignoApiErrorCodes::$error_update_users_moxtra_servers :
      $to_return->opigno_error_message = t('Error while updating the users on Moxtra servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_update_users_opigno_servers :
      $to_return->opigno_error_message =
        t('Error while updating the users on Opigno servers. Maybe you have reached your maximum number of users for collaborative workspaces.') .' '.
        l(t('Manage your subscription to add more users.'), OPIGNO_MOXTRA_APP_OPIGNO_RENEW_GATEWAY. variable_get('opigno_moxtra_app_org_id'), array('attributes' => array('target' => '_blank')));
      return $to_return;

    case OpignoApiErrorCodes::$error_get_users_moxtra_servers :
      $to_return->opigno_error_message = t('Error while getting the user information from Moxtra servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_disable_user_moxtra_servers :
      $to_return->opigno_error_message = t('Error while disabling the user on Moxtra servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_disable_user_opigno_servers :
      $to_return->opigno_error_message = t('Error while disabling the user on Opigno servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_enable_user_moxtra_servers :
      $to_return->opigno_error_message = t('Error while enabling the user on Moxtra servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_enable_user_opigno_servers :
      $to_return->opigno_error_message = t('Error while enabling the user on Opigno servers');
      return $to_return;

    case OpignoApiErrorCodes::$error_allow_user_create_meet_opigno_servers :
      $to_return->opigno_error_message = t('Error while updating the Opigno database');
      return $to_return;

    case OpignoApiErrorCodes::$error_allow_user_create_meet_maximum_reached :
      $to_return->opigno_error_message =
        t('You have reached the maximum number of users who can create meetings.') .' '.
        l(t('Manage your subscription to add more users.'), OPIGNO_MOXTRA_APP_OPIGNO_RENEW_GATEWAY. variable_get('opigno_moxtra_app_org_id'), array('attributes' => array('target' => '_blank')));
      return $to_return;

    case OpignoApiErrorCodes::$error_allow_user_create_meet_moxtra_servers :
      $to_return->opigno_error_message = t('An error occurred on Moxtra server while allowing the user to create meeting');
      return $to_return;

    case OpignoApiErrorCodes::$error_deny_user_create_meet_opigno_servers :
      $to_return->opigno_error_message = t('Error while updating the Opigno database');
      return $to_return;

    case OpignoApiErrorCodes::$error_deny_user_create_meet_moxtra_servers :
      $to_return->opigno_error_message = t('An error occurred on Moxtra server while denying the user to create meeting');
      return $to_return;

    case OpignoApiErrorCodes::$error_get_token_moxtra_servers :
      $to_return->opigno_error_message = t('Error while getting the access token from Moxtra servers');
      return $to_return;

    default:
      $to_return->opigno_error_message = t('Moxtra error unknown. Please contact the administrator or check the log');
      return $to_return;
  }
}
