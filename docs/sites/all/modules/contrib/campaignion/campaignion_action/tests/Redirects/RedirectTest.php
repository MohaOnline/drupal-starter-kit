<?php

namespace Drupal\campaignion_action\Redirects;

use Drupal\little_helpers\Webform\Submission;

/**
 * Test for the redirect model class.
 */
class RedirectTest extends \DrupalUnitTestCase {

  /**
   * Create a simple instance and test the array export.
   */
  public function testToArray() {
    $t = new Redirect([
      'label' => 'Test redirect',
      'destination' => 'node/50',
    ]);
    $m = $t->toArray();
    unset($m['prettyDestination']);
    $this->assertEqual([
      'id' => NULL,
      'label' => 'Test redirect',
      'destination' => 'node/50',
      'filters' => [],
    ], $m);
  }

  /**
   * Test constructing an instance from an array.
   */
  public function testConstructFromArray() {
    $data = [
      'label' => 'Test redirect',
      'destination' => 'node/50',
      'filters' => [],
    ];
    $t = new Redirect($data);
    $this->assertEqual('node/50', $t->destination);
  }

  /**
   * Test cloning of message templates.
   */
  public function testRedirectCloning() {
    $data = [
      'id' => 42,
      'label' => 'Test label',
      'destination' => 'node/50',
      'filters' => [
        [
          'id' => 42,
          'type' => 'test',
          'value' => 1,
        ],
      ],
    ];
    $t1 = new Redirect($data);
    $t2 = clone $t1;

    // Test that the cloned message counts as being new.
    $this->assertNull($t2->id);
    $this->assertTrue($t2->isNew());

    // Test that filters have been cloned too.
    $this->assertTrue($t2->filters[0]->isNew());
    $t2->filters[0]->config['value'] = 2;
    $this->assertEqual(1, $t1->filters[0]->config['value']);
  }

  /**
   * Test checkFilters without any filters configured (ie. default redirect).
   */
  public function testCheckFiltersWithoutFilters() {
    $stub_s['data'] = [];
    $stub_n['webform']['components'] = [];
    $submission = new Submission((object) $stub_n, (object) $stub_s);
    $redirect = new Redirect([]);
    $this->assertTrue($redirect->checkFilters($submission));
  }

  /**
   * Test filter checking.
   */
  public function testCheckFiltersWithFilters() {
    $stub_s['data'][1][0] = 'foo bar';
    $stub_n['webform']['components'][1] = [
      'cid' => 1,
      'form_key' => 'name',
      'type' => 'textfield',
    ];
    $submission = new Submission((object) $stub_n, (object) $stub_s);
    $filter = [
      'type' => 'submission-field',
      'operator' => 'contains',
      'field' => 1,
    ];
    $redirect['filters'] = [
      ['value' => 'foo'] + $filter,
      ['value' => 'baz'] + $filter,
      ['value' => 'bar'] + $filter,
    ];
    $redirect = new Redirect($redirect);
    $this->assertFalse($redirect->checkFilters($submission));
  }

  /**
   * Test various input output combinations for normalized().
   */
  public function testNormalized() {
    $test_pairs = [
      ['node/15', ['node/15', ['query' => [], 'fragment' => '']]],
      [
        'https://example.com?a=1&b=2#c',
        [
          'https://example.com',
          ['query' => ['a' => '1', 'b' => '2'], 'fragment' => 'c'],
        ],
      ],
    ];

    foreach ($test_pairs as $p) {
      $r = new Redirect(['destination' => $p[0]]);
      $this->assertEqual($p[1], $r->normalized());
    }
  }

}
