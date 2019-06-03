<?php

namespace Drupal\campaignion_manage\Filter;

class SupporterName extends Base implements FilterInterface {

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['name'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Filter by typing the supporter name'),
      '#maxlength'     => 255,
      '#size'          => 40,
      '#default_value' => isset($values['name']) ? $values['name'] : NULL,
    );
  }

  public function title() { return t('Filter by typing the supporter name'); }

  public function apply($query, array $values) {
    if (!empty($values['name'])) {
      $alias = $query->join('field_data_redhen_contact_email', 'fdrce', "%alias.entity_type='redhen_contact' AND %alias.entity_id=r.contact_id");
      $search = preg_replace('/[[:blank:]]+/', '%', $values['name']);
      $search = preg_replace('/%%+/', '%', $search);
      $search = trim($search, '%');
      $query->where("LOWER(CONCAT(r.first_name, ' ', r.middle_name, ' ', r.last_name, ' ', $alias.redhen_contact_email_value)) LIKE :search_string",
        array(':search_string' => '%' . strtolower($search) . '%')
      );
    }
  }
  public function defaults() {
    return array('name' => '');
  }
}
