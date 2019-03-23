<?php

/**
 * @file
 * DB functions for AMP.
 */

/**
 * AMP disabled value.
 */
const AMP_DISABLED = 0;

/**
 * Return TRUE if the node has amp enabled.
 *
 * @param $node_id
 *   Node ID of the node to check
 *
 * @return bool
 *   TRUE if AMP is enabled, FALSE otherwise.
 */
function amp_db_is_node_enabled($node_id) {
  $is_enabled = TRUE;
  $result = db_select('amp_node', 'n')
    ->fields('n', array('status'))
    ->condition('aid', $node_id, '=')
    ->execute()
    ->fetchAll();

  // If we don't have an entry as disabled (!empty()), AMP is enabled by default
  if (isset($result[0]->status) && $result[0]->status == AMP_DISABLED) {
    $is_enabled = FALSE;
  }

  return $is_enabled;
}

/**
 * Set a node as AMP disabled.
 *
 * @param $node_id
 *   Node ID of the node to set as disabled.
 *
 * @throws \Exception
 */
function amp_db_disable_amp($node_id) {
  db_merge('amp_node')
    ->key(array('aid' => $node_id))
    ->fields(array('status' => AMP_DISABLED))
    ->execute();
}

/**
 * Set a node as AMP enabled.
 *
 * @param $node_id
 *   Node ID of the node to set as enabled.
 *
 * @throws \Exception
 */
function amp_db_enable_amp($node_id) {
  // Enable removes the flag from the db.
  amp_db_remove($node_id);
}

/**
 * Remove a given node from AMP.
 *
 * @param $node_id
 *   Node ID to remove.
 *
 * @throws \Exception
 */
function amp_db_remove($node_id) {
  // Deleting it from the table will set it up as enabled by default and will
  // keep the table as small as possible.
  db_delete('amp_node')
    ->condition('aid', $node_id)
    ->execute();
}
