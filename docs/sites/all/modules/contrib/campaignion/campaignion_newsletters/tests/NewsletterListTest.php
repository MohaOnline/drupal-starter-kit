<?php

namespace Drupal\campaignion_newsletters;

use Drupal\campaignion\Contact;

/**
 * Test the newsletter list model.
 */
class NewsletterListTest extends \DrupalUnitTestCase {

  /**
   * Remove all test data.
   */
  public function tearDown() {
    if ($c = Contact::byEmail('test@example.com')) {
      entity_delete('redhen_contact', $c->contact_id);
    }
    db_delete('campaignion_newsletters_subscriptions')->execute();
    db_delete('campaignion_newsletters_queue')->execute();
    db_delete('campaignion_newsletters_lists')->execute();
    if (isset($this->node)) {
      node_delete($this->node->nid);
    }
    parent::tearDown();
  }

  /**
   * Test that deleting a list deletes all dependent data.
   */
  public function testDeleteDeletesSubscriptionsQueueItemsAndComponents() {
    module_load_include('components.inc', 'webform', 'includes/webform');
    $l1 = NewsletterList::fromData([
      'source' => 'test',
      'identifier' => 'l1',
      'title' => 'List1',
    ]);
    $l1->save();
    $l2 = NewsletterList::fromData([
      'source' => 'test',
      'identifier' => 'l2',
      'title' => 'List2',
    ]);
    $l2->save();
    $node = (object) ['type' => 'webform'];
    node_object_prepare($node);
    $component = [
      'cid' => 1,
      'form_key' => 'newsletter',
      'type' => 'opt_in',
      'extra' => [
        'channel' => 'email',
        'lists' => [$l1->list_id => $l1->list_id, $l2->list_id => $l2->list_id],
      ],
    ];
    webform_component_defaults($component);
    $node->webform['components'][1] = $component;
    node_save($node);
    $this->node = $node;
    $email = 'test@example.com';
    Subscription::byData($l1->list_id, $email)->save();
    QueueItem::byData([
      'list_id' => $l1->list_id,
      'email' => $email,
      'action' => QueueItem::SUBSCRIBE,
    ])->save();
    Subscription::byData($l2->list_id, $email)->save();
    QueueItem::byData([
      'list_id' => $l2->list_id,
      'email' => $email,
      'action' => QueueItem::SUBSCRIBE,
    ])->save();

    $l1->delete();

    $node = node_load($this->node->nid);
    $this->assertEquals($node->webform['components'][1]['extra']['lists'], [$l2->list_id => $l2->list_id]);

    $this->assertEmpty(QueueItem::load($l1->list_id, $email));
    $this->assertNotEmpty(QueueItem::load($l2->list_id, $email));

    $subscriptions = Subscription::byEmail($email);
    $list_ids = array_map(function ($l) {
      return (int) $l->list_id;
    }, $subscriptions);
    $this->assertNotContains($l1->list_id, $list_ids);
    $this->assertContains($l2->list_id, $list_ids);
  }

  /**
   * Test that stale lists are deleted.
   */
  public function testStaleListRemoval() {
    $l1 = NewsletterList::fromData([
      'source' => 'test',
      'identifier' => 'l1',
      'title' => 'List1',
      'updated' => 0,
    ]);
    $l1->save(FALSE);
    $this->assertEqual(0, $l1->updated);
    $l2 = NewsletterList::fromData([
      'source' => 'test',
      'identifier' => 'l2',
      'title' => 'List2',
      'updated' => 0,
    ]);
    $l2->save();
    $this->assertEqual(REQUEST_TIME, $l2->updated);
    variable_set('campaignion_newsletters_last_list_poll', 0);

    NewsletterList::deleteStaleLists();

    $this->assertNotEmpty(NewsletterList::load($l1->list_id));
    $this->assertNotEmpty(NewsletterList::load($l2->list_id));

    _campaignion_newsletters_poll();

    NewsletterList::deleteStaleLists();
    $this->assertEmpty(NewsletterList::load($l1->list_id));
    $this->assertNotEmpty(NewsletterList::load($l2->list_id));
  }

}
