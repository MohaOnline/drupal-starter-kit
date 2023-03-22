<?php

namespace Drupal\campaignion_auth;

/**
 * Test the auth app API-client.
 */
class AuthAppClientTest extends \DrupalUnitTestCase {

  /**
   * Reset cache.
   */
  public function tearDown() : void {
    cache_clear_all(AuthAppClient::TOKEN_CID, 'cache');
  }

  /**
   * Create an instrumented client object that doesn’t actually send requests.
   */
  protected function instrumentedApi() {
    $api = $this->getMockBuilder(AuthAppClient::class)
      ->setConstructorArgs([
        'http://mock',
        ['public_key' => 'pk_', 'secret_key' => 'sk_'],
        'org1',
      ])
      ->setMethods(['send'])
      ->getMock();
    return $api;
  }

  /**
   * Loading the token twice from two client objects.
   */
  public function testRequestingTokenTwiceOnTwoObjects() {
    $api = $this->instrumentedApi();
    $api->expects($this->once())
      ->method('send')
      ->with('token', [], [
        'public_key' => 'pk_',
        'secret_key' => 'sk_',
      ], [
        'method' => 'POST',
      ])
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

  /**
   * Test config validation for an empty API-key.
   */
  public function testValidateConfigEmptyKey() {
    $this->expectException(ConfigError::class);
    $api = new AuthAppClient('http://url', [], 'org');
  }

}
