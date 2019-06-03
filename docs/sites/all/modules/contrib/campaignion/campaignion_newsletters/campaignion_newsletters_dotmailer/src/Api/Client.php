<?php

namespace Drupal\campaignion_newsletters_dotmailer\Api;

use \Drupal\little_helpers\Rest\Client as _Client;
use \Drupal\little_helpers\Rest\HttpError;

use \Drupal\campaignion_newsletters\ApiPersistentError;
use \Drupal\campaignion_newsletters\ApiError;

class Client extends _Client {
  public function __construct($username, $password) {
    $endpoint = "https://$username:$password@api.dotmailer.com/v2";
    parent::__construct($endpoint);
  }

  protected function send($path, array $query = [], $data = NULL, array $options = []) {
    try {
      return parent::send($path, $query, $data, $options);
    }
    catch (HttpError $e) {
      if ($e->result->code > 100) {
        // This is an actual HTTP response.
        $code = FALSE;
        if ($data = drupal_json_decode($e->result->data)) {
          if (!empty($data['message'])) {
            $message = $data['message'];
            $code = substr($message, strrpos($message, ' ') + 1);

            $msg = $e->getMessage() . ' ' . $message;
            $link = 'https://developer.dotmailer.com/docs/error-response-types';

            switch ($code) {
              case 'ERROR_ADDRESSBOOK_DUPLICATE':
              case 'ERROR_ADDRESSBOOK_IN_USE':
              case 'ERROR_ADDRESSBOOK_INVALID':
              case 'ERROR_ADDRESSBOOK_NOT_FOUND':
              case 'ERROR_ADDRESSBOOK_NOTWRITABLE':
              case 'ERROR_CONTACT_INVALID':
              case 'ERROR_CONTACT_SUPPRESSED':
              case 'ERROR_CONTACT_SUPPRESSEDFORADDRESSBOOK':
              case 'ERROR_DATAFIELD_INVALID':
              case 'ERROR_DATAFIELD_NOTFOUND':
              case 'ERROR_INVALID_EMAIL':
                throw new ApiPersistentError('dotmailer', $msg, [], $e->getCode(), $link, $e);
              default:
                throw new ApiError('dotmailer', $msg, [], $e->getCode(), $link, $e);
            }
          }
        }
      }
      // Network error or API-Error without the usual structure.
      throw new ApiError('dotmailer', $e->getMessage(), [], $e->getCode(), NULL, $e);
    }
  }
}
