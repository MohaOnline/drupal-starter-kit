<?php

require_once('curl_multi_share_count.php');

class ShareaholicCurlMultiShareCountsTest extends PHPUnit_Framework_TestCase
{
  public function setUp() {
    $this->url = 'https://www.google.com';
    $this->services = array_keys(ShareaholicCurlMultiShareCount::get_services_config());
    $this->options = array();
    $this->share_count = new ShareaholicCurlMultiShareCount($this->url, $this->services, $this->options);

    // all callbacks take a predefined response structure
    $this->response = array(
      'response' => array(
        'code' => 200
      ),
    );
  }

  public function tearDown() {

  }


  public function testGetCount() {
    // test that this function returns the expected API response
    $response = $this->share_count->get_counts();

    $this->assertNotNull($response, 'The response array should not be null');

    foreach($this->services as $service) {
      $this->assertNotNull($response['data'][$service], 'The ' . $service . ' count should not be null');
    }
  }

  public function testRawResponseObject() {
    // test that the class is storing the raw responses
    $response = $this->share_count->get_counts();

    $this->assertNotNull($this->share_count->raw_response, 'The raw response object should not be null');

    foreach($this->services as $service) {
      $this->assertNotNull($this->share_count->raw_response[$service], 'The raw response for ' . $service . ' should not be null');
    }
  }

}
