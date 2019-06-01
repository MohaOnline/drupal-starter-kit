<?php

/**
 * @file
 *
 * Plugin to provide an argument handler for a node revision id
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t("Workbench Revision: ID"),
  'keyword' => 'revision',
  'description' => t('Creates a node context from a workbench revision ID argument.'),
  'context' => 'workbench_moderation_argument_workbench_rid_context',
  'placeholder form' => array(
    '#type' => 'textfield',
    '#description' => t('Enter the revision ID of a node for this argument'),
  ),
);

/**
 * Discover if this argument gives us the node we crave.
 */
function workbench_moderation_argument_workbench_rid_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if ($empty) {
    return ctools_context_create_empty('node');
  }

  // We can accept either a node object or a pure nid.
  if (is_object($arg)) {
    return ctools_context_create('node', $arg);
  }

  if (!is_numeric($arg)) {
    return FALSE;
  }

  $node = node_load($arg);

  // Is there a current revision?
  if (isset($node->workbench_moderation['current']->vid)) {
    // Ensure that we will return the current revision
    if ($node->vid != $node->workbench_moderation['current']->vid) {
      $node = node_load($node->nid, $node->workbench_moderation['current']->vid);
    }
  }

  if (!$node) {
    return FALSE;
  }

  return ctools_context_create('node', $node);
}
