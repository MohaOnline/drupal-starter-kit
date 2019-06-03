<?php

class opigno_filter_own_groups extends views_handler_filter {
  function query() {
    // Get all the results from the query
    $local_query = $this->query;

    // This is different if the view is based on OG membership or not
    if (!empty($local_query->tables['og_membership'])) {
      $local_query->add_field('og_membership', 'gid');
      $field = 'og_membership.gid';
    }
    else if (!empty($local_query->tables['node'])) {
      $local_query->add_field('node', 'nid');
      $field = 'node.nid';
    }
    else {
      return;
    }

    // Get all the groups for the current user
    $groups = og_get_entity_groups('user', null, array(OG_STATE_ACTIVE, OG_STATE_BLOCKED, OG_STATE_PENDING));
    if (empty($groups['node'])) {
      return;
    }

    // Remove theses courses from the results
    $this->query->add_where(
      $this->options['group'],
      $field,
      $groups['node'],
      'NOT IN'
    );
  }
}
