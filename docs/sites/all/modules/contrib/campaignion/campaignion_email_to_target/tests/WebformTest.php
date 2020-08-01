<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\campaignion_action\Loader;
use Drupal\little_helpers\Services\Container;
use Upal\DrupalUnitTestCase;

use Drupal\campaignion_email_to_target\Api\Client;

/**
 * Test webform component callbacks.
 */
class WebformTest extends DrupalUnitTestCase {

  /**
   * Create test node.
   */
  public function setUp() {
    parent::setUp();
    Container::get()->inject('campaignion_email_to_target.api.Client', $this->createMock(Client::class));
    $node = (object) [
      'type' => 'email_to_target',
    ];
    node_object_prepare($node);
    $node->field_email_to_target_options['und'][0] = [
      'dataset_name' => 'mp',
    ];
    entity_save('node', $node);
    $this->node = entity_load_single('node', $node->nid);
  }

  /**
   * Delete test node.
   */
  public function tearDown() {
    drupal_static_reset(Container::class);
    entity_delete('node', $this->node->nid);
    parent::tearDown();
  }

  /**
   * Test getting CSV header for a MP dataset.
   */
  public function testMpColumns() {
    $component['name'] = 'Component name';
    $component['nid'] = $this->node->nid;
    $cols = webform_component_invoke('e2t_selector', 'csv_headers', $component, []);
    $this->assertEqual(['', '', '', '', '', '', ''], $cols[0]);
    $this->assertEqual(['Component name', '', '', '', '', '', ''], $cols[1]);
    $this->assertEqual([
      'To',
      'Subject',
      'Message',
      'Constituency',
      'Target salutation',
      'Party',
      'Devolved country',
    ], $cols[2]);
  }

  /**
   * Test getting CSV header for a non-MP dataset.
   */
  public function testNonMpColumns() {
    $this->node->field_email_to_target_options['und'][0]['dataset_name'] = 'other';
    $component['name'] = 'Component name';
    $component['nid'] = $this->node->nid;
    $cols = webform_component_invoke('e2t_selector', 'csv_headers', $component, []);
    $this->assertEqual('', $cols[0]);
    $this->assertEqual('', $cols[1]);
    $this->assertEqual('Component name', $cols[2]);
  }

}
