<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\Alias;

/**
 * Represents a node in your service graph.
 *
 * Value is typically a definition, or an alias.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ServiceReferenceGraphNode {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $id;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $inEdges = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $outEdges = array();

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $value;

  /**
   * Constructor.
   *
   * @param string $id
   *   The node identifier.
   * @param mixed $value
   *   The node value.
   */
  public function __construct($id, $value) {
    $this->id = $id;
    $this->value = $value;
  }

  /**
   * Adds an in edge to this node.
   *
   * @param ServiceReferenceGraphEdge $edge
   *   FIX - insert comment here.
   */
  public function addInEdge(ServiceReferenceGraphEdge $edge) {
    $this->inEdges[] = $edge;
  }

  /**
   * Adds an out edge to this node.
   *
   * @param ServiceReferenceGraphEdge $edge
   *   FIX - insert comment here.
   */
  public function addOutEdge(ServiceReferenceGraphEdge $edge) {
    $this->outEdges[] = $edge;
  }

  /**
   * Checks if the value of this node is an Alias.
   *
   * @return bool
   *   True if the value is an Alias instance.
   */
  public function isAlias() {
    return $this->value instanceof Alias;
  }

  /**
   * Checks if the value of this node is a Definition.
   *
   * @return bool
   *   True if the value is a Definition instance.
   */
  public function isDefinition() {
    return $this->value instanceof Definition;
  }

  /**
   * Returns the identifier.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * Returns the in edges.
   *
   * @return array
   *   The in ServiceReferenceGraphEdge array
   */
  public function getInEdges() {
    return $this->inEdges;
  }

  /**
   * Returns the out edges.
   *
   * @return array
   *   The out ServiceReferenceGraphEdge array
   */
  public function getOutEdges() {
    return $this->outEdges;
  }

  /**
   * Returns the value of this Node.
   *
   * @return mixed
   *   The value.
   */
  public function getValue() {
    return $this->value;
  }

}
