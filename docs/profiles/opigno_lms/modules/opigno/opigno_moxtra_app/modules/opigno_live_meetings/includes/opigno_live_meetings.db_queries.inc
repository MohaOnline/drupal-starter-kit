<?php

function opigno_live_meetings_db_get_classes_nids_from_course($course_id)
{
  $query = db_select('field_data_opigno_class_courses', 't')
    ->condition(
      db_and()
        ->condition('opigno_class_courses_target_id', $course_id, '=')
        ->condition('bundle', 'class', '='))
    ->fields('t', array('entity_id'))
    ->execute();

  $classes_ids = array();
  while($row = $query->fetchAssoc())
    $classes_ids[ $row['entity_id'] ] = $row['entity_id'];

  return $classes_ids;
}

function opigno_live_meetings_db_get_live_meetings_nids_from_group($gid)
{
  $req = db_select('node', 'n');
  $req->condition('n.type', 'live_meeting', '=');
  $req->fields('n', array('nid'));
  $req->join('og_membership', 'ogm', 'n.nid = ogm.etid');
  $req->condition('ogm.entity_type', 'node', '=');
  $req->condition('ogm.gid', $gid, '=');
  $req = $req->execute();

  $live_meetings_ids = array();
  while ($row = $req->fetchAssoc())
    $live_meetings_ids[ $row['nid'] ] = $row['nid'];

  return $live_meetings_ids;
}