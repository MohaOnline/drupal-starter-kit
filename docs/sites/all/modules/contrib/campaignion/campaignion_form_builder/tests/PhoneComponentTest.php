<?php

use Upal\DrupalUnitTestCase;

/**
 * Tests for the phone webform component.
 */
class PhoneComponentTest extends DrupalUnitTestCase {

  public function testHasValidator() {
    foreach (webform_validation_get_validators() as $info) {
      if (in_array('textfield', $info['component_types'])) {
        $this->assertContains('phone_number', $info['component_types'], 'Found a validation that’s enabled for textfields but not for phone numbers.');
      }
    }
  }

}
