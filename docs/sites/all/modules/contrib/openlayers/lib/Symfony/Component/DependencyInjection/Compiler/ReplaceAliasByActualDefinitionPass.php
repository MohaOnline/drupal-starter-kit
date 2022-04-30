<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Reference;

/**
 * FIX - insert comment here.
 *
 * Replaces aliases with actual service definitions, effectively removing these
 * aliases.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ReplaceAliasByActualDefinitionPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $compiler;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $formatter;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $sourceId;

  /**
   * Process the Container to replace aliases with service definitions.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the service definition does not exist.
   */
  public function process(ContainerBuilder $container) {
    $this->compiler = $container->getCompiler();
    $this->formatter = $this->compiler->getLoggingFormatter();

    foreach ($container->getAliases() as $id => $alias) {
      $aliasId = (string) $alias;

      try {
        $definition = $container->getDefinition($aliasId);
      }
      catch (InvalidArgumentException $e) {
        throw new InvalidArgumentException(sprintf('Unable to replace alias "%s" with "%s".', $alias, $id), NULL, $e);
      }

      if ($definition->isPublic()) {
        continue;
      }

      $definition->setPublic(TRUE);
      $container->setDefinition($id, $definition);
      $container->removeDefinition($aliasId);

      $this->updateReferences($container, $aliasId, $id);

      // We have to restart the process due to concurrent modification of
      // the container.
      $this->process($container);

      break;
    }
  }

  /**
   * Updates references to remove aliases.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   The container.
   * @param string $currentId
   *   The alias identifier being replaced.
   * @param string $newId
   *   The id of the service the alias points to.
   */
  private function updateReferences(ContainerBuilder $container, $currentId, $newId) {
    foreach ($container->getAliases() as $id => $alias) {
      if ($currentId === (string) $alias) {
        $container->setAlias($id, $newId);
      }
    }

    foreach ($container->getDefinitions() as $id => $definition) {
      $this->sourceId = $id;

      $definition->setArguments(
            $this->updateArgumentReferences($definition->getArguments(), $currentId, $newId)
        );

      $definition->setMethodCalls(
            $this->updateArgumentReferences($definition->getMethodCalls(), $currentId, $newId)
        );

      $definition->setProperties(
            $this->updateArgumentReferences($definition->getProperties(), $currentId, $newId)
        );
    }
  }

  /**
   * Updates argument references.
   *
   * @param array $arguments
   *   An array of Arguments.
   * @param string $currentId
   *   The alias identifier.
   * @param string $newId
   *   The identifier the alias points to.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function updateArgumentReferences(array $arguments, $currentId, $newId) {
    foreach ($arguments as $k => $argument) {
      if (is_array($argument)) {
        $arguments[$k] = $this->updateArgumentReferences($argument, $currentId, $newId);
      }
      elseif ($argument instanceof Reference) {
        if ($currentId === (string) $argument) {
          $arguments[$k] = new Reference($newId, $argument->getInvalidBehavior());
          $this->compiler->addLogMessage($this->formatter->formatUpdateReference($this, $this->sourceId, $currentId, $newId));
        }
      }
    }

    return $arguments;
  }

}
