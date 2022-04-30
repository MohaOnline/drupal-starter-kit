<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Exception;

/**
 * FIX - insert comment here.
 *
 * This exception is thrown when a circular reference in a parameter is
 * detected.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class ParameterCircularReferenceException extends RuntimeException {

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $parameters;

  /**
   * FIX - insert comment here.
   */
  public function __construct($parameters, \Exception $previous = NULL) {
    parent::__construct(sprintf('Circular reference detected for parameter "%s" ("%s" > "%s").', $parameters[0], implode('" > "', $parameters), $parameters[0]), 0, $previous);

    $this->parameters = $parameters;
  }

  /**
   * FIX - insert comment here.
   */
  public function getParameters() {
    return $this->parameters;
  }

}
