<?php

namespace Drupal\openlayers\Plugin\Source\TileDebug;

use Drupal\openlayers\Types\Source;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "TileDebug"
 * )
 */
class TileDebug extends Source {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['maxZoom'] = array(
      '#title' => t('Maxzoom'),
      '#type' => 'textfield',
      '#default_value' => $this->getOption('maxZoom', 22),
    );
  }

}
