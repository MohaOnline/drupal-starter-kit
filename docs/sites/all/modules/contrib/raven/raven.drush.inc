<?php

/**
 * @file
 * Raven module drush integration.
 */

/**
 * Implements hook_drush_command().
 *
 * @see drush_parse_command()
 */
function raven_drush_command() {
  $items['raven-capture-message'] = array(
    'description' => dt('Sends a test message to Sentry.'),
    'drupal dependencies' => array('raven'),
    'arguments' => array(
      'message' => dt('The message text.'),
    ),
    'options' => array(
      'level' => dt('The message level (debug, info, warning, error, fatal).'),
      'logger' => dt('The logger.'),
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function raven_drush_help($section) {
  switch ($section) {
    case 'drush:raven-capture-message':
      return dt('Sends a test message to Sentry.') . PHP_EOL . PHP_EOL .
        dt('Because messages are sent to Sentry asynchronously, there is no guarantee that the message was actually delivered successfully.');
  }
}

/**
 * Sends a test message to Sentry.
 */
function drush_raven_capture_message($message = 'Test message from Drush.') {
  $client = raven_get_client();
  if (!$client) {
    return drush_set_error('RAVEN_CLIENT', dt('Raven client not available.'));
  }
  $level = drush_get_option('level', 'info');
  $logger = drush_get_option('logger', 'drush');
  $id = $client->captureMessage($message, array(), array('level' => $level, 'logger' => $logger));
  if (!$id) {
    return drush_set_error('RAVEN_SEND', dt('Send failed.'));
  }
  drush_log(dt('Message sent as event !id.', array('!id' => $id)), 'success');
}
