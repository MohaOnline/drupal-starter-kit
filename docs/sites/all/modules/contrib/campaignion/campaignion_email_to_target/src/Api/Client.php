<?php

namespace Drupal\campaignion_email_to_target\Api;

use Drupal\campaignion_auth\AuthAppClient;
use Drupal\little_helpers\Rest\Client as _Client;
use Drupal\little_helpers\Rest\HttpError;

/**
 * API client for the e2t-api service.
 */
class Client extends _Client {

  const API_VERSION = 'v3';

  /**
   * A auth app API client.
   *
   * @var \Drupal\campaignion_email_to_target\Api\AuthAppClient
   */
  protected $authClient;

  /**
   * Static cache of dataset metadata.
   *
   * @var array
   */
  protected $datasets;

  /**
   * Create a new instance.
   *
   * @param string $url
   *   The URL for the API endpoint (withut the version prefix).
   * @param \Drupal\campaignion_auth\AuthAppClient $auth_client
   *   A auth app API client.
   */
  public function __construct(string $url, AuthAppClient $auth_client) {
    parent::__construct($url . '/' . static::API_VERSION);
    $this->authClient = $auth_client;
    $this->datasets = &drupal_static(__CLASS__ . '.datasets', []);
  }

  /**
   * Return the endpoint URL.
   */
  public function getEndpoint() {
    return $this->endpoint;
  }

  /**
   * Add the JWT Authorization header to the request.
   */
  protected function sendRequest($url, array $options) {
    $token = $this->authClient->getToken();
    $options['headers']['Authorization'] = "Bearer $token";
    return parent::sendRequest($url, $options);
  }

  /**
   * Get all datasets from the API.
   *
   * @return \Drupal\campaignion_email_to_target\Api\Dataset[]
   *   An array of datasets keyed by their machine names.
   */
  public function getDatasetList() {
    $dataset_list = $this->get('');
    foreach ($dataset_list as $dataset) {
      $ds = Dataset::fromArray($dataset);
      $this->datasets[$ds->key] = $ds;
    }
    return $this->datasets;
  }

  /**
   * Get a single dataset from the API.
   *
   * @param string $key
   *   Machine name of the dataset to get.
   *
   * @return \Drupal\campaignion_email_to_target\Api\Dataset
   *   The dataset.
   */
  public function getDataset($key) {
    if (!array_key_exists($key, $this->datasets)) {
      $this->datasets[$key] = Dataset::fromArray($this->get(urlencode($key)));
    }
    return $this->datasets[$key];
  }

  /**
   * Get targets by dataset key and a selector value.
   *
   * @param string $dataset_key
   *   The key of the dataset which’s targets we want to query.
   * @param string[] $selector
   *   An associate array of URL query parameters used as filters to narrow down
   *   the number of targets. The meaning of the filters depends on the dataset.
   *
   * @return array
   *   An array of targets.
   */
  public function getTargets($dataset_key, array $selector) {
    try {
      $key = urlencode($dataset_key);
      return $this->get("$key/select", $selector);
    }
    catch (HttpError $e) {
      if (in_array($e->getCode(), [400, 404])) {
        return [];
      }
      throw $e;
    }
  }

  /**
   * Get a JWT access token for client-side access.
   */
  public function getAccessToken() {
    return $this->authClient->getEditorToken();
  }

}
