<?php

namespace Drupal\openlayers_geofield\Plugin\Control\Geofield;

use Drupal\openlayers\Types\Control;

/**
 * FIX - insert comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Geofield",
 *  description = "Display a small icon when clicked a alert message is shown."
 * )
 */
class Geofield extends Control {

  /**
   * FIX - Insert short comment here.
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['draw'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Draw actions',
      '#default_value' => $this->getOption('draw', array()),
      '#options' => array(
        'Point' => 'Point',
        'MultiPoint' => 'MultiPoint',
        'LineString' => 'LineString',
        'MultiLineString' => 'MultiLineString',
        'Polygon' => 'Polygon',
        'MultiPolygon' => 'MultiPolygon',
        'Triangle' => 'Triangle',
        'Square' => 'Square',
        'Circle' => 'Circle',
        'Box' => 'Box',
      ),
    );
    $form['options']['actions'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Edit options',
      '#default_value' => $this->getOption('actions', array()),
      '#options' => array(
        'Edit' => 'Select and edit a feature',
        'Move' => 'Move the feature',
        'Clear' => 'Clear the map',
      ),
    );
    $form['options']['options'] = array(
      '#type' => 'checkboxes',
      '#title' => 'Options',
      '#default_value' => $this->getOption('options', array()),
      '#options' => array(
        'Snap' => 'Snap the feature between each others',
      ),
    );
  }

}
