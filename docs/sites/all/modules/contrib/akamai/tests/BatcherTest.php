<?php

/**
 * @file
 * Unit tests for the Drupal\akamai\Batcher class.
 */

namespace Drupal\akamai\Tests;

use Akamai\Open\EdgeGrid\Client;
use GuzzleHttp\Psr7\Response;
use Drupal\akamai\Ccu3Client;
use Drupal\akamai\Batcher;

class BatcherTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests basic batching functionality.
   *
   * @covers Drupal\akamai\Batcher::getBatch
   */
  public function testGetBatch() {
    $hostname = 'www.example.com';
    $paths = ['node/1'];

    $batcher = $this->getMockBuilder('\Drupal\akamai\Batcher')
      ->disableOriginalConstructor()
      ->setMethods(['itemWillFitInRequest'])
      ->getMock();
    $batcher->method('itemWillFitInRequest')
         ->willReturn(TRUE);

    $item = (object) [
      'item_id' => 1,
      'data' => [
        'hostname' => $hostname,
        'paths' => $paths,
      ],
    ];
    $batcher->insertItem($item);
    $batch = $batcher->getBatch();
    $this->assertSame($batch->getHostname(), $hostname, 'Hostname of batch did not match.');
    $this->assertSame($batch->getPaths(), $paths, 'Batch paths did not match.');
    $this->assertSame($batch->getItems(), [$item], 'Batch items did not match.');
    $this->assertTrue($batcher->isEmpty(), 'Expected empty set of items after batch was created.');
  }

}