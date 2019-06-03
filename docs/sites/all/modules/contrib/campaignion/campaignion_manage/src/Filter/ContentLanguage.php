<?php

namespace Drupal\campaignion_manage\Filter;

class ContentLanguage extends Base implements FilterInterface {
  protected $query;

  public function __construct(\SelectQueryInterface $query) {
    $this->query = $query;
  }

  protected function getOptions() {
    $query = clone $this->query;
    $fields =& $query->getFields();
    $fields = array(
      'language' => array(
        'field' => 'language',
        'table' => 'n',
        'alias' => 'language',
      ),
    );
    $query->groupBy('n.language');
    $langs_in_use = $query->execute()->fetchCol();
    $options = array();
    if (in_array('', $langs_in_use)) {
      $options[''] = t('Language neutral');
    }
    $lang_list = language_list();
    foreach ($langs_in_use as $lang) {
      if (isset($lang_list[$lang])) {
        $options[$lang] = $lang_list[$lang]->native;
      }
    }

    return $options;
  }

  public function formElement(array &$form, array &$form_state, array &$values) {
    $form['language'] = array(
      '#type'          => 'select',
      '#title'         => t('Language'),
      '#options'       => $this->getOptions(),
      '#default_value' => isset($values) ? $values : NULL,
    );
  }
  public function title() { return t('Language'); }
  public function apply($query, array $values) {
    $query->condition('n.language', $values['language']);
  }
  public function defaults() {
    return array('language' => language_default()->language);
  }
  public function isApplicable($current) {
    return empty($current) && count($this->getOptions()) > 1;
  }
}
