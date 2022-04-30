<?php

namespace Drupal\openlayers\Plugin\Source\KML;

use Drupal\openlayers\Types\Source;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "KML"
 * )
 */
class KML extends Source {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['url'] = array(
      '#title' => t('URL'),
      '#type' => 'textfield',
      '#default_value' => $this->getOption('url'),
    );
    $form['options']['extract_styles'] = array(
      '#title' => t('Extract styles'),
      '#description' => t('Should styles be extracted from the KML?'),
      '#type' => 'checkbox',
      '#default_value' => $this->getOption('extract_styles', FALSE),
    );
  }

}
