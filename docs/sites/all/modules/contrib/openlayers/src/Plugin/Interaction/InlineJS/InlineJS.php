<?php

namespace Drupal\openlayers\Plugin\Interaction\InlineJS;

use OpenlayersDrupal\Core\Extension\ModuleHandlerInterface;

use Drupal\openlayers\Types\Interaction;
use Drupal\openlayers\Legacy\Drupal7;
use Drupal\openlayers\Messenger\MessengerInterface;

/**
 * FIX - Insert short comment here.
 *
 * @OpenlayersPlugin(
 *  id = "InlineJS",
 *  arguments = {
 *    "@module_handler",
 *    "@messenger",
 *    "@drupal7"
 *  }
 * )
 */
class InlineJS extends Interaction {

  /**
   * Constructs an InlineJS plugin.
   *
   * @param array $configuration
   *   The configuration array.
   * @param string $plugin_id
   *   The plugin id.
   * @param array $plugin_definition
   *   The plugin definition.
   * @param OpenlayersDrupal\Core\Extension\ModuleHandlerInterface $module_handler
   *   FIX.
   * @param Drupal\openlayers\Messenger\MessengerInterface $messenger
   *   FIX.
   * @param Drupal\openlayers\Legacy\Drupal7 $drupal7
   *   FIX.
   */
  public function __construct(array $configuration, $plugin_id, array $plugin_definition, ModuleHandlerInterface $module_handler, MessengerInterface $messenger, Drupal7 $drupal7) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moduleHandler = $module_handler;
    $this->messenger = $messenger;
    $this->drupal7 = $drupal7;
  }

  /**
   * {@inheritdoc}
   */
  public function optionsForm(array &$form, array &$form_state) {
    $attached = array();

    if ($this->moduleHandler->moduleExists('ace_editor')) {
      $attached = array(
        'library' => array(
          array('ace_editor', 'ace'),
        ),
        'js' => array(
          $this->drupal7->drupal_get_path('module', 'openlayers') . '/js/openlayers.editor.js',
        ),
      );
    }
    else {
      $this->messenger->addMessage(
        t(
          'To get syntax highlighting, you should install the module <a href="@url1">ace_editor</a> and its <a href="@url2">library</a>.',
          array(
            '@url1' => 'http://drupal.org/project/ace_editor',
            '@url2' => 'http://ace.c9.io/',
          )
        ),
        'warning'
      );
    }

    $form['options']['javascript'] = array(
      '#type' => 'textarea',
      '#title' => t('Javascript'),
      '#description' => t('Javascript to evaluate. The available variable is: <em>data</em>. You must create the openlayers variable <em>interaction</em>.'),
      '#rows' => 15,
      '#default_value' => $this->getOption('javascript'),
      '#attributes' => array(
        'data-editor' => 'javascript',
      ),
      '#attached' => $attached,
    );
  }

}
