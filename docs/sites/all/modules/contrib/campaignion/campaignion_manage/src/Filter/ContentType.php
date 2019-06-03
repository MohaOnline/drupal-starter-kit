<?php

namespace Drupal\campaignion_manage\Filter;

class ContentType extends Base implements FilterInterface {
  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function getOptions() {
    $query  = clone $this->query;
    $fields =& $query->getFields();
    $fields = array(
      'type' => array(
        'field' => 'type',
        'table' => 'n',
        'alias' => 'type',
      ),
    );
    $query->groupBy('n.type');
    $options = array();
    $type_names = node_type_get_names();
    foreach ($query->execute()->fetchCol() as $type) {
      $options[$type] = $type_names[$type];
    }
    return $options;
  }
  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['type'] = array(
      '#type'          => 'select',
      '#title'         => t('Type of page'),
      '#options'       => $this->getOptions(),
      '#default_value' => isset($values) ? $values : NULL,
    );
  }
  public function title() { return t('Type of page'); }

  public function apply($query, array $values) {
    $query->condition('n.type', $values['type']);
  }
  public function isApplicable($current) {
    return empty($current) && count($this->getOptions()) > 1;
  }

  public function defaults() {
    $options = $this->getOptions();
    return array('type' => key($options));
  }
}
