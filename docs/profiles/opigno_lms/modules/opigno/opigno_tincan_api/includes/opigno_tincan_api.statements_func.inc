<?php


/**
 * Add the current user as the actor of this statement
 *
 * @param \TinCan\Statement $statement The statement to add the actor
 */
function _opigno_tincan_api_set_actor(TinCan\Statement &$statement, $user = null)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  if ($user === null)
    global $user;
  else if (empty($user) || empty($user->name) || empty($user->mail))
    throw new Exception('The user given was not loaded');

  $actor = new TinCan\Agent(array(
    'name' => $user->name,
    'mbox_sha1sum' => sha1('mailto:' . $user->mail)
  ));

  $statement->setActor($actor);
}


/**
 * @param \TinCan\Statement $statement The statement to add the verb
 * @param array(name, id) $verb An array containing the verb name and id (url). Use TinCanVerbs.
 *
 * @throws Exception Throw an Exception if $verb is not conform
 */
function _opigno_tincan_api_set_verb(TinCan\Statement &$statement, $verb)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  if (empty($verb) || empty($verb['name']) || empty($verb['id']))
    throw new Exception('The verb given does not exist');

  $verb = new TinCan\Verb(array(
    'id' => $verb['id'],
    'display' => array(
      'en-US' => $verb['name']
    )
  ));

  $statement->setVerb($verb);
}


/**
 * @param \TinCan\Statement $statement The statement to add the object
 * @param string $activity_definition_type An array containing the activity name and type (url). Use TinCanActivityDefinitions
 * @param object $activity_id The node to use as object activity
 *
 * @throws Exception
 */
function _opigno_tincan_api_set_object_activity(TinCan\Statement &$statement, $activity_definition_type, $node)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  if (empty($activity_definition_type))
    throw new Exception('The definition type given is empty.');

  if (empty($node) || empty($node->nid) || empty($node->title)) {
    watchdog('opigno_tincan_api', 'The node given is not loaded or is not a node: <pre>'. print_r($node, true) .'</pre>');
    return false;
  }

  $object = new TinCan\Activity(array(
    'id' => url('node/' . $node->nid, array('absolute' => true)),
    'definition' => array(
      'name' => array(
        'en-US' => $node->title
      ),
      'type' => $activity_definition_type
    )
  ));

  $statement->setObject($object);
}


/**
 * @param \TinCan\Context $context The context to edit for the statement
 * @param string $language The language to add
 */
function _opigno_tincan_api_context_set_language(TinCan\Context &$context, $language)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  if (!empty($language) && $language != LANGUAGE_NONE)
    $context->setLanguage($language);
}


/**
 * @param \TinCan\Context $context The context to add the parents
 * @param array() $parents_nids The nodes IDs to add as parents
 */
function _opigno_tincan_api_context_set_parents(TinCan\Context &$context, $parents_nids, $definition_type = NULL)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  $parents = array();
  foreach ($parents_nids as $parent_nid) {
    $parent = array();
    $parent['id'] = url('node/' . $parent_nid, array('absolute' => true));

    if (!empty($definition_type)) {
      $parent['definition'] = array('type' => $definition_type);
    }

    $parents[] = $parent;
  }

  if (!empty($parents))
    $context->getContextActivities()->setParent($parents);
}


function _opigno_tincan_api_context_set_grouping(TinCan\Context &$context, $grouping_nids, $definition_type = NULL)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  $grouping = array();
  foreach ($grouping_nids as $nid) {
    $statement_group = array();
    $statement_group['id'] = url('node/' . $nid, array('absolute' => true));

    if (!empty($definition_type)) {
      $statement_group['definition'] = array('type' => $definition_type);
    }

    $grouping[] = $statement_group;
  }

  if (!empty($grouping))
    $context->getContextActivities()->setGrouping($grouping);
}


function _opigno_tincan_api_set_result(TinCan\Statement &$statement, $user_score = null, $score_max = null,
                                       $is_success = null, $response = null, $duration_s = null)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  $result = new TinCan\Result();

  if ($user_score !== null)
    _opigno_tincan_api_set_score($result, $user_score, $score_max);

  if ($is_success !== null)
    $result->setSuccess($is_success);

  $result->setCompletion(true);

  if ($response !== null)
    $result->setResponse($response);

  if ($duration_s !== null) {
    $time_now = new DateTime();
    $time_more = new DateTime();

    $time_more->add(new DateInterval('PT' . (int)($duration_s) . 'S'));
    $time = $time_now->diff($time_more);

    // Remove all the 0 in the formatted duration
    $duration_string = $time->format('P%yY%mM%dDT%hH%iM%sS');
    $duration_string = preg_replace('/(\D)0{1}\D/i', '$1', $duration_string);
    ($duration_string == 'PT' ? $duration_string = 'PT0S' : null);

    $result->setDuration($duration_string);
  }

  $statement->setResult($result);
}


/**
 * @param \TinCan\Statement $statement The statement to send.
 *
 * The variables 'opigno_tincan_api_*' will be used to send the statement to the LRS
 */
function _opigno_tincan_api_send_statement(TinCan\Statement $statement)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  $endpoint = variable_get('opigno_tincan_api_endpoint', '');
  $username = variable_get('opigno_tincan_api_username', '');
  $password = variable_get('opigno_tincan_api_password', '');

  if ( empty($endpoint) || empty($username) || empty($password) ) {
    return false;
  }

  $lrs = new TinCan\RemoteLRS(
    $endpoint,
    '1.0.1',
    $username,
    $password
  );
  $response = $lrs->saveStatement($statement);

  if ($response->success === false) {
    watchdog(
      'Opigno Tincan API',
      'The following statement could not be sent :<br />' .
      '<pre>' . print_r($statement->asVersion('1.0.1'), true) . '</pre><br/>' .
      'The response :<br />' .
      '<pre>' . print_r(json_decode($response->content), true) . '</pre>',
      array(),
      WATCHDOG_ERROR);
    return false;
  }

  return true;
}


/**
 * This function will add the current user as the actor, the verb selected in parameter, the object based on the
 *   node given in parameter and the timestamp based on the current time.
 *
 * @param array(name, id) $verb The verb name and ID. Use TinCanVerbs
 * @param string $activity_definition_type The activity definition name and ID. Use TinCanActivityDefinitionTypes
 * @param object $node The node to use as object
 *
 * @return \TinCan\Statement
 * @throws Exception
 */
function _opigno_tincan_api_statement_base_creation($verb, $activity_definition_type, $node)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  $statement = new TinCan\Statement();

  _opigno_tincan_api_set_actor($statement);
  _opigno_tincan_api_set_verb($statement, $verb);
  _opigno_tincan_api_set_object_activity($statement, $activity_definition_type, $node);
  $statement->stamp();

  return $statement;
}


function _opigno_tincan_api_set_score(TinCan\Result &$result, $raw_score, $max_score = null, $min_score = null)
{
  if (!_opigno_tincan_api_tincanphp_is_installed()) {
    return false;
  }

  $score = array();

  if ($min_score === null) {
    $min_score = 0;
  }


  if ($max_score === null || ($max_score === 0 && $min_score === 0)) {
    $score['raw'] = $raw_score;
    $result->setScore($score);
    return false;
  }


  if ($max_score <= $min_score) {
    $max_score = $min_score+1; // Max must be greater than min
  }

  $scaled_max = $max_score - $min_score;
  $scaled_raw = $raw_score - $min_score;

  $scaled = ($scaled_max > 0 ? round($scaled_raw / $scaled_max, 2) : 0);

  $result->setScore(array(
    'min' => $min_score,
    'max' => $max_score,
    'raw' => $raw_score,
    'scaled' => ($scaled < -1 ? -1 : ($scaled > 1 ? 1 : $scaled)) // Compris entre -1 et 1
  ));

  return true;
}
