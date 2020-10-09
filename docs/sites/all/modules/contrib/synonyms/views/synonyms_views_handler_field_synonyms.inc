<?php

/**
 * @file
 * Definition of synonyms_handler_field_synonyms class.
 */

/**
 * Views field handler for displaying synonyms of an entity.
 */
class synonyms_views_handler_field_synonyms extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['list'] = array(
      'default' => 'ul',
    );

    $options['separator'] = array(
      'default' => '',
      'translatable' => TRUE,
    );

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['list'] = array(
      '#type' => 'radios',
      '#title' => t('Display type'),
      '#options' => array(
        'ul' => t('Unordered list'),
        'ol' => t('Ordered list'),
        'separator' => t('Simple separator'),
      ),
      '#default_value' => $this->options['list'],
    );

    $form['separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => $this->options['separator'],
      '#dependency' => array(
        'radio:options[list]' => array('separator'),
      ),
    );
  }

  function pre_render(&$values) {
    // Load all the queried entities in a single batch.
    $entity_ids = array();
    foreach ($values as $value) {
      $entity_ids[] = $value->{$this->definition['real field']};
    }
    $entity_ids = array_unique($entity_ids);
    if (!empty($entity_ids)) {
      $entities = entity_load($this->definition['synonyms entity type'], $entity_ids);
      foreach ($values as &$value) {
        $value->synonyms_entity = $entities[$value->{$this->definition['real field']}];
        unset($value);
      }
    }
    parent::pre_render($values);
  }

  function get_value($values, $field = NULL) {
    $property = $this->field;
    $wrapper = entity_metadata_wrapper($this->definition['synonyms entity type'], $values->synonyms_entity);
    $synonyms = $wrapper->$property->value(array('sanitize' => TRUE));

    if (empty($synonyms)) {
      $synonyms = '';
    }
    else {
      switch ($this->options['list']) {
        case 'ol':
        case 'ul':
          $synonyms = theme('item_list', array(
            'type' => $this->options['list'],
            'items' => $synonyms,
          ));
          break;

        case 'separator':
          $synonyms = implode($this->options['separator'], $synonyms);
          break;
      }
    }

    return $synonyms;
  }

  function sanitize_value($value, $type = NULL) {
    if (is_null($type)) {
      $type = 'xss_admin';
    }
    return parent::sanitize_value($value, $type);
  }
}
