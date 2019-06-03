<?php
/**
 * @file
 * Configuration pages for CRM Core Data Import.
 */

/**
 * Page callback for data import dashboard.
 */
function crm_core_data_import_dashboard_form($form, &$form_state) {
  $items = array();
  $available = _crm_core_data_import_get_tasks();
  crm_core_ui_ctools_add_dropbutton_files();

  foreach ($available as $key => $importer) {
    $importer = crm_core_data_import_load_importer($key);

    // Operations.
    $links = array();
    $links[] = l(t('Edit source'), 'admin/structure/crm-core/data-import/' . $key . '/source-selection');
    $links[] = l(t('Edit source settings'), 'admin/structure/crm-core/data-import/' . $key . '/source-settings');
    if (!empty($importer->source_plugin->sourceMapping)) {
      $links[] = l(t('Edit source mapping'), 'admin/structure/crm-core/data-import/' . $key . '/source-mapping');
    }
    $links[] = l(t('Edit field mapping'), 'admin/structure/crm-core/data-import/' . $key . '/field-mapping');
    $links[] = l(t('Edit settings'), 'admin/structure/crm-core/data-import/' . $key . '/settings');
    $links[] = l(t('Delete'), 'admin/structure/crm-core/data-import/' . $key . '/delete');

    $stats = _crm_core_data_import_migration_statistic_get($importer);

    $items[$key] = array(
      'title' => $importer->title,
      'description' => $importer->description,
      'lastimport' => empty($importer->lastimport) ? t('Unknown') : format_date($importer->lastimport, 'custom', 'Y-m-d H:i:s'),
      'status' => _crm_core_data_import_migration_status($stats['status']),
      'items' => $stats['total'],
      'imported' => $stats['imported'],
      'unprocessed' => $stats['total'] - $stats['processed'],
      'operations' => theme(
        'crm_core_ui_ctools_dropbutton',
        array(
          'ops_count' => count($links),
          'links' => $links,
          'index' => $key,
        )
      ),
    );
  }

  $form['operation'] = array(
    '#type' => 'select',
    '#title' => t('With selected:'),
    '#options' => array(
      'import' => t('Start import'),
      'rollback' => t('Rollback'),
      'deregister' => t('Remove migration settings'),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
  );

  $form['items'] = array(
    '#type' => 'tableselect',
    '#header' => array(
      'title' => t('Title'),
      'description' => t('Description'),
      'status' => t('Status'),
      'items' => t('Items'),
      'imported' => t('Imported'),
      'unprocessed' => t('Unprocessed'),
      'lastimport' => t('Last imported'),
      'operations' => t('Operations'),
    ),
    '#options' => $items,
    '#empty' => t('No items available'),
  );
  $form['#attached']['css'][] = _crm_core_data_import_css_path();
  return $form;
}

/**
 * Submit handler for data import dashboard.
 */
function crm_core_data_import_dashboard_form_submit(&$form, &$form_state) {
  $importers = $form_state['values']['items'];
  $operation = $form_state['values']['operation'];
  $operations = array();
  $options = array('value' => 0, 'unit' => 'items');
  foreach ($importers as $importer_id) {
    if (!empty($importer_id)) {
      $importer = crm_core_data_import_load_importer($importer_id);
      switch ($operation) {
        case 'import':
          // If source not configured.
          $source_fields = $importer->getSourceFields();
          if (!empty($source_fields)) {
            // Run import selected operation.
            $mapping = $importer->getMappingSettings();
            if (!empty($mapping)) {
              _crm_core_data_import_build_entity_operations($operations, $mapping, $importer_id, $options);
            }
            else {
              drupal_set_message(t('Mapping is empty. Please configure your !mapping.', array('!mapping' => l(t('mapping'), 'admin/structure/crm-core/data-import/' . $importer->id . '/field-mapping'))), 'error');
            }
          }
          else {
            drupal_set_message(t('Source fields are empty. Please configure your !source.', array('!source' => l(t('source'), 'admin/structure/crm-core/data-import/' . $importer->id . '/source-settings'))), 'error');
          }
          break;

        case 'rollback':
          // Rollback for all registered migrations.
          $registered_migrations = variable_get('crm_core_data_import_migrations_' . $importer->id, array());
          foreach ($registered_migrations as $machine_name) {
            $migration = Migration::getInstance($machine_name);
            if ($migration) {
              $operations[] = array(
                'crm_core_data_import_batch',
                array('rollback', $machine_name, $options, 0),
              );
            }
          }
          break;

        case 'deregister':
          // Deregister all registered migrations.
          $registered_migrations = variable_get('crm_core_data_import_migrations_' . $importer->id, array());
          foreach ($registered_migrations as $machine_name) {
            $migration = Migration::getInstance($machine_name);
            if ($migration) {
              Migration::deregisterMigration($machine_name);
              drupal_set_message(t('Deregistered "@task_name" task', array('@task_name' => $machine_name)));
            }
          }
          _crm_core_data_import_migration_statistic_reset($importer);
          break;
      }

      if (count($operations) > 0) {
        $batch = array(
          'operations' => $operations,
          'title' => t('Import processing'),
          'file' => drupal_get_path('module', 'crm_core_data_import') . '/crm_core_data_import.admin.inc',
          'init_message' => t('Starting import process'),
          'progress_message' => '',
          'error_message' => t('An error occurred. Some or all of the import processing has failed.'),
          'finished' => 'crm_core_data_import_migration_batch_finish',
        );
        batch_set($batch);

        // Set importer id for finished callback. Might be improved.
        // There no another way to pass variable to finished callback.
        $_SESSION['data_import_batch_importer_id'] = $importer_id;
      }
    }
  }
}

/**
 * Batch API item callback.
 */
function crm_core_data_import_batch($operation, $machine_name, $limit, $force, &$context) {
  module_load_include('inc', 'migrate_ui', 'migrate_ui.pages');
  migrate_ui_batch($operation, $machine_name, $limit, $force, $context);
}

/**
 * Batch API finished callback. Trigger controllers for settings.
 */
function crm_core_data_import_migration_batch_finish($success, $results, $operations) {
  if ($success) {
    if (!empty($_SESSION['data_import_batch_importer_id'])) {
      $importer = crm_core_data_import_load_importer($_SESSION['data_import_batch_importer_id']);
      $importer->lastimport = time();
      $importer->save();
      _crm_core_data_import_migration_statistic_reset($importer);
      crm_core_data_settings_batch($_SESSION['data_import_batch_importer_id']);
      unset($_SESSION['data_import_batch_importer_id']);
    }
  }
}

/**
 * Run batch for plugins.
 */
function crm_core_data_settings_batch($importer_id) {
  // Get importer.
  $operations = array();
  $importer = crm_core_data_import_load_importer($importer_id);
  $mapping = $importer->getMappingSettings();
  if (!empty($mapping)) {
    // Collect results.
    foreach ($mapping as $key => $mapping_item) {
      list($entity_controller_type, $entity_bundle, $bundle_delta) = explode(':', $key);
      $entity_type = crm_core_data_import_get_destination_entity_type($entity_controller_type);
      $machine_name = _crm_core_data_import_migration_machine_name($importer_id, $entity_type, $entity_bundle, $bundle_delta);
      $migration = Migration::getInstance($machine_name);
      if ($migration) {
        $map = $migration->getMap();
        $source_key_map = $map->getSourceKeyMap();
        $importer_query = db_select($map->getMapTable(), 'map');
        foreach ($source_key_map as $name => $alias) {
          $importer_query->addField('map', $alias);
        }
        // There is no MigrateMap::getDestinationKeyMap() function. WTF?
        $importer_query->addField('map', 'destid1');

        foreach ($importer->getRelationDestinationEndPoints(implode(':', array($entity_type, $entity_bundle, $bundle_delta))) as $key => $ep) {
          $importer_query->addField('map', 'relation_id' . ($key + 1));
        }


        $importer_result = $importer_query
          ->execute();

        // Add to imported entities necessary information.
        $entity_type = $migration->getEntityType();
        $bundle = $migration->getEntityBundle();
        foreach ($importer_result as $row) {
          $data = array(
            'entity_type' => $entity_type,
            'bundle' => $bundle,
            'entity_id' => $row->destid1,
            'delta' => $bundle_delta,
          );
          foreach ($importer->getRelationDestinationEndPoints(implode(':', array($entity_type, $entity_bundle, $bundle_delta))) as $key => $endpoint) {
            $name = 'relation_id' . ($key + 1);
            $data[$name] = $row->{$name};
          }
          $source_keys = array();
          foreach ($source_key_map as $name => $alias) {
            $data[$alias] = $row->{$alias};
            $source_keys[$alias] = $row->{$alias};
          }

          $operations[] = array(
            'crm_core_data_run_settings_plugins_for_entity',
            array($machine_name, $source_keys, $importer, $data),
          );
        }
      }
    }

    if (count($operations) > 0) {
      // Clear static $batch array to run new batch import.
      $batch =& batch_get();
      $batch = array();
      $batch_definition = array(
        'operations' => $operations,
        'title' => t('Processing plugins settings'),
        'file' => drupal_get_path('module', 'crm_core_data_import') . '/crm_core_data_import.admin.inc',
        'init_message' => t('Starting process'),
        'progress_message' => '',
        'error_message' => t('An error occurred. Some or all of the import processing has failed.'),
        'finished' => 'crm_core_data_run_settings_plugins_finish',
      );
      batch_set($batch_definition);
      batch_process('admin/structure/crm-core/data-import');
    }
  }
}

/**
 * Batch operation which trigger plugins for each imported entity.
 *
 * @param array $source_keys
 */
function crm_core_data_run_settings_plugins_for_entity($machine_name, $source_keys, $importer, $data, &$context) {
  $importer_settings = $importer->getSettings();
  if (!empty($importer_settings['settings_controllers'])) {
    foreach ($importer_settings['settings_controllers'] as $controller) {
      $settings_controller = crm_core_data_import_load_plugin_instance('crm_core_data_import_settings', $controller);
      $settings_controller->postImport($importer, $data);
    }
  }

  $context['results'][$machine_name] = t('Processed entities from @instance_name migrate instance.', array('@instance_name' => $machine_name));

  // While in progress, show the cumulative list of messages.
  $full_message = '';
  foreach ($context['results'] as $message) {
    $full_message .= $message . '<br />';
  }
  $context['message'] = $full_message;
}

/**
 * Batch API finished callback for plugins.
 */
function crm_core_data_run_settings_plugins_finish($success, $results, $operations) {
  drupal_set_message(t('Import successfully completed.'));
}

/**
 * Page callback for source selection page.
 */
function crm_core_data_import_source_selection($form, &$form_state, $importer_id = FALSE) {
  ctools_include('plugins');
  $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_source');

  $source_plugins = array();
  foreach ($plugins as $id => $plugin) {
    $source_plugins[$id] = $plugin['label'];
  }

  if ($importer_id) {
    $importer = crm_core_data_import_load_importer($importer_id);
  }
  else {
    $importer = new CRMCoreDataImport();
  }
  $form_state['importer'] = $importer;

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Data import name'),
    '#required' => TRUE,
    '#default_value' => $importer->title,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'crm_core_data_importer_id_by_machine_name',
      'source' => array('title'),
    ),
    '#default_value' => empty($importer->machine_name) ? '' : $importer->machine_name,
    '#disabled' => empty($importer->machine_name) ? FALSE : TRUE,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $importer->description,
  );
  $form['source_plugin'] = array(
    '#type' => 'radios',
    '#title' => t('Where is your data coming from? Please select a source from the list below.'),
    '#required' => TRUE,
    '#options' => $source_plugins,
    '#default_value' => $importer->source ? $importer->source : key($source_plugins),
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
  $form['#attached']['css'][] = _crm_core_data_import_css_path();
  crm_core_data_import_attach_pager($form, $importer, __FUNCTION__);
  return $form;
}

/**
 * Submit handler for source selection form.
 */
function crm_core_data_import_source_selection_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  $importer = $form_state['importer'];

  $importer->title = $values['title'];
  $importer->machine_name = $values['machine_name'];
  $importer->description = $values['description'];
  $importer->source = $values['source_plugin'];
  $importer->save();

  $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/source-settings';
}

/**
 * Title callback for source settings page.
 */
function crm_core_data_import_source_settings_title_callback($importer_id) {
  $importer = crm_core_data_import_load_importer($importer_id);
  return $importer->title;
}

/**
 * Page callback for source settings page.
 */
function crm_core_data_import_source_settings($form, &$form_state, $importer_id) {
  $importer = crm_core_data_import_load_importer($importer_id);
  $form_state['importer'] = $importer;

  // Attach source config form.
  $importer->source_plugin->configForm($form, $form_state, $importer->getSourceSettings());
  $form['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
    '#id' => 'button-previous',
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
    '#id' => 'button-next',
  );
  $form['#attached']['css'][] = _crm_core_data_import_css_path();
  crm_core_data_import_attach_pager($form, $importer, __FUNCTION__);
  return $form;
}

/**
 * Validation callback for source settings page.
 */
function crm_core_data_import_source_settings_validate(&$form, &$form_state) {
  $importer = $form_state['importer'];

  // Validate source settings.
  $importer->source_plugin->configFormValidate($form, $form_state, $importer->getSourceSettings());
}

/**
 * Submit callback for source settings page.
 */
function crm_core_data_import_source_settings_submit(&$form, &$form_state) {
  $importer = $form_state['importer'];

  // Submit source settings.
  $settings = $importer->source_plugin->configFormSubmit($form, $form_state, $importer->getSourceSettings(), $importer->id);

  // Set source settings.
  $importer->setSourceSettings($settings);
  $importer->save();

  if ($form_state['triggering_element']['#id'] == 'button-previous') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/source-selection';
  }
  elseif ($form_state['triggering_element']['#id'] == 'button-next' && $importer->source_plugin->sourceMapping) {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/source-mapping';
  }
  elseif ($form_state['triggering_element']['#id'] == 'button-next') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/field-mapping';
  }
}

/**
 * Mapping for source.
 */
function crm_core_data_import_source_mapping($form, &$form_state, $importer_id) {
  $importer = crm_core_data_import_load_importer($importer_id);
  $form_state['importer'] = $importer;

  if ($importer->source_plugin->sourceMapping) {

  }
  // Attach source config form.
  $importer->source_plugin->sourceMappingForm($form, $form_state, $importer->getSourceSettings());
  $form['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
    '#id' => 'button-previous',
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
    '#id' => 'button-next',
  );
  $form['#attached']['css'][] = _crm_core_data_import_css_path();
  crm_core_data_import_attach_pager($form, $importer, __FUNCTION__);
  return $form;
}

/**
 * Validation callback for source mapping page.
 */
function crm_core_data_import_source_mapping_validate(&$form, &$form_state) {
  $importer = $form_state['importer'];

  // Validate source settings.
  $importer->source_plugin->sourceMappingFormValidate($form, $form_state, $importer->getSourceSettings());
}

/**
 * Submit callback for source settings page.
 */
function crm_core_data_import_source_mapping_submit(&$form, &$form_state) {
  $importer = $form_state['importer'];

  // Submit source settings.
  $settings = $importer->source_plugin->sourceMappingFormSubmit($form, $form_state, $importer->getSourceSettings(), $importer->id);

  // Set source settings.
  $importer->setSourceSettings($settings);
  $importer->save();

  if ($form_state['triggering_element']['#id'] == 'button-previous') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/source-settings';
  }
  elseif ($form_state['triggering_element']['#id'] == 'button-next') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/field-mapping';
  }
}

/**
 * Title callback for field mapping page.
 */
function crm_core_data_import_field_mapping_title_callback($importer_id) {
  $importer = crm_core_data_import_load_importer($importer_id);
  return t('Field mapping for @title', array('@title' => $importer->title));
}

/**
 * Page callback for field mapping page.
 */
function crm_core_data_import_field_mapping($form, &$form_state, $importer_id) {
  $importer = crm_core_data_import_load_importer($importer_id);
  $form_state['importer'] = $importer;

  // Build form_state from data import settings.
  if (empty($form_state['values']['mapping']) && empty($form_state['triggering_element'])) {
    $mapping_settings = $importer->getMappingSettings();
    foreach ($mapping_settings as $key => $mapping_instance) {
      list($entity_type, $entity_bundle,) = explode(':', $key);
      $form_state['values']['mapping'][$key]['fields'] = $mapping_instance;
      $entity_key = $entity_type . ':' . $entity_bundle;
      if (empty($form_state['values']['mapping'][$entity_key . '_delta'])) {
        $form_state['values']['mapping'][$entity_key . '_delta'] = 1;
      }
      else {
        $form_state['values']['mapping'][$entity_key . '_delta']++;
      }
    }
  }
  $form['#tree'] = TRUE;

  // Get available source fields.
  $source_fields = $importer->getSourceFields();
  _crm_core_data_import_attach_entity_form($form, $form_state);

  $form['mapping'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="mapping-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  _crm_core_data_import_attach_fields_form($form, $form_state, $source_fields);

  $form['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
    '#id' => 'button-previous',
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
    '#id' => 'button-next',
  );
  $form['#attached']['css'][] = _crm_core_data_import_css_path();
  crm_core_data_import_attach_pager($form, $importer, __FUNCTION__);
  return $form;
}

/**
 * Submit callback for field mapping page.
 */
function crm_core_data_import_field_mapping_submit(&$form, &$form_state) {
  /** @var CRMCoreDataImport $importer */
  $importer = $form_state['importer'];
  $mapping = array();

  // Build data import settings from form_state.
  if (!empty($form_state['values']['mapping'])) {
    $mapping_values = $form_state['values']['mapping'];
    foreach ($mapping_values as $key => $mapping_instance) {
      if (is_array($mapping_instance)) {
        foreach ($mapping_instance['fields'] as $field_key => $field_value) {
          if (!empty($field_value['remove_field'])) {
            unset($mapping_instance['fields'][$field_key]['remove_field']);
          }
        }
        $mapping[$key] = $mapping_instance['fields'];
      }
    }
    $importer->setMappingSettings($mapping);
  }
  else {
    $importer->setMappingSettings(array());
  }

  // Remove unnecessary entities from settings before save.
  $settings = $importer->getSettings();
  foreach ($settings as $settings_key => $item) {
    if (!empty($item['fields'])) {
      foreach ($item['fields'] as $settings_field_key => $settings_field) {
        $source_conrtoller_key = _crm_core_data_import_controller_key_by_entity_key($settings_field['source']);
        $destination_conrtoller_key = _crm_core_data_import_controller_key_by_entity_key($settings_field['destination']);
        if (!array_key_exists($source_conrtoller_key, $mapping) || !array_key_exists($destination_conrtoller_key, $mapping)) {
          unset($settings[$settings_key]['fields'][$settings_field_key]);
        }
      }
    }
  }
  $importer->setSettings($settings);
  $importer->save();

  if ($form_state['triggering_element']['#id'] == 'button-previous' && $importer->source_plugin->sourceMapping) {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/source-mapping';
  }
  elseif ($form_state['triggering_element']['#id'] == 'button-previous') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/source-settings';
  }
  elseif ($form_state['triggering_element']['#id'] == 'button-next') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/settings';
  }
}

/**
 * Entity types and bundles.
 */
function _crm_core_data_import_attach_entity_form(&$form, &$form_state) {
  ctools_include('plugins');
  $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_destination');
  $entity_plugins = array();
  $entity_bundles = array();
  foreach ($plugins as $id => $plugin) {
    $entity_plugins[$id] = $plugin['label'];
  }
  $form['entity_form'] = array(
    '#type' => 'container',
    '#prefix' => '<div id="entity-form-fieldset-wrapper">',
    '#suffix' => '</div>',
  );
  $form['entity_form']['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Add an:'),
    '#empty_option' => t('- Entity type -'),
    '#options' => $entity_plugins,
    '#ajax' => array(
      'callback' => '_crm_core_data_import_entity_form_entity_type_callback',
      'wrapper' => 'entity-form-fieldset-wrapper',
    ),
  );

  if (!empty($form_state['values']['entity_form']['entity_type'])) {
    $destination = crm_core_data_import_load_plugin_instance('crm_core_data_import_destination', $form_state['values']['entity_form']['entity_type']);
    $entity_bundles = $destination->getBundles();
  }

  $form['entity_form']['entity_bundle'] = array(
    '#type' => 'select',
    '#title' => t('of type:'),
    '#options' => $entity_bundles,
  );
  $form['entity_form']['add_entity'] = array(
    '#type' => 'button',
    '#id' => 'add-entity-button',
    '#value' => t('Add'),
    '#ajax' => array(
      'callback' => '_crm_core_data_import_entity_form_add_entity_callback',
      'method' => 'replace',
      'wrapper' => 'mapping-fieldset-wrapper',
    ),
  );
}

/**
 * Entity fields and bundles.
 */
function _crm_core_data_import_attach_fields_form(&$form, &$form_state, $source_fields) {
  $importer = $form_state['importer'];

  // Button which was triggered.
  $triggering_element = !empty($form_state['triggering_element']) ? $form_state['triggering_element'] : FALSE;

  // If button is remove bundle.
  if (!empty($triggering_element['#parents'][2]) && $triggering_element['#parents'][2] == 'remove_bundle') {
    $key = $triggering_element['#parents'][1];
    if (!empty($form_state['values']['mapping'][$key])) {
      unset($form_state['values']['mapping'][$key]);
    }
  }

  // If buttons are move up or move down.
  if (!empty($triggering_element['#parents'][2]) && ($triggering_element['#parents'][2] == 'move_down' || $triggering_element['#parents'][2] == 'move_up')) {
    $key = $triggering_element['#parents'][1];
    $search = FALSE;
    $swap_mapping = $form_state['values']['mapping'];

    // If move up, reverse array.
    if ($triggering_element['#parents'][2] == 'move_down') {
      $mapping = $form_state['values']['mapping'];
    }
    else {
      $mapping = array_reverse($form_state['values']['mapping']);
    }

    foreach ($mapping as $mapping_key => $mapping_item) {
      if ($mapping_key == $key) {
        $search = TRUE;
      }
      elseif ($search == TRUE && is_array($mapping_item)) {
        $search = FALSE;
        $swap_mapping = _crm_core_data_import_array_swap($swap_mapping, $key, $mapping_key);
      }
    }
    $form_state['values']['mapping'] = $swap_mapping;
  }

  // If button is remove field.
  if (!empty($triggering_element['#parents'][4]) && $triggering_element['#parents'][4] == 'remove_field') {
    $entity_bundle_key = $triggering_element['#parents'][1];
    $field_key = $triggering_element['#parents'][3];
    unset($form_state['values']['mapping'][$entity_bundle_key]['fields'][$field_key]);
  }

  // Build form from form_state.
  if (!empty($form_state['values']['mapping'])) {

    // Attach elements entity fields.
    foreach ($form_state['values']['mapping'] as $key => $mapping) {
      if (is_array($mapping)) {
        list($entity_type, $entity_bundle, $bundle_delta) = explode(':', $key);
        $destination = crm_core_data_import_load_plugin_instance('crm_core_data_import_destination', $entity_type);
        $fields = $destination->getFields($entity_bundle);
        $html_id = drupal_html_id($key);

        // Add field with count of entities.
        $entity_key = $entity_type . ':' . $entity_bundle;
        $form['mapping'][$entity_key . '_delta'] = array(
          '#type' => 'value',
          '#value' => $form_state['values']['mapping'][$entity_key . '_delta'],
        );

        // If was triggered add-more-button, add new field.
        if (strpos($triggering_element['#id'], 'add-more') !== FALSE && $triggering_element['#parents'][1] == $key) {
          $form_state['values']['mapping'][$key]['fields'][] = array('source_field' => '', 'destination_field' => '');
        }

        foreach ($form_state['values']['mapping'][$key]['fields'] as $delta => $field) {
          $source_fields_values = $source_fields;
          $importer->source_plugin->mappingSourceFieldsAlter($source_fields_values, crm_core_data_import_get_destination_entity_type($entity_type), $entity_bundle, $importer);
          if ($delta === 'primary') {
            // Attach primary field.
            _crm_core_data_import_attach_elements_primary_fieldset($form, $form_state, $entity_type, $entity_bundle, $key, $html_id, $source_fields_values, $bundle_delta);
          }
          else {
            // Attach form elements for fields.
            _crm_core_data_import_attach_elements_field($form, $form_state, $key, $delta, $source_fields_values, $fields);
          }
        }
        // Attach add more button.
        _crm_core_data_import_attach_elements_add_more_button($form, $form_state, $key, $html_id);
      }
    }
  }

  // Add fieldset form new entity.
  if (!empty($form_state['values']['entity_form']['entity_type']) && !empty($form_state['values']['entity_form']['entity_bundle']) && $triggering_element['#id'] == 'add-entity-button') {
    $entity_type = $form_state['values']['entity_form']['entity_type'];
    $entity_bundle = $form_state['values']['entity_form']['entity_bundle'];
    $key = $entity_type . ':' . $entity_bundle;

    // Save count of entity bundle. Need to support multiple bundles.
    if (empty($form_state['values']['mapping'][$key . '_delta'])) {
      // If initial adding.
      $form_state['values']['mapping'][$key . '_delta'] = 1;
      $bundle_delta = 1;
    }
    else {
      // If bundle already exist.
      $form_state['values']['mapping'][$key . '_delta']++;
      $bundle_delta = $form_state['values']['mapping'][$key . '_delta'];
    }
    $form['mapping'][$key . '_delta'] = array(
      '#type' => 'value',
      '#value' => $bundle_delta,
    );

    $key .= ':' . $bundle_delta;
    $html_id = drupal_html_id($key);
    $source_fields_values = $source_fields;
    $importer->source_plugin->mappingSourceFieldsAlter($source_fields_values, crm_core_data_import_get_destination_entity_type($entity_type), $entity_bundle, $importer);
    _crm_core_data_import_attach_elements_primary_fieldset($form, $form_state, $entity_type, $entity_bundle, $key, $html_id, $source_fields_values, $bundle_delta);
    _crm_core_data_import_attach_elements_add_more_button($form, $form_state, $key, $html_id);
  }
  $importer->source_plugin->fieldMappingFormAlter($form, $form_state);
}

/**
 * Attach primary fieldset for entity bundle.
 *
 * @param array $form
 *   Main form.
 * @param array $form_state
 *   Main form state.
 * @param string $entity_type
 *   Entity type.
 * @param string $entity_bundle
 *   Entity bundle.
 * @param string $key
 *   Key of fieldset.
 * @param string $html_id
 *   HTML id for fieldset wrapper.
 * @param array $source_fields
 *   List of available fields from source.
 * @param string $bundle_delta
 *   Delta of bundle.
 */
function _crm_core_data_import_attach_elements_primary_fieldset(&$form, &$form_state, $entity_type, $entity_bundle, $key, $html_id, $source_fields, $bundle_delta) {
  if (empty($source_fields)) {
    $source_key = !empty($form_state['values']['mapping'][$key]['fields']['primary']['source_field_primary']) ? $form_state['values']['mapping'][$key]['fields']['primary']['source_field_primary'] : 0;
    $source_fields = array($source_key => t('Unavailable'));
  }
  $form['mapping'][$key] = array(
    '#type' => 'container',
    '#prefix' => '<div id="' . $html_id . '-wrapper">',
    '#suffix' => '</div>',
    '#attributes' => array(
      'class' => array('entity-fieldset-wrapper'),
    ),
  );
  $form['mapping'][$key]['text'] = array(
    '#markup' => '<h3>' . crm_core_data_import_get_destination_label($entity_type, $entity_bundle, $bundle_delta) . '</h3>',
  );
  $form['mapping'][$key]['move_up'] = array(
    '#type' => 'button',
    '#value' => t('Move Up'),
    '#name' => $html_id . '-move-up-button',
    '#ajax' => array(
      'callback' => '_crm_core_data_import_entity_type_action_callback',
      'method' => 'replace',
      'wrapper' => 'mapping-fieldset-wrapper',
    ),
  );
  $form['mapping'][$key]['move_down'] = array(
    '#type' => 'button',
    '#value' => t('Move Down'),
    '#name' => $html_id . '-move-down-button',
    '#ajax' => array(
      'callback' => '_crm_core_data_import_entity_type_action_callback',
      'method' => 'replace',
      'wrapper' => 'mapping-fieldset-wrapper',
    ),
  );
  $form['mapping'][$key]['remove_bundle'] = array(
    '#type' => 'button',
    '#value' => t('Remove'),
    '#name' => $html_id . '-remove-button',
    '#ajax' => array(
      'callback' => '_crm_core_data_import_entity_type_action_callback',
      'method' => 'replace',
      'wrapper' => 'mapping-fieldset-wrapper',
    ),
  );
  $form['mapping'][$key]['fields'] = array(
    '#type' => 'container',
  );
  $form['mapping'][$key]['fields']['primary'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('field-fieldset-wrapper'),
    ),
  );
  $form['mapping'][$key]['fields']['primary']['source_field_primary'] = array(
    '#type' => 'select',
    '#options' => $source_fields,
    '#default_value' => !empty($form_state['values']['mapping'][$key]['fields']['primary']['source_field_primary']) ? $form_state['values']['mapping'][$key]['fields']['primary']['source_field_primary'] : FALSE,
  );
  $form['mapping'][$key]['fields']['primary']['destination_field_primary'] = array(
    '#type' => 'select',
    '#options' => array('primary_field' => t('Primary field')),
    '#default_value' => 'primary_field',
    '#disabled' => TRUE,
  );
}

/**
 * Attach add more button to entity bundle fieldset.
 *
 * @param array $form
 *   Main form.
 * @param array $form_state
 *   Main form state.
 * @param string $key
 *   Key of fieldset.
 * @param string $html_id
 *   HTML id for fieldset wrapper.
 */
function _crm_core_data_import_attach_elements_add_more_button(&$form, &$form_state, $key, $html_id) {
  $form['mapping'][$key]['add_more'] = array(
    '#type' => 'button',
    '#default_value' => t('Add Another Field'),
    '#name' => $html_id . '-button',
    '#ajax' => array(
      'callback' => '_crm_core_data_import_add_another_field_callback',
      'method' => 'replace',
      'wrapper' => $html_id . '-wrapper',
    ),
  );
}

/**
 * Attach mapping field to entity bundle fieldset.
 *
 * @param array $form
 *   Main form.
 * @param array $form_state
 *   Main form state.
 * @param string $key
 *   Key of fieldset.
 * @param int $delta
 *   Delta in the current fieldset.
 * @param array $source_fields
 *   List of available fields from source.
 * @param array $fields
 *   List of available fields from destination.
 */
function _crm_core_data_import_attach_elements_field(&$form, &$form_state, $key, $delta, $source_fields, $fields) {
  if (empty($source_fields)) {
    $source_key = !empty($form_state['values']['mapping'][$key]['fields'][$delta]['source_field']) ? $form_state['values']['mapping'][$key]['fields'][$delta]['source_field'] : 0;
    $source_fields = array($source_key => t('Unavailable'));
  }
  else {
    $source_fields = array('default_value' => t('(provide value)')) + $source_fields;
  }

  $form['mapping'][$key]['fields'][$delta] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => array('field-fieldset-wrapper'),
    ),
  );
  $form['mapping'][$key]['fields'][$delta]['source_field'] = array(
    '#type' => 'select',
    '#options' => $source_fields,
    '#default_value' => !empty($form_state['values']['mapping'][$key]['fields'][$delta]['source_field']) ? $form_state['values']['mapping'][$key]['fields'][$delta]['source_field'] : FALSE,
  );
  $form['mapping'][$key]['fields'][$delta]['destination_field'] = array(
    '#type' => 'select',
    '#options' => $fields,
    '#default_value' => !empty($form_state['values']['mapping'][$key]['fields'][$delta]['destination_field']) ? $form_state['values']['mapping'][$key]['fields'][$delta]['destination_field'] : FALSE,
  );
  $form['mapping'][$key]['fields'][$delta]['default_value'] = array(
    '#type' => 'textfield',
    '#default_value' => !empty($form_state['values']['mapping'][$key]['fields'][$delta]['default_value']) ? $form_state['values']['mapping'][$key]['fields'][$delta]['default_value'] : FALSE,
  );
  $form['mapping'][$key]['fields'][$delta]['remove_field'] = array(
    '#type' => 'button',
    '#default_value' => t('Remove'),
    '#name' => 'remove-field-button-' . drupal_html_id($key . $delta),
    '#ajax' => array(
      'callback' => '_crm_core_data_import_entity_type_action_callback',
      'method' => 'replace',
      'wrapper' => 'mapping-fieldset-wrapper',
    ),
  );
}

/**
 * Callback for entity type select.
 */
function _crm_core_data_import_entity_form_entity_type_callback($form, &$form_state) {
  return $form['entity_form'];
}

/**
 * Callback for entity fieldset submit.
 */
function _crm_core_data_import_entity_form_add_entity_callback($form, &$form_state) {
  _crm_core_data_import_theme_mapping_form($form);
  return $form['mapping'];
}

/**
 * Callback for add another field button.
 */
function _crm_core_data_import_add_another_field_callback($form, &$form_state) {
  _crm_core_data_import_theme_mapping_form($form);
  $elemets = $form_state['triggering_element']['#parents'][1];
  return $form['mapping'][$elemets];
}

/**
 * Callback for actions for entity bundle.
 */
function _crm_core_data_import_entity_type_action_callback($form, &$form_state) {
  _crm_core_data_import_theme_mapping_form($form);
  return $form['mapping'];
}

/**
 * Page callback for settings page.
 */
function crm_core_data_import_settings_page($form, &$form_state, $importer_id) {
  $importer = crm_core_data_import_load_importer($importer_id);
  $form_state['importer'] = $importer;

  $form['#tree'] = TRUE;

  $form['form_header'] = array(
    '#markup' => '<p>' . t('Use the settings on this screen to refine the data being imported into the system.') . '</p>',
    '#weight' => -50,
  );

  $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_settings');

  foreach ($plugins as $plugin) {
    $data_import_settings = crm_core_data_import_load_plugin_instance('crm_core_data_import_settings', $plugin['name']);
    if ($data_import_settings->displayConditions($importer)) {
      $data_import_settings->configForm($form, $form_state, $importer, $plugin['label']);
    }
  }

  $form['previous'] = array(
    '#type' => 'submit',
    '#value' => t('Previous'),
    '#id' => 'button-previous',
  );
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#id' => 'button-next',
  );
  $form['#attached']['css'][] = _crm_core_data_import_css_path();
  crm_core_data_import_attach_pager($form, $importer, __FUNCTION__);
  return $form;
}

/**
 * Validation callback for settings page.
 */
function crm_core_data_import_settings_page_validate(&$form, &$form_state) {
  $importer = $form_state['importer'];
  ctools_include('plugins');
  $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_settings');
  foreach ($plugins as $plugin) {
    $data_import_settings = crm_core_data_import_load_plugin_instance('crm_core_data_import_settings', $plugin['name']);
    $data_import_settings->configFormValidate($form, $form_state, $importer);
  }
}

/**
 * Submit callback for settings page.
 */
function crm_core_data_import_settings_page_submit(&$form, &$form_state) {
  $importer = $form_state['importer'];
  ctools_include('plugins');
  $plugins = ctools_get_plugins('crm_core_data_import', 'crm_core_data_import_settings');

  foreach ($plugins as $plugin) {
    $data_import_settings = crm_core_data_import_load_plugin_instance('crm_core_data_import_settings', $plugin['name']);
    if ($data_import_settings->displayConditions($importer)) {
      $settings = $data_import_settings->configFormSubmit($form, $form_state, $importer);
      $current_settings = $importer->getSettings();
      if (!empty($settings)) {
        $result_settings = array_merge($current_settings, $settings);
      }
      else {
        $result_settings = $current_settings;
      }
      $controller = get_class($data_import_settings);
      if (empty($result_settings['settings_controllers']) || !in_array($controller, $result_settings['settings_controllers'])) {
        $result_settings['settings_controllers'][] = $controller;
      }
      $importer->setSettings($result_settings);
    }
  }
  $importer->save();

  if ($form_state['triggering_element']['#id'] == 'button-previous') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import/' . $importer->id . '/field-mapping';
  }
  elseif ($form_state['triggering_element']['#id'] == 'button-next') {
    $form_state['redirect'] = 'admin/structure/crm-core/data-import';
  }
}

/**
 * Page to delete data import task.
 */
function crm_core_data_import_delete_page($form, &$form_state, $importer_id) {
  $importer = crm_core_data_import_load_importer($importer_id);
  $form_state['importer'] = $importer;
  return confirm_form($form,
    t('Are you sure you want to delete data import task %title?', array('%title' => $importer->title)),
    'admin/structure/crm-core/data-import',
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit callback to delete data import task.
 */
function crm_core_data_import_delete_page_submit($form, &$form_state) {
  $importer = $form_state['importer'];

  // Remove all registered migrations.
  $registered_migrations = variable_get('crm_core_data_import_migrations_' . $importer->id, array());
  foreach ($registered_migrations as $machine_name) {
    $migration = Migration::getInstance($machine_name);
    if ($migration) {
      Migration::deregisterMigration($machine_name);
    }
  }
  variable_del('crm_core_data_import_migrations_' . $importer->id);

  db_delete('crm_core_data_import')
    ->condition('id', $importer->id)
    ->execute();
  $form_state['redirect'] = 'admin/structure/crm-core/data-import';
}

/**
 * Build operations for queue of import.
 */
function _crm_core_data_import_build_entity_operations(&$operations, $mapping, $importer_id, $options) {
  foreach ($mapping as $key => $mapping_item) {
    list($entity_controller_type, $entity_bundle, $bundle_delta) = explode(':', $key);
    $entity_type = crm_core_data_import_get_destination_entity_type($entity_controller_type);
    crm_core_data_import_register_migration($importer_id, $entity_type, $entity_bundle, $bundle_delta);
    $machine_name = _crm_core_data_import_migration_machine_name($importer_id, $entity_type, $entity_bundle, $bundle_delta);
    $operations[] = array(
      'crm_core_data_import_batch',
      array('import', $machine_name, $options, 0),
    );
  }
}
