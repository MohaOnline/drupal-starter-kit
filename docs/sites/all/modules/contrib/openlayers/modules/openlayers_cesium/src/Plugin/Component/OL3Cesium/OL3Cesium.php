<?php

namespace Drupal\openlayers_cesium\Plugin\Component\OL3Cesium;

use Drupal\openlayers\Types\Component;

/**
 * FIX - Insert comment here.
 *
 * @OpenlayersPlugin(
 *   id = "OL3Cesium",
 *   description = "Provides a Openlayers Cesium component."
 * )
 */
class OL3Cesium extends Component {

  /**
   * FIX - insert short comment.
   */
  public function attached() {
    $attached = parent::attached();

    $attached['libraries_load'][] = array(
      'ol3-cesium',
    );

    return $attached;
  }

}
