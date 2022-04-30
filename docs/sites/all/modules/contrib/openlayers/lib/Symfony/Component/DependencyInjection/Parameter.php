<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * Parameter represents a parameter reference.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class Parameter {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $id;

  /**
   * Constructor.
   *
   * @param string $id
   *   The parameter key.
   */
  public function __construct($id) {
    $this->id = $id;
  }

  /**
   * Creates __toString.
   *
   * @return string
   *   The parameter key.
   */
  public function __toString() {
    return (string) $this->id;
  }

}
