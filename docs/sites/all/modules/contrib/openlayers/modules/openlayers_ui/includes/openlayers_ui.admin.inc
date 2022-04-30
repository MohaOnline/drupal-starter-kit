<?php

/**
 * @file
 * This file holds the functions for the main openlayers Admin settings.
 *
 * @ingroup openlayers
 */

use Drupal\openlayers\Openlayers;
use Drupal\openlayers\Config;

/**
 * Menu callback; displays the openlayers module settings page.
 *
 * @see system_settings_form()
 */
function openlayers_ui_admin_settings($form, &$form_state) {

  $form['origin'] = array(
    '#type' => 'fieldset',
    '#title' => 'Library loading configuration',
  );

  $library = libraries_detect('openlayers3');
  $current_variant = Config::get('openlayers.variant');
  if (!Openlayers::detectLibrary()) {
    $current_variant = NULL;
  }

  $options_variants = array('' => t('- Select the library variant -'));
  foreach ($library['variants'] as $version => $variant) {
    list($optgroup) = explode(':', $version, 2);
    if (empty($optgroup)) {
      $optgroup = t('Other');
    }
    $optgroup = drupal_strtoupper($optgroup);
    $options_variants[$optgroup][$version] = (isset($variant['name'])) ? $variant['name'] : $version;
  }

  $form['origin']['variant'] = array(
    '#type' => 'select',
    '#title' => 'Select the Openlayers library variant to use.',
    '#options' => $options_variants,
    '#default_value' => $current_variant,
  );

  $form['origin']['debug'] = array(
    '#type' => 'checkbox',
    '#title' => 'Load javascript debug integration files ?',
    '#default_value' => Config::get('openlayers.debug'),
  );

  if (!isset($form_state['input']['op'])) {
    $form['plugin-info'] = array(
      '#type' => 'fieldset',
      '#title' => 'Plugin information',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    foreach (Openlayers::getPluginTypes() as $plugin_type) {
      $rows = array();

      $form['plugin-info'][$plugin_type . '-group'] = array(
        '#type' => 'fieldset',
        '#title' => $plugin_type,
        '#collapsed' => TRUE,
        '#collapsible' => TRUE,
      );

      foreach (\OpenlayersDrupal::service('openlayers.' . $plugin_type)->getDefinitions() as $service => $definition) {
        $definition += array('arguments' => array('NULL'));

        list($module, $type,, $id) = explode('.', $service);

        $configuration = array(
          'name' => $definition['id'],
          'machine_name' => $definition['id'],
          'factory_service' => $module . '.' . $type . ':' . $id,
        );

        $instance = Openlayers::load($plugin_type, $configuration);
        $attached = $instance->attached();

        $sizes = array('js' => 0, 'css' => 0);
        foreach (array_keys($sizes) as $size_type) {
          foreach ($attached[$size_type] as $data) {
            if (isset($data['type']) && $data['type'] != 'external') {
              if (!is_array($data['data'])) {
                $sizes[$size_type] += filesize($data['data']);
              }
            }
          }
        }

        foreach (array('js', 'css') as $file_type) {
          if (count($attached[$file_type]) != 0) {
            $sizes[$file_type] .= ' bytes.<br/>(' . count($attached[$file_type]);
            if (count($attached[$file_type]) == 1) {
              $sizes[$file_type] .= ' file)';
            }
            else {
              $sizes[$file_type] .= ' files)';
            }
          }
        }

        $class = array(
          'Class: ' . $definition['class'],
          'Classfile: ' . $instance->getClassPath(),
        );

        $rows[] = array(
          $instance->getProvider(),
          $instance->getBaseId(),
          implode('<br/>', $class),
          implode('|', $definition['arguments']),
          $sizes['js'],
          $sizes['css'],
        );
      }

      $header = array(
        'Module provider',
        'ID',
        'Class',
        'Arguments',
        'JS size',
        'CSS size',
      );
      $table = theme(
        'table',
        array(
          'header' => $header,
          'rows' => $rows,
          'empty' => t('No plugin available.'),
        )
      );

      $form['plugin-info'][$plugin_type . '-group']['table'] = array(
        '#markup' => $table,
      );
    }
  }

  $form['buttons'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => 'Save configuration',
    ),
  );

  return $form;
}

/**
 * Submit callback of the Openlayers settings page.
 */
function openlayers_ui_admin_settings_submit($form, &$form_state) {
  $values = $form_state['values'];

  if (isset($values['variant'])) {
    Config::set('openlayers.variant', $values['variant']);
  }
  else {
    Config::clear('openlayers.variant');
  }

  if (isset($values['debug'])) {
    Config::set('openlayers.debug', $values['debug']);
  }
  else {
    Config::clear('openlayers.debug');
  }

  drupal_set_message(t('The configuration options have been saved.'));
  drupal_flush_all_caches();
}
