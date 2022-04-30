<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Checks your services for circular references.
 *
 * References from method calls are ignored since we might be able to resolve
 * these references depending on the order in which services are called.
 *
 * Circular reference from method calls will only be detected at run-time.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class CheckCircularReferencesPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $currentPath;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $checkedNodes;

  /**
   * Checks the ContainerBuilder object for circular references.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   The ContainerBuilder instances.
   */
  public function process(ContainerBuilder $container) {
    $graph = $container->getCompiler()->getServiceReferenceGraph();

    $this->checkedNodes = array();
    foreach ($graph->getNodes() as $id => $node) {
      $this->currentPath = array($id);

      $this->checkOutEdges($node->getOutEdges());
    }
  }

  /**
   * Checks for circular references.
   *
   * @param ServiceReferenceGraphEdge[] $edges
   *   An array of Edges.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException
   *   When a circular reference is found.
   */
  private function checkOutEdges(array $edges) {
    foreach ($edges as $edge) {
      $node = $edge->getDestNode();
      $id = $node->getId();

      if (empty($this->checkedNodes[$id])) {
        $searchKey = array_search($id, $this->currentPath);
        $this->currentPath[] = $id;

        if (FALSE !== $searchKey) {
          throw new ServiceCircularReferenceException($id, array_slice($this->currentPath, $searchKey));
        }

        $this->checkOutEdges($node->getOutEdges());

        $this->checkedNodes[$id] = TRUE;
        array_pop($this->currentPath);
      }
    }
  }

}
