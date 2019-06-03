<?php

class opigno_class_app_filter_courses_from_classes extends views_handler_filter {
  function query() {
    global $user;

    // Get all the results from the query
    $view_result = clone $this->view;
    $local_query = $this->query;

    if (!empty($local_query->tables['og_membership'])) {
      $local_query->add_field('og_membership', 'gid');

      // Add the user constraint
      $local_query->add_field('og_membership', 'etid');
      $local_query->add_field('og_membership', 'entity_type');
      $local_query->add_where(
        $this->options['group'],
        'og_membership.etid',
        $user->uid
      );
      $local_query->add_where(
        $this->options['group'],
        'og_membership.entity_type',
        'user'
      );

      $field = 'og_membership.gid';
    }
    else if (!empty($local_query->tables['node'])) {
      $local_query->add_field('node', 'nid');
      $field = 'node.nid';
    }
    else {
      return;
    }

    $local_query->build($view_result);
    $select_query = $local_query->query();
    if (empty($select_query)) {
      return;
    }
    $result_nids = $select_query->execute()->fetchAllKeyed(0, 0);
    if (empty($result_nids)) {
      return;
    }

    // Get the courses that are part of classes that are results from the query
    $courses_from_classes = db_select('field_data_opigno_class_courses', 'occ')
      ->fields('occ', array('opigno_class_courses_target_id'))
      ->condition('bundle', 'class')
      ->condition('entity_id', $result_nids)
      ->execute()->fetchAllAssoc('opigno_class_courses_target_id');
    $courses_from_classes = array_keys($courses_from_classes);
    if (empty($courses_from_classes)) {
      return;
    }

    // Remove theses courses from the results
    $this->query->add_where(
      $this->options['group'],
      $field,
      $courses_from_classes,
      'NOT IN'
    );
  }
}
