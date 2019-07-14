<?php

namespace Drupal\pgbar\Source;

/**
 * @file
 * A pgbar source plugin that displays a fixed count.
 */

class StaticCount implements PluginInterface {

  public static function label() {
    return t('Static count');
  }

  public static function forField($entity, $field, $instance) {
    return new static();
  }

  /**
   * Value will be set by the "offset" setting.
   */
  public function getValue($item) {
    return 0;
  }
  /**
   * We don't need any extra configuration.
   */
  public function widgetForm($item) {
    return NULL;
  }
}
