<?php

/**
 * @file
 * Filter by whether term has children.
 *
 * @ingroup views_filter_handlers
 */
class taxonomy_term_depth_handler_filter_has_children extends views_handler_filter_boolean_operator {

  function query() {
    $this->ensure_my_table();
    $this->query->add_where_expression($this->options['group'], "(SELECT count(th.tid) FROM taxonomy_term_hierarchy th WHERE th.parent = $this->table_alias.$this->real_field) = $this->value");
  }

}
