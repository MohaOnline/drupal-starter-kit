<?php

namespace Drupal\campaignion_mp_fields;

use \Drupal\campaignion_manage\Filter\Base;

class ConstituencyFilter extends Base {
  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function getOptions() {
    $query = db_select('field_data_mp_constituency', 'c')
      ->condition('c.entity_type', 'redhen_contact');
    $query->fields('c', ['mp_constituency_value']);
    $query->groupBy('c.mp_constituency_value');
    $query->orderBy('c.mp_constituency_value');

    $c = $query->execute()->fetchCol();
    return array_combine($c, $c);
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['mp_constituency'] = array(
      '#type' => 'select',
      '#title' => t('MP constituency'),
      '#options' => $this->getOptions(),
      '#default_value' => isset($values) ? $values : NULL,
      '#empty_value' => FALSE,
    );
  }

  public function title() {
    return t('MP Constituency');
  }

  public function apply($query, array $values) {
    if ($values['mp_constituency']) {
      $query->innerJoin('field_data_mp_constituency', 'mp_constituency', "mp_constituency.entity_type='redhen_contact' AND mp_constituency.entity_id=r.contact_id");
      $query->condition('mp_constituency.mp_constituency_value', $values['mp_constituency']);
    }
    else {
      $query->leftJoin('field_data_mp_constituency', 'mp_constituency', "mp_constituency.entity_type='redhen_contact' AND mp_constituency.entity_id=r.contact_id");
      $query->isNull('mp_constituency.entity_id');
    }
  }

  public function isApplicable($current) {
    return empty($current) && count($this->getOptions()) > 0;
  }

  public function defaults() {
    $options = $this->getOptions();
    return ['mp_constituency' => NULL];
  }

}
