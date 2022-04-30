<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Exception;

/**
 * FIX - insert comment here.
 *
 * This exception is thrown when you try to create a service of an inactive
 * scope.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class InactiveScopeException extends RuntimeException {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $serviceId;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $scope;

  /**
   * FIX - insert comment here.
   */
  public function __construct($serviceId, $scope, \Exception $previous = NULL) {
    parent::__construct(sprintf('You cannot create a service ("%s") of an inactive scope ("%s").', $serviceId, $scope), 0, $previous);

    $this->serviceId = $serviceId;
    $this->scope = $scope;
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
  public function getScope() {
    return $this->scope;
  }

}
