<?php

namespace Drupal\polling;

/**
 * Interface for loading registered plugins.
 */
class Loader {
  protected static $instance = NULL;

  public static function instance() {
    if (!static::$instance) {
      static::$instance = new static();
    }
    return static::$instance;
  }

  protected $fieldTypePlugins;
  protected $globalPlugins;

  public function __construct() {
    $hook = 'polling_field_type_plugin_info';
    $this->fieldTypePlugins = \module_invoke_all($hook);
    foreach ($this->fieldTypePlugins as $type => $plugins) {
      if (!is_array($plugins)) {
        $this->fieldTypePlugins[$type] = [$plugins];
      }
    }
    drupal_alter($hook, $this->fieldTypePlugins);
    $hook = 'polling_global_plugin_info';
    $this->globalPlugins = \module_invoke_all($hook);
    drupal_alter($hook, $this->globalPlugins);
  }

  /**
   * Iterator: Get all field-type plugins for an entity.
   */
  public function loadFieldTypePlugins($entity_type, $entity) {
    list(, , $bundle) = entity_extract_ids($entity_type, $entity);
    foreach ($this->iterateFields($entity_type, $bundle) as $d) {
      list($name, $field, $instance) = $d;

      if (isset($this->fieldTypePlugins[$field['type']])) {
        foreach ($this->fieldTypePlugins[$field['type']] as $class) {
          yield $class::instance($entity, $field, $instance);
        }
      }
    }
  }

  /**
   * Iterator: Get all global plugins.
   */
  public function loadGlobalPlugins() {
    foreach ($this->globalPlugins as $class) {
      yield $class::instance();
    }
  }

  /**
   * Iterate over fields.
   */
  protected function iterateFields($entity_type, $bundle) {
    foreach (field_info_instances($entity_type, $bundle) as $field_name => $instance) {
      $field = field_info_field_by_id($instance['field_id']);
      yield [$field_name, $field, $instance];
    }
  }
}
