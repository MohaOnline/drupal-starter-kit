<?php

namespace Drupal\openlayers_boxes\boxes;

use Drupal\openlayers\Openlayers;

/**
 * FIX - insert comment here.
 */
class OpenlayersBox extends boxes_box {

  /**
   * {@inheritdoc}
   */
  public function optionsDefaults() {
    return array(
      'map' => '',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function optionsForm(&$form_state) {
    $form = array();

    // Map objects.
    $form['map'] = array(
      '#type' => 'select',
      '#title' => t('Openlayers map'),
      '#description' => t('Map to display.'),
      '#options' => Openlayers::loadAllAsOptions('Map'),
      "#empty_option" => t('- Select a Map -'),
      '#default_value' => $this->options['map'] ? $this->options['map'] : '',
    );
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function render() {
    $title = isset($this->title) ? check_plain($this->title) : NULL;

    $render = array(
      '#type' => 'openlayers',
      '#map' => $this->options['map'],
    );

    return array(
      'delta' => $this->delta,
      'title' => $title,
      'subject' => $title,
      'content' => drupal_render($render),
    );
  }

}
