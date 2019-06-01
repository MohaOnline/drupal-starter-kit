<?php

require_once('seq_share_count.php');
require_once('http.php');

class ShareaholicSeqShareCountsTest extends PHPUnit_Framework_TestCase
{
  public function setUp() {
    $this->url = 'https://www.google.com';
    $this->services = array_keys(ShareaholicSeqShareCount::get_services_config());
    $this->options = array();
    $this->share_count = new ShareaholicSeqShareCount($this->url, $this->services, $this->options);

    // all callbacks take a predefined response structure
    $this->response = array(
      'response' => array(
        'code' => 200
      ),
    );
  }

  public function tearDown() {

  }

  public function testHasHttpError() {
    $response = NULL;
    $this->assertTrue($this->share_count->has_http_error($response), 'should return true for null response');

    $response = array();
    $this->assertTrue($this->share_count->has_http_error($response), 'should return true for empty response');

    $response = array(
      'response' => array(
        'code' => 200
      ),
      'body' => ''
    );
    $this->assertFalse($this->share_count->has_http_error($response), 'should return false');
  }

  public function testGetClientIp() {
    $expected = '12345';
    $_SERVER['HTTP_CLIENT_IP'] = $expected;
    $result = $this->share_count->get_client_ip();
    $this->assertEquals($expected, $result);

    $expected = '23456';
    $_SERVER['HTTP_CLIENT_IP'] = '';
    $_SERVER['HTTP_X_FORWARDED_FOR'] = $expected;
    $result = $this->share_count->get_client_ip();
    $this->assertEquals($expected, $result);

    $expected = '34567';
    $_SERVER['HTTP_CLIENT_IP'] = '';
    $_SERVER['HTTP_X_FORWARDED_FOR'] = '';
    $_SERVER['REMOTE_ADDR'] = $expected;
    $result = $this->share_count->get_client_ip();
    $this->assertEquals($expected, $result);
  }

  public function testFacebookCountCallback() {
    // given a typical facebook counts api response, test that
    // it gives back the expected result (the total_count which is 16)
    $json = '{ "id": "https://blog.shareaholic.com", "shares": 16 }';
    $this->response['body'] = $json;
    $facebook_count = $this->share_count->facebook_count_callback($this->response);
    $this->assertEquals(16, $facebook_count, 'It should get the correct fb count');
  }


 public function testLinkedinCountCallback() {
    // given a typical linkedin counts api response, test that
    // it gives back the expected result (the count which is 8)
    $json = '{"count":8,"fCnt":"8","fCntPlusOne":"9","url":"https:\/\/blog.shareaholic.com\/"}';
    $this->response['body'] = $json;
    $linkedin_count = $this->share_count->linkedin_count_callback($this->response);
    $this->assertEquals(8, $linkedin_count, 'It should get the correct linkedin count');
 }


 public function testGoogleplusCountCallback() {
    // given a typical google+ counts api response, test that
    // it gives back the expected result (the count which is 10)
    $json = '[{"id": "p", "result": {"kind": "pos#plusones", "id": "https://blog.shareaholic.com/", "isSetByViewer": false, "metadata": {"type": "URL", "globalCounts": {"count": 10.0}}}}]';
    $this->response['body'] = $json;
    $google_plus_count = $this->share_count->google_plus_count_callback($this->response);
    $this->assertEquals(10, $google_plus_count, 'It should get the correct google_plus count');

    // test when google returns unexpected json response in case they change
    $json = '{"test": "test"}';
    $this->response['body'] = $json;
    $google_plus_count = $this->share_count->google_plus_count_callback($this->response);
    $this->assertEquals(0, $google_plus_count, 'It should return zero google plus count for unexpected JSON');

    // test when google return non JSON response
    $json = 'hello';
    $this->response['body'] = $json;
    $google_plus_count = $this->share_count->google_plus_count_callback($this->response);
    $this->assertEquals(0, $google_plus_count, 'It should return zero google plus count for non JSON');
 }


 public function testPinterestCountCallback() {
    // given a typical pinterest counts api response, test that
    // it gives back the expected result (the count which is 1)
    $body = 'f({"count": 1, "url": "https://blog.shareaholic.com"})';
    $this->response['body'] = $body;
    $count = $this->share_count->pinterest_count_callback($this->response);
    $this->assertEquals(1, $count, 'It should get the correct pinterest count');
 }


  public function testBufferCountCallback() {
     // given a typical buffer counts api response, test that
     // it gives back the expected result (the shares which is 3)
     $body = '{"shares":3}';
     $this->response['body'] = $body;
     $count = $this->share_count->buffer_count_callback($this->response);
     $this->assertEquals(3, $count, 'It should get the correct buffer count');
  }

  public function testStumbleuponCountCallback() {
     // given a typical stumbleupon counts api response, test that
     // it gives back the expected result (the views which is 1)
     $body = '{"result":{"url":"https:\/\/blog.shareaholic.com\/","in_index":true,"publicid":"1Qat7p","views":1,"title":"Blog \/ Shareaholic (@shareaholic)","thumbnail":"http:\/\/cdn.stumble-upon.com\/mthumb\/672\/157433672.jpg","thumbnail_b":"http:\/\/cdn.stumble-upon.com\/bthumb\/672\/157433672.jpg","submit_link":"http:\/\/www.stumbleupon.com\/submit\/?url=https:\/\/blog.shareaholic.com\/","badge_link":"http:\/\/www.stumbleupon.com\/badge\/?url=https:\/\/blog.shareaholic.com\/","info_link":"http:\/\/www.stumbleupon.com\/url\/https%253A\/\/blog.shareaholic.com\/"},"timestamp":1394771877,"success":true}';
     $this->response['body'] = $body;
     $count = $this->share_count->stumbleupon_count_callback($this->response);
     $this->assertEquals(1, $count, 'It should get the correct stumbleupon count');
  }

  public function testRedditCountCallback() {
     // given a typical reddit counts api response, test that
     // it gives back the expected result (the ups which is 1)
     // NOTE: the actual JSON output was too long so some keys were removed
     $body = '{"kind": "Listing", "data": {"modhash": "", "children": [{"kind": "t3", "data": {"domain": "reddit.com", "banned_by": null, "likes": null, "clicked": false, "stickied": false, "score": 1, "downs": 0, "url": "http://reddit.com", "ups": 1, "num_comments": 0, "distinguished": null}}], "after": null, "before": null}}';
     $this->response['body'] = $body;
     $count = $this->share_count->reddit_count_callback($this->response);
     $this->assertEquals(1, $count, 'It should get the correct reddit count');
  }

  public function testVkCountCallback() {
     // given a typical vk counts api response, test that
     // it gives back the expected result (3781)
     $body = 'VK.Share.count(0, 3781);';
     $this->response['body'] = $body;
     $count = $this->share_count->vk_count_callback($this->response);
     $this->assertEquals(3781, $count, 'It should get the correct vk count');
  }

 public function testOdnoklassnikiCountCallback() {
   // given a typical odnoklassniki counts api response, test that
   // it gives back the expected result (1)
   $body = "ODKL.updateCount('odklcnt0','1');";
   $this->response['body'] = $body;
   $count = $this->share_count->odnoklassniki_count_callback($this->response);
   $this->assertEquals(1, $count, 'It should get the correct odnoklassniki count');
 }

 public function testYummlyCountCallback() {
   $body = '{"count":760}';
   $this->response['body'] = $body;
   $count = $this->share_count->yummly_count_callback($this->response);
   $this->assertEquals(760, $count, 'It should get the correct yummly count');
 }

 public function testFancyCountCallback() {
   $body = '__FIB.collectCount({"url": "http://www.google.com", "count": 26, "thing_url": "http://fancy.com/things/263001623", "showcount": 1});';
   $this->response['body'] = $body;
   $count = $this->share_count->fancy_count_callback($this->response);
   $this->assertEquals(26, $count, 'It should get the correct fancy count');
 }

 public function testGooglePlusPrepareRequest() {
   $config = ShareaholicSeqShareCount::get_services_config();

   // check that the function sets the post body in the $config object
   $this->share_count->google_plus_prepare_request($this->url, $config);
   $this->assertNotNull($config['google_plus']['body'], 'The post body for google plus should not be null');

   // mock the ip address and check that the userIp is set
   $mockIp = 'mockIp';
   $_SERVER['REMOTE_ADDR'] = $mockIp;
   $this->share_count->google_plus_prepare_request($this->url, $config);
   $this->assertEquals($mockIp, $config['google_plus']['body'][0]['params']['userIp']);
 }

  /**
   * This test may fail if the APIs fail
   */
  public function testGetCount() {
    // test that this function returns the expected API response
    $response = $this->share_count->get_counts();

    $this->assertNotNull($response, 'The response array should not be null');

    foreach($this->services as $service) {
      $this->assertNotNull($response['data'][$service], 'The ' . $service . ' count should not be null');
    }
  }

  /**
   * This test may fail if the APIs fail
   */
  public function testMissingServices() {
    // test that this function returns response WITHOUT facebook
    $share_count = new ShareaholicSeqShareCount('https://dev.losloslos.com/', $this->services, $this->options);
    $response = $share_count->get_counts();

    $this->assertNotNull($response, 'The response array should not be null');

    $this->assertNull($response['data']['facebook'], 'The facebook count should be null');
  }

  /**
   * This test may fail if the APIs fail
   */
  public function testIsUrlEncoded() {
    $url = 'http://eatnabout.com/2014/03/29/bestie/#more-10144';
    $encoded_url = 'http%3A%2F%2Featnabout.com%2F2014%2F03%2F29%2Fbestie%2F%23more-10144';

    $this->assertTrue($this->share_count->is_url_encoded($encoded_url), 'It should return true when the url is encoded');
    $this->assertFalse($this->share_count->is_url_encoded($url), 'It should return false when the url is not encoded');
  }

  /**
   * This test may fail if the APIs fail
   */
  public function testRawResponseObject() {
    // test that the class is storing the raw responses
    $response = $this->share_count->get_counts();

    $this->assertNotNull($this->share_count->raw_response, 'The raw response object should not be null');

    foreach($this->services as $service) {
      $this->assertNotNull($this->share_count->raw_response[$service], 'The raw response for ' . $service . ' should not be null');
    }
  }

}
