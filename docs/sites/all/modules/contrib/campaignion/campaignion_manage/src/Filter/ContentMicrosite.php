<?php

namespace Drupal\campaignion_manage\Filter;

class ContentMicrosite extends ContentNodeReference {
  public function __construct(\SelectQueryInterface $query) {
    $reference_field  = variable_get('campaignion_microsite_node_reference_field', 'field_reference_to_campaign');
    $reference_column = variable_get('campaignion_microsite_node_reference_column', 'field_reference_to_campaign_nid');
    parent::__construct($query, $reference_field, $reference_column);
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    parent::formElement($form, $form_state, $values);
    $form['nid']['#title'] = t('Micro-Site');
  }
  public function title() { return t('Micro-Site'); }
}
