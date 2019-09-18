<?php

namespace Drupal\campaignion_newsletters;

use Drupal\campaignion\Contact;
use Drupal\campaignion\CRM\Import\Source\WebformSubmission;
use Drupal\campaignion_newsletters\Subscription;

/**
 * Test the Component functionality.
 */
class ComponentTest extends \DrupalUnitTestCase {

  protected $component = [
    'type' => 'opt_in',
    'cid' => 1,
    'pid' => 0,
    'form_key' => 'newsletter',
    'extra' => [
      'lists' => [1 => 1],
      'channel' => 'email',
    ],
  ];

  /**
   * Create a contact and some lists for testing.
   */
  public function setUp() {
    $this->submission = $this->createMock(WebformSubmission::class);
    parent::setUp();
  }

  /**
   * Clean up the test contact.
   */
  public function tearDown() {
    if ($c = Contact::byEmail('test@example.com')) {
      entity_delete('redhen_contact', $c->contact_id);
    }
    db_delete('campaignion_newsletters_subscriptions')->execute();
    db_delete('campaignion_newsletters_queue')->execute();
    db_delete('campaignion_newsletters_lists')->execute();
    parent::tearDown();
  }

  /**
   * Test subscribing to a new list.
   */
  public function testSubscribe() {
    $c = new Component($this->component, FALSE);
    $s = $this->createMock(WebformSubmission::class);
    $s->node = (object) [
      'webform' => [
        'components' => [1 => $this->component]
      ],
    ];
    $subscriptions = $c->subscribe('test@example.com', $s);
    $this->assertCount(1, $subscriptions);
    $this->assertFalse($subscriptions[0]->delete);
  }

  /**
   * Test unscribing a contact.
   */
  public function testUnsubscribe() {
    $e = 'test@example.com';
    Subscription::byData(1, $e)->save();
    Subscription::byData(2, $e)->save();
    $this->assertCount(2, Subscription::byEmail($e));

    $this->submission->method('valuesByCid')->willReturn(['checkbox:opt-out']);

    $component = $this->component;
    $component['extra']['optout_all_lists'] = FALSE;
    $c = new Component($component, FALSE);
    $subscriptions = $c->getSubscriptions($e, $this->submission);
    $this->assertCount(1, $subscriptions);
    $this->assertTrue($subscriptions[0]->delete);

    $component['extra']['optout_all_lists'] = TRUE;
    $c = new Component($component, FALSE);
    $c->setAllListIds([1, 2, 3]);
    $subscriptions = $c->getSubscriptions($e, $this->submission);
    $this->assertCount(2, $subscriptions);
    foreach ($subscriptions as $s) {
      $this->assertTrue($s->delete);
    }

    $c = new Component($component, TRUE);
    $c->setAllListIds([1, 2, 3]);
    $subscriptions = $c->getSubscriptions($e, $this->submission);
    $this->assertCount(3, $subscriptions);
    foreach ($subscriptions as $s) {
      $this->assertTrue($s->delete);
    }
  }

  /**
   * Test no change.
   */
  public function testNoChange() {
    $this->submission->method('valuesByCid')->willReturn(['radios:no-change']);
    $component = $this->component;
    $c = new Component($component, FALSE);
    $subscriptions = $c->getSubscriptions($c, $this->submission);
    $this->assertEqual([], $subscriptions);
  }

  /**
   * Test getting list IDs from the database.
   */
  public function testGetAllListIds() {
    $l = NewsletterList::fromData([
      'source' => 'test',
      'identifier' => 'test',
      'title' => 'Test',
    ]);
    $l->save();

    $c = new Component(['type' => 'opt_in'], TRUE);
    $this->assertEqual([$l->list_id], $c->getAllListIds());
  }

}
