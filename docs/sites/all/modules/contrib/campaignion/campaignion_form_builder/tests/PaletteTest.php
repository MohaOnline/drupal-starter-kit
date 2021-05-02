<?php

namespace Drupal\campaignion_form_builder;

use Drupal\form_builder\Loader;

/**
 * Test for additional fields we’ve put into the form builder palette.
 */
class PaletteTest extends \DrupalUnitTestCase {

  /**
   * Create a test node.
   */
  public function setUp() : void {
    parent::setUp();
    $node = (object) ['type' => 'webform'];
    entity_save('node', $node);
    $this->node = $node;
  }

  /**
   * Remove test node.
   */
  public function tearDown() : void {
    entity_delete('node', $this->node->nid);
    parent::tearDown();
  }

  /**
   * Test whether opt-in fields are added.
   */
  public function testOptInFields() {
    $form_type = 'webform';
    $loader = Loader::instance();
    $fields = $loader->getElementTypeInfo('webform', $this->node->nid);

    $this->assertArrayHasKey('post_opt_in', $fields);
    $this->assertArrayHasKey('phone_opt_in', $fields);
  }

}
