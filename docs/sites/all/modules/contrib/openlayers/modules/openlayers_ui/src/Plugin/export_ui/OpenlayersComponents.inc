<?php

/**
 * @file
 * CTools Export UI plugin definition for components.
 */

use Drupal\openlayers\Openlayers;
use Drupal\openlayers_ui\UI\OpenlayersComponents;

/**
 * CTools Export UI plugin definition.
 */
function openlayers_ui_OpenlayersComponents_ctools_export_ui() {
  return array(
    'schema' => 'openlayers_components',
    'access' => 'administer openlayers',
    'menu' => array(
      'menu prefix' => 'admin/structure/openlayers',
      'menu item' => 'components',
      'menu title' => 'Components',
      'menu description' => 'Administer Openlayers components.',
    ),

    'handler' => array(
      'class' => '\\Drupal\\openlayers_ui\\UI\\OpenlayersComponents',
      'file' => 'OpenlayersComponents.php',
    ),

    'export' => array(
      'admin_title' => 'name',
      'admin_description' => 'description',
    ),

    'use wizard' => TRUE,
    'form info' => array(
      'order' => array(
        'start' => t('Administrative settings'),
        'type' => t('Component type'),
        'options' => t('Component type options'),
      ),
      'forms' => array(
        'start' => array(
          'form id' => 'openlayers_component_form_start',
        ),
        'type' => array(
          'form id' => 'openlayers_component_form_type',
        ),
        'options' => array(
          'form id' => 'openlayers_component_form_options',
        ),
      ),
      'wrapper' => 'openlayers_objects_ui_form_wrapper',
    ),

    'title singular' => t('component'),
    'title plural' => t('components'),
    'title singular proper' => t('Openlayers component preset'),
    'title plural proper' => t('Openlayers component presets'),

    'strings' => array(
      'confirmation' => array(
        'add' => array(
          'success' => t('Component saved.'),
        ),
        'delete' => array(
          'success' => t('Component was deleted.'),
        ),
      ),
    ),
  );
}

/**
 * Component base config form handler.
 */
function openlayers_component_form_start($form, &$form_state) {
  $class = new OpenlayersComponents();
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
    $components = $mapObject->getOption('components', array());
    if (in_array($form_state['item']->machine_name, $components)) {
      $defaultOptions[] = $machine_name;
    }
  }

  // Define which map(s) this component is added to.
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
 * Component base config form validation handler.
 */
function openlayers_component_form_start_validate($form, &$form_state) {
  $class = new OpenlayersComponents();
  $class->init($form_state['plugin']);
  $class->edit_form_validate($form, $form_state);
}

/**
 * Component base config form submit handler.
 */
function openlayers_component_form_start_submit($form, &$form_state) {
  $class = new OpenlayersComponents();
  $class->init($form_state['plugin']);
  $class->edit_form_submit($form, $form_state);

  $attachMaps = $form_state['input']['attachToMap'];
  $maps = Openlayers::loadAll('map');

  foreach ($attachMaps as $key => $map) {
    $components = $maps[$key]->getOption('components', array());
    $newComponent = $form_state['values']['machine_name'];
    
    // if new component is being added to a map
    if (!is_null($attachMaps[$key]) && !in_array($newComponent, $components)) {
      $components[] = $newComponent;
      $maps[$key]->setOption('components', $components);
      Openlayers::save($maps[$key]);
    }
    
    // if new component is being deleted from a map
    if (is_null($attachMaps[$key]) && in_array($newComponent, $components)) {
      if (($key2 = array_search($newComponent, $components)) !== false) {
        unset($components[$key2]);
      }
      $maps[$key]->setOption('components', $components);
      Openlayers::save($maps[$key]);
    }    
  }
}

/**
 * Component type config form handler.
 */
function openlayers_component_form_type($form, &$form_state) {
  $form['factory_service'] = array(
    '#type' => 'select',
    '#title' => t('Component Type'),
    '#empty_option' => t('- Select a @plugin type -', array('@plugin' => 'Component')),
    '#default_value' => isset($form_state['item']->factory_service) ? $form_state['item']->factory_service : '',
    '#description' => t('Select the type of component.'),
    '#options' => Openlayers::getOlObjectsOptions('Component'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Component type config form submit handler.
 */
function openlayers_component_form_type_submit($form, &$form_state) {
  $form_state['item']->factory_service = $form_state['values']['factory_service'];
}

/**
 * Component options form handler.
 */
function openlayers_component_form_options($form, &$form_state) {
  if (($component = Openlayers::load('Component', $form_state['item'])) == TRUE) {
    $component->optionsForm($form, $form_state);
    $form['options']['#tree'] = TRUE;
  }

  return $form;
}

/**
 * Component options form validation handler.
 */
function openlayers_component_form_options_validate($form, &$form_state) {
  if (($component = Openlayers::load('Component', $form_state['item'])) == TRUE) {
    $component->optionsFormValidate($form, $form_state);
  }
}

/**
 * Component options form submit handler.
 */
function openlayers_component_form_options_submit($form, &$form_state) {
  if (isset($form_state['values']['options'])) {
    $form_state['item']->options = array_merge((array) $form_state['item']->options, (array) $form_state['values']['options']);
  }

  if (($component = Openlayers::load('Component', $form_state['item'])) == TRUE) {
    $component->optionsFormSubmit($form, $form_state);
  }
}
