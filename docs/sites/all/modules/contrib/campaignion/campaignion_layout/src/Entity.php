<?php

namespace Drupal\campaignion_layout;

/**
 * Wrapper around a tiny part of the Drupal field-API.
 */
class Entity {

  /**
   * The entity type.
   *
   * @var string
   */
  protected $type;

  /**
   * The entity object.
   *
   * @var object
   */
  protected $entity;

  /**
   * Create a new instance by passing the entity type and object.
   */
  public function __construct($entity_type, $entity) {
    $this->type = $entity_type;
    $this->entity = $entity;
  }

  /**
   * Get the names of all fields with a certain type.
   *
   * @return string[]
   *   A list of field names.
   */
  public function fieldsOfType(string $field_type) {
    list($_, $_, $bundle) = entity_extract_ids($this->type, $this->entity);
    $instances = array_filter(field_info_instances($this->type, $bundle), function ($instance) use ($field_type) {
      $field = field_info_field($instance['field_name']);
      return $field['type'] == $field_type;
    });
    return array_keys($instances);
  }

  /**
   * Get the field items for a field.
   *
   * @param string $field_name
   *   Get name of the field.
   *
   * @return array
   *   The field items or an empty array if there are none.
   */
  public function getItems(string $field_name) {
    return field_get_items($this->type, $this->entity, $field_name) ?: [];
  }

}
