<?php

namespace Drupal\campaignion\Test\CRM\Export;

use Drupal\campaignion\CRM\ExporterBase;
use Drupal\campaignion\CRM\Export\FixedValue;
use Upal\DrupalUnitTestCase;

/**
 * Test for the fixed value exporter.
 */
class FixedValueTest extends DrupalUnitTestCase {

  /**
   * Prepare a contact and exporter object for testing.
   *
   * We use the contact type provided by the campaignion_test module here.
   */
  public function setUp() : void {
    parent::setUp();
    $this->contact = entity_create('redhen_contact', ['type' => 'contact']);
    $this->exporter = new ExporterBase([]);
    $this->exporter->setContact($this->contact);
  }

  /**
   * Test exporting values.
   */
  public function testValues() {
    $w = new FixedValue('Label', ['value', 'two']);
    $w->setExporter($this->exporter);
    $this->assertEqual(['value', 'two'], $w->value());
    $this->assertEqual('two', $w->value(1));
  }

  /**
   * Test CSV header.
   */
  public function testHeader() {
    $w = new FixedValue('Label', 'value');
    $w->setExporter($this->exporter);
    $this->assertEqual('Label', $w->header(0));
    $this->assertEqual('', $w->header(1));
  }

}
