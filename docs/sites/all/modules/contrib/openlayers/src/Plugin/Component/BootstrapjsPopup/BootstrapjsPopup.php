<?php

namespace Drupal\openlayers\Plugin\Component\BootstrapjsPopup;

use Drupal\openlayers\Types\Component;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "BootstrapjsPopup"
 * )
 */
class BootstrapjsPopup extends Component {

  /**
   * {@inheritdoc}
   */
  public function attached() {
    $attached = parent::attached();
    $attached['libraries_load'][] = array('bootstrap');
    return $attached;
  }

  /**
   * {@inheritdoc}
   */
  public function dependencies() {
    return array(
      'bootstrap_library',
    );
  }

}
