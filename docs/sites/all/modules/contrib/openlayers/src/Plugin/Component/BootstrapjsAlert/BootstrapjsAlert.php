<?php

namespace Drupal\openlayers\Plugin\Component\BootstrapjsAlert;

use Drupal\openlayers\Types\Component;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "BootstrapjsAlert"
 * )
 */
class BootstrapjsAlert extends Component {

  /**
   * {@inheritdoc}
   */
  public function attached() {
    $attached = parent::attached();
    $attached['libraries_load'][] = array('bootstrap');
    return $attached;
  }

  /**
   * {@inheritdoc}
   */
  public function dependencies() {
    return array(
      'bootstrap_library',
    );
  }

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $form['options']['message'] = array(
      '#type' => 'textarea',
      '#title' => t('Text to display'),
      '#default_value' => $this->getOption('message'),
    );
  }

}
