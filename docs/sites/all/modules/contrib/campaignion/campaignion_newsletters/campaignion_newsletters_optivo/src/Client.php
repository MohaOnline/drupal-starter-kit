<?php

namespace Drupal\campaignion_newsletters_optivo;

use \Drupal\campaignion_newsletters\ApiError;

/**
 * A SoapClient + error-handling.
 */
class Client extends \SoapClient {

  /**
   * Override SoapClient::__call().
   *
   * All SoapFault exceptions are considered temporary APIErrors.
   */
  public function __call($name, $arguments) {
    try {
      return parent::__call($name, $arguments);
    }
    catch (\SoapFault $e) {
      $this->handleException($e, $name, $arguments);
    }
  }

  /**
   * Callback to handle exception for this client.
   *
   * Some errors might require specific reactions. So here is a method that can
   * be easily overridden in child classes.
   *
   * @param \SoapFault $e
   *   The actual API-error.
   * @param string $name
   *   The name of the API function that was called.
   * @param array $arguments
   *   The arguments the API function was called with.
   */
  protected function handleException(\SoapFault $e, $name, $arguments) {
    throw new ApiError('Optivo', 'Exception during API-call: @message', ['@message' => $e->getMessage()], 0, NULL, $e);
  }

}
