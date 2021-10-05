<?php

/**
 * @file
 * Hooks provided by the OpenID Connect module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Pre authorize hook that runs before a user is authorized.
 *
 * @param array $tokens
 *   ID token and access token that we received as a result of the OpenID
 *   Connect flow.
 * @param object $account
 *   The user account if it exists, false if not.
 * @param array $userinfo
 *   The user claims returned by the OpenID Connect provider.
 * @param string $client_name
 *   The machine name of the OpenID Connect client plugin.
 *
 * @return bool
 *   TRUE if user should be logged into Drupal. FALSE if not.
 */
function hook_openid_connect_pre_authorize(array $tokens, $account, array $userinfo, $client_name) {
  $allowed_users = array('user1@example.com', 'user2@example.com');
  // Allow only specific users to log in.
  if (in_array($userinfo['email'], $allowed_users)) {
    return TRUE;
  }

  // Block all others.
  return FALSE;
}

/**
 * Perform an action after a successful authorization.
 *
 * @param array $tokens
 *   ID token and access token that we received as a result of the OpenID
 *   Connect flow.
 * @param object $account
 *   The user account that has just been logged in.
 * @param array $userinfo
 *   The user claims returned by the OpenID Connect provider.
 * @param string $client_name
 *   The machine name of the OpenID Connect client plugin.
 * @param bool $is_new
 *   Whether the account has just been created via OpenID Connect.
 */
function hook_openid_connect_post_authorize(array $tokens, $account, array $userinfo, $client_name, $is_new) {
  drupal_set_message($is_new ? t('Welcome!') : t('Welcome back!'));
}

/**
 * Alter the list of possible scopes and claims.
 *
 * @param array &$claims
 *   Array of claims to be altered.
 *
 * @see openid_connect_claims
 */
function hook_openid_connect_claims_alter(array &$claims) {
  $claims['my_custom_claim'] = array(
    'scope' => 'profile',
  );
}

/**
 * @} End of "addtogroup hooks".
 */
