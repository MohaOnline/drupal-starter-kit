<?php

namespace Drupal\campaignion_manage\Filter;

class SupporterTag extends Base implements FilterInterface {
  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function getOptions() {
    $query  = clone $this->query;
    $query->innerJoin('field_data_supporter_tags', 'st', "r.contact_id = st.entity_id AND st.entity_type = 'redhen_contact'");
    $query->innerJoin('taxonomy_term_data', 't', "st.supporter_tags_tid = t.tid");
    $fields =& $query->getFields();
    $fields = array();
    $query->fields('t', array('tid', 'name'));
    $query->groupBy('t.tid');

    return $query->execute()->fetchAllKeyed();
  }
  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['operator'] = array(
      '#type'          => 'select',
      '#title'         => t('Operator'),
      '#options'       => array('IN' => t('is'), 'NOT IN' => t('is not')),
      '#default_value' => isset($values['operator']) ? $values['operator'] : NULL,
    );
    $form['tag'] = array(
      '#type'          => 'select',
      '#title'         => t('Tag'),
      '#options'       => $this->getOptions(),
      '#default_value' => isset($values['tag']) ? $values['tag'] : NULL,
    );
  }

  public function title() { return t('Is tagged with'); }

  public function apply($query, array $values) {
    $inner = db_select('field_data_supporter_tags', 'st_inner')
      ->fields('st_inner', array('entity_id'))
      ->condition('entity_type', 'redhen_contact')
      ->condition('supporter_tags_tid', $values['tag']);
    $query->condition('r.contact_id', $inner, $values['operator']);
  }

  public function isApplicable($current) {
    return count($current) <= 3 && count($this->getOptions()) > 0;
  }

  public function defaults() {
    $options = $this->getOptions();
    return array('operator' => 'IN', 'tag' => key($options));
  }
}
