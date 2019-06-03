<?php

namespace Drupal\campaignion_manage\BulkOp;

class SupporterTagBatch extends BatchBase {
  protected $tids;
  protected $tag;
  public function __construct(&$data) {
    $this->tids = $data['tids'];
    $this->tag = (bool) $data['tag'];
  }
  public function apply($contact, &$results) {
    $needs_save = FALSE;
    if ($this->tag) {
      $tids = $this->tids;
      // Remove all already-set tags from te todo list.
      foreach ($contact->supporter_tags['und'] as $already_set) {
        unset($tids[$already_set['tid']]);
      }
      $needs_save = !empty($tids);
      // Add new field-items for the rest.
      foreach ($tids as $tid) {
        $contact->supporter_tags['und'][] = array('tid' => $tid);
      }
    }
    else {
      // delete tags
      foreach ($contact->supporter_tags['und'] as $tag_index => $tag) {
        if (isset($this->tids[$tag['tid']])) {
          unset($contact->supporter_tags['und'][$tag_index]);
          $needs_save = TRUE;
        }
      }
    }
    if ($needs_save) {
      try {
        $contact->save();
      } catch (Exception $e) {
        $results['failed_contacts'][$contact->contact_id] = $e->getMessage();
      }
    }
  }
}
