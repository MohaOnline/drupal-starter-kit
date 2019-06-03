<?php

namespace Drupal\campaignion_email_to_target_counter;

use Drupal\polling\FieldTypePluginInterface;

/**
 * Polling plugin for target counts.
 */
class CountPolling implements FieldTypePluginInterface {

  /**
   * @var int|null
   *
   * Node ID of the node to poll.
   */
  protected $nid;

  /**
   * {@inheritdoc}
   */
  public static function instance($entity, $field, $instance) {
    $nid = $instance['entity_type'] == 'node' ? $entity->nid : NULL;
    return new static($nid);
  }

  /**
   * Construct a new plugin instance.
   *
   * @param int|null $nid
   *   Node ID of the node to poll.
   */
  public function __construct($nid) {
    $this->nid = $nid;
  }

  /**
   * Get the current counts for this node.
   *
   * @return array
   *   If this is a node it returns the targets in a namespaced sub-array. Each
   *   member of the sub-array is a pair of label and current count. The members
   *   are keyed by target ID.
   */
  public function getData() {
    $data = [];
    if (!$this->nid) {
      return $data;
    }

    $counts = [];
    $result = db_select('campaignion_email_to_target_counter', 'c')
      ->fields('c')
      ->condition('nid', $this->nid)
      ->orderBy('count', 'DESC')
      ->orderBy('label')
      ->execute();
    foreach ($result as $row) {
      $counts[$row->target_id] = [
        'count' => (int) $row->count,
        'label' => $row->label,
      ];
    }
    $data['campaignion_email_to_target_counter'] = $counts;
    return $data;
  }

}

