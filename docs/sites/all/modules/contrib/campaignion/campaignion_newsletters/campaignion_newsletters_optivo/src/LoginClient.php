<?php

namespace Drupal\campaignion_newsletters_optivo;

/**
 * Handles login/logout operations and acts as a factory for SessionClients.
 */
class LoginClient extends Client {

  protected $sessionId;
  protected $credentials;

  /**
   * Setter for credentials.
   */
  public function setCredentials(array $credentials) {
    $this->credentials = $credentials;
  }

  /**
   * Gets the current session ID - login if needed.
   */
  public function getSessionId() {
    if (!$this->sessionId) {
      list($m, $u, $p) = $this->credentials;
      $this->sessionId = $this->login($m, $u, $p);
    }
    return $this->sessionId;
  }

  /**
   * Logout when this objects is destroyed.
   */
  public function __destruct() {
    if ($this->sessionId) {
      $this->logout($this->sessionId);
    }
  }

  /**
   * Wrap another client to automatically handle passing the session ID.
   */
  public function wrapClient(Client $client) {
    return new SessionWrapper($this, $client);
  }

}
