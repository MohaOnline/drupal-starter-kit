<?php

/**
 * @file
 * This file holds all the theme related functions.
 */

/**
 * Implements hook_preprocess_HOOK().
 */
function template_preprocess_openlayers(&$variables) {
  /** @var \Drupal\openlayers\Types\MapInterface $map */
  $map = $variables['openlayers']['#map'];

  /*
   * Add a theme suggestion so the template can be customized for a particular
   * map if needed.
   *
   *  Default template: openlayers.tpl.php
   *  Custom map template: openlayers--[map-machine-name].tpl.php
   */
  $variables['theme_hook_suggestions'][] = 'openlayers__' . str_replace('-', '_', $map->getMachineName());

  /*
   * Compute attributes for the divs composing the map.
   */
  $openlayers_container_classes = isset($variables['openlayers']['openlayers-container']['#attributes']['class']) ? (array) $variables['openlayers']['openlayers-container']['#attributes']['class'] : array();
  $openlayers_container_classes[] = 'openlayers-container';

  $attributes = array(
    'openlayers-container' => array(
      'id' => 'openlayers-container-' . $map->getId(),
      'class' => $openlayers_container_classes,
    ),
    'openlayers-map-container' => array(
      'id' => 'openlayers-map-container-' . $map->getId(),
      'class' => array(
        'openlayers-map-container',
      ),
      'style' => implode(array_map(function ($key) use ($map) {
        return $key . ':' . $map->getOption($key) . ';';
      }, array('width', 'height'))),
    ),
    'openlayers-map' => array(
      'id' => $map->getId(),
      'class' => array_filter(
        array(
          'openlayers-map',
          $map->getMachineName(),
          ((bool) $map->isAsynchronous()) ? 'asynchronous' : NULL,
        )
      ),
      'tabindex' => 0,
    ),
  );

  $variables['openlayers']['attributes'] = array_map(function ($value) {
    return drupal_attributes($value);
  }, $attributes);
}
