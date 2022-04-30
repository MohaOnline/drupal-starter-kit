<?php

/**
 * @file
 * CTools Export UI plugin definition for interactions.
 */

use Drupal\openlayers\Openlayers;
use Drupal\openlayers_ui\UI\OpenlayersInteractions;

/**
 * CTools Export UI plugin definition.
 */
function openlayers_ui_OpenlayersInteractions_ctools_export_ui() {
  return array(
    'schema' => 'openlayers_interactions',
    'access' => 'administer openlayers',
    'menu' => array(
      'menu prefix' => 'admin/structure/openlayers',
      'menu item' => 'interactions',
      'menu title' => 'Interactions',
      'menu description' => 'Administer Openlayers interactions.',
    ),

    'handler' => array(
      'class' => '\\Drupal\\openlayers_ui\\UI\\openlayersInteractions',
      'file' => 'OpenlayersInteractions.php',
    ),

    'export' => array(
      'admin_title' => 'name',
      'admin_description' => 'description',
    ),

    'use wizard' => TRUE,
    'form info' => array(
      'order' => array(
        'start' => t('Administrative settings'),
        'type' => t('Interaction type'),
        'options' => t('Interaction type options'),
      ),
      'forms' => array(
        'start' => array(
          'form id' => 'openlayers_interaction_form_start',
        ),
        'type' => array(
          'form id' => 'openlayers_interaction_form_type',
        ),
        'options' => array(
          'form id' => 'openlayers_interaction_form_options',
        ),
      ),
      'wrapper' => 'openlayers_objects_ui_form_wrapper',
    ),

    'title singular' => t('interaction'),
    'title plural' => t('interactions'),
    'title singular proper' => t('Openlayers interaction preset'),
    'title plural proper' => t('Openlayers interaction presets'),

    'strings' => array(
      'confirmation' => array(
        'add' => array(
          'success' => t('Interaction saved.'),
        ),
        'delete' => array(
          'success' => t('Interaction was deleted.'),
        ),
      ),
    ),
  );
}

/**
 * Interaction base config form handler.
 */
function openlayers_interaction_form_start($form, &$form_state) {
  $class = new OpenlayersInteractions();
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
    $interactions = $mapObject->getOption('interactions', array());
    if (in_array($form_state['item']->machine_name, $interactions)) {
      $defaultOptions[] = $machine_name;
    }
  }

  // Define which map(s) this interaction is added to.
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
 * Interaction base config form validation handler.
 */
function openlayers_interaction_form_start_validate($form, &$form_state) {
  $class = new OpenlayersInteractions();
  $class->init($form_state['plugin']);
  $class->edit_form_validate($form, $form_state);
}

/**
 * Interaction base config form submit handler.
 */
function openlayers_interaction_form_start_submit($form, &$form_state) {
  $class = new OpenlayersInteractions();
  $class->init($form_state['plugin']);
  $class->edit_form_submit($form, $form_state);
  
  $attachMaps = $form_state['input']['attachToMap'];
  $maps = Openlayers::loadAll('map');

  foreach ($attachMaps as $key => $map) {
    $interactions = $maps[$key]->getOption('interactions', array());
    $newInteraction = $form_state['values']['machine_name'];
   
    // if new interaction is being added to a map
    if (!is_null($attachMaps[$key]) && !in_array($newInteraction, $interactions)) {
      $interactions[] = $newInteraction;
      $maps[$key]->setOption('interactions', $interactions);
      Openlayers::save($maps[$key]);
    }
    
    // if new interaction is being deleted from a map
    if (is_null($attachMaps[$key]) && in_array($newInteraction, $interactions)) {
      if (($key2 = array_search($newInteraction, $interactions)) !== false) {
        unset($interactions[$key2]);
      }
      $maps[$key]->setOption('interactions', $interactions);
      Openlayers::save($maps[$key]);
    }    
  }

}

/**
 * Interaction type config form handler.
 */
function openlayers_interaction_form_type($form, &$form_state) {
  $form['factory_service'] = array(
    '#type' => 'select',
    '#title' => t('Interaction Type'),
    '#empty_option' => t('- Select a @plugin type -', array('@plugin' => 'Interaction')),
    '#default_value' => isset($form_state['item']->factory_service) ? $form_state['item']->factory_service : '',
    '#description' => t('Select the type of interaction.'),
    '#options' => Openlayers::getOlObjectsOptions('Interaction'),
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Interaction type config form submit handler.
 */
function openlayers_interaction_form_type_submit($form, &$form_state) {
  $form_state['item']->factory_service = $form_state['values']['factory_service'];
}

/**
 * Interaction options config form handler.
 */
function openlayers_interaction_form_options($form, &$form_state) {
  if (($interaction = Openlayers::load('Interaction', $form_state['item'])) == TRUE) {
    $interaction->optionsForm($form, $form_state);
    $form['options']['#tree'] = TRUE;
  }

  return $form;
}

/**
 * Interaction options config form validation handler.
 */
function openlayers_interaction_form_options_validate($form, &$form_state) {
  if (($interaction = Openlayers::load('Interaction', $form_state['item'])) == TRUE) {
    $interaction->optionsFormValidate($form, $form_state);
  }
}

/**
 * Interaction options config form submit handler.
 */
function openlayers_interaction_form_options_submit($form, &$form_state) {
  if (isset($form_state['values']['options'])) {
    $form_state['item']->options = array_merge((array) $form_state['item']->options, (array) $form_state['values']['options']);
  }

  if (($interaction = Openlayers::load('Interaction', $form_state['item'])) == TRUE) {
    $interaction->optionsFormSubmit($form, $form_state);
  }
}
