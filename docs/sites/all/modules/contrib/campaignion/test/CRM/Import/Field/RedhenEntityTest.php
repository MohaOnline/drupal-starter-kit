<?php

namespace Drupal\campaignion\CRM\Import\Field;

abstract class RedhenEntityTest extends \DrupalUnitTestCase {

  public function setUp() : void {
    parent::setUp();
    $this->contact = $this->newRedhenContact();
  }

  protected function newRedhenContact() {
    $contact = new \RedhenContact(array('type' => 'contact'));
    return entity_metadata_wrapper($contact->entityType(), $contact);
  }

}
