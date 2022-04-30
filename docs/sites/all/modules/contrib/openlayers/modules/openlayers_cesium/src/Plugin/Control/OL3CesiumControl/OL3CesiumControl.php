<?php

namespace Drupal\openlayers_cesium\Plugin\Control\OL3CesiumControl;

use Drupal\openlayers\Types\Control;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *   id = "OL3CesiumControl",
 *   description = "Provides a Openlayers Cesium control."
 * )
 */
class OL3CesiumControl extends Control {

  /**
   * FIX - Insert short comment here.
   */
  public function attached() {
    $attached = parent::attached();

    $attached['libraries_load'][] = array(
      'ol3-cesium',
    );

    return $attached;
  }

}
