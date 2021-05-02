<?php

namespace Drupal\campaignion\CRM\Import\Field;

require_once dirname(__FILE__) . '/RedhenEntityTest.php';
use Drupal\campaignion\CRM\Import\Source\ArraySource;

/**
 * Tests for the address field importer.
 */
class AddressTest extends RedhenEntityTest {

  static protected $mapping = array(
    'thoroughfare' => 'street_address',
    'postal_code' => 'zip_code',
    'locality' => 'state',
    'country' => 'country',
  );

  static protected $testdataAT = array(
    'street_address' => 'Hütteldorferstraße 253',
    'zip_code' => '1140',
    'state' => 'Wien',
    'country' => 'AT',
  );

  static protected $testdataUK = [
    'street_address' => '34b York Way, King’s Cross',
    'zip_code' => 'N1 9AB',
    'city' => 'London',
    'country' => 'GB',
  ];

  /**
   * Map an array of data from form keys to address fields format.
   *
   * @param string[] $data
   *   Address with form keys.
   *
   * @return string[]
   *   Address field item.
   */
  protected static function mapped(array $data) {
    $mapped = array();
    foreach (self::$mapping as $field_key => $data_key) {
      if (isset($data[$data_key])) {
        $mapped[$field_key] = $data[$data_key];
      }
    }
    if (!isset($mapped['country']) && ($c = variable_get('site_default_country', ''))) {
      $mapped['country'] = $c;
    }
    return $mapped;
  }

  /**
   * Extract some keys from an array.
   *
   * @param array $data
   *   An associative array.
   * @param array $keys
   *   Keys that should be extracted.
   *
   * @return array
   *   An array with all keys from $data that are also in $keys.
   */
  protected static function filtered(array $data, array $keys) {
    return array_intersect_key($data, array_flip($keys));
  }

  /**
   * Set up test data.
   */
  public function setUp() : void {
    parent::setUp();
    $this->importer = new Address('field_address', self::$mapping);
    $this->fakeContact = $this->createMock('EntityMetadataWrapper');
    $this->fakeContact->field_address = $this->contact->field_address[0];
  }

  /**
   * Shortcut for importing data into a single address field.
   */
  protected function importSingle($data) {
    return $this->import($data, $this->fakeContact);
  }

  /**
   * Shortcut for importing data into a contact.
   */
  protected function import($data, \EntityMetadataWrapper $contact = NULL) {
    return $this->importer->import(new ArraySource($data), $contact ?? $this->contact);
  }

  /**
   * Test importing a full address.
   */
  public function testWithAllFields() {
    $data = self::$testdataAT;
    $this->import($data);
    $this->assertEqual($this->mapped($data), $this->contact->field_address->value()[0]);
  }

  /**
   * Test importing only the country field.
   */
  public function testWithOnlyCountry() {
    $data = self::filtered(self::$testdataAT, ['country']);
    $this->import($data);
    $this->assertEqual($this->mapped($data), $this->contact->field_address->value()[0]);
  }

  /**
   * Test importing only the locality.
   */
  public function testWithOnlyLocality() {
    $data = self::filtered(self::$testdataAT, ['street_address']);
    $this->import($data);
    $this->assertEqual($this->mapped($data), $this->contact->field_address->value()[0]);
  }

  /**
   * Test return value for identical imports.
   */
  public function testIdenticalImportsReturnFalse() {
    $this->assertTrue($this->import(self::$testdataAT), 'Import into new contact returned FALSE intead of TRUE.');
    $this->assertFalse($this->import(self::$testdataAT), 'Import of identical datat returned TRUE twice.');
    $data = self::filtered(self::$testdataAT, ['street_address']);
    $this->assertFalse($this->import($data), 'Import of identical street_address/throughfare returned TRUE instead of FALSE.');
  }

  /**
   * Test single-value field with full address.
   */
  public function testSingleFullAddress() {
    // Import full address.
    $this->assertTrue($this->importSingle(self::$testdataAT));
    $expected = $this->mapped(self::$testdataAT);
    $this->assertEqual($expected, $this->contact->field_address->value()[0]);

    // Setting again should not change anything.
    $this->assertFalse($this->importSingle(self::$testdataAT));
  }

  /**
   * Test adding new data to existing address.
   */
  public function testSingleChangeAddress() {
    // Import only country.
    $data = self::filtered(self::$testdataAT, ['country']);
    $expected = $this->mapped($data);
    $this->assertTrue($this->importSingle($data));
    $this->assertEqual($expected, $this->contact->field_address->value()[0]);

    // Add rest of the address data.
    $expected = $this->mapped(self::$testdataAT);
    $this->assertTrue($this->importSingle(self::$testdataAT));
    $this->assertEqual($expected, $this->contact->field_address->value()[0]);
  }

  /**
   * Test importing address with multiple spaces.
   */
  public function testImportMultipleSpaces() {
    $data['street_address'] = 'Multiple  spaces ';
    $expected = $this->mapped(['street_address' => 'Multiple spaces']);
    $this->assertTrue($this->import($data));
    $this->assertEqual($expected, $this->contact->field_address->value()[0]);
  }

  /**
   * Test importing multiple addresses in turn.
   */
  public function testImportMultipleAdresses() {
    $full_at = self::$testdataAT;
    $partial_at = self::filtered(self::$testdataAT, ['country']);
    $full_uk = self::$testdataUK;
    $partial_uk = self::filtered(self::$testdataUK, ['country']);

    // New address should be at the top.
    $this->assertTrue($this->import($partial_at));
    $this->assertEqual('AT', $this->contact->field_address->value()[0]['country']);

    // New address should be at the top.
    $this->assertTrue($this->import($partial_uk));
    $this->assertEqual('GB', $this->contact->field_address->value()[0]['country']);
    $this->assertEqual('AT', $this->contact->field_address->value()[1]['country']);

    // Updated address should be at the top.
    $this->assertTrue($this->import($full_at));
    $this->assertEqual('AT', $this->contact->field_address->value()[0]['country']);

    // Updated address should be at the top.
    $this->assertTrue($this->import($full_uk));
    $this->assertEqual('GB', $this->contact->field_address->value()[0]['country']);

    // Re-confirmed address should be at the top even if nothing changed for it.
    $this->assertTrue($this->import($partial_at));
    $this->assertEqual('AT', $this->contact->field_address->value()[0]['country']);

    // Re-confirming the top address shouldn’t change anything.
    $this->assertFalse($this->import($partial_at));
  }

}
