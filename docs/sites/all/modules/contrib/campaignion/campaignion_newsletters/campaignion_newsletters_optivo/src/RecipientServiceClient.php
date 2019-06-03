<?php

namespace Drupal\campaignion_newsletters_optivo;

/**
 *
 */
class RecipientServiceClient extends Client {

  /**
   * Override exception handling to add specific behavior.
   *
   * - Trying to remove a non-existing subscription is perfectly fine.
   */
  protected function handleException(\SoapFault $e, $name, $arguments) {
    if ($name == 'remove') {
      $message = $e->getMessage();
      if (strpos($message, 'Recipient does not exist for call') !== FALSE) {
        // Trying to remove a non-existing subscription is not bad.
        return;
      }
    }
    parent::handleException($e, $name, $arguments);
  }

}
