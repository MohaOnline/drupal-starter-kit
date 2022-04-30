<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * Represents a variable.
 *
 *     $var = new Variable('a');
 *
 * will be dumped as
 *
 *     $a
 *
 * by the PHP dumper.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class Variable {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $name;

  /**
   * Constructor.
   *
   * @param string $name
   *   FIX - insert comment here.
   */
  public function __construct($name) {
    $this->name = $name;
  }

  /**
   * Converts the object to a string.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function __toString() {
    return $this->name;
  }

}
