<?php

/**
 * @file
 * OpenID Connect client for Google.
 */

/**
 * Implements OpenID Connect Client plugin for Google.
 */
class OpenIDConnectClientGoogle extends OpenIDConnectClientBase {

  /**
   * {@inheritdoc}
   */
  public function getEndpoints() {
    return array(
      'authorization' => 'https://accounts.google.com/o/oauth2/auth',
      'token' => 'https://accounts.google.com/o/oauth2/token',
      'userinfo' => 'https://openidconnect.googleapis.com/v1/userinfo',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function retrieveUserInfo($access_token) {
    $userinfo = parent::retrieveUserInfo($access_token);
    if (!empty($userinfo['picture'])) {
      // For some reason Google returns the URI of the profile picture in a
      // weird format: "https:" appears twice in the beginning of the URI.
      // Using a regular expression matching for fixing it guarantees that
      // things won't break if Google changes the way the URI is returned.
      preg_match('/https:\/\/*.*/', $userinfo['picture'], $matches);
      $userinfo['picture'] = $matches[0];
    }

    return $userinfo;
  }

}
