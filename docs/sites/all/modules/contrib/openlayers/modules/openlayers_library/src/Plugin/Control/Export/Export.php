<?php

namespace Drupal\openlayers_library\Plugin\Control\Export;

use Drupal\openlayers\Types\Control;

/**
 * FIX - insert comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Export",
 *  description = "Export button"
 * )
 */
class Export extends Control {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['exportTipLabel'] = array(
      '#type' => 'textfield',
      '#title' => 'Label',
      '#default_value' => $this->getOption('exportTipLabel', 'Export as image'),
    );
  }

}
