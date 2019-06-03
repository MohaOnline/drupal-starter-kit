<?php
/**
 * @file The HostingNode class.
 */

class HostingNode {

  use HostingFieldable;

  // The node object.
  protected $node = FALSE;

  // Fields to format in line with Aegir standards.
  protected $info_fields = [];

  function __construct($node) {
    $this->node = $node;
  }

  public function getNid() {
    return $this->node->nid;
  }

  public function getTitle() {
    return $this->node->title;
  }

  public function getLanguage() {
    return $this->node->language;
  }

  public function getNode() {
    return $this->node;
  }

  /**
   * Alter the display of a node.
   */
  public function nodeView() {
    foreach ($this->getInfoFields() as $field) {
      $this->fixNodeFieldDisplay($field);
    }
  }

  protected function getInfoFields() {
    return $this->info_fields;
  }

  /**
   * Display a field as an Aegir-themed 'info' item.
   */
  public function fixNodeFieldDisplay($field) {
    if (isset($this->node->content[$field])) {
      $this->node->content['info'][$field] = array(
        '#type' => 'item',
        '#title' => $this->node->content[$field]['#title'],
        '#markup' => $this->node->content[$field][0]['#markup'],
      );
      unset($this->node->content[$field]);
    }
  }

  /**
   * Save the node.
   */
  public function save() {
    $this->node = node_submit($this->node);
    node_save($this->node);
    return $this->node->nid;
  }

}
