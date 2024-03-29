<?php

namespace OpenlayersDrupal\Core\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Registers all lazy route enhancers onto the lazy route enhancers.
 */
class RegisterLazyRouteEnhancers implements CompilerPassInterface {

  /**
   * {@inheritdoc}
   */
  public function process(ContainerBuilder $container) {
    if (!$container->hasDefinition('route_enhancer.lazy_collector')) {
      return;
    }

    $service_ids = [];

    foreach ($container->findTaggedServiceIds('route_enhancer') as $id => $attributes) {
      $service_ids[$id] = $id;
    }

    $container
      ->getDefinition('route_enhancer.lazy_collector')
      ->addArgument($service_ids);
  }

}
