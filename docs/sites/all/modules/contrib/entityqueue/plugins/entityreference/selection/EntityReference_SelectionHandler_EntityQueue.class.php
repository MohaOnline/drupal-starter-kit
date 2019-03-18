<?php

/**
 * @file
 * Definition of EntityReference_SelectionHandler_EntityQueue.
 */

/**
 * Defines a Entityreference selection handler for Entityqueue.
 */
class EntityReference_SelectionHandler_EntityQueue extends EntityReference_SelectionHandler_Generic {

  /**
   * Overrides EntityReference_SelectionHandler_Generic::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    return new EntityReference_SelectionHandler_EntityQueue($field, $instance, $entity_type, $entity);
  }

  /**
   * Constructs the EntityQueue selection handler.
   */
  protected function __construct($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    parent::__construct($field, $instance, $entity_type, $entity);

    $queue_name = NULL;
    if (!empty($entity->queue)) {
      $queue_name = $entity->queue;
    }
    elseif (!empty($instance['bundle'])) {
      $queue_name = $instance['bundle'];
    }
    if (!empty($queue_name)) {
      $this->queue = entityqueue_queue_load($queue_name);
    }

    // Override the entityreference settings with our own.
    $this->field['settings']['handler_settings']['target_bundles'] = NULL;
  }

  /**
   * Overrides EntityReference_SelectionHandler_Generic::settingsForm().
   */
  public static function settingsForm($field, $instance) {
    $form = parent::settingsForm($field, $instance);
    $access = $instance['entity_type'] === 'entityqueue_subqueue';

    // Show an explanation describing where bundles may be selected.
    $form['warning'] = array(
      '#type' => 'item',
      '#title' => t('DO NOT USE'),
      '#markup' => t('This mode should only be used on Entity Queues. This field is on a %type entity type and is not supported.', array('%type' => $instance['entity_type'])),
      '#weight' => -1,
      '#access' => !$access,
    );

    // Force all bundles to be accepted.
    $form['target_bundles'] = array(
      '#type' => 'value',
      '#value' => array(),
    );

    // Show an explanation describing where bundles may be selected.
    $form['target_bundles_help'] = array(
      '#type' => 'item',
      '#title' => t('Target bundles'),
      '#markup' => t('Bundles are filtered on a per-queue basis from the <a href="!url">queue\'s settings</a>.', array('!url' => url('admin/structure/entityqueue/list/' . $instance['bundle'] . '/edit'))),
      '#weight' => -1,
      '#access' => $access,
    );

    $form['sort']['#access'] = $access;

    return $form;
  }

  /**
   * Overrides EntityReference_SelectionHandler_Generic::buildEntityFieldQuery().
   */
  public function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    // Ensure that the 'target_bundles' setting from the field is not used.
    $this->field['settings']['handler_settings']['target_bundles'] = NULL;

    $handler = EntityReference_SelectionHandler_Generic::getInstance($this->field, $this->instance, $this->entity_type, $this->entity);
    $query = $handler->buildEntityFieldQuery($match, $match_operator);

    if (!empty($this->queue->settings['target_bundles'])) {
      $query->entityCondition('bundle', $this->queue->settings['target_bundles'], 'IN');
    }

    return $query;
  }

  /**
   * Implements EntityReferenceHandler::validateReferencableEntities().
   */
  public function validateReferencableEntities(array $ids) {
    $referencable = parent::validateReferencableEntities($ids);
    // Allow users to save the queue even if they don't have access to an
    // existing entity in the queue. See https://www.drupal.org/node/2383903
    $existing = $this->getCurrentlyReferencedEntityIds();

    return array_unique(array_merge($referencable, $existing));
  }

  /**
   * Gets ids of existing entities in the queue.
   *
   * @return array
   *   Entity ids that are currently referenced by the entity.
   */
  public function getCurrentlyReferencedEntityIds() {
    $ret = array();
    if (isset($this->entity) && isset($this->field)) {
      $entity_type = $this->entity_type;
      $field_name = $this->field['field_name'];
      $wrapper = entity_metadata_wrapper($entity_type, $this->entity);
      $ret = $wrapper->{$field_name}->raw();
    }

    return $ret;
  }

}
