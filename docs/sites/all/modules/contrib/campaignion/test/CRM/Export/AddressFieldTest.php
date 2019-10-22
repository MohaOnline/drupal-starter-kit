<?php

namespace Drupal\campaignion\CRM\Exporter;

use Drupal\campaignion\CRM\ExporterBase;
use Drupal\campaignion\CRM\Export\AddressField;
use Upal\DrupalUnitTestCase;

/**
 * Test the address field exporter.
 */
class AddressFieldTest extends DrupalUnitTestCase {

  /**
   * Prepare a contact and exporter object for testing.
   *
   * We use the contact type provided by the campaignion_test module here.
   */
  public function setUp() {
    parent::setUp();
    $this->contact = entity_create('redhen_contact', ['type' => 'contact']);
    $this->exporter = new ExporterBase([]);
  }

  /**
   * Test exporting with default values.
   */
  public function testDefaultValues() {
    $w = new AddressField('field_address');
    $w->setExporter($this->exporter);
    $contact = $this->contact->wrap();
    $addresses[] = [
      'country' => 'AT',
    ];
    $contact->field_address->set($addresses);

    $this->exporter->setContact($this->contact);
    $this->assertEqual('AT', $w->value()['country']);
  }

}
