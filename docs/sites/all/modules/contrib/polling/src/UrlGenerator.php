<?php

namespace Drupal\polling;


class UrlGenerator {
  protected static $instance = NULL;

  public static function instance() {
    if (!static::$instance) {
      static::$instance = new static();
    }
    return static::$instance;
  }

  public function entityUrl($entity_type, $entity_id) {
    if ($entity_type == 'node') {
      return url("/node/$entity_id/polling");
    }
  }

  public function globalUrl() {
    return url('polling');
  }
}
