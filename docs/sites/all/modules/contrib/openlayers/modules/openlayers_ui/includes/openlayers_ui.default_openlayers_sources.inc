<?php

/**
 * @file
 * Default sources.
 */

/**
 * Implements hook_default_openlayers_sources().
 */
function openlayers_ui_default_openlayers_sources() {
  $export = array();

  $ol_source = new stdClass();
  $ol_source->disabled = FALSE; /* Edit this to true to make a default ol_source disabled initially */
  $ol_source->api_version = 1;
  $ol_source->machine_name = 'openlayers_ui_source_style_demo';
  $ol_source->name = 'Openlayers UI source style demo';
  $ol_source->description = '';
  $ol_source->factory_service = 'openlayers.Source:GeoJSON';
  $ol_source->options = array(
    'geojson_data' => array(
      'type' => 'FeatureCollection',
      'features' => array(
        array(
          'type' => 'Feature',
          'geometry' => array(
            'type' => 'Point',
            'coordinates' => array(
              0 => -5,
              1 => -5,
            ),
          ),
        ),
        array(
          'type' => 'Feature',
          'geometry' => array(
            'type' => 'Polygon',
            'coordinates' => array(
              array(
                array(
                  0 => 15,
                  1 => 15,
                ),
                array(
                  0 => 0,
                  1 => 15,
                ),
                array(
                  0 => 0,
                  1 => 0,
                ),
                array(
                  0 => 15,
                  1 => 0,
                ),
              ),
            ),
          ),
        ),
        array(
          'type' => 'Feature',
          'geometry' => array(
            'type' => 'LineString',
            'coordinates' => array(
              array(
                0 => -15,
                1 => -15,
              ),
              array(
                0 => 15,
                1 => -15,
              ),
            ),
          ),
        ),
      ),
    ),
  );
  $export['openlayers_ui_source_style_demo'] = $ol_source;

  return $export;
}
