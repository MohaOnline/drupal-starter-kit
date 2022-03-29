<?php

/**
 * @file
 * Unit tests for the Drupal\akamai\Batch class.
 */

namespace Drupal\akamai\Tests;

use Drupal\akamai\Batch;

class BatchTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests that items are added to a batch correctly.
   *
   * @covers Drupal\akamai\Batch::addItem
   */
  public function testAddItem() {
    $hostname = 'www.example.com';
    $num_items = 3;
    $items = [];

    $batch = new Batch();

    for ($i = 1; $i <= $num_items; $i++) {
      $item = (object) [
        'item_id' => $i,
        'data' => [
          'hostname' => $hostname,
          'paths' => ['node/' . $i],
        ],
      ];
      $items[] = $item;
      $batch->addItem($item);
    }

    $this->assertSame($batch->getHostname(), $hostname, 'Hostname of batch did not match.');
    $this->assertSame($batch->getItems(), $items, 'Batch items did not match.');
  }

  /**
   * Verifies that an InvalidArgumentException is thrown when expected.
   *
   * An exception should be thrown when adding items with differing hostnames.
   *
   * @covers Drupal\akamai\Batch::addItem
   */
  public function testAddItemException() {
    $this->setExpectedException('InvalidArgumentException');

    // Attempt to add two items with differing hostnames.
    $num_items = 2;
    $items = [];

    $batch = new Batch();

    for ($i = 1; $i <= $num_items; $i++) {
      $item = (object) [
        'item_id' => $i,
        'data' => [
          'hostname' => "www{$i}.example.com",
          'paths' => ['node/' . $i],
        ],
      ];
      $items[] = $item;
      $batch->addItem($item);
    }
  }

}