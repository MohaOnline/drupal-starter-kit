<?php

class opigno_quiz_app_field_course_class_progress extends views_handler_field {
  function query() {
    // Nothing, to override parent query.
  }

  function render($values) {
    $output = '';

    if (!empty($values->node_og_membership_nid)) {
      $nid = $values->node_og_membership_nid;
    }
    else if (!empty($values->nid)) {
      $nid = $values->nid;
    }

    if (!empty($nid)) {
      $node = node_load($nid);

      switch($node->type) {
        case 'course':
          $output = (string) opigno_quiz_app_get_course_progression($nid);
          break;

        case 'class':
          $output = (string) opigno_quiz_app_get_class_progression($nid);
          break;
      }
    }

    return $output;
  }
}
