<?php

/**
 * @file
 * Definition of views_handler_filter_menu.
 */

/**
 * Simple filter to handle greater than/less than filters
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_menu extends views_handler_filter_boolean_operator {

  function construct() {
    parent::construct();
    $this->definition['type'] = 'included-excluded';
  }

  function get_value_options() {
    if (isset($this->definition['type'])) {
      if ($this->definition['type'] = 'included-excluded') {
        $this->value_options = array(1 => t('In menu'), 0 => t('Not in menu'));
      }
    }
  }

  function query() {
    $this->ensure_my_table();
    $field = "$this->table_alias.$this->real_field";

    if ($this->value) {
      $operator = "IS NOT NULL";
    }
    else {
      $operator = "IS NULL";
    }

    $this->query->add_where($this->options['group'], $field, NULL, $operator);
  }
}
