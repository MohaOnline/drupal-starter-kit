<?php

namespace Drupal\campaignion_manage;

use Drupal\campaignion\Contact;
use Drupal\campaignion_manage\Query\Supporter as BaseQuery;

/**
 * Some random integrations tests for the manage supporter interface.
 */
class SupporterFiltersTest extends \DrupalUnitTestCase {

  protected $contacts = [];

  /**
   * Create test contacts.
   */
  public function setUp() : void {
    parent::setUp();
    $c1 = Contact::fromEmail('test@example.com');
    $c1->save();
    $this->contacts[] = $c1;

    $c2 = Contact::fromEmail('archived@example.com');
    $c2->redhen_state = REDHEN_STATE_ARCHIVED;
    $c2->save();
    $this->contacts[] = $c2;
  }

  /**
   * Remove test contacts.
   */
  public function tearDown() : void {
    foreach ($this->contacts as $c) {
      $c->delete();
    }
    parent::tearDown();
  }

  /**
   * Test that applying default filters excludes archived contacts.
   */
  public function testDefaultFilters() {
    $base_query = new BaseQuery();
    $filter_info = module_invoke_all('campaignion_manage_filter_info')['supporter'];
    foreach ($filter_info as $name => $class) {
      $filters[$name] = new $class($base_query->query());
    }
    $default[] = ['type' => 'name', 'removable' => FALSE];
    $default[] = ['type' => 'state', 'values' => ['value' => REDHEN_STATE_ACTIVE]];
    $filter_form = new FilterForm('supporter', $filters, $default);

    $form = [];
    $form_state = [];
    $filter_form->form($form, $form_state);
    $this->assertEquals('Status', $form['filter'][1]['#title']);

    $ids = array_map(function ($c) {
      return $c->contact_id;
    }, $this->contacts);
    $query = db_select('redhen_contact', 'r')
      ->fields('r', ['contact_id'])
      ->condition('contact_id', $ids);
    $filter_form->applyFilters($query);

    $contact_ids = $query->execute()->fetchCol();
    $this->assertEquals([$this->contacts[0]->contact_id], $contact_ids);
  }

}
