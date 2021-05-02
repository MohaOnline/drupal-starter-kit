<?php

namespace Drupal\campaignion\CRM;

use Drupal\campaignion\ContactTypeManager;
use Drupal\campaignion\CRM\Export\Label;
use Drupal\campaignion\CRM\Export\LabelFactory;
use Drupal\campaignion\CRM\Export\SingleValueField;
use Drupal\campaignion\CRM\Export\SubField;
use Drupal\campaignion\CRM\Export\WrapperField;
use Drupal\campaignion\CRM\Import\Source\ArraySource;

/**
 * Integration tests for the CsvExporter.
 */
class CsvExporterTest extends \DrupalUnitTestCase {

  /**
   * Create test contact.
   */
  public function setUp() : void {
    parent::setUp();

    $importer = ContactTypeManager::instance()->importer('campaignion_action_taken');
    $source = new ArraySource([
      'email' => 'csvexporter@test.com',
      'country' => 'AT',
      'street_address' => 'Hütteldorferstaße 253a',
    ]);
    $this->contact = $importer->findOrCreateContact($source);
    $importer->import($source, $this->contact);
    $this->contact->save();
  }

  /**
   * Delete test contact.
   */
  public function tearDown() : void {
    $this->contact->delete();
    parent::tearDown();
  }

  /**
   * Test exporting a single contact and the headers.
   */
  public function testExportOneContact() {
    $labels = new LabelFactory('redhen_contact', 'contact');
    $address = $labels->fromExporter(new WrapperField('field_address'));

    $map['contact_id']                 = new Label('Contact ID', new SingleValueField('contact_id'));
    $map['field_address.thoroughfare'] = new SubField($address, 'thoroughfare', 'Address line 1');

    $exporter = new CsvExporter($map);
    $this->assertEqual([
      'contact_id' => 'Contact ID',
      'field_address.thoroughfare' => 'Address',
    ], $exporter->header(0));
    $this->assertEqual([
      'contact_id' => '',
      'field_address.thoroughfare' => 'Address line 1',
    ], $exporter->header(1));
    $exporter->setContact($this->contact);
    $this->assertEqual([
      'contact_id' => $this->contact->contact_id,
      'field_address.thoroughfare' => 'Hütteldorferstaße 253a',
    ], $exporter->row());
  }

  /**
   * Test exporting a single contact and the headers.
   */
  public function testExportOneContactFiltered() {
    $labels = new LabelFactory('redhen_contact', 'contact');
    $address = $labels->fromExporter(new WrapperField('field_address'));

    $map['contact_id']                 = new Label('Contact ID', new SingleValueField('contact_id'));
    $map['field_address.thoroughfare'] = new SubField($address, 'thoroughfare', 'Address line 1');
    $exporter = new CsvExporter($map);

    $this->assertEqual([
      'contact_id' => 'Contact ID',
      'field_address.thoroughfare' => 'Address (Address line 1)',
    ], $exporter->columnOptions());

    $filter['contact_id'] = 'contact_id';
    $exporter->filterColumns($filter);

    $this->assertEqual([
      'contact_id' => 'Contact ID',
    ], $exporter->header(0));
    $this->assertEqual([
      'contact_id' => '',
    ], $exporter->header(1));
    $exporter->setContact($this->contact);
    $this->assertEqual([
      'contact_id' => $this->contact->contact_id,
    ], $exporter->row());
  }

}
