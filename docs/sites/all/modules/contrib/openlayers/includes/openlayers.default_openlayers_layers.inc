<?php

/**
 * @file
 * Provide default layers.
 */

/**
 * Implements hook_default_openlayers_layers().
 */
function openlayers_default_openlayers_layers() {
  $export = array();

  $ol_layer = new stdClass();
  $ol_layer->api_version = 1;
  $ol_layer->machine_name = 'openlayers_layer_tile_osm';
  $ol_layer->name = 'OpenStreetMap';
  $ol_layer->description = 'Layer OpenStreetMap';
  $ol_layer->factory_service = 'openlayers.Layer:Tile';
  $ol_layer->options = array(
    'source' => 'openlayers_source_osm',
  );
  $export['openlayers_layer_tile_osm'] = $ol_layer;

  return $export;
}
