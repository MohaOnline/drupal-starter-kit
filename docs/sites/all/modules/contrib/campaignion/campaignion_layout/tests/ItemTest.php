<?php

namespace Drupal\campaignion_layout;

use Upal\DrupalUnitTestCase;

/**
 * Test the field item wrapper.
 */
class ItemTest extends DrupalUnitTestCase {

  /**
   * Test getting the page order without any config.
   */
  public function testReversedOrderWithoutConfig() {
    $item = new Item([], []);
    $this->assertFalse($item->pageOrderIsReversed());
  }

  /**
   * Test getting page order when disabled in the layout.
   */
  public function testReversedOrderDisabled() {
    $item = new Item([], ['reversed' => 1]);
    $this->assertFalse($item->pageOrderIsReversed());
  }

  /**
   * Test getting page order with enabled layout and config.
   */
  public function testReversedOrderEnbaledAndConfigured() {
    $item = new Item(['reversible' => TRUE], ['reversed' => 1]);
    $this->assertFalse($item->pageOrderIsReversed());
  }

}
