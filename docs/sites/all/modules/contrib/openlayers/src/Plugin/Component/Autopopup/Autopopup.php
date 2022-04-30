<?php

namespace Drupal\openlayers\Plugin\Component\Autopopup;

use Drupal\openlayers\Openlayers;
use Drupal\openlayers\Types\Component;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Autopopup",
 *  description = "Automatically opens a popup on the first feature of
 *    the source."
 * )
 */
class Autopopup extends Component {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['source'] = array(
      '#type' => 'select',
      '#title' => t('Source'),
      '#empty_option' => t('- Select a Source -'),
      '#default_value' => isset($form_state['item']->options['source']) ? $form_state['item']->options['source'] : '',
      '#description' => t('Select the source.'),
      '#options' => Openlayers::loadAllAsOptions('Source'),
      '#required' => TRUE,
    );
    $form['options']['zoom'] = array(
      '#type' => 'textfield',
      '#title' => t('Zoom'),
      '#default_value' => isset($form_state['item']->options['zoom']) ? $form_state['item']->options['zoom'] : 10,
      '#description' => t('Integer or <em>auto</em> or <em>disabled</em>.'),
      '#required' => TRUE,
    );
    $form['options']['closer'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display close button ?'),
      '#default_value' => isset($form_state['item']->options['closer']) ? $form_state['item']->options['closer'] : FALSE,
    );
    $form['options']['enableAnimations'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable animations'),
      '#default_value' => isset($form_state['item']->options['enableAnimations']) ? $form_state['item']->options['enableAnimations'] : FALSE,
      '#description' => t('Enable pan and zoom animation.'),
    );
    $form['options']['animations'] = array(
      '#type' => 'fieldset',
      '#title' => 'Animations options',
      '#states' => array(
        'visible' => array(
          'input[name="options[enableAnimations]"' => array('checked' => TRUE),
        ),
      ),
    );
    $form['options']['animations']['pan'] = array(
      '#type' => 'textfield',
      '#title' => t('Pan animation duration'),
      '#default_value' => isset($form_state['item']->options['animations']['pan']) ? $form_state['item']->options['animations']['pan'] : '500',
      '#description' => t('Duration of the pan animation.'),
    );
    $form['options']['animations']['zoom'] = array(
      '#type' => 'textfield',
      '#title' => t('Zoom animation duration'),
      '#default_value' => isset($form_state['item']->options['animations']['zoom']) ? $form_state['item']->options['animations']['zoom'] : '500',
      '#description' => t('Duration of the zoom animation.'),
    );
    $form['options']['positioning'] = array(
      '#type' => 'select',
      '#title' => t('Positioning'),
      '#default_value' => isset($form_state['item']->options['positioning']) ? $form_state['item']->options['positioning'] : 'top-left',
      '#description' => t('Defines how the overlay is actually positioned. Default is top-left.'),
      '#options' => Openlayers::positioningOptions(),
      '#required' => TRUE,
    );
  }

}
