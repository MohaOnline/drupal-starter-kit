<?php

namespace Drupal\openlayers\Plugin\Component\Tooltip;

use Drupal\openlayers\Openlayers;
use Drupal\openlayers\Types\Component;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Tooltip"
 * )
 */
class Tooltip extends Component {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['positioning'] = array(
      '#type' => 'select',
      '#title' => t('Positioning'),
      '#default_value' => isset($form_state['item']->options['positioning']) ? $form_state['item']->options['positioning'] : 'top-left',
      '#description' => t('Defines how the overlay is actually positioned. Default is top-left.'),
      '#options' => Openlayers::positioningOptions(),
      '#required' => TRUE,
    );
    
    $form['options']['hitTolerance'] = array(
      '#type' => 'textfield',
      '#title' => t('Hit Tolerance'),
      '#description' => t('Tooltip-detection tolerance in pixels. Pixels inside the radius around the given position will be checked for features. The default is zero.'),
      '#default_value' => $this->getOption('hitTolerance', 0),
    ); 
  }

}
