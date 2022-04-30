<?php

namespace Drupal\openlayers_library\Plugin\Control\OL3LayerSwitcher;

use Drupal\openlayers\Types\Control;

use Drupal\openlayers\Types\ObjectInterface;

/**
 * FIX - insert comment here.
 *
 * @OpenlayersPlugin(
 *  id = "OL3LayerSwitcher",
 *  description = ""
 * )
 */
class OL3LayerSwitcher extends Control {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['collapsible'] = array(
      '#type' => 'checkbox',
      '#title' => t('Collapsible'),
      '#default_value' => $this->getOption('collapsible'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function preBuild(array &$build, ObjectInterface $context = NULL) {
    array_map(function ($layer) {
      /** @var \Drupal\openlayers\Types\LayerInterface $layer */
      if (!in_array($layer->getFactoryService(), array(
        'openlayers.Layer:Vector',
        'openlayers.Layer:Heatmap',
      ))) {
        $layer->setOption('type', 'base');
      }
    }, $context->getObjects('layer'));
  }

}
