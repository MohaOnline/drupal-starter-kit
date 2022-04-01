<?php

/**
 * @file Contains \api\test1\TraitG.
 */

namespace api\test1;

/**
 * Another sample trait.
 *
 * Has a method that conflicts with TraitF.
 */
trait TraitG {

  /**
   * Non-conflicting method to inherit.
   */
  protected function abc() {
    return 7;
  }

  /**
   * Conflicting method to inherit.
   */
  protected function def() {
    return 'Value from TraitG';
  }

}
