<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\campaignion_action\Loader;
use Drupal\campaignion_email_to_target\Api\Client;
use Drupal\campaignion_email_to_target\Channel\NoOp;
use Drupal\little_helpers\Webform\Submission;

class ActionTest extends \DrupalUnitTestCase {

  /**
   * New Action with all methods mocked that would need database access.
   */
  protected function mockAction($targets) {
    $api = $this->getMockBuilder(Client::class)
      ->disableOriginalConstructor()
      ->getMock();
    $api->method('getTargets')->will($this->returnValue($targets));
    $api->method('getDataset')->will($this->returnValue((object) [
      'dataset_name' => 'test_dataset',
      'selectors' => [['title' => 'test_selector', 'filters' => []]],
    ]));
    $node_array['field_no_target_message'][LANGUAGE_NONE][0] = [
      'value' => '<p>Default exclusion</p>',
      'format' => 'full_html_with_editor',
    ];
    $node = (object) ([
      'nid' => 47114711,
      'type' => 'email_to_target'
    ] + $node_array);
    $type = Loader::instance()->type('email_to_target');
    $action = $this->getMockBuilder(Action::class)
      ->setConstructorArgs([$type->parameters, $node, $api])
      ->setMethods(['getOptions', 'getExclusion', 'getMessage'])
      ->getMock();
    $action->method('getOptions')->will($this->returnValue([
      'dataset_name' => 'test_dataset',
    ]));
    $submission_o = $this->getMockBuilder(Submission::class)
      ->disableOriginalConstructor()
      ->getMock();
    return [$action, $api, $submission_o];
  }

  /**
   * Create a message with the replaceTokens() method mocked.
   */
  protected function createMessage($data) {
    $data += ['type' => 'message'];
    $class = $data['type'] == 'exclusion' ? Exclusion::class : Message::class;
    return $this->getMockBuilder($class)
      ->setConstructorArgs([$data])
      ->setMethods(['replaceTokens'])
      ->getMock();
  }

  /**
   * Test targetMessagePairs() with messages and all types of exclusions.
   */
  public function testTargetMessagePairsWithExclusions() {
    $c1 = ['name' => 'Constituency 1'];
    $contacts = [
      ['first_name' => 'Alice', 'constituency' => $c1],
      ['first_name' => 'Bob', 'constituency' => $c1],
      ['first_name' => 'Claire', 'constituency' => $c1],
      ['first_name' => 'David', 'constituency' => ['name' => 'Excluded']],
    ];
    list($action, $api, $submission_o) = $this->mockAction($contacts);
    $m = $this->createMessage([
      'type' => 'message',
      'label' => 'Default message',
      'subject' => 'Default subject',
      'header' => 'Default header',
      'message' => 'Default message',
      'footer' => 'Default footer',
    ]);
    $self = $this;
    $action->method('getMessage')->will($this->returnCallback(function ($t) use ($m, $self) {
      if ($t['first_name'] == 'Bob') {
        return $self->createMessage([
          'type' => 'exclusion',
          'message' => 'excluded first!',
        ]);
      }
      if ($t['constituency']['name'] == 'Excluded') {
        return $self->createMessage([
          'type' => 'exclusion',
          'message' => 'excluded!',
        ]);
      }
      return $m;
    }));
    $pairs = $action->targetMessagePairs($submission_o, new NoOp(), FALSE);
    $this->assertEqual([[$contacts[0], $m], [$contacts[2], $m]], $pairs);
  }

  /**
   * Test that the first exclusion is returned if all targets are excluded.
   */
  public function testTargetMessagePairsReturnsExclusionIfEmpty() {
    $contacts = [
      ['first_name' => 'Bob'],
      ['first_name' => 'David'],
    ];
    list($action, $api, $submission_o) = $this->mockAction($contacts);
    $self = $this;
    $action->method('getMessage')->will($this->returnCallback(function ($t) use ($self) {
      if ($t['first_name'] == 'Bob') {
        return $self->createMessage([
          'type' => 'exclusion',
          'message' => 'excluded first!',
        ]);
      }
      return $self->createMessage([
        'type' => 'exclusion',
        'message' => 'excluded!',
      ]);
    }));
    $exclusion = $action->targetMessagePairs($submission_o, new NoOp(), FALSE);
    $this->assertEqual(['#markup' => "<p>excluded first!</p>\n"], $exclusion->renderable());
  }

  /**
   * Test getting the default exclusion.
   */
  public function testTargetMessagePairsDefaultExclusion() {
    list($action, $api, $submission_o) = $this->mockAction([]);
    $exclusion = $action->targetMessagePairs($submission_o, new NoOp(), FALSE);
    $this->assertEqual(['#markup' => '<p>Default exclusion</p>'], $exclusion->renderable()[0]);
  }

}
