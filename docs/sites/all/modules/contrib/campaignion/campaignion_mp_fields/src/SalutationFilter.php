<?php

namespace Drupal\campaignion_mp_fields;

use \Drupal\campaignion_manage\Filter\Base;

class SalutationFilter extends Base {
  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function getOptions() {
    $query = db_select('field_data_mp_salutation', 'c')
      ->condition('c.entity_type', 'redhen_contact');
    $query->fields('c', ['mp_salutation_value']);
    $query->groupBy('c.mp_salutation_value');
    $query->orderBy('c.mp_salutation_value');

    $c = $query->execute()->fetchCol();
    return array_combine($c, $c);
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['mp_salutation'] = array(
      '#type' => 'select',
      '#title' => t('MP salutation'),
      '#options' => $this->getOptions(),
      '#default_value' => isset($values) ? $values : NULL,
      '#empty_value' => FALSE,
    );
  }

  public function title() {
    return t('MP salutation');
  }

  public function apply($query, array $values) {
    if ($values['mp_salutation']) {
      $query->innerJoin('field_data_mp_salutation', 'mp_salutation', "mp_salutation.entity_type='redhen_contact' AND mp_salutation.entity_id=r.contact_id");
      $query->condition('mp_salutation.mp_salutation_value', $values['mp_salutation']);
    }
    else {
      $query->leftJoin('field_data_mp_salutation', 'mp_salutation', "mp_salutation.entity_type='redhen_contact' AND mp_salutation.entity_id=r.contact_id");
      $query->isNull('mp_salutation.entity_id');
    }
  }

  public function isApplicable($current) {
    return empty($current) && count($this->getOptions()) > 0;
  }

  public function defaults() {
    $options = $this->getOptions();
    return ['mp_salutation' => NULL];
  }

}
