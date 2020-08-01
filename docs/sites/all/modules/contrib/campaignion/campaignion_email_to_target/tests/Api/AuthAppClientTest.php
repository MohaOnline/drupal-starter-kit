<?php

namespace Drupal\campaignion_email_to_target\Api;

/**
 * Test the API-client class.
 */
class AuthAppClientTest extends \DrupalUnitTestCase {

  /**
   * Reset cache.
   */
  public function tearDown() {
    cache_clear_all(AuthAppClient::TOKEN_CID, 'cache');
  }

  /**
   * Create an instrumented Api object that doesn’t actually send requests.
   */
  protected function instrumentedApi() {
    $api = $this->getMockBuilder(AuthAppClient::class)
      ->setConstructorArgs(['http://mock', [
        'public_key' => 'pk_',
        'secret_key' => 'sk_',
      ]])
      ->setMethods(['send'])
      ->getMock();
    return $api;
  }

  /**
   * Loading the token twice from two Client objects.
   */
  public function testRequestingDatasetTwiceOnTwoObjects() {
    $api = $this->instrumentedApi();
    $api->expects($this->once())
      ->method('send')
      ->will($this->returnValue([
        'token' => 'test token',
      ]));
    $token = $api->getToken();
    $this->assertEqual('test token', $token);
    $this->assertEqual($token, $api->getToken());

    $api2 = $this->instrumentedApi();
    $api2->expects($this->never())->method('send');
    $token2 = $api2->getToken();
    $this->assertEqual($token, $token2);
  }

}
