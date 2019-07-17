<?php
// $Id$

/**
 * @file
 * Contains the 'smart_ip_views_bridge_handler_filter_location_country' filter handler.
 */

class smart_ip_views_bridge_handler_filter_location_country extends location_handler_filter_location_country {
  /**
   * {@inheritdoc}
   */
  public function get_value_options() {
    parent::get_value_options();
    $smart_ip_options = smart_ip_views_bridge_form_select_field_tokens();
    $this->value_options = array_merge($this->value_options, $smart_ip_options);
  }

  /**
   * {@inheritdoc}
   */
  public function operator_options($which = 'title') {
    $operators = parent::operator_options();
    $operators['empty'] = t('Is empty (NULL)');
    $operators['not empty'] = t('Is not empty (NOT NULL)');
    return $operators;
  }

  /**
   * {@inheritdoc}
   */
  public function query() {
    if ($this->operator != 'empty' && $this->operator != 'not empty') {
      parent::query();
    }

    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";
    
    if ($this->operator == 'empty') {
      $operator = "IS NULL";
      $this->query->add_where($this->options['group'], $field, NULL, $operator);
    }
    elseif ($this->operator == 'not empty') {
      $operator = "IS NOT NULL";
      $this->query->add_where($this->options['group'], $field, NULL, $operator);
    }
  }
}
