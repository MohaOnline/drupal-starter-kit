<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * FIX - insert comment here.
 *
 * Run this pass before passes that need to know more about the relation of
 * your services.
 *
 * This class will populate the ServiceReferenceGraph with information. You can
 * retrieve the graph in other passes from the compiler.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class AnalyzeServiceReferencesPass implements RepeatablePassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $graph;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $container;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $currentId;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $currentDefinition;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $repeatedPass;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $onlyConstructorArguments;

  /**
   * Constructor.
   *
   * @param bool $onlyConstructorArguments
   *   Sets this Service Reference pass to ignore method calls.
   */
  public function __construct($onlyConstructorArguments = FALSE) {
    $this->onlyConstructorArguments = (bool) $onlyConstructorArguments;
  }

  /**
   * {@inheritdoc}
   */
  public function setRepeatedPass(RepeatedPass $repeatedPass) {
    $this->repeatedPass = $repeatedPass;
  }

  /**
   * FIX - insert comment here.
   *
   * Processes a ContainerBuilder object to populate the service reference
   * graph.
   *
   * @param OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $this->container = $container;
    $this->graph = $container->getCompiler()->getServiceReferenceGraph();
    $this->graph->clear();

    foreach ($container->getDefinitions() as $id => $definition) {
      if ($definition->isSynthetic() || $definition->isAbstract()) {
        continue;
      }

      $this->currentId = $id;
      $this->currentDefinition = $definition;

      $this->processArguments($definition->getArguments());
      if ($definition->getFactoryService(FALSE)) {
        $this->processArguments(array(new Reference($definition->getFactoryService(FALSE))));
      }
      if (is_array($definition->getFactory())) {
        $this->processArguments($definition->getFactory());
      }

      if (!$this->onlyConstructorArguments) {
        $this->processArguments($definition->getMethodCalls());
        $this->processArguments($definition->getProperties());
        if ($definition->getConfigurator()) {
          $this->processArguments(array($definition->getConfigurator()));
        }
      }
    }

    foreach ($container->getAliases() as $id => $alias) {
      $this->graph->connect($id, $alias, (string) $alias, $this->getDefinition((string) $alias), NULL);
    }
  }

  /**
   * FIX - insert comment here.
   *
   * Processes service definitions for arguments to find relationships for the
   * service graph.
   *
   * @param array $arguments
   *   An array of Reference or Definition objects relating to service
   *   definitions.
   */
  private function processArguments(array $arguments) {
    foreach ($arguments as $argument) {
      if (is_array($argument)) {
        $this->processArguments($argument);
      }
      elseif ($argument instanceof Reference) {
        $this->graph->connect(
              $this->currentId,
              $this->currentDefinition,
              $this->getDefinitionId((string) $argument),
              $this->getDefinition((string) $argument),
              $argument
          );
      }
      elseif ($argument instanceof Definition) {
        $this->processArguments($argument->getArguments());
        $this->processArguments($argument->getMethodCalls());
        $this->processArguments($argument->getProperties());

        if (is_array($argument->getFactory())) {
          $this->processArguments($argument->getFactory());
        }
        if ($argument->getFactoryService(FALSE)) {
          $this->processArguments(
            array(new Reference($argument->getFactoryService(FALSE)))
          );
        }
      }
    }
  }

  /**
   * Returns a service definition given the full name or an alias.
   *
   * @param string $id
   *   A full id or alias for a service definition.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\Definition|null
   *   The definition related to the supplied id.
   */
  private function getDefinition($id) {
    $id = $this->getDefinitionId($id);

    return NULL === $id ? NULL : $this->container->getDefinition($id);
  }

  /**
   * FIX - insert comment here.
   *
   * @param string $id
   *   FIX - insert comment here.
   */
  private function getDefinitionId($id) {
    while ($this->container->hasAlias($id)) {
      $id = (string) $this->container->getAlias($id);
    }

    if (!$this->container->hasDefinition($id)) {
      return;
    }

    return $id;
  }

}
