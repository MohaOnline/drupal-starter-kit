<?php

namespace OpenlayersSymfony\Component\DependencyInjection\ParameterBag;

use OpenlayersSymfony\Component\DependencyInjection\Exception\LogicException;

/**
 * Holds read-only parameters.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
class FrozenParameterBag extends ParameterBag {

  /**
   * Constructor.
   *
   * For performance reasons, the constructor assumes that
   * all keys are already lowercased.
   *
   * This is always the case when used internally.
   *
   * @param array $parameters
   *   An array of parameters.
   */
  public function __construct(array $parameters = array()) {
    $this->parameters = $parameters;
    $this->resolved = TRUE;
  }

  /**
   * {@inheritdoc}
   *
   * @api
   */
  public function clear() {
    throw new LogicException('Impossible to call clear() on a frozen ParameterBag.');
  }

  /**
   * {@inheritdoc}
   *
   * @api
   */
  public function add(array $parameters) {
    throw new LogicException('Impossible to call add() on a frozen ParameterBag.');
  }

  /**
   * {@inheritdoc}
   *
   * @api
   */
  public function set($name, $value) {
    throw new LogicException('Impossible to call set() on a frozen ParameterBag.');
  }

}
