<?php

namespace Drupal\campaignion\CRM\Import\Field;
require_once dirname(__FILE__) . '/RedhenEntityTest.php';

use \Drupal\campaignion\CRM\Import\Source\ArraySource;

class DateTest extends RedhenEntityTest {
  protected function import($string) {
    $field = 'field_date_of_birth';
    $importer = new Date($field);
    $data[$field] = $string;
    $entity = $this->newRedhenContact();
    $importer->import(new ArraySource($data), $entity, TRUE);
    return $entity->{$field}->value();
  }

  function testValidBirthDate() {
    $string = '1988-10-22';
    $imported = strftime('%Y-%m-%d', (int) $this->import($string));
    $this->assertEqual($string, $imported);
  }

  function testCampaignionFormat() {
    $string = '01/04/1976';
    $imported = strftime('%d/%m/%Y', (int) $this->import($string));
    $this->assertEqual($string, $imported);
  }
  
  function testInValidBirthDate_IsSetAsNull() {
    $string = '1988;55yz10-22';
    $this->assertNull($this->import($string));
  }

}
