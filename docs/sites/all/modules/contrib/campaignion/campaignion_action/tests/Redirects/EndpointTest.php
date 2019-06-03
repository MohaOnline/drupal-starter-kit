<?php

namespace Drupal\campaignion_action\Redirects;

/**
 * Test the redirect editing API endpoint.
 */
class EndpointTest extends \DrupalWebTestCase {

  /**
   * Clear all redirects.
   */
  public function tearDown() {
    db_delete('campaignion_action_redirect')->execute();
    db_delete('campaignion_action_redirect_filter')->execute();
  }

  /**
   * Test PUT request on a node that doesn’t have redirects yet.
   */
  public function testPutOneRedirectOnEmptyNode() {
    $data[] = [
      'label' => 'Test redirect',
      'destination' => 'node/50',
      'filters' => [
        ['type' => 'test', 'something' => 'something else'],
      ],
    ];
    $fakenode = (object) ['nid' => 30551];
    $endpoint = new Endpoint($fakenode, 0);
    $answer = $endpoint->put(['redirects' => $data])['redirects'];
    $tpls = Redirect::byNid($fakenode->nid, 0);
    $this->assertEquals(1, count($tpls));
    $m = $answer[array_keys($answer)[0]];
    unset($m['id']);
    unset($m['prettyDestination']);
    foreach ($m['filters'] as &$filter) {
      unset($filter['id']);
    }
    $this->assertEquals($data[0], $m);
  }

  /**
   * Test PUT request on a node with existing redirects.
   */
  public function testPutWithExistingData() {
    $data = ['nid' => 1, 'delta' => 0];
    $r1 = new Redirect([
      'label' => 'First',
      'filters' => [['test' => 1, 'type' => 'test']],
    ] + $data);
    $r2 = new Redirect(['label' => 'Second', 'weight' => 1] + $data);
    $r3 = new Redirect(['label' => 'Third', 'weight' => 2] + $data);
    $r1->save(); $r2->save(); $r3->save();

    $redirect_ids = [$r1->id, $r2->id, $r3->id];

    $fakenode = (object) ['nid' => 1];
    $endpoint = new Endpoint($fakenode, 0);

    $answer = $endpoint->put([
      'redirects' => [
        ['label' => 'New first'],
        [
          'id' => $r1->id,
          'label' => 'Was first is now second',
          'filters' => [
            ['id' => $r1->filters[0]->id, 'test' => 1, 'type' => 'test'],
          ],
        ],
        ['id' => $r2->id, 'label' => 'Was second is now third'],
      ],
    ])['redirects'];

    $a_labels = [];
    foreach ($answer as $m) {
      $a_labels[] = $m['label'];
    }
    $this->assertEqual([
      'New first',
      'Was first is now second',
      'Was second is now third',
    ], $a_labels);
    $this->assertEqual($r2->id, $answer[2]['id']);

    $new_rs = array_values(Redirect::byNid($fakenode->nid, 0));
    $s_labels = [];
    foreach ($new_rs as $m) {
      $s_labels[] = $m->label;
    }
    $this->assertEqual([
      'New first',
      'Was first is now second',
      'Was second is now third',
    ], $s_labels);
    $this->assertEqual($r2->id, $new_rs[2]->id);
  }

  /**
   * Test replacing a filter.
   */
  public function testPutExchangeFilter() {
    $data = ['nid' => 1, 'delta' => 0];
    $filter = Filter::fromArray(['test' => 1, 'type' => 'test']);
    $r1 = new Redirect(['label' => 'First', 'filters' => [$filter]] + $data);
    $r1->save();

    $fakenode = (object) ['nid' => 1];
    $endpoint = new Endpoint($fakenode, 0);
    $new_filter = ['test' => 2, 'type' => 'test'];
    $put['redirects'][] = [
      'id' => $r1->id,
      'label' => 'Still first',
      'filters' => [$new_filter],
    ];
    $answer = $endpoint->put($put)['redirects'];

    $this->assertEqual(1, count($answer));
    $this->assertEqual(1, count($answer[0]['filters']));

    $answer = $endpoint->get()['redirects'];
    $this->assertEqual(1, count($answer));
    $this->assertEqual(1, count($answer[0]['filters']));
    $this->assertEqual(2, $answer[0]['filters'][0]['test']);
  }

  /**
   * Test changing a filter value.
   */
  public function testPutChangeFilterValue() {
    $data = ['nid' => 1, 'delta' => 0];
    $filter = Filter::fromArray(['test' => 'unchanged', 'type' => 'test']);
    $r1 = new Redirect(['label' => 'First', 'filters' => [$filter]] + $data);
    $r1->save();

    $fakenode = (object) ['nid' => 1];
    $endpoint = new Endpoint($fakenode, 0);

    $data = $endpoint->get();
    $data['redirects'][0]['filters'][0]['test'] = 'changed';
    $new_data = $endpoint->put($data);
    $this->assertEqual($data, $new_data);
  }

}
