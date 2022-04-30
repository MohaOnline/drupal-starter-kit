<?php

namespace OpenlayersDrupal\Core\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Compiler\CompilerPassInterface;

/**
 * FIX - insert comment below.
 *
 * Adds services tagged "needs_destruction" to the "kernel_destruct_subscriber"
 * service.
 *
 * @see \OpenlayersDrupal\Core\DestructableInterface
 */
class RegisterServicesForDestructionPass implements CompilerPassInterface {

  /**
   * FIX - insert comment below.
   *
   * Implements
   * \OpenlayersSymfony\Component\DependencyInjection\Compiler\CompilerPassInterface::process().
   */
  public function process(ContainerBuilder $container) {
    if (!$container->hasDefinition('kernel_destruct_subscriber')) {
      return;
    }

    $definition = $container->getDefinition('kernel_destruct_subscriber');

    $services = $container->findTaggedServiceIds('needs_destruction');
    foreach ($services as $id => $attributes) {
      $definition->addMethodCall('registerService', array($id));
    }
  }

}
