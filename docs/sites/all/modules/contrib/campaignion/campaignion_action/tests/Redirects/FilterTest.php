<?php

namespace Drupal\campaignion_action\Redirects;

use Drupal\little_helpers\Webform\Submission;
use Drupal\campaignion_opt_in\Values;
use Drupal\campaignion_newsletters\Subscription;

/**
 * Test the filter model class.
 */
class FilterTest extends \DrupalWebTestCase {

  /**
   * Cleanup after testing.
   */
  public function tearDown() {
    db_delete('campaignion_action_redirect_filter')->execute();
    db_delete('campaignion_newsletters_subscriptions')->execute();
  }

  /**
   * Test creating a single filter.
   */
  public function testPutOneMessageOnEmptyNode() {
    $f = Filter::fromArray(['type' => 'test', 'config' => 'something']);
    $this->assertEquals(['config' => 'something'], $f->config);
    $f->redirect_id = 1;
    $f->weight = 0;
    $f->save();
    $fs = Filter::byRedirectIds([1]);
    $this->assertCount(1, $fs);
    $this->assertEquals(['config' => 'something'], array_values($fs)[0]->config);
  }

  /**
   * Test opt-in filter with opt-in in the submission.
   */
  public function testMatchOptInSubmission() {
    $stub_s['data'][1][0] = 'radios:opt-in';
    $stub_n['webform']['components'][1] = [
      'cid' => 1,
      'form_key' => 'emailopt',
      'type' => 'opt_in',
      'extra' => [
        'channel' => 'email',
        'optin_statement' => 'Opt-in statement',
      ],
    ];
    $submission = new Submission((object) $stub_n, (object) $stub_s);
    $submission->opt_in = new Values($submission);

    $fs = Filter::fromArray(['type' => 'opt-in', 'value' => TRUE]);
    $this->assertTrue($fs->match($submission));

    $fs = Filter::fromArray(['type' => 'opt-in', 'value' => FALSE]);
    $this->assertFalse($fs->match($submission));
  }

  /**
   * Test opt-in filter with opt-in in the submission.
   */
  public function testMatchNoOptInSubmission() {
    $stub_s['data'] = [];
    $stub_n['webform']['components'] = [];
    $submission = new Submission((object) $stub_n, (object) $stub_s);
    $submission->opt_in = new Values($submission);

    $fs = Filter::fromArray(['type' => 'opt-in', 'value' => TRUE]);
    $this->assertFalse($fs->match($submission));

    $fs = Filter::fromArray(['type' => 'opt-in', 'value' => FALSE]);
    $this->assertTrue($fs->match($submission));
  }

  /**
   * Test opt-in filter with subscribed contact.
   */
  public function testMatchOptInSubscription() {
    $email = __FUNCTION__ . '@campaignion-action.test';
    $stub_s['data'][1][0] = $email;
    $stub_n['webform']['components'][1] = [
      'cid' => 1,
      'form_key' => 'email',
      'type' => 'email',
    ];
    $submission = new Submission((object) $stub_n, (object) $stub_s);
    $submission->opt_in = new Values($submission);
    $subscription = Subscription::fromData(4711, $email);
    $subscription->save(TRUE);

    $fs = Filter::fromArray(['type' => 'opt-in', 'value' => TRUE]);
    $this->assertTrue($fs->match($submission));

    $fs = Filter::fromArray(['type' => 'opt-in', 'value' => FALSE]);
    $this->assertFalse($fs->match($submission));
  }

  /**
   * Test opt-in filter with existing subscriptions, but opt-out.
   */
  public function testMatchOptInWithOptOut() {
    $email = __FUNCTION__ . '@campaignion-action.test';
    $stub_s['data'][1][0] = $email;
    $stub_n['webform']['components'][1] = [
      'cid' => 1,
      'form_key' => 'email',
      'type' => 'email',
      'extra' => [
        'channel' => 'email',
        'optin_statement' => 'Opt-in statement',
      ],
    ];
    $stub_s['data'][2][0] = 'radios:opt-out';
    $stub_n['webform']['components'][2] = [
      'cid' => 2,
      'form_key' => 'emailopt',
      'type' => 'opt_in',
      'extra' => [
        'channel' => 'email',
        'optin_statement' => 'Opt-in statement',
      ],
    ];
    $submission = new Submission((object) $stub_n, (object) $stub_s);
    $submission->opt_in = new Values($submission);
    $subscription = Subscription::fromData(4711, $email);
    $subscription->save(TRUE);

    $fs = Filter::fromArray(['type' => 'opt-in', 'value' => TRUE]);
    $this->assertFalse($fs->match($submission));
  }

  /**
   * Test submission value filter.
   */
  public function testMatchSubmissionValueContains() {
    $stub_s['data'][1][0] = 'Some foo';
    $stub_n['webform']['components'][1] = [
      'cid' => 1,
      'form_key' => 'name',
      'type' => 'textfield',
    ];
    $submission = new Submission((object) $stub_n, (object) $stub_s);
    $submission->opt_in = new Values($submission);

    $filter = [
      'type' => 'submission-field',
      'operator' => 'contains',
      'field' => 1,
    ];
    $fs = Filter::fromArray(['value' => 'foo'] + $filter);
    $this->assertTrue($fs->match($submission));

    $fs = Filter::fromArray(['value' => 'bar'] + $filter);
    $this->assertFalse($fs->match($submission));
  }

  /**
   * Test changing config with setData.
   */
  public function testSetDataChangeConfig() {
    $c = ['type' => 'test', 'test' => 'unchanged'];
    $f = Filter::fromArray($c);
    $this->assertEqual('unchanged', $f->config['test']);

    $c['test'] = 'changed';
    $f->setData($c);
    $this->assertEqual('changed', $f->config['test']);
  }

}
