<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;

/**
 * FIX - insert comment here.
 *
 * Emulates the invalid behavior if the reference is not found within the
 * container.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ResolveInvalidReferencesPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $container;

  /**
   * Process the ContainerBuilder to resolve invalid references.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $this->container = $container;
    foreach ($container->getDefinitions() as $definition) {
      if ($definition->isSynthetic() || $definition->isAbstract()) {
        continue;
      }

      $definition->setArguments(
            $this->processArguments($definition->getArguments())
        );

      $calls = array();
      foreach ($definition->getMethodCalls() as $call) {
        try {
          $calls[] = array($call[0], $this->processArguments($call[1], TRUE));
        }
        catch (RuntimeException $ignore) {
          // This call is simply removed.
        }
      }
      $definition->setMethodCalls($calls);

      $properties = array();
      foreach ($definition->getProperties() as $name => $value) {
        try {
          $value = $this->processArguments(array($value), TRUE);
          $properties[$name] = reset($value);
        }
        catch (RuntimeException $ignore) {
          // Ignore property.
        }
      }
      $definition->setProperties($properties);
    }
  }

  /**
   * Processes arguments to determine invalid references.
   *
   * @param array $arguments
   *   An array of Reference objects.
   * @param bool $inMethodCall
   *   FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When the config is invalid.
   */
  private function processArguments(array $arguments, $inMethodCall = FALSE) {
    foreach ($arguments as $k => $argument) {
      if (is_array($argument)) {
        $arguments[$k] = $this->processArguments($argument, $inMethodCall);
      }
      elseif ($argument instanceof Reference) {
        $id = (string) $argument;

        $invalidBehavior = $argument->getInvalidBehavior();
        $exists = $this->container->has($id);

        // Resolve invalid behavior.
        if (!$exists && ContainerInterface::NULL_ON_INVALID_REFERENCE === $invalidBehavior) {
          $arguments[$k] = NULL;
        }
        elseif (!$exists && ContainerInterface::IGNORE_ON_INVALID_REFERENCE === $invalidBehavior) {
          if ($inMethodCall) {
            throw new RuntimeException('Method shouldn\'t be called.');
          }

          $arguments[$k] = NULL;
        }
      }
    }

    return $arguments;
  }

}
