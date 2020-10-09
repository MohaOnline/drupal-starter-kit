<?php

/**
 * @file
 * Synonyms module integration with Features.
 */

/**
 * Implements hook_features_export().
 */
function synonyms_features_export($data, &$export, $module_name) {
  $pipe = array();

  $export['features']['synonyms'] = array();
  foreach ($data as $v) {
    list($entity_type, $bundle, $provider, $behavior) = explode(':', $v);
    $behavior_definition = synonyms_behaviors();
    $behavior_definition = $behavior_definition[$behavior];
    $export['dependencies'][] = $behavior_definition['module'];

    $export['features']['synonyms'][$v] = $v;

    $behavior_implementation = synonyms_behavior_get_all_enabled($entity_type, $bundle, $provider, $behavior);
    $behavior_implementation = reset($behavior_implementation);

    $provider_info = synonyms_behavior_implementation_info($entity_type, $bundle, $behavior);
    $provider_info = $provider_info[$provider];
    $export['dependencies'][] = $provider_info['module'];

    $pipe = array_merge_recursive($pipe, $behavior_implementation['object']->featuresExportPipe());
  }

  $export['dependencies'][] = 'synonyms';
  $export['dependencies'] = drupal_map_assoc(array_unique($export['dependencies']));
  return $pipe;
}

/**
 * Implements hook_features_export_options().
 */
function synonyms_features_export_options() {
  $options = array();

  foreach (synonyms_behavior_get_all_enabled() as $behavior_implementation) {
    $key = array(
      $behavior_implementation['entity_type'],
      $behavior_implementation['bundle'],
      $behavior_implementation['provider'],
      $behavior_implementation['behavior'],
    );
    if ($behavior_implementation['entity_type'] == $behavior_implementation['bundle']) {
      $label = t('@entity_type @provider @behavior', array(
        '@entity_type' => $behavior_implementation['entity_type'],
        '@provider' => $behavior_implementation['label'],
        '@behavior' => $behavior_implementation['behavior'],
      ));
    }
    else {
      $label = t('@entity_type @bundle @provider @behavior', array(
        '@entity_type' => $behavior_implementation['entity_type'],
        '@bundle' => $behavior_implementation['bundle'],
        '@provider' => $behavior_implementation['label'],
        '@behavior' => $behavior_implementation['behavior'],
      ));
    }
    $options[implode(':', $key)] = $label;
  }

  return $options;
}

/**
 * Implements hook_features_export_render().
 */
function synonyms_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $synonyms = array();';
  foreach ($data as $name) {
    list($entity_type, $bundle, $provider, $behavior) = explode(':', $name);
    $behavior_implementation = synonyms_behavior_get_all_enabled($entity_type, $bundle, $provider, $behavior);
    $behavior_implementation = reset($behavior_implementation);
    if (is_array($behavior_implementation)) {
      $behavior_implementation = array_intersect_key($behavior_implementation, drupal_map_assoc(array(
        'entity_type', 'bundle', 'provider', 'settings', 'behavior',
      )));
    }
    $code[] = "  \$synonyms['{$name}'] = " . features_var_export($behavior_implementation, '  ') .";";
  }
  $code[] = "  return \$synonyms;";
  $code = implode("\n", $code);
  return array('default_synonyms' => $code);
}

/**
 * Implements hook_features_revert().
 */
function synonyms_features_revert($module_name) {
  $synonyms = module_invoke($module_name, 'default_synonyms');
  foreach ($synonyms as $v) {
    synonyms_behavior_implementation_save($v);
  }
}

/**
 * Implements hook_features_rebuild().
 */
function synonyms_features_rebuild($module_name) {
  $synonyms = module_invoke($module_name, 'default_synonyms');
  foreach ($synonyms as $v) {
    synonyms_behavior_implementation_save($v);
  }
}
