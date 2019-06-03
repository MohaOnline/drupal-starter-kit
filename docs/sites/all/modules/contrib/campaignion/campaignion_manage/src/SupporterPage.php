<?php

namespace Drupal\campaignion_manage;

use Drupal\campaignion_manage\Query\Base as BaseQuery;

class SupporterPage extends Page {
  public function __construct(BaseQuery $query) {
    $this->baseQuery = $query;

    $filter_info = module_invoke_all('campaignion_manage_filter_info')['supporter'];
    foreach ($filter_info as $name => $class) {
      $filters[$name] = new $class($this->baseQuery->query());
    }
    $default[] = ['type' => 'name', 'removable' => FALSE];
    $default[] = [
      'type' => 'state',
      'values' => ['value' => REDHEN_STATE_ACTIVE],
    ];
    $this->filterForm = new FilterForm('supporter', $filters, $default);

    $bulkOps = array();
    if (module_exists('campaignion_supporter_tags')) {
      $bulkOps['tag']   = new BulkOp\SupporterTag(TRUE);
      $bulkOps['untag'] = new BulkOp\SupporterTag(FALSE);
    }
    $bulkOps['export'] = new BulkOp\SupporterExport();
    if (module_exists('campaignion_newsletters')) {
      $bulkOps['newsletter'] = new BulkOp\SupporterNewsletter();
    }
    $this->listing = new SupporterListing(20);
    $this->bulkOpForm = new BulkOpForm($bulkOps);
  }

  protected function getSelectedIds($form, &$form_state) {
    $element = &$form['listing'];
    $result = $this->baseQuery->result();
    $values = &drupal_array_get_nested_value($form_state['values'], $element['#array_parents']);
    if (empty($values['bulkop_select_all_matching'])) {
      $result->purge();
      $query = db_insert('campaignion_manage_result')
        ->fields(array('meta_id', 'contact_id'));
      $ids = array();
      foreach ($values['bulk_id'] as $id => $selected) {
        if ($selected) {
          $query->values(array(
            'meta_id' => $result->id,
            'contact_id' => $id,
          ));
        }
      }
      $query->execute();
    }
    return $result;
  }
}
