<?php

namespace Drupal\openlayers\Plugin\Interaction\Translate;

use Drupal\openlayers\Openlayers;
use Drupal\openlayers\Types\Interaction;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Translate",
 *  description = "Interaction for translating (moving) features."
 * )
 */
class Translate extends Interaction {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['select'] = array(
      '#type' => 'select',
      '#title' => t('Select interaction'),
      '#empty_option' => t('- Select an Interaction -'),
      '#default_value' => $this->getOption('select', ''),
      '#description' => t('Select the select interaction.'),
      '#options' => Openlayers::loadAllAsOptions('Interaction'),
      '#required' => TRUE,
    );
  }

  /**
   * {@inheritdoc}
   */
  public function optionsToObjects() {
    $import = parent::optionsToObjects();

    if ($select = $this->getOption('select')) {
      $select = Openlayers::load('interaction', $select);
      $import = array_merge($select->getCollection()->getFlatList(), $import);
    }

    return $import;
  }

}
