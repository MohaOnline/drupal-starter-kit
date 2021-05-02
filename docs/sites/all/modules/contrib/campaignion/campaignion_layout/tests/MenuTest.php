<?php

namespace Drupal\campaignion_layout;

use Upal\DrupalUnitTestCase;

/**
 * Test whether menu-entries are manipulated as expected.
 *
 * NOTE: If you change anything in the menu alters you have to clear caches
 *       before the test results will change.
 */
class MenuTest extends DrupalUnitTestCase {

  const ENTITY_CALLBACK = 'campaignion_layout_get_theme_for_entity';
  const NO_CALLBACK = '';

  /**
   * Create a test node.
   */
  public function setUp() : void {
    parent::setUp();
    $node = (object) ['type' => 'petition', 'title' => __CLASS__];
    node_object_prepare($node);
    node_save($node);
    $this->node = $node;
  }

  /**
   * Delete the test node.
   */
  public function tearDown() : void {
    node_delete($this->node->nid);
    parent::tearDown();
  }

  /**
   * Check that node paths get the right custom theme callback.
   */
  public function testNodePaths() {
    $item = menu_get_item("node/{$this->node->nid}/view");
    $this->assertEqual(self::ENTITY_CALLBACK, $item['theme_callback']);

    $item = menu_get_item("node/{$this->node->nid}");
    $this->assertEqual(self::ENTITY_CALLBACK, $item['theme_callback']);

    $item = menu_get_item("node/{$this->node->nid}/share");
    $this->assertEqual(self::ENTITY_CALLBACK, $item['theme_callback']);

    $item = menu_get_item("node/{$this->node->nid}/continue");
    $this->assertEqual(self::ENTITY_CALLBACK, $item['theme_callback']);

    $item = menu_get_item("node/{$this->node->nid}/edit");
    $this->assertEqual(self::NO_CALLBACK, $item['theme_callback']);

    $item = menu_get_item("node/{$this->node->nid}/webform");
    $this->assertEqual(self::NO_CALLBACK, $item['theme_callback']);
  }

}
