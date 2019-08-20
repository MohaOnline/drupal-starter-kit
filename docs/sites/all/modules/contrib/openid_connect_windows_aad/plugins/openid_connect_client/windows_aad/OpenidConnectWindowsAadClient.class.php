<?php


/**
 * @file
 * OpenID Connect client for Windows Azure AD.
 */

/**
 * Class OpenidConnectWindowsAadClient adds the client to OpenID Connect.
 */
class OpenidConnectWindowsAadClient extends OpenIDConnectClientBase {

  /**
   * Overrides OpenIDConnectClientBase::settingsForm().
   */
  public function settingsForm() {
    $form = parent::settingsForm();

    $default_site = 'https://login.windows.net/[tenant]';
    $form['authorization_endpoint_wa'] = array(
      '#title' => t('Authorization endpoint'),
      '#type' => 'textfield',
      '#default_value' => $this->getSetting('authorization_endpoint_wa', $default_site . '/oauth2/authorize'),
    );
    $form['token_endpoint_wa'] = array(
      '#title' => t('Token endpoint'),
      '#type' => 'textfield',
      '#default_value' => $this->getSetting('token_endpoint_wa', $default_site . '/oauth2/token'),
    );
    $form['userinfo_endpoint_wa'] = array(
      '#title' => t('UserInfo endpoint'),
      '#type' => 'textfield',
      '#default_value' => $this->getSetting('userinfo_endpoint_wa', $default_site . '/openid/userinfo'),
    );
    $form['userinfo_graph_api_wa'] = array(
      '#title' => t('Use Graph API for user info'),
      '#type' => 'checkbox',
      '#default_value' => $this->getSetting('userinfo_graph_api_wa'),
      '#description' => t('This option will omit the Userinfo endpoint and will use the Graph API ro retrieve the userinfo.'),
    );
    $form['userinfo_graph_api_use_other_mails'] = array(
      '#title' => t('Use Graph API otherMails property for email address'),
      '#type' => 'checkbox',
      '#default_value' => $this->getSetting('userinfo_graph_api_use_other_mails'),
      '#description' => t('Find the first occurrence of an email address in the Graph otherMails property and use this as email address.'),
    );
    $form['userinfo_update_email'] = array(
      '#title' => t('Update email address in user profile'),
      '#type' => 'checkbox',
      '#default_value' => $this->getSetting('userinfo_update_email'),
      '#description' => t('If email address has been changed for existing user, save the new value to the user profile.'),
    );
    $form['hide_email_address_warning'] = array(
      '#title' => t('Hide missing email address warning'),
      '#type' => 'checkbox',
      '#default_value' => $this->getSetting('hide_email_address_warning'),
      '#description' => t('By default, when email address is not found, a message will appear on the screen. This option hides that message (as it might be confusing for end users).'),
    );

    return $form;
  }

  /**
   * Overrides OpenIDConnectClientBase::getEndpoints().
   */
  public function getEndpoints() {
    return array(
      'authorization' => $this->getSetting('authorization_endpoint_wa'),
      'token' => $this->getSetting('token_endpoint_wa'),
      'userinfo' => $this->getSetting('userinfo_endpoint_wa'),
    );
  }

  /**
   * Overrides OpenIDConnectClientInterface::retrieveIDToken().
   */
  public function retrieveTokens($authorization_code) {
    // Exchange `code` for access token and ID token.
    $redirect_uri = OPENID_CONNECT_REDIRECT_PATH_BASE . '/' . $this->name;
    $post_data = array(
      'code' => $authorization_code,
      'client_id' => $this->getSetting('client_id'),
      'client_secret' => $this->getSetting('client_secret'),
      'redirect_uri' => url($redirect_uri, array('absolute' => TRUE)),
      'grant_type' => 'authorization_code',
    );

    // Add Graph API as resource if option is set.
    if ($this->getSetting('userinfo_graph_api_wa') == 1) {
      $post_data['resource'] = 'https://graph.windows.net';
    }

    $request_options = array(
      'method' => 'POST',
      'data' => drupal_http_build_query($post_data),
      'timeout' => 15,
      'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
    );
    $endpoints = $this->getEndpoints();
    $response = drupal_http_request($endpoints['token'], $request_options);
    if (!isset($response->error) && $response->code == 200) {
      $response_data = drupal_json_decode($response->data);
      return array(
        'id_token' => $response_data['id_token'],
        'access_token' => $response_data['access_token'],
        'expire' => REQUEST_TIME + $response_data['expires_in'],
      );
    }
    else {
      openid_connect_log_request_error(__FUNCTION__, $this->name, $response);
      return FALSE;
    }
  }

  /**
   * Overrides OpenIDConnectClientBase::retrieveUserInfo().
   *
   * @todo -- map the Graph attribute names on userinfo, as they are different.
   */
  public function retrieveUserInfo($access_token) {
    // Determine if we use Graph API or default Openid Userinfo as this will
    // affect the data we collect and use in the Userinfo array.
    switch ($this->getSetting('userinfo_graph_api_wa')) {
      case 1:
        $userinfo = $this->buildUserinfo($access_token, 'https://graph.windows.net/me?api-version=1.6', 'userPrincipalName', 'displayName');
        break;

      default:
        $endpoints = $this->getEndpoints();
        $userinfo = $this->buildUserinfo($access_token, $endpoints['userinfo'], 'upn', 'name');
        break;
    }

    // Check to see if we have changed email data, openid_connect doesn't
    // give us the possibility to add a mapping for it, so we do the change
    // now, first checking if this is wanted by checking the setting for it.
    if ($this->getSetting('userinfo_update_email') == 1) {
      $user = user_load_by_name($userinfo['name']);
      if ($user && ($user->mail <> $userinfo['email'])) {
        $edit = array('mail' => $userinfo['email']);
        user_save($user, $edit);
      }
    }

    return $userinfo;
  }

  /**
   * Helper function to do the call to the endpoint and build userinfo array.
   *
   * @param string $access_token
   *   The access token.
   * @param string $url
   *   The endpoint we want to send the request to.
   * @param string $upn
   *   The name of the property that holds the Azure username.
   * @param string $name
   *   The name of the property we want to map to Drupal username.
   *
   * @return array
   *   The userinfo array or FALSE.
   */
  private function buildUserinfo($access_token, $url, $upn, $name) {
    // Perform the request.
    $options = array(
      'method' => 'GET',
      'headers' => array(
        'Content-Type' => 'application/json',
        'Authorization' => 'Bearer ' . $access_token,
      ),
    );
    $result = drupal_http_request($url, $options);

    if (in_array($result->code, array(200, 304))) {
      $profile_data = json_decode($result->data, TRUE);

      $profile_data['name'] = $profile_data[$name];

      if (!isset($profile_data['email'])) {
        // See if we have the Graph otherMails property and use it if available,
        // if not, add the principal name as email instead, so Drupal still will
        // create the user anyway.
        if ($this->getSetting('userinfo_graph_api_use_other_mails') == 1) {
          if (!empty($profile_data['otherMails'])) {
            // Use first occurrence of otherMails attribute.
            $profile_data['email'] = current($profile_data['otherMails']);
          }
        }
        else {
          // Show message to user.
          if ($this->getSetting('hide_email_address_warning') <> 1) {
            drupal_set_message(t('Email address not found in UserInfo. Used username instead, please check this in your profile.'), 'warning');
          }
          // Write watchdog warning.
          $type = 'warning';
          $message = 'Email address of user @user not found in UserInfo. Used username instead, please check.';
          $variables = array('@user' => $profile_data[$upn]);

          watchdog($type, $message, $variables);

          $profile_data['email'] = $profile_data[$upn];
        }
      }

      return $profile_data;
    }
    else {
      drupal_set_message(t('The UserInfo cannot be retrieved. Please check your settings.'), 'error');

      return FALSE;
    }

  }

}
