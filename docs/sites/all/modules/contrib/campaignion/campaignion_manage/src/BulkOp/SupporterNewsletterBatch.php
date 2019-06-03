<?php

namespace Drupal\campaignion_manage\BulkOp;

class SupporterNewsletterBatch extends BatchBase {
  protected $lists;
  public function __construct(&$data) {
    $this->lists = array_map(
      array('\Drupal\campaignion_newsletters\NewsletterList', 'load'),
      array_values($data['list_ids'])
    );
  }
  public function apply($contact, &$result) {
    foreach ($this->lists as $list) {
      $list->subscribe($contact->email());
    }
  }
}
