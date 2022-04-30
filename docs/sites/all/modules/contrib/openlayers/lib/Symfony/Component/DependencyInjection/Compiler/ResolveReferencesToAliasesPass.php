<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Alias;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Replaces all references to aliases with references to the actual service.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ResolveReferencesToAliasesPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $container;

  /**
   * FIX - insert comment here.
   *
   * Processes the ContainerBuilder to replace references to aliases with
   * actual service references.
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

      $definition->setArguments($this->processArguments($definition->getArguments()));
      $definition->setMethodCalls($this->processArguments($definition->getMethodCalls()));
      $definition->setProperties($this->processArguments($definition->getProperties()));
    }

    foreach ($container->getAliases() as $id => $alias) {
      $aliasId = (string) $alias;
      if ($aliasId !== $defId = $this->getDefinitionId($aliasId)) {
        $container->setAlias($id, new Alias($defId, $alias->isPublic()));
      }
    }
  }

  /**
   * Processes the arguments to replace aliases.
   *
   * @param array $arguments
   *   An array of References.
   *
   * @return array
   *   An array of References.
   */
  private function processArguments(array $arguments) {
    foreach ($arguments as $k => $argument) {
      if (is_array($argument)) {
        $arguments[$k] = $this->processArguments($argument);
      }
      elseif ($argument instanceof Reference) {
        $defId = $this->getDefinitionId($id = (string) $argument);

        if ($defId !== $id) {
          $arguments[$k] = new Reference($defId, $argument->getInvalidBehavior(), $argument->isStrict());
        }
      }
    }

    return $arguments;
  }

  /**
   * Resolves an alias into a definition id.
   *
   * @param string $id
   *   The definition or alias id to resolve.
   *
   * @return string
   *   The definition id with aliases resolved.
   */
  private function getDefinitionId($id) {
    $seen = array();
    while ($this->container->hasAlias($id)) {
      if (isset($seen[$id])) {
        throw new ServiceCircularReferenceException($id, array_keys($seen));
      }
      $seen[$id] = TRUE;
      $id = (string) $this->container->getAlias($id);
    }

    return $id;
  }

}
