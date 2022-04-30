<?php

namespace OpenlayersSymfony\Component\DependencyInjection\LazyProxy\Instantiator;

use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Definition;

/**
 * FIX - insert comment here.
 *
 * Noop proxy instantiator - simply produces the real service instead of a
 * proxy instance.
 *
 * @author Marco Pivetta <ocramius@gmail.com>
 */
class RealServiceInstantiator implements InstantiatorInterface {

  /**
   * {@inheritdoc}
   */
  public function instantiateProxy(ContainerInterface $container, Definition $definition, $id, $realInstantiator) {
    return call_user_func($realInstantiator);
  }

}
