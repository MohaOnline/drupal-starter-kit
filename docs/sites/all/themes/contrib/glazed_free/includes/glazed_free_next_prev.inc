<?php

/**
 *  Function that builds the list of nodes for next/previous node pager
 *  Adapted from Flippy module flippy_build_list function
 */
function glazed_free_next_prev($node) {
  $master_list = &drupal_static(__FUNCTION__);
  if (!isset($master_list)) {
    $master_list = array();
  }
  if (!isset($master_list[$node->nid])) {
    $order = 'ASC';
    $sort  = 'created';
    // Validate that the sort criteria is OK to use
    $base_table_properties = array_keys(array(
      'created' => t('Post date'),
      'title' => t('Title'),
      'nid' => t('Node ID'),
    ));
    // Depending on order, decide what before and after means
    $before = ($order == 'ASC') ? '<' : '>';
    $after  = ($order == 'ASC') ? '>' : '<';
    // Also decide what up and down means
    $up   = ($order == 'ASC') ? 'ASC' : 'DESC';
    $down = ($order == 'ASC') ? 'DESC' : 'ASC';
    // Create a starting-point EntityFieldQuery object
    global $language;
    $query = db_select('node', 'n');
    $query->condition('type', $node->type)
      ->condition('status', 1)
      ->condition('n.nid', $node->nid, '!=')
      ->condition(db_or()
        ->condition('n.language', array($language->language, LANGUAGE_NONE), 'IN')
        ->condition('n.language', $node->language, '=')
      )
      ->fields('n', array('nid', 'title'))
      ->range(0, 1)
      ->addTag('node_access')
      ->addTag('alternextprev_query');
    // Create the individual queries
    $prev   = clone $query;
    $next   = clone $query;

    // previous and next queries
    $prev->condition(db_or()
      ->condition($sort, $node->{$sort}, $before)
      ->condition(db_and()
        ->condition($sort, $node->{$sort}, '=')
        ->condition('n.nid', $node->nid, $before)
      )
    );
    $next->condition(db_or()
      ->condition($sort, $node->{$sort}, $after)
      ->condition(db_and()
        ->condition($sort, $node->{$sort}, '=')
        ->condition('n.nid', $node->nid, $after)
      )
    );
    // set the ordering
    $prev->orderBy($sort, $down);
    $next->orderBy($sort, $up);
    $prev->orderBy('n.nid', $down);
    $next->orderBy('n.nid', $up);
    // Execute the queries
    $list = array();
    $list['prev'] = $prev->execute()->fetchAssoc();
    $list['next'] = $next->execute()->fetchAssoc();

    // finally set the current info for themers to use
    $list['current'] = array(
      'nid' => $node->nid,
      'title' => $node->title,
    );

    $master_list[$node->nid] = $list;
  }
  return $master_list[$node->nid];
}
