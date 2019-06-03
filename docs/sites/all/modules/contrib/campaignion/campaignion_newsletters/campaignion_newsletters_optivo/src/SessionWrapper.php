<?php

namespace Drupal\campaignion_newsletters_optivo;

/**
 * Wraps SoapClient objects and automatically adds a sessionId to each calls.
 */
class SessionWrapper {

  protected $loginClient;
  protected $wrapped;

  public function __construct(LoginClient $login_client, Client $client) {
    $this->loginClient = $login_client;
    $this->wrapped = $client;
  }

  public function __call($name, $arguments) {
    array_unshift($arguments, $this->loginClient->getSessionId());
    return $this->wrapped->__call($name, $arguments);
  }

}
