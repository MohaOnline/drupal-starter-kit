<?php
/**
 * @file
 * Provides Features integration for CRM Core Data Import.
 */

/**
 * Implements hook_features_export_options().
 */
function crm_core_data_import_features_export_options() {
  $options = array();
  $importers = _crm_core_data_import_get_tasks();
  foreach ($importers as $importer) {
    $options[$importer->machine_name] = $importer->title . ' (' . $importer->machine_name . ')';
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function crm_core_data_import_features_export($data, &$export, $module_name = '') {
  // Set dependencies.
  $export['dependencies']['crm_core_data_import'] = 'crm_core_data_import';

  // Set features data.
  foreach ($data as $machine_name) {
    $export['features']['crm_core_data_import'][$machine_name] = $machine_name;
  }
  return array();
}

/**
 * Implements hook_features_export_render().
 */
function crm_core_data_import_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = "  \$crm_core_data_import = array();\n";
  foreach ($data as $machine_name) {
    $importer = crm_core_data_import_load_importer_machine_name($machine_name);
    // Remove source settings.
    if (!empty($importer->source_settings) && is_a($importer->source_plugin, 'CsvDataSourceHandler')) {
      unset($importer->source_settings);
    }
    unset($importer->source_plugin);
    unset($importer->id);
    unset($importer->lastimport);

    $code[] = "  \$crm_core_data_import['" . $machine_name . "'] = " . features_var_export($importer, '  ') . ";\n";
  }
  $code[] = '  return $crm_core_data_import;';
  $code = implode("\n", $code);
  return array('crm_core_data_import_features_default_settings' => $code);
}

/**
 * Implements of hook_features_rebuild().
 */
function crm_core_data_import_features_rebuild($module) {
  $items = module_invoke($module, 'crm_core_data_import_features_default_settings');

  // Loop over the items we need to recreate.
  foreach ($items as $machine_name => $item) {
    // Save importer to database.
    $importer = new CRMCoreDataImport();

    // If importer already exist - revert.
    $importer_id = crm_core_data_importer_id_by_machine_name($machine_name);
    if (!empty($importer_id)) {
      $importer = crm_core_data_import_load_importer($importer_id);
    }
    foreach ($item as $field_key => $field) {
      $importer->{$field_key} = $field;
    }
    $importer->save();
  }
}

/**
 * Implements of hook_features_revert().
 */
function crm_core_data_import_features_revert($module) {
  crm_core_data_import_features_rebuild($module);
}
