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
      $query->where('LOWER(n.title) LIKE :search_string', array( ':search_string' => '%' . strtolower($search) . '%'));
    }
  }
  public function defaults() {
    return array('title' => '');
  }
}
