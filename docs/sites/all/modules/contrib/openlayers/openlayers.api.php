<?php

/**
 * @file
 * FIX - insert comment here.
 */

/**
 * This hook will be triggered before a map is built and on each of its object.
 *
 * @param array $build
 *   The render array that will be rendered later.
 * @param \Drupal\openlayers\Types\ObjectInterface $context
 *   The context, this will be an openlayers object.
 */
function hook_openlayers_object_preprocess_alter(array &$build, \Drupal\openlayers\Types\ObjectInterface $context) {

}

/**
 * This hook will be triggered after a map is built and on each of its object.
 *
 * @param array $build
 *   The render array that will be rendered after this hook.
 * @param \Drupal\openlayers\Types\ObjectInterface $context
 *   The context, this will be an openlayers object.
 */
function hook_openlayers_object_postprocess_alter(array &$build, \Drupal\openlayers\Types\ObjectInterface $context) {

}

/**
 * This hook allows modules to provide their own maps.
 */
function hook_default_openlayers_maps() {

}

/**
 * This hook allows modules to alter default maps.
 *
 * @param array $exports
 *   Array of default maps.
 */
function hook_default_openlayers_maps_alter(array &$exports) {

}

/**
 * This hook allows modules to provide their own styles.
 */
function hook_default_openlayers_styles() {

}

/**
 * This hook allows modules to alter default styles.
 *
 * @param array $exports
 *   Array of default styles.
 */
function hook_default_openlayers_styles_alter(array &$exports) {

}

/**
 * This hook allows modules to provide their own layers.
 */
function hook_default_openlayers_layers() {

}

/**
 * This hook allows modules to alter default layers.
 *
 * @param array $exports
 *   Array of default layers.
 */
function hook_default_openlayers_layers_alter(array &$exports) {

}

/**
 * This hook allows modules to provide their own sources.
 */
function hook_default_openlayers_sources() {

}

/**
 * This hook allows modules to alter default sources.
 *
 * @param array $exports
 *   Array of default sources.
 */
function hook_default_openlayers_sources_alter(array &$exports) {

}

/**
 * This hook allows modules to provide their own controls.
 */
function hook_default_openlayers_controls() {

}

/**
 * This hook allows modules to alter default controls.
 *
 * @param array $exports
 *   Array of default controls.
 */
function hook_default_openlayers_controls_alter(array &$exports) {

}

/**
 * This hook allows modules to provide their own interactions.
 */
function hook_default_openlayers_interactions() {

}

/**
 * This hook allows modules to alter default interactions.
 *
 * @param array $exports
 *   Array of default interactions.
 */
function hook_default_openlayers_interactions_alter(array &$exports) {

}

/**
 * This hook allows modules to provide their own components.
 */
function hook_default_openlayers_components() {

}

/**
 * This hook allows modules to alter default components.
 *
 * @param array $exports
 *   Array of default components.
 */
function hook_default_openlayers_components_alter(array &$exports) {

}
