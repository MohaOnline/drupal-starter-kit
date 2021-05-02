<?php

require_once drupal_get_path('module', 'webform') . '/includes/webform.components.inc';

/**
 * Test rendering a overlay field collection.
 */
class OverlayTest extends \DrupalUnitTestCase {

  /**
   * Create test nodes.
   */
  public function setUp() : void {
    parent::setUp();
    $this->petition = entity_create('node', ['type' => 'petition']);
    $this->petition->webform = webform_node_defaults();
    $component['type'] = 'email';
    webform_component_defaults($component);
    $this->petition->webform['components'][1] = $component;
    node_save($this->petition);
    $this->node = entity_create('node', ['type' => 'thank_you_page']);
    node_save($this->node);
  }

  /**
   * Delete test nodes.
   */
  public function tearDown() : void {
    node_delete($this->node->nid);
    node_delete($this->petition->nid);
    parent::tearDown();
  }

  /**
   * Add overlay options to a node and test rendering the node.
   */
  public function testRenderOverlay() {
    $w_node = entity_metadata_wrapper('node', $this->node);
    $item = entity_create('field_collection_item', ['field_name' => 'campaignion_overlay_options']);
    $w_item = entity_metadata_wrapper('field_collection_item', $item);
    $w_item->campaignion_overlay_enabled->set(1);
    $w_item->campaignion_overlay_content->set($this->petition->nid);
    $w_item->campaignion_overlay_introduction->set(['value' => 'test intro']);
    $item->setHostEntity('node', $this->node);
    $w_node->save();

    $output = field_view_field('node', $this->node, 'campaignion_overlay_options');
    $items = reset($output[0]['entity']);
    $build = reset($items);
    $this->assertEqual('campaignion_overlay_options', $build['#theme']);
  }

}
