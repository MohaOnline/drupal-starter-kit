<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceNotFoundException;
use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Checks that all references are pointing to a valid service.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class CheckExceptionOnInvalidReferenceBehaviorPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $container;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $sourceId;

  /**
   * FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $this->container = $container;

    foreach ($container->getDefinitions() as $id => $definition) {
      $this->sourceId = $id;
      $this->processDefinition($definition);
    }
  }

  /**
   * FIX - insert comment here.
   */
  private function processDefinition(Definition $definition) {
    $this->processReferences($definition->getArguments());
    $this->processReferences($definition->getMethodCalls());
    $this->processReferences($definition->getProperties());
  }

  /**
   * FIX - insert comment here.
   */
  private function processReferences(array $arguments) {
    foreach ($arguments as $argument) {
      if (is_array($argument)) {
        $this->processReferences($argument);
      }
      elseif ($argument instanceof Definition) {
        $this->processDefinition($argument);
      }
      elseif ($argument instanceof Reference && ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE === $argument->getInvalidBehavior()) {
        $destId = (string) $argument;

        if (!$this->container->has($destId)) {
          throw new ServiceNotFoundException($destId, $this->sourceId);
        }
      }
    }
  }

}
