<?php

namespace Drupal\openlayers_library\Plugin\Control\AutoZoom;

use Drupal\openlayers\Plugin\Component\ZoomToSource\ZoomToSource;
use Drupal\openlayers\Types\ControlInterface;

/**
 * FIX - insert column here.
 *
 * @OpenlayersPlugin(
 *  id = "AutoZoom",
 *  description = "Autozoom button"
 * )
 */
class AutoZoom extends ZoomToSource implements ControlInterface {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    parent::optionsForm($form, $form_state);

    unset($form['options']['source']);
  }

}
