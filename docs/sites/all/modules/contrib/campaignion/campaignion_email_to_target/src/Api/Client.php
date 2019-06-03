<?php

namespace Drupal\campaignion_email_to_target\Api;

use Dflydev\Hawk\Credentials\Credentials;
use Dflydev\Hawk\Client\ClientBuilder;

use Drupal\little_helpers\Rest\Client as _Client;
use Drupal\little_helpers\Rest\HttpError;

/**
 * API client for the e2t-api service.
 */
class Client extends _Client {
  const API_VERSION = 'v2';
  protected $hawk;
  protected $credentials;
  protected $datasets;

  /**
   * Create a new instance from global configuration.
   */
  public static function fromConfig() {
    $c = variable_get('campaignion_email_to_target_credentials', []);
    foreach (['url', 'public_key', 'secret_key'] as $v) {
      if (!isset($c[$v])) {
        throw new ConfigError(
          'No valid e2t_api credentials found. The credentials must contain ' .
          'at least values for "url", "public_key" and "private_key".'
        );
      }
    }
    return new static($c['url'], $c['public_key'], $c['secret_key']);
  }

  /**
   * Create a new instance.
   *
   * @param string $url
   *   The URL for the API endpoint (withut the version prefix).
   * @param string $pk
   *   The public API-key used for HAWK authentication.
   * @param string $sk
   *   The secret API-key used for HAWK authentication.
   */
  public function __construct($url, $pk, $sk) {
    parent::__construct($url . '/' . static::API_VERSION);
    $this->credentials = new Credentials($sk, 'sha256', $pk);
    $this->hawk = ClientBuilder::create()->build();
    $this->datasets = &drupal_static(__CLASS__ . '.datasets', []);
  }

  /**
   * Return the endpoint URL.
   */
  public function getEndpoint() {
    return $this->endpoint;
  }

  /**
   * Add HAWK authentication headers to the request.
   */
  protected function sendRequest($url, array $options) {
    $options += ['method' => 'GET'];
    $method = $options['method'];
    $hawk_options = [];
    if (!empty($options['data']) || in_array($method, ['POST', 'PUT'])) {
      $options += ['data' => '', 'headers' => []];
      $options['headers'] += ['Content-Type' => ''];
      $hawk_options['payload'] = $options['data'];
      $hawk_options['content_type'] = $options['headers']['Content-Type'];
    }
    $hawk = $this->hawk->createRequest($this->credentials, $url, $method, $hawk_options);
    $header = $hawk->header();
    $options['headers'][$header->fieldName()] = $header->fieldValue();
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
      $this->datasets[$key] = Dataset::fromArray($this->get($key));
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
      return $this->get("$dataset_key/select", $selector);
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
    $res = $this->post('access-token');
    return $res['access_token'];
  }

}
