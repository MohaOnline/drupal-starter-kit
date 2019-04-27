<?php

/**
 * @file
 * All hooks required to export and import PowerTagging configurations with the
 * Features module.
 */

/**
 * Implementation of hook_features_export_options()
 */
function powertagging_config_features_export_options() {
  $options = array();
  $powertagging_configs = powertagging_config_load();
  foreach ($powertagging_configs as $powertagging_config) {
    $options[$powertagging_config->powertagging_id] = $powertagging_config->title;
  }
  return $options;
}

/**
 * Implementation of hook_features_export()
 */
function powertagging_config_features_export($data, &$export, $module_name = '') {
  $export['dependencies'][] = 'powertagging';

  foreach ((array) $data as $powertagging_id) {
    $export['features']['powertagging_config'][$powertagging_id] = $powertagging_id;
  }

  return array();
}

/**
 * Implementation of hook_features_export_render()
 */
function powertagging_config_features_export_render($module_name, $data) {
  $powertagging_config_array = array();
  foreach ($data as $powertagging_id) {
    $powertagging_config = powertagging_config_load($powertagging_id);
    if (!empty($powertagging_config)) {
      $powertagging_config->connection_details = array(
        'title' => $powertagging_config->connection->getTitle(),
        'url' => $powertagging_config->connection->getUrl(),
      );
      unset($powertagging_config->connection);
      $powertagging_config_array[$powertagging_id] = $powertagging_config;
    }
  }
  $code = '  $data = ' . features_var_export($powertagging_config_array, '  ') . ';' . PHP_EOL;
  $code .= '  return $data;';

  return array('powertagging_config_features_settings' => $code);
}

/**
 * Implementation of hook_features_rebuild()
 */
function powertagging_config_features_rebuild($module) {
  powertagging_config_features_revert($module);
}

/**
 * Implementation of hook_features_revert()
 */
function powertagging_config_features_revert($module) {
  if ($data = features_get_default('powertagging_config', $module)) {
    foreach ($data as $powertagging_id => $powertagging_config) {
      // Check if the connection already exists, otherwise create it.
      $potential_connections = SemanticConnector::searchConnections(array(
        'type' => 'pp_server',
        'url' => $powertagging_config['connection_details']['url'],
      ));

      // Connection already exists, use the first one found.
      if (!empty($potential_connections)) {
        $connection = reset($potential_connections);
      }
      // Connection doesn't exist yet, create it.
      else {
        $connection = SemanticConnector::createConnection('pp_server', $powertagging_config['connection_details']['url'], $powertagging_config['connection_details']['title']);
      }

      // Restore the PowerTagging configurations.
      db_query("DELETE FROM {powertagging} WHERE powertagging_id = :powertagging_id", array(':powertagging_id' => $powertagging_id));
      $ptid = db_insert('powertagging')
        ->fields(array(
          'powertagging_id' => $powertagging_id,
          'title' => $powertagging_config['title'],
          'connection_id' => $connection->getId(),
          'project_id' => $powertagging_config['project_id'],
          'config' => serialize($powertagging_config['config']),
        ))
        ->execute();

      // Set the last batch tagging variable.
      variable_set('powertagging_last_batch_tagging_' . $ptid, time());
    }
  }
}
