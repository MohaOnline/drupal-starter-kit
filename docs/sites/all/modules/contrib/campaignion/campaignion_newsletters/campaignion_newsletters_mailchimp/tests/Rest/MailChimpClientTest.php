<?php

namespace Drupal\campaignion_newsletters_mailchimp\Rest;

/**
 * Test the MailChimpClient class.
 */
class MailChimpClientTest extends \DrupalUnitTestCase {

  /**
   * Produce a MailChimpClient class with stubbed get and send methods.
   */
  protected function mockClient() {
    $api = $this->getMockBuilder(MailChimpClient::class)
      ->setMethods(['get', 'send'])
      ->disableOriginalConstructor()
      ->getMock();
    return $api;
  }

  /**
   * Test MailChimpClient::getPaged() for empty result sets.
   */
  public function testGetPagedEmpty() {
    $api = $this->mockClient();
    $api->expects($this->once())->method('get')->with(
      $this->equalTo('/lists'),
      $this->equalTo(['count' => 10, 'offset' => 0])
    )->willReturn(
      ['lists' => [], 'total_items' => 0]
    );
    $api->getPaged('/lists');
  }

  /**
   * Test MailChimpClient::getPaged() for one page.
   */
  public function testGetPagedOnePage() {
    $api = $this->mockClient();
    $list = ['id' => 'a1234', 'name' => 'mocknewsletters'];
    $api->expects($this->once())->method('get')->with(
      $this->equalTo('/lists'),
      $this->equalTo(['count' => 10, 'offset' => 0])
    )->willReturn(
      ['lists' => [$list], 'total_items' => 1]
    );
    $api->getPaged('/lists');
  }

  /**
   * Test MailChimpClient::getPaged() for two pages.
   */
  public function testGetPagedTwoPages() {
    $api = $this->mockClient();
    $list = ['id' => 'a1234', 'name' => 'mocknewsletters'];
    $api->expects($this->exactly(2))->method('get')->withConsecutive(
      [$this->equalTo('/lists'), $this->equalTo(['count' => 3, 'offset' => 0])],
      [$this->equalTo('/lists'), $this->equalTo(['count' => 3, 'offset' => 3])]
    )->will($this->onConsecutiveCalls(
      ['lists' => [$list, $list, $list], 'total_items' => 6],
      ['lists' => [$list, $list, $list], 'total_items' => 6]
    ));
    $api->getPaged('/lists', [], [], 3);
  }

}
