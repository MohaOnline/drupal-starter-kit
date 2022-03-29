<?php

/**
 * @file
 * Unit tests for the Drupal\akamai\Ccu3Client class.
 */

namespace Drupal\akamai\Tests;

use Akamai\Open\EdgeGrid\Client;
use GuzzleHttp\Psr7\Response;
use Drupal\akamai\Ccu3Client;

class ClientTest extends \PHPUnit_Framework_TestCase {

  /**
   * Tests the checkProgress method.
   *
   * @covers Drupal\akamai\Ccu3Client::checkProgress
   */
  public function testCheckProgress() {
    $progress_uri = '/ccu/v2/purges/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx';

    // Create stub for response class.
    $response_stub = $this->getMockBuilder('GuzzleHttp\Psr7\Response')
      ->disableOriginalConstructor()
      ->getMock();
    $response_stub->method('getBody')
      ->willReturn('{
        "originalEstimatedSeconds": 480,
        "progressUri": "/ccu/v2/purges/xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
        "originalQueueLength": 6,
        "purgeId": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
        "supportId": "xxxxxxxxxxxxxxxxxxxx-xxxxxxxxx",
        "httpStatus": 200,
        "completionTime": null,
        "submittedBy": "test1",
        "purgeStatus": "In-Progress",
        "submissionTime": "2014-02-19T21:16:20Z",
        "pingAfterSeconds": 60
      }');

    // Create stub for the EdgeGrid client class.
    $edgegrid_client = $this->getMockBuilder('\Akamai\Open\EdgeGrid\Client')
      ->disableOriginalConstructor()
      ->setMethods(['get'])
      ->getMock();
    $edgegrid_client->method('get')
         ->willReturn($response_stub);

    // Ensure that the `getBody` method of the response object is called.
    $response_stub->expects($this->once())
      ->method('getBody');

    // Ensure that the `get` method of the EdgeGrid client is called.
    $edgegrid_client->expects($this->once())
      ->method('get')
      ->with($this->equalTo($progress_uri));

    $ccu_client = new Ccu3Client($edgegrid_client);
    $result = $ccu_client->checkProgress($progress_uri);
    $this->assertSame($result->progressUri, $progress_uri, 'Method checkProgress did not return decoded JSON response.');
  }

  /**
   * Tests the postPurgeRequest method.
   *
   * @covers Drupal\akamai\Ccu3Client::postPurgeRequest
   */
  public function testPostPurgeRequest() {
    // Setup purge request parameters.
    $hostname = 'www.example.com';
    $path = '/robots.txt';
    $operation = 'invalidate';

    // Create stub for response class.
    $response_stub = $this->getMockBuilder('GuzzleHttp\Psr7\Response')
      ->disableOriginalConstructor()
      ->getMock();
    $response_stub->method('getBody')
      ->willReturn('{
        "estimatedSeconds": 5,
        "purgeId": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
        "supportId": "xxxxxxxxxxxxxxxxxxxx-xxxxxxxxx",
        "httpStatus": 201,
        "detail": "Request accepted"
      }');

    // Create stub for the EdgeGrid client class.
    $edgegrid_client = $this->getMockBuilder('\Akamai\Open\EdgeGrid\Client')
      ->disableOriginalConstructor()
      ->setMethods(['post'])
      ->getMock();
    $edgegrid_client->method('post')
         ->willReturn($response_stub);

    // Ensure that the `getBody` method of the response object is called.
    $response_stub->expects($this->once())
      ->method('getBody');

    // Ensure that the `post` method of the EdgeGrid client is called.
    // Ensure that the `Content-Type: application/json` header is set.
    // Also verify that the payload is encoded as JSON and contains the
    // expected 'hostname' and 'objects' parameters.
    $edgegrid_client->expects($this->once())
      ->method('post')
      ->with(
        $this->equalTo("/ccu/v3/{$operation}/url/production"),
        $this->callback(function($payload) use ($hostname, $path){
          if (!isset($payload['body'])) {
            return FALSE;
          }
          if (!isset($payload['headers']) && !isset($payload['headers']['Content-Type'])) {
            return FALSE;
          }
          if ($payload['headers']['Content-Type'] != 'application/json') {
            return FALSE;
          }
          $decoded = json_decode($payload['body'], TRUE);
          if (empty($decoded) || !is_array($decoded)) {
            return FALSE;
          }
          if (!isset($decoded['hostname']) || !isset($decoded['objects'])) {
            return FALSE;
          }
          return $decoded['hostname'] == $hostname && in_array($path, $decoded['objects']);
        })
      );

    $ccu_client = new Ccu3Client($edgegrid_client);
    $result = $ccu_client->postPurgeRequest($hostname, [$path], 'invalidate');
    $this->assertSame($result->estimatedSeconds, 5, 'Method postPurgeRequest did not return decoded JSON response.');
  }

  /**
   * Tests the bodyIsBelowLimit method.
   *
   * @covers Drupal\akamai\Ccu3Client::bodyIsBelowLimit
   */
  public function testBodyIsBelowLimit() {
    // Create stub for the EdgeGrid client class.
    $edgegrid_client = $this->getMockBuilder('\Akamai\Open\EdgeGrid\Client')
      ->disableOriginalConstructor()
      ->setMethods(['post'])
      ->getMock();

    $ccu_client = new Ccu3Client($edgegrid_client);
    $hostname = 'www.example.com';
    $paths = ['/a'];
    $this->assertTrue($ccu_client->bodyIsBelowLimit($hostname, $paths), 'Body size is not below limit.');

    for ($i = 0; $i < 15000; $i++) {
      $paths[] = '/a' . $i;
    }
    $this->assertFalse($ccu_client->bodyIsBelowLimit($hostname, $paths), 'Expected body size to exceed limit.');
  }
}