<?php

namespace Drupal\campaignion_newsletters_optivo;

/**
 * Factory object for client objects.
 */
class ClientFactory {

  /**
   * Class mapping for services. Defaults to Client::class.
   */
  protected static $class = [
    'Session' => LoginClient::class,
    'Recipient' => RecipientServiceClient::class,
  ];

  /**
   * Cache for client objects.
   *
   * @var array
   */
  protected $clients = [];

  /**
   * Get a new factory and initialize the login service.
   *
   * @param string[] $credentials
   *   Array containing mandatorId, username and password in this order.
   *
   * @return static
   */
  public static function fromCredentials($credentials) {
    $f = new static();
    $l = $f->getClient('Session', FALSE);
    $l->setCredentials($credentials);
    return $f;
  }

  /**
   * Get a specific service client from the factory.
   *
   * @param string $name
   *   Name of the service.
   * @param bool $wrap
   *   Whether or not to wrap this client into a SessionWrapper.
   */
  public function getClient($name, $wrap = TRUE) {
    if (!isset($this->clients[$name])) {
      $wsdl = "https://api.campaign.episerver.net/soap11/Rpc{$name}?wsdl";
      $class = isset(static::$class[$name]) ? static::$class[$name] : Client::class;
      $service = new $class($wsdl);
      if ($wrap) {
        $login = $this->getClient('Session', FALSE);
        $service = $login->wrapClient($service);
      }
      $this->clients[$name] = $service;
    }
    return $this->clients[$name];
  }

}
