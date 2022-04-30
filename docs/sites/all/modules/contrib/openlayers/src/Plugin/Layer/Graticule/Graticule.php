<?php

namespace Drupal\openlayers\Plugin\Layer\Graticule;

use Drupal\openlayers\Types\Layer;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Graticule"
 * )
 */
class Graticule extends Layer {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['rgba'] = array(
      '#type' => 'textfield',
      '#title' => t('RGBA'),
      '#default_value' => $this->getOption('rgba', '0, 0, 0, 0.2'),
      '#description' => t('RGBA, a string of 4 numbers, separated by a comma.'),
    );
    $form['options']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $this->getOption('width', 2),
      '#description' => t('Width'),
    );
    $form['options']['lineDash'] = array(
      '#type' => 'textfield',
      '#title' => t('Line dash'),
      '#default_value' => $this->getOption('lineDash', '0.5, 4'),
      '#description' => t('Line dash, a string of 2 numbers, separated by a comma.'),
    );
    $form['options']['showLabels'] = array(
      '#type' => 'textfield',
      '#title' => t('Show labels'),
      '#description' => 'Show labels (0, 1). Default is 1.',
      '#default_value' => $this->getOption('showLabels', 1),
    );
  }

}
