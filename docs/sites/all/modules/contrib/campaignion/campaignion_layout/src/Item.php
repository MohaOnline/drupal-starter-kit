<?php

namespace Drupal\campaignion_layout;

/**
 * An object encapsulating a field item and its configured layout.
 */
class Item {

  /**
   * The layout definition of the active layout.
   *
   * @var array
   */
  public $layout;

  /**
   * The field item.
   *
   * @var array
   */
  protected $item;

  /**
   * Create a new field item.
   *
   * @param array $layout
   *   The layout configured by this field item.
   * @param array $item
   *   The field item.
   */
  public function __construct(array $layout, array $item) {
    $this->layout = $layout;
    $this->item = $item;
  }

  /**
   * Call the layout context condition if context is enabled.
   */
  public function executeContextCondition() : void {
    if (module_exists('context')) {
      if ($plugin = context_get_plugin('condition', 'campaignion_layout_context_condition_layout')) {
        $plugin->execute($this->layout['name']);
      }
    }
  }

  /**
   * Check whether the page order should be reversed.
   *
   * @return bool
   *   Returns TRUE if the layout is reversable and reversing the order is
   *   configured in the field item, otherwise FALSE.
   */
  public function pageOrderIsReversed() : bool {
    return ($this->layout['reversable'] ?? FALSE) && ($this->item['reversed'] ?? FALSE);
  }

}
