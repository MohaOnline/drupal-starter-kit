<?php

/**
 * @file
 * Contains the Drupal\akamai\Ccu2Client class.
 *
 * This class is used for interacting with v2 of Akamai's CCU API.
 */

namespace Drupal\akamai;

use Akamai\Open\EdgeGrid\Client as EdgeGridClient;
use InvalidArgumentException;

class Ccu2Client extends BaseCcuClient implements CcuClientInterface {

  /**
   * The string used when removing objects.
   */
  const OPERATION_DELETE = 'remove';

  /**
   * The version of the CCU API.
   *
   * @var string
   */
  protected $version = 'v2';

  /**
   * The queue to use when issuing a purge request.
   *
   * @var string
   */
  protected $queuename = 'default';

  /**
   * Sets the queue name.
   *
   * @param string $queuename
   *   Valid values are 'default' and 'emergency'.
   */
  public function setQueueName($queuename) {
    if ($queuename != 'default' && $queuename != 'emergency') {
      throw new InvalidArgumentException('Invalid queue name supplied.');
    }
    $this->queuename = $queuename;
  }

  /**
   * Gets the number of items in the queue.
   *
   * @return int
   *   The number of pending purge requests in the queue.
   */
  public function getQueueLength() {
    $uri = "/ccu/{$this->version}/queues/{$this->queuename}";
    $response = $this->client->get($uri);
    return json_decode($response->getBody())->queueLength;
  }

  /**
   * Implements CcuClientInterface::getPurgeApiEndpoint().
   */
  public function getPurgeApiEndpoint() {
    return "/ccu/{$this->version}/queues/{$this->queuename}";
  }

  /**
   * Implements CcuClientInterface::getPurgeBody().
   */
  public function getPurgeBody($hostname, array $paths) {
    // Strip whitespace from paths and ensure each path begins with a '/'.
    // CCU API v2 requires absolute URLs, so prepend hostname and schemes.
    foreach ($paths as $key => $path) {
      $path = rtrim(preg_match("/^\//", $path) ? $path : "/{$path}");
      $paths[$key] = 'http://' . $hostname . $path;
      $paths[] = 'https://' . $hostname . $path;
    }
    $purge_body = array(
      'action' => $this->operation,
      'objects' => array_unique($paths),
      'domain' => $this->network,
    );
    return json_encode($purge_body);
  }

}
