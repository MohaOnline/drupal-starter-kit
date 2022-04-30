<?php

/**
 * @file
 * FIX - insert comment here.
 */

/**
 * Default sources from views.
 */
function openlayers_views_default_openlayers_sources() {

  $sources = array();

  foreach (openlayers_views_get_views() as $data) {
    list($style_plugin, $view, $display) = $data;
    $machine_name = 'openlayers_views_source_' . $view->name . '_' . $display;

    $ol_source = new stdClass();
    $ol_source->api_version = 1;
    $ol_source->machine_name = $machine_name;
    $ol_source->name = 'Openlayers views source: ' . $view->name . ':' . $display;
    $ol_source->description = t('Source for !link views map', array('!link' => l(t('this'), 'admin/structure/views/view/' . $view->name . '/edit/' . $display)));
    $ol_source->factory_service = 'openlayers.Source:Views';
    $ol_source->options = array(
      'view' => $view->name . ':' . $display,
    );

    $sources[$ol_source->machine_name] = $ol_source;
  }

  return $sources;
}
