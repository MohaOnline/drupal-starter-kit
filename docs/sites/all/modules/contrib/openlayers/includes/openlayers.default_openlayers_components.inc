<?php

/**
 * @file
 * Provide default components.
 */

/**
 * Implements hook_default_openlayers_components().
 */
function openlayers_default_openlayers_components() {
  $export = array();

  $ol_component = new stdClass();
  $ol_component->disabled = TRUE; /* Edit this to true to make a default component disabled initially */
  $ol_component->api_version = 1;
  $ol_component->machine_name = 'openlayers_component_map_edit_form_values';
  $ol_component->name = 'Map edit form values';
  $ol_component->description = 'Provides values for the center, rotation and zoom in the map edit form.';
  $ol_component->factory_service = 'openlayers.Component:SetValues';
  $ol_component->options = array(
    'latitude' => 'edit-options-view-center-lat',
    'longitude' => 'edit-options-view-center-lon',
    'rotation' => 'edit-options-view-rotation',
    'zoom' => 'edit-options-view-zoom',
    'extent' => 'edit-options-view-extent',
  );
  $export['openlayers_component_map_edit_form_values'] = $ol_component;

  // @FIX: Transform this into a real component, without inlinejs.
  $ol_component = new stdClass();
  $ol_component->disabled = TRUE; /* Edit this to true to make a default component disabled initially */
  $ol_component->api_version = 1;
  $ol_component->machine_name = 'openlayers_component_map_edit_form_setcenter';
  $ol_component->name = 'Map center on mouse click';
  $ol_component->description = 'Upon clicking, center the map at the mouse position.';
  $ol_component->factory_service = 'openlayers.Component:InlineJS';
  $ol_component->options = array(
    'javascript' => 'data.map.on(\'click\', function(evt){
      if (ol.hasOwnProperty(\'animation\')) {
        //  Deprecated in v3.20.0 - map.beforeRender() and ol.animation functions
        var pan = ol.animation.pan({
          duration: 2000,
          source: (data.map.getView().getCenter())
        });
        data.map.beforeRender(pan);
        data.map.getView().setCenter(evt.coordinate);
      } else {
        //  Introduced in v3.20.0 - view.animate() instead of map.beforeRender() and ol.animation functions
        data.map.getView().animate({
          center: evt.coordinate,
          duration: 2000
        });
      }
    });',
  );
  $export['openlayers_component_map_edit_form_setcenter'] = $ol_component;

  return $export;
}
