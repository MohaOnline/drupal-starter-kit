<?php

namespace Drupal\openlayers\Types;

use OpenlayersDrupal\Core\Logger\LoggerChannelInterface;
use Drupal\openlayers\Messenger\MessengerInterface;

/**
 * FIX: Insert short comment here.
 *
 * @OpenlayersPlugin(
 *   id = "Error",
 *   arguments = {
 *     "@logger.channel.default",
 *     "@messenger"
 *   }
 * )
 *
 * Dummy class to avoid breaking the whole processing if a plugin class is
 * missing.
 */
class Error extends Base implements ControlInterface, ComponentInterface, LayerInterface, SourceInterface, StyleInterface {

  /**
   * Contains the error message string.
   *
   * @var string
   */
  public $errorMessage;

  /**
   * The loggerChannel service.
   *
   * @var \OpenlayersDrupal\Core\Logger\LoggerChannelInterface
   */
  protected $loggerChannel;

  /**
   * The messenger service.
   *
   * @var \Drupal\openlayers\Messenger\MessengerInterface
   */
  protected $messenger;

  /**
   * {@inheritdoc}
   */
  public function __construct($configuration, $plugin_id, $plugin_definition, LoggerChannelInterface $logger_channel, MessengerInterface $messenger) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->loggerChannel = $logger_channel;
    $this->messenger = $messenger;

    $this->errorMessage = 'Error while loading @type @machine_name having service @service.';

    if (!empty($configuration['errorMessage'])) {
      $this->errorMessage = $configuration['errorMessage'];
    }
  }

  /**
   * {@inheritdoc}
   */
  public function init() {
    $this->loggerChannel->error($this->getMessage(), array('channel' => 'openlayers'));
    $this->messenger->addMessage($this->getMessage(), 'error', FALSE);
    return parent::init();
  }

  /**
   * {@inheritdoc}
   */
  public function getMessage() {
    $machine_name = $this->getMachineName();
    $service = $this->getFactoryService() ? $this->getFactoryService() : t('undefined');
    $type = isset($this->configuration['type']) ? $this->configuration['type'] : 'undefined';

    return t($this->errorMessage, array(
      '@machine_name' => $machine_name,
      '@service' => $service,
      '@type' => $type,
    ));
  }

  /**
   * {@inheritdoc}
   */
  public function getStyle() {

  }

  /**
   * {@inheritdoc}
   */
  public function getSource() {

  }

  /**
   * {@inheritdoc}
   */
  public function setStyle(StyleInterface $style) {

  }

  /**
   * {@inheritdoc}
   */
  public function setSource(SourceInterface $source) {

  }

  /**
   * {@inheritdoc}
   */
  public function setVisible($visibility) {

  }

  /**
   * {@inheritdoc}
   */
  public function setOpacity($opacity) {

  }

  /**
   * {@inheritdoc}
   */
  public function setZindex($zindex) {

  }

  /**
   * {@inheritdoc}
   */
  public function getVisible() {

  }

  /**
   * {@inheritdoc}
   */
  public function getOpacity() {

  }

  /**
   * {@inheritdoc}
   */
  public function getZindex() {

  }

}
