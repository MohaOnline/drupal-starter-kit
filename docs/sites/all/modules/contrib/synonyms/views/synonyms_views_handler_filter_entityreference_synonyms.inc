<?php

/**
 * @file
 * Definition of synonyms_views_handler_filter_entityreference_synonyms class.
 */

/**
 * Definition of synonyms friendly entity reference field filter.
 */
class synonyms_views_handler_filter_entityreference_synonyms extends views_handler_filter_numeric {

  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array(
      'default' => 'numeric',
    );

    return $options;
  }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Type'),
      '#options' => array(
        'numeric' => t('Numeric'),
        'synonyms_autocomplete' => t('Synonyms friendly autocomplete'),
        'synonyms_select' => t('Synonyms friendly select list'),
      ),
      '#default_value' => $this->options['type'],
    );
  }

  function has_extra_options() {
    return TRUE;
  }

  function operators() {
    $operators = parent::operators();
    switch ($this->options['type']) {
      case 'synonyms_autocomplete':
      case 'synonyms_select':
        // Only "equals" and "not equals" make sense, other operators are rather
        // funky when it comes to IDs and not simple integers.
        $operators = array_intersect_key($operators, drupal_map_assoc(array(
          '=', '!=',
        )));
        break;
    }
    return $operators;
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    if (isset($form['value']['#type'])) {
      $element = &$form['value'];
    }
    elseif (isset($form['value']['value']['#type'])) {
      $element = &$form['value']['value'];
    }

    if (isset($element)) {
      $field = field_info_field($this->definition['field_name']);
      $entity_type = array_keys($field['bundles']);
      $entity_type = reset($entity_type);
      $bundle = reset($field['bundles'][$entity_type]);
      $instance = field_info_instance($entity_type, $field['field_name'], $bundle);

      switch ($this->options['type']) {
        case 'synonyms_autocomplete':
          $widget = field_info_widget_settings('synonyms_autocomplete_entity');
          $element['#autocomplete_path'] = $widget['synonyms_autocomplete_path'] . '/' . $field['field_name'] . '/' . $instance['entity_type'] . '/' . $instance['bundle'];
          $element['#attached']['js'][drupal_get_path('module', 'synonyms') . '/js/synonyms-autocomplete.js'] = array();
          $element['#attributes']['class'][] = 'synonyms-autocomplete';
          break;

        case 'synonyms_select':
          $element['#type'] = 'select';
          $element['#options'] = synonyms_select_entity_options($field, $instance);
          if (!$this->is_exposed()) {
            $element['#empty_option'] = t('- None -');
          }
          $element['#element_validate'][] = 'synonyms_select_validate';
          $element['#element_validate'][] = 'synonyms_select_views_entityreference_filter_validate';
          unset($element['#size']);
          break;
      }
    }
  }

  function query() {
    switch ($this->options['type']) {
      case 'synonyms_autocomplete':
        $field = field_info_field($this->definition['field_name']);
        $this->value['value'] = synonyms_get_entity_by_synonym($field['settings']['target_type'], $this->value['value'], synonyms_field_target_bundles($field));
        break;
    }
    parent::query();
  }

  function admin_summary() {
    if ($this->is_a_group()) {
      return t('grouped');
    }
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    switch ($this->options['type']) {
      case 'numeric':
      case 'synonyms_autocomplete':
        return parent::admin_summary();
        break;

      case 'synonyms_select':
        $field = field_info_field($this->definition['field_name']);
        $entity = entity_load($field['settings']['target_type'], array($this->value['value']));
        $entity = reset($entity);
        if (is_object($entity)) {
          $label = entity_label($field['settings']['target_type'], $entity);
          $options = $this->operator_options('short');
          return check_plain($options[$this->operator]) . ' ' . check_plain($label);
        }
        break;
    }
  }
}

/**
 * Form element validate handler.
 *
 * Simply convert select value from an array (as mostly operated within Field
 * module) to a single value.
 */
function synonyms_select_views_entityreference_filter_validate($element, &$form_state) {
  $value = drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  $value = reset($value);
  form_set_value($element, $value, $form_state);
}
