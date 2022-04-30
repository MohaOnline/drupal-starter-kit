<?php

/**
 * @file
 * CTools Export UI plugin definition for layers.
 */

use Drupal\openlayers\Openlayers;
use Drupal\openlayers_ui\UI\OpenlayersLayers;

/**
 * CTools Export UI plugin definition.
 */
function openlayers_ui_OpenlayersLayers_ctools_export_ui() {
  return array(
    'schema' => 'openlayers_layers',
    'access' => 'administer openlayers',
    'menu' => array(
      'menu prefix' => 'admin/structure/openlayers',
      'menu item' => 'layers',
      'menu title' => 'Layers',
      'menu description' => 'Administer Openlayers layers presets.',
    ),

    'handler' => array(
      'class' => '\\Drupal\\openlayers_ui\\UI\\OpenlayersLayers',
      'file' => 'OpenlayersLayers.php',
    ),

    'use wizard' => TRUE,
    'form info' => array(
      'order' => array(
        'start' => t('Administrative settings'),
        'type' => t('Layer type'),
        'style' => t('Layer style'),
        'options' => t('Layer type options'),
      ),
      'forms' => array(
        'start' => array(
          'form id' => 'openlayers_layer_form_start',
        ),
        'type' => array(
          'form id' => 'openlayers_layer_form_type',
        ),
        'style' => array(
          'form id' => 'openlayers_layer_form_style',
        ),
        'options' => array(
          'form id' => 'openlayers_layer_form_options',
        ),
      ),
      'wrapper' => 'openlayers_objects_ui_form_wrapper',
    ),

    'export' => array(
      'admin_title' => 'name',
      'admin_description' => 'description',
    ),

    'title singular' => t('layer'),
    'title plural' => t('layers'),
    'title singular proper' => t('Openlayers preset'),
    'title plural proper' => t('Openlayers Layers presets'),

    'strings' => array(
      'confirmation' => array(
        'add' => array(
          'success' => t('saved.'),
        ),
        'delete' => array(
          'success' => t('was deleted.'),
        ),
      ),
    ),
  );
}

/**
 * Layer base config form handler.
 */
function openlayers_layer_form_start($form, &$form_state) {
  $class = new OpenlayersLayers();
  $class->init($form_state['plugin']);
  $class->edit_form($form, $form_state);

  $options = array();
  $defaultOptions = array();

  foreach (Openlayers::loadAllExportable('Map') as $machine_name => $map) {
    if (!is_object($map) || (property_exists($map, 'disabled') && ($map->disabled == 1 || $map->disabled == TRUE))) {
      continue;
    }
    $options[$machine_name] = $map->name;
    $mapObject = Openlayers::load('map', $map);
    $layers = $mapObject->getOption('layers', array());
    if (in_array($form_state['item']->machine_name, $layers)) {
      $defaultOptions[] = $machine_name;
    }
  }

  // Define which map(s) this layer is added to.
  $form['attachMaps'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attach to Maps ?'),
    '#description' => t('Select one or maps to add this object to.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['attachMaps']['attachToMap'] = array(
    '#type' => 'checkboxes',
    '#title' => 'Maps:',
    '#options' => $options,
    '#default_value' => $defaultOptions,
  );

  return $form;
}

/**
 * Layer base config form validation handler.
 */
function openlayers_layer_form_start_validate($form, &$form_state) {
  $class = new OpenlayersLayers();
  $class->init($form_state['plugin']);
  $class->edit_form_validate($form, $form_state);
}

/**
 * Layer base config form submit handler.
 */
function openlayers_layer_form_start_submit($form, &$form_state) {
  $class = new OpenlayersLayers();
  $class->init($form_state['plugin']);
  $class->edit_form_submit($form, $form_state);
  
  $attachMaps = $form_state['input']['attachToMap'];
  $maps = Openlayers::loadAll('map');

  foreach ($attachMaps as $key => $map) {
    $layers = $maps[$key]->getOption('layers', array());
    $newLayer = $form_state['values']['machine_name'];
    
    // if new layer is being added to a map
    if (!is_null($attachMaps[$key]) && !in_array($newLayer, $layers)) {
      $layers[] = $newLayer;
      $maps[$key]->setOption('layers', $layers);
      Openlayers::save($maps[$key]);
    }
    
    // if new layer is being deleted from a map
    if (is_null($attachMaps[$key]) && in_array($newLayer, $layers)) {
      if (($key2 = array_search($newLayer, $layers)) !== false) {
        unset($layers[$key2]);
      }
      $maps[$key]->setOption('layers', $layers);
      Openlayers::save($maps[$key]);
    }    
  }
  
}

/**
 * Layer type config form handler.
 */
function openlayers_layer_form_type($form, &$form_state) {
  $form['factory_service'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#empty_option' => t('- Select a @plugin type -', array('@plugin' => 'Layer')),
    '#default_value' => isset($form_state['item']->factory_service) ? $form_state['item']->factory_service : '',
    '#description' => t('Select the type of layer.'),
    '#options' => Openlayers::getOlObjectsOptions('Layer'),
    '#required' => TRUE,
  );

  $form['options'] = array(
    '#tree' => TRUE,
  );

  $form['options']['source'] = array(
    '#type' => 'select',
    '#title' => t('Source'),
    '#empty_option' => t('- Select a Source -'),
    '#default_value' => isset($form_state['item']->options['source']) ? $form_state['item']->options['source'] : '',
    '#description' => t('Select the source.'),
    '#options' => Openlayers::loadAllAsOptions('Source'),
    '#required' => FALSE,
  );

  $form['options']['visible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Visible ?'),
    '#default_value' => isset($form_state['item']->options['visible']) ? (bool) $form_state['item']->options['visible'] : TRUE,
    '#description' => t('TODO'),
  );

  return $form;
}

/**
 * Layer type config form submit handler.
 */
function openlayers_layer_form_type_submit($form, &$form_state) {
  $form_state['item']->factory_service = $form_state['values']['factory_service'];
  if (isset($form_state['values']['options'])) {
    $form_state['item']->options = array_merge((array) $form_state['item']->options, (array) $form_state['values']['options']);
  }
}

/**
 * Layer style config form handler.
 */
function openlayers_layer_form_style($form, &$form_state) {
  $form['options'] = array(
    '#tree' => TRUE,
  );

  $form['options']['style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#empty_option' => t('- Select a Style -'),
    '#default_value' => isset($form_state['item']->options['style']) ? $form_state['item']->options['style'] : '',
    '#description' => t('Select the style.'),
    '#options' => Openlayers::loadAllAsOptions('Style'),
    '#required' => FALSE,
  );

  return $form;
}

/**
 * Layer style config form submit handler.
 */
function openlayers_layer_form_style_submit($form, &$form_state) {
  if (isset($form_state['values']['options'])) {
    $form_state['item']->options = array_merge((array) $form_state['item']->options, (array) $form_state['values']['options']);
  }
}

/**
 * Layer options config form handler.
 */
function openlayers_layer_form_options($form, &$form_state) {
  if (($layer = Openlayers::load('Layer', $form_state['item'])) == TRUE) {
    $layer->optionsForm($form, $form_state);
    $form['options']['#tree'] = TRUE;
  }

  return $form;
}

/**
 * Layer options config form validation handler.
 */
function openlayers_layer_form_options_validate($form, &$form_state) {
  if (($layer = Openlayers::load('Layer', $form_state['item'])) == TRUE) {
    $layer->optionsFormValidate($form, $form_state);
  }
}

/**
 * Layer options config form submit handler.
 */
function openlayers_layer_form_options_submit($form, &$form_state) {
  if (isset($form_state['values']['options'])) {
    $form_state['item']->options = array_merge((array) $form_state['item']->options, (array) $form_state['values']['options']);
  }

  if (($layer = Openlayers::load('Layer', $form_state['item'])) == TRUE) {
    $layer->optionsFormSubmit($form, $form_state);
  }
}
