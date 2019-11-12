<?php

namespace Drupal\campaignion_wizard;

use Drupal\campaignion_action\Loader;
use Upal\DrupalUnitTestCase;

/**
 * Test rendering node forms.
 */
class NodeFormTest extends DrupalUnitTestCase {

  /**
   * Test that the node form for content can be initialized.
   */
  public function testNodeContent() {
    $form = campaignion_wizard_new('petition');
    $this->assertNotEmpty($form[0]['title']);
    $this->assertNotEmpty($form[0]['webform']);
    $this->assertNotEmpty($form[0]['wizard_secondary']);
  }

  /**
   * Test that the node form for an action type is not modified by save_draft.
   */
  public function testSaveDraftDisabledOnActionTypes() {
    module_load_include('pages.inc', 'node');
    $node = (object) ['type' => 'petition'];
    node_object_prepare($node);
    $this->assertTrue(Loader::instance()->isActionType($node->type));
    $this->assertTrue(module_exists('save_draft'));
    $form = drupal_get_form('petition_node_form', $node);
    $this->assertNotEmpty($form['options']['status']['#type']);
    $this->assertArrayNotHasKey('draft', $form['actions']);
  }

}
