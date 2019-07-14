<?php

namespace Drupal\pgbar;


class PgbarPolling implements \Drupal\polling\FieldTypePluginInterface {
  protected $source;
  protected $name;
  protected $items;

  public static function instance($entity, $field, $instance) {
    return new static($entity, $field, $instance);
  }

  public function __construct($entity, $field, $instance) {
    module_load_include('inc.php', 'pgbar', 'fields');
    $this->source = _pgbar_source_plugin_load($entity, $field, $instance);
    $entity_type = $instance['entity_type'];
    $this->name = $field['field_name'];
    $this->items = field_get_items($entity_type, $entity, $this->name);
  }

  public function getData() {
    $data['pgbar'] = [];
    foreach ($this->items as $delta => $item) {
      $item = _pgbar_add_item_defaults($item);
      $offset = isset($item['options']['target']['offset']) ? $item['options']['target']['offset'] : 0;
      $data['pgbar'][$this->name][$delta] = $this->source->getValue($item) + $offset;
    }
    return $data;
  }
}
