<?php

/**
 * @file
 * Contains the Drupal\akamai\Ccu3Client class.
 *
 * This class is used for interacting with v3 of Akamai's CCU API.
 */

namespace Drupal\akamai;

class Ccu3Client extends BaseCcuClient implements CcuClientInterface {

  /**
   * The version of the CCU API.
   *
   * @var string
   */
  protected $version = 'v3';

  /**
   * Implements CcuClientInterface::getPurgeApiEndpoint().
   */
  public function getPurgeApiEndpoint() {
    return "/ccu/{$this->version}/{$this->operation}/url/{$this->network}";
  }

  /**
   * Implements CcuClientInterface::getPurgeBody().
   */
  public function getPurgeBody($hostname, array $paths) {
    // Strip whitespace from paths and ensure each path begins with a '/'.
    foreach ($paths as $key => $path) {
      $paths[$key] = rtrim(preg_match("/^\//", $path) ? $path : "/{$path}");
    }
    $purge_body = array(
      'hostname' => $hostname,
      'objects' => array_unique($paths),
    );
    // Ensure a non-associative array for json_encode() so it will output an
    // array instead of an object.
    $purge_body['objects'] = array_values($purge_body['objects']);

    // Use JSON_UNESCAPED_SLASHES to reduce amount of data in request body.
    return json_encode($purge_body, JSON_UNESCAPED_SLASHES);
  }
}
