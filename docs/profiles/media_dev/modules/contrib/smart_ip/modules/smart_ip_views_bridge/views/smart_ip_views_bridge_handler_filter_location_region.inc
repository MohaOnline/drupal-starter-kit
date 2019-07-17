<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_filter_location_region' filter handler.
 */

class smart_ip_views_bridge_handler_filter_location_region extends location_handler_filter_location_province {
  /**
   * {@inheritdoc}
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    if ($this->options['type'] == 'textfield') {
      // Add description showing Smart IP tokens
      $form['value']['#description'] .= smart_ip_views_bridge_form_fields_description();
    }
    elseif ($this->options['type'] == 'select') {
      // Add Smart IP tokens options
      $smart_ip_options = smart_ip_views_bridge_form_select_field_tokens();
      $form['value']['#default_value'] = $this->value;
      foreach ($smart_ip_options as $option_value => $option_label) {
        if (strpos($option_value, 'region_code')!== FALSE) {
          $form['value']['#options'][$option_value] = $option_label;
        }
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);
    $value = $this->value;
    $smart_ip_index = explode('.', $value);
    $smart_ip_index_count = count($smart_ip_index);
    if ($smart_ip_index_count == 3 && $smart_ip_index[0] == 'smart_ip') {
      // Enforce default value if Smart IP region is selected as default value
      $form[$this->field]['#default_value'] = $value;
    }
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    $value = $this->value;
    if (is_array($value)) {
      $value = $this->value[0];
    }
    $smart_ip_index = explode('.', $value);
    $smart_ip_index_count = count($smart_ip_index);
    if ($smart_ip_index_count == 3 && $smart_ip_index[0] == 'smart_ip') {
      // Smart IP module query
      $this->ensure_my_table();
      $field = "$this->table_alias.$this->real_field";
      $operator = ($this->operator == 'is') ? '=' : '!=';
      $this->query->add_where($this->options['group'], $field, $value, $operator);
    }
    else {
      // Location module query
      parent::query();
    }
  }
}