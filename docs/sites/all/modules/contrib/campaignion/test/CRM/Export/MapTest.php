<?php

namespace Drupal\campaignion\CRM\Export;

use Upal\DrupalUnitTestCase;

/**
 * Test the map export wrapper.
 */
class MapTest extends DrupalUnitTestCase {

  /**
   * Create a mock exporter.
   */
  protected function mockExporter() {
    return $this->createMock(ExportMapperInterface::class);
  }

  /**
   * Test mapped values.
   */
  public function testMappedValue() {
    $map = [
      1 => 'one',
      2 => 'two',
    ];
    $wrapped = $this->mockExporter();
    $map = new Map($wrapped, $map);

    $wrapped->method('value')->will($this->onConsecutiveCalls(1, 2, 3));
    $this->assertEqual('one', $map->value());
    $this->assertEqual('two', $map->value());
    $this->assertEqual(NULL, $map->value());
  }

}

