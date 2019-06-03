<?php

/**
 * @file
 * Implements functions to query statements stored on Learning Record Store server
 */

/**
 * Get query end point using tincanapi "tincanapi_endpoint" setting
 */
function opigno_lrs_stats_end_point() {
  $end_point = &drupal_static(__FUNCTION__);
  if(!isset($end_point))
  {
    $url_parts = parse_url(variable_get('opigno_tincan_api_endpoint'));
    $end_point = "{$url_parts['scheme']}://{$url_parts['host']}/api/v1/statements/aggregate";
  }
  return $end_point;
}

/**
 * Generate basic auth header for the request
 *
 * @param string $username
 *   Learning Record Store username to use
 * @param string $password
 *   Learning Record Store password for the user
 *
 * @return array
 *   A structured header option array.
 */
function opigno_lrs_stats_query_request_basic_auth_header($username, $password) {
  return array(
    'Authorization' => 'Basic '. base64_encode("{$username}:{$password}")
  );
}

/**
 * Query Learning Record Store server using mongodb aggregate structure
 *
 * @param array $pipeline
 *   A structured aggregate query (http://docs.mongodb.org/manual/core/aggregation-pipeline)
 *
 * @return array
 *   Array of statements.
 */
function opigno_lrs_stats_query($pipeline) {
  $statements = array();

  //Define request options
  $options = array(
    'headers' => opigno_lrs_stats_query_request_basic_auth_header(
      variable_get('opigno_tincan_api_username'),
      variable_get('opigno_tincan_api_password')
    ),
    'data' => drupal_http_build_query(array(
      'pipeline' => json_encode($pipeline)
    ))
  );

  //Send request
  $response = drupal_http_request(opigno_lrs_stats_end_point(), $options);

  //Process response
  if(isset($response->error))
  {
    drupal_set_message("An error occured while requesting Learning Record Store data ({$response->error})", 'error');
  }
  else
  {
    $statements = array_map('opigno_lrs_stats_query_record_map_statement', json_decode($response->data)->result);
    $statements = opigno_lrs_stats_statement_mutate_timestamp($statements);
  }

  return $statements;
}

/**
 * Used to retrieve statements inside of an array of results
 *
 * @param stdClass $result
 *   Result item extracted from the response
 *
 * @return stdClass
 *   Statement object
 */
function opigno_lrs_stats_query_record_map_statement($result) {
  return $result->statement;
}

/**
 * Used to mutate statements timestamp to PHP dateTime objects
 *
 * @param stdClass $statements
 *
 * @return array
 *   Statements
 */
function opigno_lrs_stats_statement_mutate_timestamp($statements) {
  foreach($statements as $statement)
    $statement->timestamp = new DateTime($statement->timestamp);

  return $statements;
}