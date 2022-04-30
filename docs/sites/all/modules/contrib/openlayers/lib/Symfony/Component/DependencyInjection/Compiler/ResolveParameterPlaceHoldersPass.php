<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ParameterNotFoundException;

/**
 * Resolves all parameter placeholders "%somevalue%" to their real values.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ResolveParameterPlaceHoldersPass implements CompilerPassInterface {

  /**
   * Processes the ContainerBuilder to resolve parameter placeholders.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ParameterNotFoundException
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $parameterBag = $container->getParameterBag();

    foreach ($container->getDefinitions() as $id => $definition) {
      try {
        $definition->setClass($parameterBag->resolveValue($definition->getClass()));
        $definition->setFile($parameterBag->resolveValue($definition->getFile()));
        $definition->setArguments($parameterBag->resolveValue($definition->getArguments()));
        if ($definition->getFactoryClass(FALSE)) {
          $definition->setFactoryClass($parameterBag->resolveValue($definition->getFactoryClass()));
        }

        $factory = $definition->getFactory();

        if (is_array($factory) && isset($factory[0])) {
          $factory[0] = $parameterBag->resolveValue($factory[0]);
          $definition->setFactory($factory);
        }

        $calls = array();
        foreach ($definition->getMethodCalls() as $name => $arguments) {
          $calls[$parameterBag->resolveValue($name)] = $parameterBag->resolveValue($arguments);
        }
        $definition->setMethodCalls($calls);

        $definition->setProperties($parameterBag->resolveValue($definition->getProperties()));
      }
      catch (ParameterNotFoundException $e) {
        $e->setSourceId($id);

        throw $e;
      }
    }

    $aliases = array();
    foreach ($container->getAliases() as $name => $target) {
      $aliases[$parameterBag->resolveValue($name)] = $parameterBag->resolveValue($target);
    }
    $container->setAliases($aliases);

    $parameterBag->resolve();
  }

}
