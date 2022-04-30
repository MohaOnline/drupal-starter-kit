<?php

namespace Drupal\openlayers\Plugin\Source\BingMaps;

use Drupal\openlayers\Types\Source;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "BingMaps"
 * )
 */
class BingMaps extends Source {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $layer_types = array(
      'Road',
      'Aerial',
      'AerialWithLabels',
      'collinsBart',
      'ordnanceSurvey',
    );

    $form['options']['key'] = array(
      '#title' => t('Key'),
      '#type' => 'textfield',
      '#default_value' => $this->getOption('key', ''),
    );
    $form['options']['imagerySet'] = array(
      '#title' => t('Imagery set'),
      '#type' => 'select',
      '#default_value' => $this->getOption('imagerySet', 'Road'),
      '#options' => array_combine($layer_types, $layer_types),
    );
  }

}
