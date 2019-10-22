<?php

namespace Drupal\campaignion\CRM\Import\Field;

require_once dirname(__FILE__) . '/RedhenEntityTest.php';

use Drupal\campaignion\CRM\Import\Source\ArraySource;

/**
 * Test the phone importer.
 */
class PhoneTest extends RedhenEntityTest {

  /**
   * Create the test importer.
   */
  public function setUp() {
    parent::setUp();
    $this->importer = new Phone('field_phone_number');
  }

  /**
   * Shortcut for importing data into a contact.
   */
  protected function import($data, \EntityMetadataWrapper $contact = NULL) {
    return $this->importer->import(new ArraySource($data), $contact ?? $this->contact);
  }

  /**
   * Test importing a single value.
   */
  public function testPhoneImport1Value() {
    $data['field_phone_number'] = '+43 664 87592345';
    $this->assertTrue($this->import($data));
    $this->assertEqual([$data['field_phone_number']], $this->contact->field_phone_number->value());
  }

  /**
   * Test importing two different phone numbers.
   */
  public function testPhoneImport2Value() {
    $data[]['field_phone_number'] = '+43 664 87592345';
    $this->assertTrue($this->import($data[0]));
    $data[]['field_phone_number'] = '+43 664 87592333';
    $this->assertTrue($this->import($data[1]));
    $this->assertEqual([
      $data[1]['field_phone_number'],
      $data[0]['field_phone_number'],
    ], $this->contact->field_phone_number->value());
    $data[]['field_phone_number'] = '+43 664 87592345';
    $this->assertTrue($this->import($data[0]));
    $this->assertEqual([
      $data[0]['field_phone_number'],
      $data[1]['field_phone_number'],
    ], $this->contact->field_phone_number->value());
  }

  /**
   * Test importing the same phone number twice.
   */
  public function testIdenticalPhone() {
    $data[]['field_phone_number'] = '+43 664 87592345';
    $this->assertTrue($this->import($data[0]));
    $data[]['field_phone_number'] = '+43 664 87592345  ';
    $this->assertFalse($this->import($data[1]));
    $this->assertEqual([$data[0]['field_phone_number']], $this->contact->field_phone_number->value());
  }

}
