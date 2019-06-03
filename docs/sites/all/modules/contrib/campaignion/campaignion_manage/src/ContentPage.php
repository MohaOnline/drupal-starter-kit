<?php

namespace Drupal\campaignion_manage;

use Drupal\campaignion_manage\BulkOp\ContentPublish;
use Drupal\campaignion_manage\BulkOp\ContentUnpublish;
use Drupal\campaignion_manage\Filter\ContentCampaign;
use Drupal\campaignion_manage\Filter\ContentLanguage;
use Drupal\campaignion_manage\Filter\ContentMicrosite;
use Drupal\campaignion_manage\Filter\ContentTitle;
use Drupal\campaignion_manage\Filter\ContentStatus;
use Drupal\campaignion_manage\Filter\ContentType;
use Drupal\campaignion_manage\Query\Content;

/**
 * Manage content page.
 */
class ContentPage extends Page {

  /**
   * Construct a new page including all it’s forms and listings.
   *
   * @param \Drupal\campaignion_manage\Query\Content $query
   *   The current filtered query.
   */
  public function __construct(Content $query) {
    $this->baseQuery = $query;
    $select = $query->query();

    $filters['title'] = new ContentTitle();
    $defaultActive = array('title');
    if (module_exists('campaignion_microsite')) {
      $filters['microsite'] = new ContentMicrosite($select);
    }
    if (module_exists('campaignion_campaign')) {
      $filters['campaign'] = new ContentCampaign($select);
    }
    $filters['language'] = new ContentLanguage($select);
    $filters['type'] = new ContentType($select);
    $filters['status'] = new ContentStatus();
    $default[] = array('type' => 'title', 'removable' => FALSE);
    $this->filterForm = new FilterForm('content', $filters, $default);

    $this->listing = new ContentListing(20);
    $ops = [];
    if (!module_exists('change_publishing_status_permission') || user_access('change publishing status')) {
      $ops['publish'] = new ContentPublish();
      $ops['unpublish'] = new ContentUnpublish();
    }
    $this->bulkOpForm = new BulkOpForm($ops);
  }

  /**
   * Get the node IDs for the current query (ie. for bulk operations).
   *
   * @return int[]
   *   node IDs for all nodes matched by the current filters.
   */
  protected function getSelectedIds($form, &$form_state) {
    return $this->listing->selectedIds($form['listing'], $form_state, $this->baseQuery);
  }

}
