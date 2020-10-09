<?php

/**
 * @file
 * Definition of AbstractFieldSynonymsBehavior class.
 */

/**
 * Abstract class for providing synonyms from fields attached to entities.
 */
abstract class AbstractFieldSynonymsBehavior extends AbstractSynonymsBehavior {

  /**
   * Field definition array on which this provider was initialized.
   *
   * @var array
   */
  protected $field;

  /**
   * Field instance definition on which this provider was initialized.
   *
   * @var array
   */
  protected $instance;

  public function __construct($behavior_implementation) {
    parent::__construct($behavior_implementation);

    $this->field = field_info_field(synonyms_provider_field_field_name($behavior_implementation['provider']));
    $this->instance = field_info_instance($behavior_implementation['entity_type'], $this->field['field_name'], $behavior_implementation['bundle']);
  }

  public function featuresExportPipe() {
    $pipe = parent::featuresExportPipe();

    // We include the Features component for the underlying field on which this
    // synonyms provider is built.
    $pipe['field'][] = implode('-', array($this->behavior_implementation['entity_type'], $this->behavior_implementation['bundle'], $this->field['field_name']));
    return $pipe;
  }

  /**
   * Retrieve items of the underlying field in this behavior implementation.
   *
   * @param object $entity
   *   Entity whose items should be retrieved
   * @param string $langcode
   *   Language code for which to retrieve items from the entity, if one is
   *   known
   *
   * @return array
   *   Array of items that provided entity has in the field on which behavior
   *   implementation is set up
   */
  protected function entityItems($entity, $langcode = NULL) {
    $items = field_get_items($this->instance['entity_type'], $entity, $this->field['field_name'], $langcode);
    return is_array($items) ? $items : array();
  }

  /**
   * Filter $items only to contain unique values.
   *
   * @param array $items
   *   Array of field items that should be filtered to contain only unique
   *   values
   * @param array $unique_index
   *   Array of column names that define uniqueness for an item
   *
   * @return array
   *   Only unique items from the provided $items array
   */
  protected function uniqueItems($items, $unique_index) {
    $index = array();
    foreach ($items as $item) {
      $item_index = array();
      foreach ($unique_index as $column) {
        $item_index[] = is_scalar($column) ? $column : serialize($column);
      }
      $index[serialize($item_index)] = $item;
    }
    return array_values($index);
  }
}
