<?php

namespace Drupal\campaignion\CRM\Import\Field;

require_once dirname(__FILE__) . '/RedhenEntityTest.php';

use \Drupal\campaignion\CRM\Import\Source\ArraySource;

/**
 * Test boolean opt-in importer.
 */
class BooleanOptInTest extends RedhenEntityTest {

  /**
   * Prepare importer and redhen contact.
   */
  public function setUp() : void {
    parent::setUp();
    $this->importer = new BooleanOptIn('field_opt_in_phone', 'phone_opt_in');
    $this->entity = $this->newRedhenContact();
  }

  /**
   * Test importing an opt-in.
   */
  public function testImportOptIn() {
    $this->importer->import(new ArraySource(['phone_opt_in' => 'checkbox:opt-in']), $this->entity);
    $this->assertTrue($this->entity->field_opt_in_phone->value());
  }

  /**
   * Test importing an opt-out.
   */
  public function testImportOptOut() {
    $this->importer->import(new ArraySource(['phone_opt_in' => 'radios:opt-out']), $this->entity);
    $this->assertFalse($this->entity->field_opt_in_phone->value());
  }

  /**
   * Test importing without value.
   */
  public function testImportNoChange() {
    $this->entity->field_opt_in_phone->set(TRUE);
    $this->importer->import(new ArraySource(['phone_opt_in' => 'radios:no-change']), $this->entity);
    $this->assertTrue($this->entity->field_opt_in_phone->value());
  }

}
