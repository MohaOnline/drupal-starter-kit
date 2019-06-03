<?php

namespace Drupal\campaignion\CRM\Import\Field;
require_once dirname(__FILE__) . '/RedhenEntityTest.php';

use \Drupal\campaignion\CRM\Import\Source\ArraySource;

class PhoneTest extends RedhenEntityTest {
  public function setUp() {
    parent::setUp();
    $this->importer = new Phone('field_phone_number');
  }

  public function testPhoneImport1Value() {
    $data['field_phone_number'] = '+43 664 87592345';
    $this->importer->import(new ArraySource($data), $this->contact, TRUE);
    $this->assertEqual(array($data['field_phone_number']), $this->contact->field_phone_number->value());
  }

  public function testPhoneImport2Value() {
    $data[]['field_phone_number'] = '+43 664 87592345';
    $this->importer->import(new ArraySource($data[0]), $this->contact, TRUE);
    $data[]['field_phone_number'] = '+43 664 87592333';
    $this->importer->import(new ArraySource($data[1]), $this->contact, TRUE);
    $this->assertEqual(array($data[0]['field_phone_number'], $data[1]['field_phone_number']), $this->contact->field_phone_number->value());
  }

  public function testIdenticalPhone() {
    $data[]['field_phone_number'] = '+43 664 87592345';
    $this->importer->import(new ArraySource($data[0]), $this->contact, TRUE);
    $data[]['field_phone_number'] = '+43 664 87592345  ';
    $this->importer->import(new ArraySource($data[1]), $this->contact, TRUE);
    $this->assertEqual(array($data[0]['field_phone_number']), $this->contact->field_phone_number->value());
  }
}