<?php

namespace Drupal\campaignion_newsletters_mailchimp\Rest;

use \Drupal\campaignion_newsletters\ApiError as _ApiError;
use \Drupal\little_helpers\Rest\HttpError;

/**
 * An exception that signifies an MailChimp API error.
 */
class ApiError extends _ApiError {

  /**
   * Create an API-Error instance from a HttpError exception.
   */
  public static function fromHttpError(HttpError $e, $verb, $path) {
    if ($data = drupal_json_decode($e->result->data)) {
      $code = $e->getCode();
      $msg = "Got @code for %verb %path: @title - @detail %errors";
      $vars = [
        '%verb' => $verb,
        '%path' => $path,
        '@title' => $data['title'],
        '@detail' => $data['detail'],
        '%errors' => '',
      ];
      if (!empty($data['errors'])) {
        $errors = [];
        foreach ($data['errors'] as $error) {
          $errors[] = "{$error['field']}: {$error['message']}";
        }
        $vars['%errors'] = '{ ' . implode(",\n", $errors) . ' }';
      }
      return new static('campaignion_newsletters', $msg, $vars, $code, NULL, $e);
    }
  }

  /**
   * Decide whether this is a persistent error.
   *
   * @return bool
   *   Whether or not the queue item yielding this error should be dropped.
   */
  public function isPersistent() {
    $code = $this->getCode();
    return in_array($code, [400]);
  }

}
