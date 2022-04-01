<?php

namespace Drupal\campaignion_manage\Filter;

class ContentTitle extends Base implements FilterInterface {

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['title'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Filter by typing the title'),
      '#maxlength'     => 255,
      '#size'          => 40,
      '#default_value' => isset($values['title']) ? $values['title'] : NULL,
    );
  }

  public function title() { return t('Filter by typing the title'); }

  public function apply($query, array $values) {
    if (!empty($values['title'])) {
      $search = preg_replace('/[[:blank:]]+/', '%', $values['title']);
      $search = preg_replace('/%%+/', '%', $search);
      $search = preg_replace('/^%/', '', $search);
      $search = preg_replace('/%$/', '', $search);
      $search = strtolower($search);
      $search_condition = db_or()
        ->where('LOWER(n.title) LIKE :search_string', array( ':search_string' => '%' . $search . '%'));

      if (module_exists('campaignion_node_admin_title')) {
        $query->leftJoin('field_data_field_admin_title', 'admin_title', "%alias.entity_type='node' AND %alias.entity_id=n.nid");
        $search_condition->where('LOWER(admin_title.field_admin_title_value) LIKE :search_string', array( ':search_string' => '%' . $search . '%'));
      }
      $query->condition($search_condition);
    }
  }
  public function defaults() {
    return array('title' => '');
  }
}
