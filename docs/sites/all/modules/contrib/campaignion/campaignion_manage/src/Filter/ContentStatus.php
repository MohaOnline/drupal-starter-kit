<?php

namespace Drupal\campaignion_manage\Filter;

class ContentStatus extends Base implements FilterInterface {
  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['status'] = array(
      '#type' => 'select',
      '#title' => t('Publishing state'),
      '#options' => array(1 => t('Published'), 0 => t('Unpublished')),
      '#default_value' => isset($values) ? $values : NULL,
    );
  }
  public function title() { return t('Publishing state'); }
  public function apply($query, array $values) {
    $query->condition('n.status', $values['status']);
  }
  public function defaults() {
    return array('status' => 1);
  }
}
