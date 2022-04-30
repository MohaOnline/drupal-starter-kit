<?php

namespace Drupal\openlayers\Plugin\Style\Icon;

use Drupal\openlayers\Types\Style;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "Icon"
 * )
 */
class Icon extends Style {

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['path'] = array(
      '#type' => 'textfield',
      '#description' => 'eg. sites/default/files/styles/icon.png.<br>You may also use Views field tokens in this field - in the form ${field_xxxxxx}.',
      '#title' => 'Path',
      '#default_value' => $this->getOption('path', ''),
    );
    $form['options']['scale'] = array(
      '#type' => 'textfield',
      '#title' => 'Scale',
      '#default_value' => $this->getOption('scale', ''),
    );
    $form['options']['anchor'][0] = array(
      '#type' => 'textfield',
      '#title' => 'Anchor X',
      '#default_value' => $this->getOption(array('anchor', 0), 0.5),
    );
    $form['options']['anchor'][1] = array(
      '#type' => 'textfield',
      '#title' => 'Anchor Y',
      '#default_value' => $this->getOption(array('anchor', 1), 0.5),
    );
    $form['options']['color'] = array(
      '#type' => 'textfield',
      '#title' => 'Color',
      '#description' => 'Color to tint the icon. If not specified, the icon will be left as is.<br>eg. rgba(255,0,0,1)',
      '#field_prefix' => 'rgba(',
      '#field_suffix' => ')',
      '#default_value' => $this->getOption('color', ''),
    );
  }

}
