<?php

namespace Drupal\campaignion_manage\BulkOp;

use \Drupal\campaignion_manage\BatchJob;

class SupporterTag implements BatchInterface {
  protected $tag;
  /**
    * @param $tag if set to TRUE, the operation is to add tags
    *    to the supporters; in every other case it means it will
    *    remove the tag
    */
  public function __construct($tag) {
    $this->tag = (bool) $tag;
  }

  public function title() { return $this->tag ? t('Add tag') : t('Remove tag'); }

  public function helpText() {
    return $this->tag ? t('Add one or more tags to the currently selected supporters.') : t('Remove one or more tags to the currently selected supporters.');
  }

  public function formElement(&$element, &$form_state) {
    $options = array();
    foreach (taxonomy_get_tree(taxonomy_vocabulary_machine_name_load('supporter_tags')->vid) as $term) {
      $options[$term->tid] = $term->name;
    }
    $element['tag'] = array(
      '#type' => 'select',
      '#title'   => t('Select one or more tags'),
      '#options' => $options,
      '#multiple' => TRUE,
      '#select2' => [
        'allowClear' => TRUE,
      ],
    );
  }

  public function apply($resultset, $values) {
    $term_ids = array();
    foreach($values['tag'] as $tid => $value) {
      if ($value) {
        $term_ids[$tid] = $tid;
      }
    }
    $data['tids'] = $term_ids;
    $data['tag'] = $this->tag;
    if ($this->tag) {
      $messages = array(
        'title'            => t('Add tags to supporters ...'),
        'init_message'     => t('Start adding tags to supporters...'),
        'progress_message' => t('Start adding tags to supporters...'),
        'error_message'    => t('Encountered an error while adding tags to supporters.'),
        'status_message'   => t('Added tags to @current out of @total supporters.'),
      );
    }
    else {
      $messages = array(
        'title'            => t('Delete tags from supporters ...'),
        'init_message'     => t('Start deleting tags from supporters...'),
        'progress_message' => t('Start deleting tags from supporters...'),
        'error_message'    => t('Encountered an error while deleting tags from supporters.'),
        'status_message'   => t('Deleted tags from @current out of @total supporters.'),
      );
    }
    $job = new BatchJob($this, $resultset, $data, $messages);
    $job->set();
  }

  public function getBatch(&$data) {
    return new SupporterTagBatch($data);
  }

  public function batchFinish(&$data, &$results) {
    if (isset($results['failed_contacts'])) {
      $count = 0;
      foreach($results['failed_contacts'] as $contact_id => $error_message) {
        $count++;
        if ($count < 11) {
          if ($this->tag) {
            drupal_set_message(
              t("Couldn't add tags to contact with ID @contact_id: @message",
                array('@contact_id' => $contact_id, '@message' => $error_message)
              )
            );
          }
          else {
            drupal_set_message(
              t("Couldn't remove tags from contact with ID @contact_id: @message",
                array('@contact_id' => $contact_id, '@message' => $error_message)
              )
            );
          }
        }
      }
    }
  }
}
