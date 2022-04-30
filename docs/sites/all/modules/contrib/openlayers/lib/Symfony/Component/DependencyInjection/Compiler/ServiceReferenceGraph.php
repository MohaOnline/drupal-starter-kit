<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;

/**
 * This is a directed graph of your services.
 *
 * This information can be used by your compiler passes instead of collecting
 * it themselves which improves performance quite a lot.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ServiceReferenceGraph {

  /**
   * FIX - insert coment here.
   *
   * @var ServiceReferenceGraphNode[]
   */
  private $nodes = array();

  /**
   * Checks if the graph has a specific node.
   *
   * @param string $id
   *   Id to check.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function hasNode($id) {
    return isset($this->nodes[$id]);
  }

  /**
   * Gets a node by identifier.
   *
   * @param string $id
   *   The id to retrieve.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\Compiler\ServiceReferenceGraphNode
   *   The node matching the supplied identifier
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If no node matches the supplied identifier.
   */
  public function getNode($id) {
    if (!isset($this->nodes[$id])) {
      throw new InvalidArgumentException(sprintf('There is no node with id "%s".', $id));
    }

    return $this->nodes[$id];
  }

  /**
   * Returns all nodes.
   *
   * @return ServiceReferenceGraphNode[]
   *   An array of all ServiceReferenceGraphNode objects.
   */
  public function getNodes() {
    return $this->nodes;
  }

  /**
   * Clears all nodes.
   */
  public function clear() {
    $this->nodes = array();
  }

  /**
   * Connects 2 nodes together in the Graph.
   *
   * @param string $sourceId
   *   FIX - insert comment here.
   * @param string $sourceValue
   *   FIX - insert comment here.
   * @param string $destId
   *   FIX - insert comment here.
   * @param string $destValue
   *   FIX - insert comment here.
   * @param string $reference
   *   FIX - insert comment here.
   */
  public function connect($sourceId, $sourceValue, $destId, $destValue = NULL, $reference = NULL) {
    $sourceNode = $this->createNode($sourceId, $sourceValue);
    $destNode = $this->createNode($destId, $destValue);
    $edge = new ServiceReferenceGraphEdge($sourceNode, $destNode, $reference);

    $sourceNode->addOutEdge($edge);
    $destNode->addInEdge($edge);
  }

  /**
   * Creates a graph node.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param string $value
   *   FIX - insert comment here.
   *
   * @return ServiceReferenceGraphNode
   *   FIX - insert comment here.
   */
  private function createNode($id, $value) {
    if (isset($this->nodes[$id]) && $this->nodes[$id]->getValue() === $value) {
      return $this->nodes[$id];
    }

    return $this->nodes[$id] = new ServiceReferenceGraphNode($id, $value);
  }

}
