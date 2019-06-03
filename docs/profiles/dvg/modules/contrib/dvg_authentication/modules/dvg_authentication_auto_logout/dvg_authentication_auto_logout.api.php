<?php

/**
 * @file
 * Hooks provided by DvG Authentication Auto Logout.
 */

/**
 * Allows modules to provide custom profiles for auto logout.
 *
 * Each profiles can have custom timings and a callback to determine
 * which profile a user belongs to.
 *
 * @return array
 *   One or more profiles.
 *   Key as name of the profile.
 *   Value an array:
 *   - 'title'
 *     Display title for this profile in the admin interface.
 *   - 'callback'
 *     A callback that returns TRUE if a user should use that auto logout
 *     profile.
 *     The callback gets passed the user object and the data value if available.
 *     The first profile (order determined by module_invoke_all) that returns
 *     TRUE will be used for a user.
 *   - 'data' (optional)
 *     When set will be passed as third parameter to the callback.
 *   - 'login_message_callback' (optional)
 *     Callback to set a custom login message in the auto logout status bar.
 *     Gets passed the same parameters as the normal callback.
 *   - 'logout_open_blank' (default FALSE)
 *     If TRUE, then the logout link is rendered with target="_blank".
 *     This can be useful when the logout callback to an external provider
 *     results in a 'dead end', away from our website.
 *   - 'default_enabled' (default FALSE)
 *     If this profile should be enabled by default.
 */
function hook_auto_logout_profiles() {
  $profiles = [];
  $rid = user_role_load_by_name('administrator')->rid;
  $profiles['role_based'] = [
    'title' => t('Role based logout'),
    'callback' => 'dvg_authentication_auto_logout_is_role_based',
    'data' => $rid,
    'login_message_callback' => 'dvg_authentication_auto_logout_login_message',
    'logout_open_blank' => TRUE,
    'default_enabled' => TRUE,
  ];

  return $profiles;
}

/**
 * Provides the opportunity to modify the profiles returned by other modules.
 *
 * @param array $profiles
 *   List of profiles returned by hook_auto_logout_profiles().
 *
 * @see hook_auto_logout_profiles()
 */
function hook_auto_logout_profiles_alter(array &$profiles) {
  if (isset($profiles['my_favorite_profile'])) {
    $profiles['my_favorite_profile']['default_enabled'] = TRUE;
  }
}

/**
 * Prevent dvg_authentication_auto_logout logging a user out.
 *
 * This allows other modules to indicate that a page should not be included
 * in the auto logout checks. This works in the same way as not ticking the
 * enforce on admin pages option for auto logout which stops a user being logged
 * out of admin pages.
 *
 * @return bool
 *   Return TRUE if you do not want the user to be logged out.
 *   Return FALSE (or nothing) if you want to leave the auto logout
 *   process alone.
 */
function hook_prevent_auto_logout() {
  // Don't include auto logout JS checks on ajax callbacks.
  if (in_array(arg(0), [
    'ajax',
    'auto_logout',
    'auto_logout_set_last',
  ])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Keep a login alive while the user is on a particular page.
 *
 * @return bool
 *   By returning TRUE from this function the JS which talks to auto logout
 *   module is included in the current page request and periodically dials
 *   back to the server to keep the login alive.
 *   Return FALSE (or nothing) to just use the standard behaviour.
 */
function hook_auto_logout_keep_alive() {
  // Check to see if an open admin page will keep
  // login alive.
  return arg(0) === 'admin' && !variable_get('dvg_authentication_auto_logout_enforce_admin', FALSE);
}
