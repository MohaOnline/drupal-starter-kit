<?php
/**
 * @file
 * Enables modules and site configuration for a Drupal Voor Gemeenten site installation.
 */

/**
 * Profile details callback.
 */
function dvg_profile_details() {
  $details['language'] = 'nl';
  return $details;
}

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 */
function dvg_form_install_configure_form_alter(&$form, $form_state) {
  $form['site_information']['site_name']['#default_value'] = st('Drupal voor Gemeenten');
  $form['server_settings']['#access'] = FALSE;
  $form['server_settings']['site_default_country']['#default_value'] = 'NL';
  $form['server_settings']['date_default_timezone']['#default_value'] = 'Europe/Amsterdam';

  // Disable javascript timezone select.
  unset($form['server_settings']['date_default_timezone']['#attributes']);
}

/**
 * Wrapper around node_export functionality to import nodes.
 *
 * @see: node_export_import();
 */
function _dvg_import_nodes($original_nodes) {
  module_load_include('inc', 'node_export', 'formats/drupal');

  $used_format = 'drupal';
  $save = TRUE;

  foreach ($original_nodes as $original_node) {
    $node = node_export_node_clone($original_node);
    $nodes = array($node);

    drupal_alter('node_export_import', $nodes, $used_format, $save);
    node_export_file_field_import($nodes[0], $original_node);
    node_export_save($nodes[0]);
    drupal_alter('node_export_node_import', $node, $original_node, $save);
  }
}
