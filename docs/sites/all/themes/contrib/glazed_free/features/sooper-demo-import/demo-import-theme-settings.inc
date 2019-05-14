<?php

/**
 * Helper function export settings to .info format
 *
 * @return string
 *   The full .info settings.
 */
function _glazed_free_settings_export($theme) {
  $settings = variable_get('theme_' . $theme . '_settings');
  if (!$settings) {
    return false;
  }
  unset(
    $settings['glazed_free_settings__active_tab'],
    $settings['global__active_tab'],
    $settings['info'],
    $settings['jquery_update_jquery_version'],
    $settings['theme']
  );
  $info = array();
  $info['settings'] = !empty($info['settings']) ? $info['settings'] : array();
  $info['settings'] = $settings;
  $info = _glazed_free_build_info_file($info);
  return $info;
}

/**
 * Helper function to build the contents of the .info file.
 * Based on Magic module's _magic_build_info_file
 *
 * This function will take the current theme information, and create the export
 * based upon the current info file and settings saved within the database.
 *
 * @param array $array
 *   An array of all the current theme settings to be parsed.
 * @param string $prefix
 *   (Optional)
 *
 * @return string
 *   The full, prettified .info file.
 */
function _glazed_free_build_info_file($array, $prefix = '') {
  $info = '';
  foreach ($array as $key => $value) {
    if (is_array($value)) {
      $info .= _glazed_free_build_info_file($value, (empty($prefix) ? $key : "{$prefix}[{$key}]"));
    }
    else {
      if (is_string($value)) {
        $value = '"' .str_replace('"', "'", $value) . '"';
      }
      $info .= $prefix ? ("{$prefix}[$key]") : $key;
      $info .= " = " . str_replace("'", "\'", $value) . "\n";
    }
  }
  return $info;
}

$form['glazed_free_settings']['import_export'] = array(
  '#title' => t('Import/Export'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#weight' => 70,
);

$form['glazed_free_settings']['import_export']['settings_export'] = array(
  '#title' => t('Export Theme Settings'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['glazed_free_settings']['import_export']['settings_export']['settings_export_box'] = array(
  '#title' => t("Export for the theme's .info file", array('@theme' => $theme)),
  '#prefix' => '<p>' . t("For import in Theme Settings form or for your  subtheme's .info file") . '</p>',
  '#markup' => '<textarea class="glazed-settings-export"  onClick="this.select();">' . _glazed_free_settings_export($subject_theme) . '</textarea>',
);

$form['glazed_free_settings']['import_export']['settings_import'] = array(
  '#title' => t('Import Theme Settings'),
  '#type' => 'fieldset',
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
);

$form['glazed_free_settings']['import_export']['settings_import']['settings_import_box'] = array(
  '#type' => 'textarea',
  '#description' => t('Enter settings in .info file format. Any settings you enter here will override your current theme settings. This cannot be undone.'),
  '#rows' => 20,
  '#default_value' => '',
);

$form['glazed_free_settings']['import_export']['demo_import'] = array(
  '#title' => t('Import Demo Content'),
  '#type' => 'fieldset',
  '#description' => t("Demo content packages will help learn to use Glazed elements and modules. If you are on very poor hosting import packages one by one to avoid timeout issues. If you disable a demo content package it won't delete any content! Imported content can only be removed manually."),
  '#collapsible' => TRUE,
  '#collapsed' => TRUE
);

$form['glazed_free_settings']['import_export']['demo_import'] = array(
  '#title' => t('Import Demo Content'),
  '#type' => 'fieldset',
  '#description' => t("Demo content packages will help you get started quickly and learn to use Glazed elements and modules."),
  '#collapsible' => TRUE,
  '#collapsed' => TRUE
);

$form['glazed_free_settings']['import_export']['subtheme_demo_import'] = array(
  '#title' => t('Import Complete Site Demo'),
  '#type' => 'fieldset',
  '#description' => t("Demo content packages will help you get started quickly and learn to use Glazed elements and modules."),
  '#collapsible' => TRUE,
  '#collapsed' => TRUE
);

if (module_exists('uuid_features')) {
  module_load_include('module', 'uuid');
  module_load_include('inc', 'features', 'features.admin');
  $demo_content_modules = array_filter(_features_get_features_list(), '_glazed_free_is_demo_content_exclude_subtheme');

  usort($demo_content_modules, function($a, $b) {
    return (count($a->info['features']['uuid_node']) < count($b->info['features']['uuid_node'])) ? 1 : -1;
  });

  foreach ($demo_content_modules as $module) {
    if (isset($module->info['features']) && isset($module->info['features']['uuid_node'])) {
      $node_sample = $module->info['features']['uuid_node'][0];
      if (entity_get_id_by_uuid('node', array($node_sample))) {
        $checkbox = 1;
      }
      else {
        $checkbox = 0;
      }

      $form['glazed_free_settings']['import_export']['demo_import'][$module->name] = array(
        '#type' => 'checkbox',
        '#title' => $module->info['name'],
        '#description' => count($module->info['features']['uuid_node']) . ' ' . t('Pages'),
        '#default_value' => $checkbox,
      );
    }
  }
}
else {
  $form['glazed_free_settings']['demo_import']['#prefix'] = '<p><em>' . 'Please enable the UUID Features module to enable demo import' . '</em></p>';
}

$form['glazed_free_settings']['import_export']['subtheme_demo_import'] = array(
  '#title' => t('Import Complete Site Demo'),
  '#type' => 'fieldset',
  '#description' => t("Demo content packages will help you get started quickly and learn to use Glazed elements and modules."),
  '#collapsible' => TRUE,
  '#collapsed' => TRUE
);

if (module_exists('uuid_features')) {
  module_load_include('module', 'uuid');
  module_load_include('inc', 'features', 'features.admin');
  $demo_content_modules = array_filter(_features_get_features_list(), '_glazed_free_is_subtheme');

  usort($demo_content_modules, function($a, $b) {
    return (count($a->info['features']['uuid_node']) < count($b->info['features']['uuid_node'])) ? 1 : -1;
  });

  foreach ($demo_content_modules as $module) {
    if (isset($module->info['features']) && isset($module->info['features']['uuid_node'])) {
      $node_sample = $module->info['features']['uuid_node'][0];
      if (entity_get_id_by_uuid('node', array($node_sample))) {
        $checkbox = 1;
      }
      else {
        $checkbox = 0;
      }

      $form['glazed_free_settings']['import_export']['subtheme_demo_import'][$module->name] = array(
        '#type' => 'checkbox',
        '#title' => $module->info['name'],
        '#description' => count($module->info['features']['uuid_node']) . ' ' . t('Pages'),
        '#default_value' => $checkbox,
      );
    }
  }
}
else {
  $form['glazed_free_settings']['demo_import']['#prefix'] = '<p><em>' . 'Please enable the UUID Features module to enable demo import' . '</em></p>';
}
