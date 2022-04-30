<?php

namespace Drupal\openlayers\Plugin\Layer\Tile;

use Drupal\openlayers\Types\Layer;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Tile"
 * )
 */
class Tile extends Layer {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['opacity'] = array(
      '#type' => 'textfield',
      '#title' => t('Opacity'),
      '#default_value' => $this->getOption('opacity', 1),
      '#description' => t('Opacity, between 0 and 1.'),
    );
  }

}
