<?php

namespace Drupal\campaignion_auth;

use Drupal\little_helpers\Rest\Client;

/**
 * Client for the auth-app.
 */
class AuthAppClient extends Client {

  const API_VERSION = 'v1';
  const TOKEN_CID = 'campaignion_auth:token';

  /**
   * The API key for calling backend apps.
   *
   * @var string[]
   */
  protected $key;

  /**
   * The current organization.
   *
   * @var string
   */
  protected $organization;

  /**
   * Number of seconds a token is cached.
   *
   * @var int
   */
  protected $tokenLifetime;

  /**
   * Create a new instance by loading data from the config variables.
   */
  public static function validateConfig(string $url, array $key) {
    foreach (['public_key', 'secret_key'] as $v) {
      if (!isset($key[$v])) {
        throw new ConfigError('No valid API key found. The key must contain at least values for “public_key” and “secret_key”.');
      }
    }
    if (!$url) {
      throw new ConfigError('No auth app URL given.');
    }
  }

  /**
   * Create a new instance.
   *
   * @param string $url
   *   The URL for the auth-app API endpoint (withut the version prefix).
   * @param string[] $key
   *   The API key for this site.
   * @param int $token_lifetime
   *   The minimum amount of time for which the JWT is expected to be valid.
   */
  public function __construct(string $url, array $key, string $organization, int $token_lifetime = 3600) {
    static::validateConfig($url, $key);
    parent::__construct($url . '/' . static::API_VERSION);
    $this->key = $key;
    $this->organization = $organization;
    $this->tokenLifetime = $token_lifetime;
  }

  /**
   * Get a valid JWT token using the configured API-key.
   */
  public function getToken() : string {
    if (($cache = cache_get(static::TOKEN_CID)) && $cache->expire > REQUEST_TIME) {
      return $cache->data;
    }
    $token = $this->post('token/' . urlencode($this->organization), [], $this->key)['token'];
    cache_set(static::TOKEN_CID, $token, 'cache', REQUEST_TIME + $this->tokenLifetime);
    return $token;
  }

  /**
   * Get editor token.
   */
  public function getEditorToken() : string {
    $token = $this->getToken();
    $options['headers']['Authorization'] = "Bearer $token";
    $token = $this->post('session', [], ['roles' => ['editor']], $options)['token'];
    return $token;
  }

}
