<?php

/**
 * @file
 * Constains the Drupal\akamai\BaseCcuClient class.
 *
 * Abstract implementation of CcuClientInterface.
 */

namespace Drupal\akamai;

use Akamai\Open\EdgeGrid\Client as EdgeGridClient;
use InvalidArgumentException;

abstract class BaseCcuClient implements CcuClientInterface {

  /**
   * The string used when invalidating objects.
   */
  const OPERATION_INVALIDATE = 'invalidate';

  /**
   * The string used when removing objects.
   */
  const OPERATION_DELETE = 'remove';

  /**
   * An instance of an OPEN EdgeGrid Client.
   *
   * @var \Akamai\Open\EdgeGrid\Client
   */
  protected $client;

  /**
   * The network to use when issuing purge requests.
   *
   * @var string
   */
  protected $network = self::NETWORK_PRODUCTION;

  /**
   * The operation to use when issuing purge requests.
   *
   * @var string
   */
  protected $operation = self::OPERATION_INVALIDATE;

  /**
   * Implements CcuClientInterface::__construct().
   */
  public function __construct(EdgeGridClient $client) {
    $this->client = $client;
  }

  /**
   * Implements CcuClientInterface::setNetwork().
   */
  public function setNetwork($network) {
    if ($network != self::NETWORK_PRODUCTION && $network != self::NETWORK_STAGING) {
      throw new InvalidArgumentException('Invalid network supplied.');
    }
    $this->network = $network;
    return $this;
  }

  /**
   * Implements CcuClientInterface::setOperation().
   */
  public function setOperation($operation) {
    if ($operation != self::OPERATION_INVALIDATE && $operation != self::OPERATION_DELETE) {
      throw new InvalidArgumentException('Invalid operation supplied.');
    }
    $this->operation = $operation;
    return $this;
  }

  /**
   * Implements CcuClientInterface::checkProgress().
   */
  public function checkProgress($progress_uri) {
    $response = $this->client->get($progress_uri);
    return json_decode($response->getBody());
  }

  /**
   * Implements CcuClientInterface::postPurgeRequest().
   */
  public function postPurgeRequest($hostname, array $paths) {
    if (empty($hostname)) {
      throw new InvalidArgumentException("Expected hostname to be a non-empty string.");
    }
    $uri = $this->getPurgeApiEndpoint();
    $response = $this->client->post($uri, [
      'body' => $this->getPurgeBody($hostname, $paths),
      'headers' => ['Content-Type' => 'application/json']
    ]);
    return json_decode($response->getBody());
  }

  /**
   * Implements CcuClientInterface::bodyIsBelowLimit().
   */
  public function bodyIsBelowLimit($hostname, array $paths) {
    $body = $this->getPurgeBody($hostname, $paths);
    $bytes = mb_strlen($body, '8bit');
    return $bytes < self::MAX_BODY_SIZE;
  }

}
