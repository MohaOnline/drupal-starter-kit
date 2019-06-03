<?php

namespace Drupal\campaignion_newsletters_mailchimp\Rest;

use \Drupal\little_helpers\Rest\Client;
use \Drupal\little_helpers\Rest\HttpError;

/**
 * MailChimp specific REST-client.
 *
 * Extends the generic REST-client with paging and error handling capabilities.
 */
class MailChimpClient extends Client {

  /**
   * Get a list of data from the API and page through all pages.
   *
   * This function issues multiple get requests to the API to get all items from
   * a list. It uses the APIs "total_amount" count to decide whether there are
   * additional items left.
   */
  public function getPaged($path, $query = [], $options = [], $size = 10, $list_key = NULL) {
    $items = [];
    $query['count'] = $size;
    // We always want to get the total count for paging purposes.
    if (!empty($query['fields'])) {
      $query['fields'] .= ',total_items';
    }
    if (!$list_key) {
      $list_key = strtr(substr($path, strrpos($path, '/') + 1), '-', '_');
    }
    $offset = 0;
    $next_page = TRUE;
    while ($next_page) {
      $result = $this->get($path, ['offset' => $offset] + $query, $options);
      $new_items = $result[$list_key];
      $items = array_merge($items, $new_items);

      // Only fetch the next page if there is more items than our next offset.
      $offset += $size;
      $next_page = $new_items && ($result['total_items'] > $offset);
    }
    return $items;
  }

  /**
   * Wrap the send method to generate ApiError instances in case of an error.
   */
  protected function send($path, array $query = [], $data = NULL, array $options = []) {
    try {
      return parent::send($path, $query, $data, $options);
    }
    catch (HttpError $e) {
      if ($new_e = ApiError::fromHttpError($e, $options['method'], $path)) {
        throw $new_e;
      }
      throw $e;
    }
  }

}
