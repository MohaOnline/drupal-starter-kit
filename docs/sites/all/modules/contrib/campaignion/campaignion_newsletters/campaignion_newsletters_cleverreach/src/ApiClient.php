<?php

namespace Drupal\campaignion_newsletters_cleverreach;

/**
 * A SoapClient that adds the session-id parameter to all it's calls.
 */
class ApiClient extends \SoapClient {
  const WSDL_URL = 'http://api.cleverreach.com/soap/interface_v5.1.php?wsdl';
  protected $key;

  public function __construct($key) {
    parent::__construct(self::WSDL_URL);
    $this->key = $key;
  }

  /**
   * Override SoapClient::__call() to add the API-key.
   */
  public function __call($name, $arguments) {
    array_unshift($arguments, $this->key);
    return parent::__call($name, $arguments);
  }
}
