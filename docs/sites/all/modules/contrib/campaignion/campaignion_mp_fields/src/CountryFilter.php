<?php

namespace Drupal\campaignion_mp_fields;

use \Drupal\campaignion_manage\Filter\Base;

/**
 * Filter supporters by MP country.
 */
class CountryFilter extends Base {

  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function getOptions() {
    $query = db_select('field_data_mp_country', 'mp_country');
    $query->innerJoin('taxonomy_term_data', 'mp_country_term', 'mp_country.mp_country_tid = mp_country_term.tid');
    $fields =& $query->getFields();
    $fields = array();
    $query->fields('mp_country_term', ['tid', 'name']);
    $query->groupBy('mp_country_term.tid');

    $countries = [];
    foreach ($query->execute() as $row) {
      $countries[$row->tid] = $row->name;
    }
    return $countries;
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['mp_country'] = array(
      '#type'          => 'select',
      '#title'         => t('Devolved country'),
      '#options'       => $this->getOptions(),
      '#default_value' => isset($values) ? $values : NULL,
      '#empty_value' => '',
    );
  }

  public function apply($query, array $values) {
    if ($values['mp_country']) {
      $query->innerJoin('field_data_mp_country', 'mp_country', "mp_country.entity_type='redhen_contact' AND mp_country.entity_id=r.contact_id");
      $query->condition('mp_country.mp_country_tid', $values['mp_country']);
    }
    else {
      $query->leftJoin('field_data_mp_country', 'mp_country', "mp_country.entity_type='redhen_contact' AND mp_country.entity_id=r.contact_id");
      $query->isNull('mp_country.entity_id');
    }
  }

  public function isApplicable($current) {
    return empty($current) && count($this->getOptions()) > 0;
  }

  public function defaults() {
    return ['mp_country' => NULL];
  }

  public function title() {
    return t('Devolved country');
  }

}
