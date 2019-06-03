<?php

function opigno_collaborative_workspaces_db_get_collaborative_workspaces_ids_from_user_id($uid)
{
  $query = db_select('field_data_field_users_invited', 't')
    ->condition(
      db_and()
      ->condition('field_users_invited_target_id', $uid, '=')
      ->condition('bundle', 'collaborative_workspace', '='))
    ->fields('t', array('entity_id'))
    ->execute();

  $collaborative_workspaces_ids = array();
  while($row = $query->fetchAssoc())
    $collaborative_workspaces_ids[ $row['entity_id'] ] = $row['entity_id'];

  return $collaborative_workspaces_ids;
}

function opigno_collaborative_workspaces_db_get_users_ids_from_group($gid)
{
  $query = db_select('og_membership', 'og')
    ->condition(db_and()
      ->condition('gid', $gid, '=')
      ->condition('entity_type', 'user', '=')
      ->condition('state', '1', '='))
    ->fields('og', array('etid'))
    ->execute();

  $users_ids = array();
  while($row = $query->fetchAssoc())
    $users_ids[ $row['etid'] ] = $row['etid'];

  return $users_ids;
}


function opigno_collaborative_workspaces_db_get_users_ids_from_groups($gids)
{
  if (empty($gids))
    return array();

  $query = db_select('og_membership', 'og')
    ->condition(db_and()
      ->condition('gid', $gids, 'in')
      ->condition('entity_type', 'user', '=')
      ->condition('state', '1', '='))
    ->fields('og', array('etid'))
    ->execute();

  $users_ids = array();
  while($row = $query->fetchAssoc())
    $users_ids[ $row['etid'] ] = $row['etid'];

  return $users_ids;
}
