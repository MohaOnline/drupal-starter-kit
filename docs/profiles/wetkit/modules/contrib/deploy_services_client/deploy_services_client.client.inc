<?php

/**
 * @file
 * Defines a Services client class which communicates with Deployment endpoints.
 */

/**
 * Class which defines a Services client based on a Deployment endpoint.
 *
 * This class is currently hardcoded to use a REST-based JSON communication
 * method, and it makes other assumptions as well (e.g., a session-based
 * authentication method). A significant part of its code is borrowed from the
 * Deployment module, but rewritten so that it can be extended to make other
 * kinds of requests (e.g., GET and DELETE) and accessed outside the execution
 * of a deployment plan.
 *
 * @see DeployAuthenticatorSession::deploy()
 * @see DeployServiceRestJSON::deploy()
 * @see DeployServiceRest::httpRequest()
 */
class DeployServicesClient {
  /**
   * The Deployment endpoint to which Services requests will be made.
   */
  protected $endpoint;

  /**
   * The login cookie for session-based authentication.
   */
  protected $loginCookie;

  /**
   * The CSRF token for session-based authentication.
   */
  protected $tokenResponse;

  /**
   * Initializes the DeployServicesClient class.
   *
   * @param DeployEndpoint $endpoint
   *   The Deployment endpoint object to which Services requests will be made.
   */
  public function __construct(DeployEndpoint $endpoint) {
    $this->endpoint = $endpoint;
  }

  /**
   * Logs in to the deployment endpoint via session authentication.
   *
   * @return
   *   TRUE on successful login, or FALSE if the login was skipped due to an
   *   absence of login credentials.
   *
   * @throws DeployAuthenticationException
   *
   * @see DeployAuthenticatorSession::deploy()
   */
  public function login() {
    // Make sure we are starting a new session.
    $this->logout();

    if (!empty($this->endpoint->authenticator_config['username'])) {
      $login_url = $this->endpoint->service_config['url'] . '/user/login';
      $options = array(
        'method' => 'POST',
        'headers' => array('Content-Type' => 'application/json'),
        'data' => drupal_json_encode(array(
          'username' => $this->endpoint->authenticator_config['username'],
          'password' => $this->endpoint->authenticator_config['password'],
        )),
      );

      if ($this->debugEnabled()) {
        watchdog('deploy_services_client', 'Authentication request: %url <pre>@options</pre>', array('%url' => $login_url, '@options' => print_r($options, TRUE)), WATCHDOG_DEBUG);
      }

      // Perform the authentication request.
      $response = drupal_http_request($login_url, $options);

      if ($this->debugEnabled()) {
        watchdog('deploy_services_client', 'Authentication response: <pre>@response</pre>', array('@response' => print_r($response, TRUE)), WATCHDOG_DEBUG);
      }

      if (isset($response->error) || !in_array($response->code, array(200, 304))) {
        throw new DeployAuthenticationException(t('Authentication error: @code @error', array('@code' => $response->code, '@error' => $response->error)));
      }

      // Store the login cookie and CSRF token so we can use them elsewhere.
      $response_data = drupal_json_decode($response->data);
      if (!empty($response_data['session_name']) && !empty($response_data['sessid'])) {
        $this->endpoint->service_config['headers']['Cookie'] = $response_data['session_name'] . "=" . $response_data['sessid'] . ";";
        // We have to get a CSFR Token from the server and pass along with
        // each services call (introduced in Services 7.x-3.4
        // as per SA-CONTRIB-2013-051; https://drupal.org/node/2012982).
        $parts = parse_url($this->endpoint->service_config['url']);
        $port = '';
        $user = '';
        $pass = '';

        if (isset($parts['port'])) {
          $port = ':' . $parts['port'];
        }

       if (isset($parts['user'])) {
          $user = $parts['user'];
        }

        if (isset($parts['pass'])) {
          $pass = $parts['pass'];
        }

        $token_url = $parts['scheme'] . '://' . ($user ? $user . ':' . $pass . '@' : '') . $parts['host'] . $port . '/services/session/token';

        $token_response = drupal_http_request($token_url, array('method' => 'GET', 'headers' => $this->endpoint->service_config['headers']));
        if (isset($token_response->error) || !in_array($token_response->code, array(200, 304))) {
          throw new DeployAuthenticationException(t('Authentication error: @code @error', array('@code' => $token_response->code, '@error' => t('Failed to retrieve CSRF token.'))));
        }

        if ($this->debugEnabled()) {
          watchdog('deploy_services_client', 'Session CSRF Token response: @response', array('@response' => print_r($token_response, TRUE)), WATCHDOG_DEBUG);
        }

        $this->loginCookie = $this->endpoint->service_config['headers']['Cookie'];
        $this->tokenResponse = trim($token_response->data);
        $this->endpoint->service_config['headers']['X-CSRF-Token'] = $this->tokenResponse;

        return TRUE;
      }
      else {
        throw new DeployAuthenticationException(t("No session was returned from the authentication request."));
      }
    }
    else {
      watchdog('deploy_services_client', 'Deployment authentication was done without user credentials.', array(), WATCHDOG_WARNING);
      return FALSE;
    }
  }

  /**
   * Logs out of the deployment endpoint.
   *
   * @return
   *   TRUE on successful logout, or FALSE if there was no active session and
   *   therefore no logout could be performed.
   *
   * @see DeployAuthenticatorSession::deploy()
   */
  public function logout() {
    if ($this->loginCookie) {
      $logout_url = $this->endpoint->service_config['url'] . '/user/logout';
      $options = array(
        'method' => 'POST',
        'headers' => array(
          'Cookie' => $this->loginCookie,
          'X-CSRF-Token' => $this->tokenResponse,
        ),
      );
      $response = drupal_http_request($logout_url, $options);
      if ($this->debugEnabled()) {
        watchdog('deploy_services_client', 'Logout response: <pre>@response</pre>', array('@response' => print_r($response, TRUE)), WATCHDOG_DEBUG);
      }
      $this->loginCookie = NULL;
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Performs a GET request to retrieve data about an entity from the endpoint.
   *
   * @param EntityMetadataWrapper $entity
   *   The entity object to retrieve data about. Construct this by calling
   *   entity_metadata_wrapper() on a normal Drupal entity.
   *
   * @return
   *   The response data from the endpoint. This will be FALSE if the entity is
   *   not found on the endpoint site.
   *
   * @throws DeployServiceException
   */
  public function get(EntityMetadataWrapper $entity) {
    $response = $this->entityRequest($entity, 'GET');
    return empty($response->data) ? FALSE : drupal_json_decode($response->data);
  }

  /**
   * Performs a DELETE request to delete an entity on the endpoint.
   *
   * @param EntityMetadataWrapper $entity
   *   The entity object to delete. Construct this by calling
   *   entity_metadata_wrapper() on a normal Drupal entity.
   *
   * @throws DeployServiceException
   */
  public function delete(EntityMetadataWrapper $entity) {
    // @todo: We'd like to return something here, but there isn't much point
    //   because it seems that $reponse->data is always empty (regardless of
    //   whether the delete request was successful or not).
    $this->entityRequest($entity, 'DELETE');
  }

  /**
   * Performs a request to unpublish an entity on the endpoint.
   *
   * This only works on entities that have a 'status' property indicating the
   * publication status (e.g., nodes). Using this method on other entities can
   * cause problems, and therefore an exception will be thrown in the case
   * where an entity without a 'status' property is provided.
   *
   * @param EntityMetadataWrapper $entity
   *   The entity object to unpublish. Construct this by calling
   *   entity_metadata_wrapper() on a normal Drupal entity.
   *
   * @throws DeployServiceException
   */
  public function unpublish(EntityMetadataWrapper $entity) {
    // Replace IDs with UUIDs before sending. (This also has the effect of
    // getting a fresh copy of the entity, which may not match the provided
    // object. It's not clear if that's what we want, but it shouldn't be too
    // bad since the entity is being unpublished on the endpoint anyway.)
    $entities_to_send = entity_uuid_load($entity->type(), array($entity->uuid->value()));
    $entity_to_send = reset($entities_to_send);

    // Bail out if we don't have a status property.
    if (!isset($entity_to_send->status) && $entity->type() != 'menu_link') {
      throw new DeployServiceException(t('Entity of type "@type" with UUID "@uuid" did not have a "status" property and could not be unpublished.', array(
        '@type' => $entity->type(),
        '@uuid' => $entity->uuid->value(),
      )));
    }
    else if (!isset($entity_to_send->hidden) && $entity->type() == 'menu_link') {
      throw new DeployServiceException(t('Entity of type "@type" with UUID "@uuid" did not have a "hidden" property and could not be unpublished.', array(
        '@type' => $entity->type(),
        '@uuid' => $entity->uuid->value(),
      )));
    }

    // Set the status to unpublished / hidden (menu_link) and send the request.
    // @todo: We'd like to return something here to indicate success or
    //   failure, but it's not clear what to return.
    if ($entity->type() == 'menu_link') {
      $entity_to_send->hidden = 1;
    }
    else {
      $entity_to_send->status = 0;
    }

    $json_data = drupal_json_encode($entity_to_send);
    $this->entityRequest($entity, 'PUT', $json_data);
  }

  /**
   * Performs a request to the endpoint to perform an action on an entity.
   *
   * @param EntityMetadataWrapper $entity
   *   The entity object to be used in the request. Construct this by calling
   *   entity_metadata_wrapper() on a normal Drupal entity.
   * @param $method
   *   The HTTP method (e.g., 'GET' or 'DELETE') for the request.
   * @param $data
   *   (Optional) A string containing the data to send (e.g., for a 'PUT'
   *   request). If not provided, no data will be sent.
   *
   * @return
   *   The drupal_http_request() response object returned from the request.
   *
   * @throws DeployServiceException
   *
   * @see drupal_http_request()
   */
  public function entityRequest(EntityMetadataWrapper $entity, $method, $data = NULL) {
    $path = $entity->type() . '/' . $entity->uuid->value() . '.json';
    return $this->request($path, $method, $data);
  }

  /**
   * Performs a request to a REST path on the endpoint.
   *
   * @param $path
   *   The path (relative to the endpoint URL) to request.
   * @param $method
   *   The HTTP method (e.g., 'GET' or 'DELETE') for the request.
   * @param $data
   *   (Optional) A string containing the data to send (e.g., for a 'PUT'
   *   request). If not provided, no data will be sent.
   *
   * @return
   *   The drupal_http_request() response object returned from the request.
   *
   * @throws DeployServiceException
   *
   * @see drupal_http_request()
   * @see DeployServiceRestJSON::deploy()
   * @see DeployServiceRest::httpRequest()
   */
  public function request($path, $method, $data = NULL) {
    $url = $this->endpoint->service_config['url'] . '/' . $path;
    $headers['Content-Type'] = 'application/json';
    // Make the request as an authenticated user if we are already logged in to
    // the endpoint.
    if ($this->loginCookie) {
      $headers['Cookie'] = $this->loginCookie;
      $headers['X-CSRF-Token'] = $this->tokenResponse;
    }
    $options = array(
      'method' => $method,
      'headers' => $headers,
      'data' => $data,
    );

    if ($this->debugEnabled()) {
      watchdog('deploy_services_client', 'Service request: %url <pre>@options</pre>', array('%url' => $url, '@options' => print_r($options, TRUE)), WATCHDOG_DEBUG);
    }

    // Perform the request.
    $response = drupal_http_request($url, $options);

    if ($this->debugEnabled()) {
      watchdog('deploy_services_client', 'Service response: <pre>@response</pre>', array('@response' => print_r($response, TRUE)), WATCHDOG_DEBUG);
    }

    if (isset($response->error) || !in_array($response->code, array(200, 304))) {
      throw new DeployServiceException(t('Service error: @code @error', array('@code' => $response->code, '@error' => $response->error)));
    }

    return $response;
  }

  /**
   * Determines if the endpoint has debugging turned on.
   *
   * @return
   *   TRUE if debugging is enabled, or FALSE if it isn't.
   */
  protected function debugEnabled() {
    return !empty($this->endpoint->debug);
  }
}
