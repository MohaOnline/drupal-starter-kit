<?php

namespace Drupal\campaignion_manage\BulkOp;

use Drupal\campaignion_manage\BatchJob;
use Drupal\campaignion_newsletters\NewsletterList;

class SupporterNewsletter implements BatchInterface {
  public function title() {
    return 'Subscribe to Newsletter';
  }

  public function helpText() {
    return t('Subscribe the selected supporters to one or more newsletters.');
  }

  public function formElement(&$element, &$form_state) {
    $element['lists'] = array(
      '#type'    => 'checkboxes',
      '#title'   => t('Select one or lists'),
      '#options' => NewsletterList::options(),
    );
  }

  public function apply($resultset, $values) {
    $list_ids = array();
    foreach($values['lists'] as $list_id => $value) {
      if ($value) {
        $list_ids[] = $list_id;
      }
    }
    $data['list_ids'] = $list_ids;
    $messages = array(
      'title'            => t('Subscribing supporters to newsletters ...'),
      'init_message'     => t('Start subscribing supporters to newsletters...'),
      'progress_message' => t('Start subscribing supporters to newsletters...'),
      'error_message'    => t('Encountered an error while subscribing supporters to newsletters.'),
      'status_message'   => t('Subscribed @current out of @total supporters to @lists newsletters.'),
    );
    $job = new BatchJob($this, $resultset, $data, $messages);
    $job->set();
  }

  public function getBatch(&$data) {
    return new SupporterNewsletterBatch($data);
  }

  public function batchFinish(&$data, &$results) {}
}
