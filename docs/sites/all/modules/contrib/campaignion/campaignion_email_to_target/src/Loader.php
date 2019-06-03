<?php

namespace Drupal\campaignion_email_to_target;

/**
 * Loader for selection mode plugins.
 */
class Loader {

  /**
   * A loader instance.
   *
   * @var this
   */
  protected static $instance = NULL;

  /**
   * Get a singleton instance of this class.
   */
  public static function instance() {
    if (!static::$instance) {
      static::$instance = static::fromHooks();
    }
    return static::$instance;
  }

  /**
   * Get a new instance from info gained by invoking hooks.
   */
  public static function fromHooks() {
    $selection_modes = \module_invoke_all('campaignion_email_to_target_selection_modes');
    drupal_alter('campaignion_email_to_target_selection_modes', $selection_modes);
    return new static($selection_modes);
  }

  protected $selectionModes;

  /**
   * Create new loader instance by explicitly giving the array of class names.
   *
   * @param string[] $modes
   *   Plugin class names keyed by unique string IDs.
   */
  public function __construct(array $modes) {
    $this->selectionModes = $modes;
  }

  /**
   * Get a plugin class name by its ID.
   *
   * @param string $mode_id
   *   The plugin ID.
   *
   * @return string
   *   The class name.
   */
  public function getMode($mode_id) {
    return $this->selectionModes[$mode_id]['class'];
  }

  /**
   * Get options array for choosing a selection mode.
   *
   * @return string[]
   *   Plugin titles keyed by plugin ID.
   */
  public function options() {
    return array_map(function ($info) {
      return $info['title'];
    }, $this->selectionModes);
  }

}
