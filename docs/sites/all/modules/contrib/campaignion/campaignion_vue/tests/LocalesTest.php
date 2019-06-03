<?php

namespace Drupal\campaignion_vue;

/**
 * Test loading locales.
 */
class LocalesTest extends \DrupalUnitTestCase {

  /**
   * Test loading a country specific locale with fallback.
   */
  public function testCountryLocaleFallback() {
    $stub_language = (object) ['language' => 'de-AT'];
    $json = _campaignion_vue_get_strings($stub_language);
    $this->assertNotEmpty($json);
    $strings = json_decode($json, TRUE);
    $this->assertEqual('Keine Einträge', $strings['el']['tree']['emptyText']);
  }

}
