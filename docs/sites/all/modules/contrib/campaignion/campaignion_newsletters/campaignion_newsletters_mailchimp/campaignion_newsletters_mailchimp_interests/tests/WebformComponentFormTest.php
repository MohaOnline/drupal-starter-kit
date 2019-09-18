<?php

use Drupal\campaignion_newsletters\NewsletterList;
use Upal\DrupalUnitTestCase;

/**
 * Test the webform component edit form modifications for opt_in components.
 */
class WebformComponentFormTest extends DrupalUnitTestCase {

  /**
   * Create test node and list.
   */
  public function setUp() {
    parent::setUp();
    module_load_include('inc', 'webform', 'includes/webform.components');
    $this->node = (object) ['type' => 'webform', 'title' => __CLASS__];
    node_save($this->node);
    $this->list = NewsletterList::fromData([
      'source' => 'test',
      'identifier' => 'test',
      'title' => 'Test list',
    ]);
    $this->list->data = (object) [];
    $this->list->data->groups = [
      'g1' => 'Group1',
    ];
    $this->list->save();
  }

  /**
   * Remove test node and list.
   */
  public function tearDown() {
    $this->list->delete();
    node_delete($this->node->nid);
    parent::tearDown();
  }

  /**
   * Test that interest group options are rendered and defaults are set.
   */
  public function testRenderForm() {
    $list_id = $this->list->list_id;
    $component['type'] = 'opt_in';
    $component['extra']['mc_groups'][$list_id]['g1'] = 'g1';
    webform_component_defaults($component);
    $form = drupal_get_form('webform_component_edit_form', $this->node, $component);
    $this->assertNotEmpty($form['mc_groups'][$list_id]);
    $checkboxes = $form['mc_groups'][$list_id];
    $this->assertEqual(['g1' => 'g1'], $checkboxes['#default_value']);
    $this->assertArrayHasKey('g1', $checkboxes['#options']);
  }

}
