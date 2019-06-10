<?php

/**
 * @file
 * Sample hooks demonstrating usage of Raven.
 */

/**
 * Provide user information for logging.
 *
 * @param array $user_info
 *   A reference to array of user account info.
 */
function hook_raven_user_alter(array &$user_info) {
  global $user;
  if (user_is_logged_in()) {
    $user_info['id'] = $user->uid;
    $user_info['name'] = $user->name;
    $user_info['email'] = $user->mail;
    $user_info['roles'] = implode(', ', $user->roles);
  }
}

/**
 * Provide tags for logging.
 *
 * @param array $tags
 *   A reference to array of sentry tags.
 */
function hook_raven_tags_alter(array &$tags) {
  $tags['foo_version'] = get_foo_version();
}

/**
 * Provide extra information for logging.
 *
 * @param array $extra
 *   A reference to array of extra error info.
 */
function hook_raven_extra_alter(array &$extra) {
  $extra['foo'] = 'bar';
}

/**
 * Modify or suppress watchdog entries before logging them to Sentry.
 *
 * @param array $filter
 *   A reference to array containing the log entry and data to submit to Sentry.
 */
function hook_raven_watchdog_filter_alter(array &$filter) {
  // Ignore "foo" log entries.
  if ($filter['log_entry']['type'] === 'foo') {
    $filter['process'] = FALSE;
  }
  // Set the flavor tag.
  $filter['data']['tags']['flavor'] = 'strawberry';
}

/**
 * Alter the array of fields that should be sanitized.
 *
 * The field will be used in a regular expression, so you may need to run
 * preg_quote($field, '/') on the field name.
 *
 * @param array $fields
 *   A reference to array containing the field names.
 */
function hook_raven_sanitize_fields_alter(array &$fields) {
  $fields[] = 'data';
}

/**
 * Modify or suppress breadcrumbs.
 *
 * @param array $breadcrumb
 *   A reference to array containing the breadcrumb data.
 */
function hook_raven_breadcrumb_alter(array &$breadcrumb) {
  // Don't record breadcrumbs.
  $breadcrumb['process'] = FALSE;
}
