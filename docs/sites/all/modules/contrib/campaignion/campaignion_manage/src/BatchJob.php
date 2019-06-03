<?php

namespace Drupal\campaignion_manage;

class BatchJob {
  protected $bulkOp;
  protected $result;
  protected $data;
  protected $batchSize;
  protected $messages = array();

  public function __construct(BulkOp\BatchInterface $bulkOp, ResultSet $result, $data, $messages = array(), $batchSize = 100) {
    $messages += array(
      'title' => t('Applying bulk-operation ...'),
      'init_message' => t('Starting bulk-operation ...'),
      'progress_message' => t('Bulk-operation in progress ...'),
      'error_message' => t('Encountered an error while applying the bulk-operation.'),
      'status_message' => t('Applied bulk-opration to @current out of @total supporters.'),
    );
    $this->bulkOp = $bulkOp;
    $this->result = $result;
    $this->messages = $messages;
    $this->batchSize = $batchSize;
    $this->data = $data;
  }

  public function set() {
    $batch = array(
      'operations' => array(
        array('campaignion_manage_batch_process', array($this)),
      ),
      'finished' => 'campaignion_manage_batch_finished',
    ) + $this->messages;
    batch_set($batch);
  }

  public function init(&$context) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_id'] = 0;
    $context['sandbox']['max'] = $this->result->count();
    $context['results']['batch'] = $this;
  }

  public function apply(&$context) {
    if (!isset($context['sandbox']['progress'])) {
      $this->init($context);
    }
    $ids = $this->result->nextIds($context['sandbox']['current_id'], $this->batchSize);
    $contacts = redhen_contact_load_multiple($ids, array(), TRUE);
    $batch = $this->bulkOp->getBatch($this->data);
    $batch->start($context);
    foreach ($contacts as $contact) {
      $batch->apply($contact, $context['results']);
      $context['sandbox']['current_id'] = $contact->contact_id;
      $context['sandbox']['progress']++;
      $context['message'] = format_string($this->messages['status_message'], array('@current' => $context['sandbox']['progress'], '@total' => $context['sandbox']['max']));
    }
    $batch->commit();
    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
  }

  public function finish($success, $results, $operations) {
    if ($success) {
      if (isset($results['errors'])) {
        foreach($results['errors'] as $error_message) {
          drupal_set_message($error_message);
        }
      }
      else {
        $this->bulkOp->batchFinish($this->data, $results);
      }
    }
  }
}
