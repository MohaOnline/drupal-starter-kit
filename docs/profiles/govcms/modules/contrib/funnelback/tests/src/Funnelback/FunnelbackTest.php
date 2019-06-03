<?php

require_once __DIR__ . '/../../vendor/e0ipso/drupal-unit-autoload/autoload.php';
require_once __DIR__.'/../../../src/Funnelback.class.php';
require_once __DIR__.'/../../../src/FunnelbackClient.class.php';
require_once __DIR__.'/../../../src/FunnelbackQueryString.class.php';

use PHPUnit\Framework\TestCase;

class FunnelbackTestCase extends TestCase {

  protected $successResponse;

  protected $suggestResponse;

  protected $failResponse;

  protected $emptyResponse;

  public function setUp() {
    parent::setUp();

    $this->successResponse = (object) [
      'request' => NULL,
      'data' => file_get_contents(dirname(__FILE__) . '/../../mocks/response.json', FILE_USE_INCLUDE_PATH),
      'protocol' => 'HTTP/1.1',
      'status_message' => 'OK',
      'headers' => [],
      'code' => 200,
    ];

    $this->failResponse = (object) [
      'request' => NULL,
      'data' => '',
      'protocol' => 'HTTP/1.1',
      'status_message' => 'Not found',
      'headers' => [],
      'code' => 404,
    ];

    $this->emptyResponse = (object) [
      'request' => NULL,
      'data' => file_get_contents(dirname(__FILE__) . '/../../mocks/failresponse.json', FILE_USE_INCLUDE_PATH),
      'protocol' => 'HTTP/1.1',
      'status_message' => 'OK',
      'headers' => [],
      'code' => 200,
    ];

    $this->suggestResponse = (object) [
      'request' => NULL,
      'data' => file_get_contents(dirname(__FILE__) . '/../../mocks/suggestresponse.json', FILE_USE_INCLUDE_PATH),
      'protocol' => 'HTTP/1.1',
      'status_message' => 'OK',
      'headers' => [],
      'code' => 200,
    ];
  }

  /**
   * Test funnelback_json_query function.
   */
  public function testDoQuery() {
    $funnelbackClient = Mockery::mock('FunnelbackClient');
    $funnelbackClient
      ->shouldReceive('request')
      ->andReturn($this->successResponse);


    $funnelbackClient
      ->shouldReceive('debug')
      ->andReturnNull();
    $funnelback = new Funnelback(NULL,NULL, NULL, NULL);

    $response = $funnelback->funnelbackJsonQuery(json_decode($funnelbackClient->request(NULL, NULL, NULL, NULL)->data, TRUE), '');

    $this->assertEquals($this->getJsonQueryResult(), $response['results']);

    $funnelbackBadRequest = Mockery::mock('FunnelbackClient');
    $funnelbackBadRequest
      ->shouldReceive('request')
      ->andReturn($this->failResponse);

    $funnelback = new Funnelback(NULL,NULL, NULL, NULL);
    $response = $funnelback->funnelbackJsonQuery(json_decode($funnelbackBadRequest->request(NULL, NULL, NULL, NULL)->data, TRUE), '');

    $this->assertEquals(NULL, $response);

    $funnelbackEmptyRequest = Mockery::mock('FunnelbackClient');
    $funnelbackEmptyRequest
      ->shouldReceive('request')
      ->andReturn($this->emptyResponse);

    $funnelback = new Funnelback(NULL,NULL, NULL, NULL);
    $response = $funnelback->funnelbackJsonQuery(json_decode($funnelbackEmptyRequest->request(NULL, NULL, NULL, NULL)->data, TRUE), '');

    $this->assertEquals([], $response);
  }

  /**
   * Test autocomplete response.
   */
  public function testSuggestQuery() {
    $funnelbackClient = Mockery::mock('FunnelbackClient');
    $funnelbackClient
      ->shouldReceive('request')
      ->andReturn($this->suggestResponse);


    $funnelbackClient
      ->shouldReceive('debug')
      ->andReturnNull();
    $funnelback = new Funnelback(NULL,NULL, NULL, NULL);

    $response = $funnelback->funnelbackJsonQuery(json_decode($funnelbackClient->request(NULL, NULL, NULL, NULL)->data, TRUE), '');

    $this->assertEquals($this->getSuggestJsonQueryResult(), $response);
  }

  /**
   * Test filter facet query string function.
   */
  public function testFilterFacetQueryString() {
    $funnelback = new FunnelbackQueryString();
    $raw_strings = [
      'query=holden',
      'f.Condition%7Ccondition=<span>demo</span>',
      'f.Extras%7Extras=aircondition',
      'f.Extras%7Extras=<span>cruise+control</span>',
    ];

    $facet_strings = [
      'f.Condition%7Ccondition' => [
        'demo',
      ],
      'f.Extras%7Extras' => [
        'aircondition',
        'cruise+control',
      ]
    ];

    $this->assertEquals($facet_strings, $funnelback->funnelbackFilterFacetQueryString($raw_strings));
  }

  /**
   * Test filter system query string function.
   */
  public function testFilterSystemQueryString() {
    $funnelback = new FunnelbackQueryString();
    $testQueryString = '?remote_ip=10.20.1.1&query=holden&profile=_default_preview&f.Condition%7CAllDocumentsFill=All&collection=fb-bargain-store-meta&form=custom_json';
    $fiteredQueryString = '?query=holden&f.Condition%7CAllDocumentsFill=All';

    $this->assertEquals($fiteredQueryString, $funnelback->funnelbackFilterSystemQueryString($testQueryString));
  }

  /**
   * Test filter contextual query string function.
   */
  public function testContextualFilter() {
    $funnelback = new FunnelbackQueryString();
    $raw_strings = [
      'cluster1=my+holden',
      'cluster0=holden',
      'start_rank=0',
      'query=About+my+holden',
      'cluster2=<span>about+holden</span>'
    ];

    $contextual_strings = [
      'cluster1' => 'my+holden',
      'cluster0' => 'holden',
      'cluster2' => 'about+holden',
    ];

    $this->assertEquals($contextual_strings, $funnelback->funnelbackFilterContextualQueryString($raw_strings));
  }

  /**
   * Test query normaliser.
   */
  public function testQueryNormaliser() {
    $funnelback = new FunnelbackQueryString();
    $raw_string = "collection=test&query=`my holden is awesome`&f_Condition%7Ccondition[0]=<script>demo</script>";
    $expected_string = "collection=test&query=my+holden+is+awesome&f.Condition|condition=demo";

    $this->assertEquals($expected_string, $funnelback->funnelbackQueryNormaliser($raw_string));
  }

  /**
   * Test qurator link filter.
   */
  public function testQuratorLink() {
    $funnelback = new FunnelbackQueryString();
    $raw_string = '/s/redirect?collection=cars&url=http%3A%2F%2Fgoogle.com&auth=21XsizHQ7f0vU3r3%2Fmw%2BAg&profile=_default_preview&type=FP';
    $expected_string = 'http://google.com';

    $this->assertEquals($expected_string, $funnelback->funnelbackFilterCuratorLink($raw_string));
  }

  /**
   * Test string remover from raw query strings.
   */
  public function testStringRemover() {
    $funnelback = new FunnelbackQueryString();
    $queryString = [
      'f.Profile|profile=cars',
      'query=holden',
      'f.Content+type|contentType=node',
      'start_rank=1',
      'start_rank=11',
    ];
    $expected_string = [
      'f.Profile|profile=cars',
      'query=holden',
      'f.Content+type|contentType=node',
    ];
    $this->assertEquals($expected_string, $funnelback::funnelbackQueryRemove('start_rank', $queryString));
  }

  protected function getJsonQueryResult() {
    return [
      [
        'title' => 'https://sample.data.io/Holden/Demo/Blue/2',
        'date' => '1485781200000',
        'summary' => NULL,
        'live_url' => 'sample.data.io/Holden/Demo/Blue/2',
        'cache_url' => '/s/cache?collection=fb-cars-xml&doc=funnelback-web-crawl.warc&off=408&len=353&url=https%3A%2F%2Fsample.data.io%2FHolden%2FDemo%2FBlue%2F2&profile=_default_preview',
        'display_url' => 'https://sample.data.io/Holden/Demo/Blue/2',
        'metaData' => [
          'condition' => 'Demo',
          'colour' => 'Blue',
          'd' => '31 January 2017',
          'price' => '25000',
          'extras' => 'Aircondition, Power Steering, Electric Windows, Cruise Control',
          'make' => 'Holden',
          'nodeId' => 1,
        ],
      ]
    ];
  }

  protected function getSuggestJsonQueryResult() {
    return [
      "hdmi" => "hdmi",
      "hp" => "hp",
      "holden" => "holden"
    ];
  }
}
