<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

/**
 * Represents an edge in your service graph.
 *
 * Value is typically a reference.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ServiceReferenceGraphEdge {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $sourceNode;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $destNode;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $value;

  /**
   * Constructor.
   *
   * @param ServiceReferenceGraphNode $sourceNode
   *   FIX - insert comment here.
   * @param ServiceReferenceGraphNode $destNode
   *   FIX - insert comment here.
   * @param string $value
   *   FIX - insert comment here.
   */
  public function __construct(ServiceReferenceGraphNode $sourceNode, ServiceReferenceGraphNode $destNode, $value = NULL) {
    $this->sourceNode = $sourceNode;
    $this->destNode = $destNode;
    $this->value = $value;
  }

  /**
   * Returns the value of the edge.
   *
   * @return ServiceReferenceGraphNode
   *   FIX - insert comment here.
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Returns the source node.
   *
   * @return ServiceReferenceGraphNode
   *   FIX - insert comment here.
   */
  public function getSourceNode() {
    return $this->sourceNode;
  }

  /**
   * Returns the destination node.
   *
   * @return ServiceReferenceGraphNode
   *   FIX - insert comment here.
   */
  public function getDestNode() {
    return $this->destNode;
  }

}
