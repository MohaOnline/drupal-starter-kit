<?php

/**
 * @file
 * Views Relationship handler to allow joins from an arbitrary entity.
 *
 * This is similar to recurly_entity_owner_handler, but works for joining from
 * an entity table instead of to an entity table.
 *
 * To use this handler, the Views table definition must contain an
 * 'base type' key specifying the base table for the specific handler.
 */

class recurly_entity_owner_reverse_handler extends views_handler_relationship {

  /**
   * Override query().
   *
   * This method is overridden to add $def['extra'] to allow us to filter by
   * entity type when joining to an entity table. There's no way in Views to
   * modify just the definition at a point where we have access to table
   * aliases.
   */
  public function query() {
    // Figure out what base table this relationship brings to the party.
    $table_data = views_fetch_data($this->definition['base']);
    $base_field = empty($this->definition['base field']) ? $table_data['table']['base']['field'] : $this->definition['base field'];

    $this->ensure_my_table();

    $def = $this->definition;
    $def['table'] = $this->definition['base'];
    $def['field'] = $base_field;
    $def['left_table'] = $this->table_alias;
    $def['left_field'] = $this->real_field;
    if (!empty($this->options['required'])) {
      $def['type'] = 'INNER';
    }

    // Use a short alias for this:
    $alias = $def['table'] . '_' . $this->table;

    $def['extra'] = sprintf("%s.entity_type = '%s'", $alias, $def['base type']);

    if (!empty($def['join_handler']) && class_exists($def['join_handler'])) {
      $join = new $def['join_handler']();
    }
    else {
      $join = new views_join();
    }

    $join->definition = $def;
    $join->options = $this->options;
    $join->construct();
    $join->adjusted = TRUE;

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);

    // Add access tags if the base table provide it.
    if (empty($this->query->options['disable_sql_rewrite']) && isset($table_data['table']['base']['access query tag'])) {
      $access_tag = $table_data['table']['base']['access query tag'];
      $this->query->add_tag($access_tag);
    }
  }
}
