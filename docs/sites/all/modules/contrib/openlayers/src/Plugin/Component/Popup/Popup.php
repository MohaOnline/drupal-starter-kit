<?php

namespace Drupal\openlayers\Plugin\Component\Popup;

use Drupal\openlayers\Openlayers;
use Drupal\openlayers\Types\Component;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Popup",
 *  description = "Display a popup when a feature is clicked."
 * )
 */
class Popup extends Component {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
/*
    $form['options']['frontend_layers'] = array(
      '#type' => 'select',
//      '#type' => 'checkboxes',
      '#title' => t('Layers'),
      '#empty_option' => t('- Select a Layer -'),
      '#default_value' => $this->getOption('frontend_layers', ''),
      '#description' => t('Select the layers.'),
      '#options' => Openlayers::loadAllAsOptions('Layer'),
      '#required' => TRUE,
      '#multiple' => TRUE,
    );
*/
    $form['options']['closer'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display close button ?'),
      '#default_value' => $this->getOption('closer', FALSE),
    );
    $form['options']['positioning'] = array(
      '#type' => 'select',
      '#title' => t('Positioning'),
      '#default_value' => $this->getOption('positioning', 'top-left'),
      '#description' => t('Defines how the overlay is actually positioned. Default is top-left.'),
      '#options' => Openlayers::positioningOptions(),
      '#required' => TRUE,
    );
    $form['options']['hitTolerance'] = array(
      '#type' => 'textfield',
      '#title' => t('Hit Tolerance'),
      '#description' => t('Popup-detection tolerance in pixels. Pixels inside the radius around the given position will be checked for features. The default is zero.'),
      '#default_value' => $this->getOption('hitTolerance', 0),
    ); 
    $form['options']['autoPan'] = array(
      '#type' => 'checkbox',
      '#title' => t('Autopan'),
      '#description' => t('If set to true the map is panned when calling setPosition, so that the overlay is entirely visible in the current viewport. The default is false.'),
      '#default_value' => $this->getOption('autoPan', FALSE),
    );
    $form['options']['autoPanAnimation'] = array(
      '#type' => 'textfield',
      '#title' => t('Autopan animation duration'),
      '#default_value' => $this->getOption('autoPanAnimation', 1000),
      '#description' => t('The options used to create a ol.animation.pan animation. This animation is only used when autoPan is enabled. By default the default options for ol.animation.pan are used. If set to zero the panning is not animated. The duration of the animation is in milliseconds. Default is 1000.'),
    );
    $form['options']['autoPanMargin'] = array(
      '#type' => 'textfield',
      '#title' => t('Autopan Animation'),
      '#default_value' => $this->getOption('autoPanMargin', 20),
      '#description' => t('The margin (in pixels) between the overlay and the borders of the map when autopanning. The default is 20.'),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function optionsFormSubmit(array $form, array &$form_state) {
    $form_state['values']['options']['autoPan'] = (bool) $form_state['values']['options']['autoPan'];
  }

}
