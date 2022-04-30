<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Removes unused service definitions from the container.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class RemoveUnusedDefinitionsPass implements RepeatablePassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $repeatedPass;

  /**
   * {@inheritdoc}
   */
  public function setRepeatedPass(RepeatedPass $repeatedPass) {
    $this->repeatedPass = $repeatedPass;
  }

  /**
   * Processes the ContainerBuilder to remove unused definitions.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $compiler = $container->getCompiler();
    $formatter = $compiler->getLoggingFormatter();
    $graph = $compiler->getServiceReferenceGraph();

    $hasChanged = FALSE;
    foreach ($container->getDefinitions() as $id => $definition) {
      if ($definition->isPublic()) {
        continue;
      }

      if ($graph->hasNode($id)) {
        $edges = $graph->getNode($id)->getInEdges();
        $referencingAliases = array();
        $sourceIds = array();
        foreach ($edges as $edge) {
          $node = $edge->getSourceNode();
          $sourceIds[] = $node->getId();

          if ($node->isAlias()) {
            $referencingAliases[] = $node->getValue();
          }
        }
        $isReferenced = (count(array_unique($sourceIds)) - count($referencingAliases)) > 0;
      }
      else {
        $referencingAliases = array();
        $isReferenced = FALSE;
      }

      if (1 === count($referencingAliases) && FALSE === $isReferenced) {
        $container->setDefinition((string) reset($referencingAliases), $definition);
        $definition->setPublic(TRUE);
        $container->removeDefinition($id);
        $compiler->addLogMessage($formatter->formatRemoveService($this, $id, 'replaces alias ' . reset($referencingAliases)));
      }
      elseif (0 === count($referencingAliases) && FALSE === $isReferenced) {
        $container->removeDefinition($id);
        $compiler->addLogMessage($formatter->formatRemoveService($this, $id, 'unused'));
        $hasChanged = TRUE;
      }
    }

    if ($hasChanged) {
      $this->repeatedPass->setRepeat();
    }
  }

}
