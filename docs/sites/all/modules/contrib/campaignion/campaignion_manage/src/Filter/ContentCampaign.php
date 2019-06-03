<?php

namespace Drupal\campaignion_manage\Filter;

class ContentCampaign extends ContentNodeReference {
  public function __construct(\SelectQueryInterface $query) {
    $reference_field  = 'field_reference_to_campaign';
    $reference_column = 'field_reference_to_campaign_nid';
    parent::__construct($query, $reference_field, $reference_column);
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    parent::formElement($form, $form_state, $values);
    $form['nid']['#title'] = t('Campaign');
  }
  public function title() { return t('Campaign'); }
}
