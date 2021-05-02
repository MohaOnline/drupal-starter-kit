<?php

namespace Drupal\campaignion\Test\CRM\Export;

use Drupal\campaignion\CRM\ExporterBase;
use Drupal\campaignion\CRM\Export\BooleanField;

/**
 * Test for the boolean field exporter.
 */
class BooleanFieldTest extends \DrupalUnitTestCase {

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
   * Test exporting with default values.
   */
  public function testDefaultValues() {
    $contact = $this->exporter->getWrappedContact();
    $w = new BooleanField('field_opt_in_phone');
    $w->setExporter($this->exporter);

    $contact->field_opt_in_phone->set(TRUE);
    $this->assertEqual('Yes', $w->value());

    $contact->field_opt_in_phone->set(FALSE);
    $this->assertEqual('No', $w->value());
  }

  /**
   * Test exporting with custom values.
   */
  public function testCustomValues() {
    $contact = $this->exporter->getWrappedContact();
    $w = new BooleanField('field_opt_in_phone', [1, -1]);
    $w->setExporter($this->exporter);

    $contact->field_opt_in_phone->set(TRUE);
    $this->assertEqual(1, $w->value());

    $contact->field_opt_in_phone->set(FALSE);
    $this->assertEqual(-1, $w->value());
  }

  /**
   * Test that no value gives the same value as FALSE.
   */
  public function testEmptyValue() {
    $w = new BooleanField('field_opt_in_phone');
    $w->setExporter($this->exporter);
    $this->assertEqual('No', $w->value());
  }

}
