<?php

namespace Drupal\campaignion_mp_fields;

use \Drupal\campaignion_manage\Filter\Base;

/**
 * Filter supporters by MP party.
 */
class PartyFilter extends Base {

  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function getOptions() {
    $query = db_select('field_data_mp_party', 'mp_party');
    $query->innerJoin('taxonomy_term_data', 'mp_party_term', 'mp_party.mp_party_tid = mp_party_term.tid');
    $fields =& $query->getFields();
    $fields = array();
    $query->fields('mp_party_term', ['tid', 'name']);
    $query->groupBy('mp_party_term.tid');

    $countries = [];
    foreach ($query->execute() as $row) {
      $countries[$row->tid] = $row->name;
    }
    return $countries;
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['mp_party'] = array(
      '#type'          => 'select',
      '#title'         => t('MP party'),
      '#options'       => $this->getOptions(),
      '#default_value' => isset($values) ? $values : NULL,
      '#empty_value' => '',
    );
  }

  public function apply($query, array $values) {
    if ($values['mp_party']) {
      $query->innerJoin('field_data_mp_party', 'mp_party', "mp_party.entity_type='redhen_contact' AND mp_party.entity_id=r.contact_id");
      $query->condition('mp_party.mp_party_tid', $values['mp_party']);
    }
    else {
      $query->leftJoin('field_data_mp_party', 'mp_party', "mp_party.entity_type='redhen_contact' AND mp_party.entity_id=r.contact_id");
      $query->isNull('mp_party.entity_id');
    }
  }

  public function isApplicable($current) {
    return empty($current) && count($this->getOptions()) > 0;
  }

  public function defaults() {
    return ['mp_party' => NULL];
  }

  public function title() {
    return t('MP party');
  }

}
