<?php

/**
 * @file
 * Constains the Drupal\akamai\CcuClientInterface interface.
 *
 * This class is used for interacting with v3 of Akamai's CCU API.
 */

namespace Drupal\akamai;

use Akamai\Open\EdgeGrid\Client as EdgeGridClient;

interface CcuClientInterface {

  /**
   * String constant for the production network.
   */
  const NETWORK_PRODUCTION = 'production';

  /**
   * String constant for the staging network.
   */
  const NETWORK_STAGING = 'staging';

  /**
   * The maximum size, in bytes, of a request body allowed by the API.
   */
  const MAX_BODY_SIZE = 50000;

  /**
   * Constructor.
   *
   * @param \Akamai\Open\EdgeGrid\Client $client
   *   An instance of the EdgeGrid HTTP client class.
   */
  public function __construct(EdgeGridClient $client);

  /**
   * Sets the network on which purge requests will be executed.
   *
   * @param string $network
   *   Must be either 'production' or 'staging'.
   */
  public function setNetwork($network);

  /**
   * Sets the operation to use when issuing a purge request.
   *
   * @param string $operation
   *   An operation such as 'invalidate' or 'remove'.
   */
  public function setOperation($operation);

  /**
   * Checks the progress of a purge request.
   *
   * @param string $progress_uri
   *   A URI as provided in response to a purge request.
   */
  public function checkProgress($progress_uri);

  /**
   * Submits a purge request for one or more URLs.
   *
   * @param string $hostname
   *   The name of the URL that contains the objects you want to purge.
   * @param array $paths
   *   An array of paths to be purged.
   */
  public function postPurgeRequest($hostname, array $paths);

  /**
   * Generates the URL to use when posting a purge request.
   *
   * @return string
   *   The URL to use for creating a purge request,
   *   e.g. '/ccu/v3/invalidate/url/production'.
   */
  public function getPurgeApiEndpoint();

  /**
   * Generates a JSON-encoded body for a purge request.
   *
   * @return string
   *   A JSON-encoded object containing 'hostname' and 'objects' properties.
   */
  public function getPurgeBody($hostname, array $paths);

  /**
   * Verifies that the body of a purge request will be under 50,000 bytes.
   *
   * @param string $hostname
   *   The name of the URL that contains the objects you want to purge.
   * @param array $paths
   *   An array of paths to be purged.
   *
   * @return bool
   *   TRUE if the body size is below the limit, otherwise FALSE.
   */
  public function bodyIsBelowLimit($hostname, array $paths);

}
