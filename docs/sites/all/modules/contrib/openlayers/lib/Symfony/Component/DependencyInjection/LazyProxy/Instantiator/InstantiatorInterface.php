<?php

namespace OpenlayersSymfony\Component\DependencyInjection\LazyProxy\Instantiator;

use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Definition;

/**
 * FIX - insert comment here.
 *
 * Lazy proxy instantiator, capable of instantiating a proxy given a container,
 * the service definitions and a callback that produces the real service
 * instance.
 *
 * @author Marco Pivetta <ocramius@gmail.com>
 */
interface InstantiatorInterface {

  /**
   * Instantiates a proxy object.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerInterface $container
   *   The container from which the service is being requested.
   * @param \Definition $definition
   *   The definition of the requested service.
   * @param string $id
   *   Identifier of the requested service.
   * @param callable $realInstantiator
   *   Zero-argument callback that is capable of producing the real
   *   service instance.
   *
   * @return object
   *   FIX - insert comment here.
   */
  public function instantiateProxy(ContainerInterface $container, Definition $definition, $id, callable $realInstantiator);

}
