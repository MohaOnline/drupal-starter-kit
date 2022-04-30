<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Inline service definitions where this is possible.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class InlineServiceDefinitionsPass implements RepeatablePassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $repeatedPass;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $graph;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $compiler;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $formatter;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $currentId;

  /**
   * FIX - insert comment here.
   */
  public function setRepeatedPass(RepeatedPass $repeatedPass) {
    $this->repeatedPass = $repeatedPass;
  }

  /**
   * Processes the ContainerBuilder for inline service definitions.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $this->compiler = $container->getCompiler();
    $this->formatter = $this->compiler->getLoggingFormatter();
    $this->graph = $this->compiler->getServiceReferenceGraph();

    foreach ($container->getDefinitions() as $id => $definition) {
      $this->currentId = $id;

      $definition->setArguments(
            $this->inlineArguments($container, $definition->getArguments())
        );

      $definition->setMethodCalls(
            $this->inlineArguments($container, $definition->getMethodCalls())
        );

      $definition->setProperties(
            $this->inlineArguments($container, $definition->getProperties())
        );

      $configurator = $this->inlineArguments($container, array($definition->getConfigurator()));
      $definition->setConfigurator($configurator[0]);

      $factory = $this->inlineArguments($container, array($definition->getFactory()));
      $definition->setFactory($factory[0]);
    }
  }

  /**
   * Processes inline arguments.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   The ContainerBuilder.
   * @param array $arguments
   *   An array of arguments.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function inlineArguments(ContainerBuilder $container, array $arguments) {
    foreach ($arguments as $k => $argument) {
      if (is_array($argument)) {
        $arguments[$k] = $this->inlineArguments($container, $argument);
      }
      elseif ($argument instanceof Reference) {
        if (!$container->hasDefinition($id = (string) $argument)) {
          continue;
        }

        if ($this->isInlineableDefinition($container, $id, $definition = $container->getDefinition($id))) {
          $this->compiler->addLogMessage($this->formatter->formatInlineService($this, $id, $this->currentId));

          if (ContainerInterface::SCOPE_PROTOTYPE !== $definition->getScope()) {
            $arguments[$k] = $definition;
          }
          else {
            $arguments[$k] = clone $definition;
          }
        }
      }
      elseif ($argument instanceof Definition) {
        $argument->setArguments($this->inlineArguments($container, $argument->getArguments()));
        $argument->setMethodCalls($this->inlineArguments($container, $argument->getMethodCalls()));
        $argument->setProperties($this->inlineArguments($container, $argument->getProperties()));
      }
    }

    return $arguments;
  }

  /**
   * Checks if the definition is inlineable.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   * @param string $id
   *   FIX - insert comment here.
   * @param \OpenlayersSymfony\Component\DependencyInjection\Definition $definition
   *   FIX - insert comment here.
   *
   * @return bool
   *   If the definition is inlineable.
   */
  private function isInlineableDefinition(ContainerBuilder $container, $id, Definition $definition) {
    if (ContainerInterface::SCOPE_PROTOTYPE === $definition->getScope()) {
      return TRUE;
    }

    if ($definition->isPublic() || $definition->isLazy()) {
      return FALSE;
    }

    if (!$this->graph->hasNode($id)) {
      return TRUE;
    }

    if ($this->currentId == $id) {
      return FALSE;
    }

    $ids = array();
    foreach ($this->graph->getNode($id)->getInEdges() as $edge) {
      $ids[] = $edge->getSourceNode()->getId();
    }

    if (count(array_unique($ids)) > 1) {
      return FALSE;
    }

    if (count($ids) > 1 && is_array($factory = $definition->getFactory()) && ($factory[0] instanceof Reference || $factory[0] instanceof Definition)) {
      return FALSE;
    }

    if (count($ids) > 1 && $definition->getFactoryService(FALSE)) {
      return FALSE;
    }

    return $container->getDefinition(reset($ids))->getScope() === $definition->getScope();
  }

}
