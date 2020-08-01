<?php

namespace Drupal\campaignion_action;

use Drupal\little_helpers\Services\Container;

/**
 * Class for loading Action-Type plugins and general dependency injection.
 */
class Loader {
  protected static $instance  = NULL;
  protected $info;
  protected $types = [];

  /**
   * Get a singleton instance of this class.
   */
  public static function instance() {
    return Container::get()->loadService('campaignion_action.loader');
  }

  public static function fromGlobalInfo() {
    return new static(\module_invoke_all('campaignion_action_info'));
  }

  public function __construct($types_info) {
    foreach ($types_info as $type => &$info) {
      $info += [
        'action_class' => '\\Drupal\\campaignion_action\\ActionBase',
        'parameters' => [],
      ];
      // Fail early if wizard_class is not defined.
      if (empty($info['wizard_class'])) {
        throw new \InvalidArgumentException("wizard_class not defined for action-type '$type'.");
      }
      // We explicitly don't check whether the class exists because this would
      // mean autoloading every class every time the Loader is used.
    }
    $this->info = $types_info;
    $this->types = &drupal_static(__CLASS__ . '::types', []);
  }

  /**
   * Get all action type instances.
   *
   * @return array
   *   Array of action type classes keyed by their machine-name.
   */
  public function allTypes() {
    $types = [];
    foreach (array_keys($this->info) as $type) {
      $types[$type] = $this->type($type);
    }
    return $types;
  }

  /**
   * Get all node-types that are actions.
   *
   * @return array
   *   Array of all node-types that are also action-types.
   */
  public function actionNodeTypes() {
    return array_keys($this->info);
  }

  /**
   * Check if a node-type is an action-type.
   *
   * @param string $type
   *   Machine name of the node-type.
   * @return boolean
   *   TRUE if the node-type $type is an action-type.
   */
  public function isActionType($type) {
    return isset($this->info[$type]);
  }

  /**
   * Get instance of an action type.
   *
   * @param string $type
   *   Machine name of the action-type.
   * @return \Drupal\campaignion_action\TypeBase
   *   The action-type identified by $type.
   */
  public function type($type) {
    if (!isset($this->types[$type])) {
      $this->types[$type] = FALSE;
      if ($info = $this->info[$type] ?? NULL) {
        $this->types[$type] = new ActionType($type, $info + $info['parameters']);
      }
    }
    return $this->types[$type];
  }

  /**
   * Get action instance by node-type.
   */
  public function actionFromNode($node) {
    if (!isset($node->action)) {
      $node->action = NULL;
      if ($info = $this->info[$node->type] ?? NULL) {
        $class = $info['action_class'];
        $parameters = $info + $info['parameters'];
        $node->action = new $class($parameters, $node);
      }
    }
    return $node->action;
  }

  /**
   * Return a wizard object for a node-type.
   *
   * @param string $type
   *   The node-type.
   * @param object|null $node
   *   The node to edit. Create a new one if NULL.
   *
   * @return \Drupal\oowizard\Wizard
   *  The wizard responsible for changing/adding actions of this type.
   */
  public function wizard($type, $node = NULL) {
    if ($info = $this->info[$type] ?? NULL) {
      $class = $info['wizard_class'];
      return new $class($info, $node, $type);
    }
  }

  /**
   * Get all node-types that are referenced as thank-you pages.
   */
  protected function thankYouPageTypes() {
    $tyTypes = [];
    foreach ($this->info as $type => $p) {
      if (isset($p['thank_you_page'])) {
        $tyTypes[$p['thank_you_page']['type']][$p['thank_you_page']['reference']] = TRUE;
      }
    }
    return $tyTypes;
  }

  /**
   * Get names of all fields referencing a thank-you page type.
   *
   * @param string $type
   *   Node-type of the thank-you page.
   * @return array
   *   Array of field names that may reference nodes of this type.
   */
  protected function referenceFieldsByType($type) {
    $types = $this->thankYouPageTypes();
    if (isset($types[$type])) {
      return array_keys($types[$type]);
    }
    return [];
  }

  /**
   * Get an action-node's nid by one of it's thank-you page nodes.
   */
  public function actionNidByThankYouNode($node) {
    foreach ($this->referenceFieldsByType($node->type) as $field) {
      // Lookup the action that uses this thank you page.
      $sql = "SELECT entity_id FROM {field_data_$field} WHERE entity_type='node' AND {$field}_node_reference_nid=:nid LIMIT 1";
      $result = db_query($sql, array(':nid' => $node->nid));
      if ($nid = $result->fetchField()) {
        return $nid;
      }
    }
  }

}
