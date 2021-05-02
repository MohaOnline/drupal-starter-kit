<?php

namespace Drupal\campaignion\CRM\Export;

use Drupal\campaignion\CRM\ExporterBase;

/**
 * Test the WrapperField exporter.
 */
class WrapperFieldTest extends \DrupalUnitTestCase {

  /**
   * Prepare a contact and exporter object for testing.
   *
   * We use the contact type provided by the campaignion_test module here.
   */
  public function setUp() : void {
    parent::setUp();
    $this->contact = entity_create('redhen_contact', ['type' => 'contact']);
    $w = $this->contact->wrap();
    $w->field_address->set([[
      'country' => 'GB',
    ]]);
    $w->field_gender->set('o');
    $this->exporter = new ExporterBase([]);
    $this->exporter->setContact($this->contact);
  }

  /**
   * Test getting values from a multi-value field.
   */
  public function testMultiValueField() {
    $w = new WrapperField('field_address');
    $w->setExporter($this->exporter);
    $this->assertEqual(['country' => 'GB'], $w->value());
    $this->assertEqual(['country' => 'GB'], $w->value(0));
    $this->assertEqual([['country' => 'GB']], $w->value(NULL));
    $this->assertNull($w->value(1));
  }

  /**
   * Test getting values from a single value field.
   */
  public function testSingleValueField() {
    $w = new WrapperField('field_gender');
    $w->setExporter($this->exporter);
    $this->assertEqual('o', $w->value());
    $this->assertEqual('o', $w->value(0));
    $this->assertEqual(['o'], $w->value(NULL));
    $this->assertEqual('o', $w->value(1));
  }

  /**
   * Test non-existing field.
   */
  public function testNonExistingField() {
    $w = new WrapperField('field_doesnotexist');
    $w->setExporter($this->exporter);
    $this->assertEqual(NULL, $w->value(0));
    $this->assertEqual([], $w->value(NULL));
  }

}
