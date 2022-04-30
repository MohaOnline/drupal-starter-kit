<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Exception;

/**
 * This exception is thrown when a circular reference is detected.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ServiceCircularReferenceException extends RuntimeException {

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $serviceId;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $path;

  /**
   * FIX - insert comment here.
   */
  public function __construct($serviceId, array $path, \Exception $previous = NULL) {
    parent::__construct(sprintf('Circular reference detected for service "%s", path: "%s".', $serviceId, implode(' -> ', $path)), 0, $previous);

    $this->serviceId = $serviceId;
    $this->path = $path;
  }

  /**
   * FIX - insert comment here.
   */
  public function getServiceId() {
    return $this->serviceId;
  }

  /**
   * FIX - insert comment here.
   */
  public function getPath() {
    return $this->path;
  }

}
