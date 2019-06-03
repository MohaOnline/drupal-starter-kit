<?php

/**
 * @file
 * Describe hooks provided by the autologout module.
 */

/**
 * Prevent dvg_digid_autologout logging a user out.
 *
 * This allows other modules to indicate that a page should not be included
 * in the autologout checks. This works in the same way as not ticking the
 * enforce on admin pages option for autologout which stops a user being logged
 * out of admin pages.
 *
 * @return bool
 *   Return TRUE if you do not want the user to be logged out.
 *   Return FALSE (or nothing) if you want to leave the autologout
 *   process alone.
 */
function hook_dvg_digid_autologout_prevent() {
  // Don't include autologout JS checks on ajax callbacks.
  if (in_array(arg(0), array('ajax', 'autologout_ahah_logout', 'autologout_ahah_set_last'))) {
    return TRUE;
  }
}

/**
 * Keep a login alive whilst the user is on a particular page.
 *
 * @return bool
 *   By returning TRUE from this function the JS which talks to autologout
 *   module is included in the current page request and peridoically dials
 *   back to the server to keep the login alive.
 *   Return FALSE (or nothing) to just use the standard behaviour.
 */
function hook_dvg_digid_autologout_refresh_only() {
  // Check to see if an open admin page will keep
  // login alive.
  if (arg(0) == 'admin' && !variable_get('dvg_digid_autologout_enforce_admin', FALSE)) {
    return TRUE;
  }
}

/**
 * Let others act when session is extended.
 *
 * Use case: Some applications might be embedding the some other
 * applications via iframe which also requires to extend its sessions.
 */
 function hook_dvg_digid_auto_logout_session_reset($user) {
   $myOtherIframeApplication->resetSession($user->uid);
 }
