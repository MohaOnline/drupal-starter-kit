<?php

namespace OpenlayersDrupal\Core\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Adds services tagged 'stream_wrapper' to the stream_wrapper_manager service.
 */
class RegisterStreamWrappersPass implements CompilerPassInterface {

  /**
   * FIX - insert comment below.
   */
  public function process(ContainerBuilder $container) {
    if (!$container->hasDefinition('stream_wrapper_manager')) {
      return;
    }

    $stream_wrapper_manager = $container->getDefinition('stream_wrapper_manager');

    foreach ($container->findTaggedServiceIds('stream_wrapper') as $id => $attributes) {
      $class = $container->getDefinition($id)->getClass();
      $scheme = $attributes[0]['scheme'];

      $stream_wrapper_manager->addMethodCall(
        'addStreamWrapper',
        array($id, $class, $scheme)
      );
    }
  }

}
