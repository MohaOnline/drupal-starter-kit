<?php

/**
 * @file Contains \api\test2\ClassE.
 */

namespace api\test2;

use api\test1\TraitF;
use api\test1\TraitG;

/**
 * Still another sample class in a namespace.
 */
class ClassE {

  use TraitF;

  use TraitG {
    TraitG::def insteadof TraitF;
  }

  /**
   * A class variable to test linking to the type class.
   *
   * @var \api\test1\InterfaceD
   */
  public $foobar;

  /**
   * A really exciting method.
   */
  function eMethod() {
    $this->fvar = $this
      ->xyz();
  }

  /**
   * A static method.
   */
  static function staticMethod() {
    // Test links/uses of class methods in the top-level namespace.
    $foo = \ClassQ::bMethod();
  }
}
