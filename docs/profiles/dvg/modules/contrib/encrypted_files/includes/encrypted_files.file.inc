<?php

/**
 * @file
 * Defines helper functions for working with encrypted files.
 */

/**
 * Gets a file's info based on its URI.
 *
 * @param string $uri
 *   Full URI to parse
 *
 * @return object|null
 *   File's file object, or NULL if file not found
 */
function encrypted_files_load_file_by_uri($uri) {
  // Parse the URI if necessary.
  $scheme = ENCRYPTED_FILES_SCHEME . '://';
  if (strpos($uri, $scheme) === FALSE) {
    $menu_entry = 'system/' . ENCRYPTED_FILES_DIRECTORY . '/';
    $uri = drupal_substr($uri, strpos($uri, $menu_entry) +
      drupal_strlen($menu_entry));
    $uri = $scheme . $uri;
  }
  // Load the file entity.
  $loader = new EntityFieldQuery();
  $loader->entityCondition('entity_type', 'file');
  $loader->propertyCondition('uri', $uri);
  $result = $loader->execute();
  if (empty($result)) {
    return NULL;
  }
  // Return file object.
  $result = array_shift($result['file']);
  return file_load($result->fid);
}

/**
 * Loads a node associated with a given encrypted file.
 *
 * @param object $file
 *   File object to use in targeting the node
 *
 * @return object|null
 *   Node's node object, or NULL if node not found
 */
function encrypted_files_load_files_node($file) {
  if (!isset($file)) {
    return NULL;
  }
  // Load references to the file.
  $results = file_get_file_references($file);
  if (empty($results)) {
    return NULL;
  }
  // Load node from first result, as each file
  // should only be associated with one node.
  $result = array_shift($results);
  $result_node = current($result['node']);
  $node = node_load($result_node->nid);
  if (!$node) {
    return NULL;
  }
  return $node;
}
