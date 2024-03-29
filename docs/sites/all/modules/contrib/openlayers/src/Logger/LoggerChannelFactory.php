<?php

namespace Drupal\openlayers\Logger;

use OpenlayersDrupal\Core\Logger\LoggerChannelFactoryInterface;
use OpenlayersPsr\Log\LoggerInterface;

/**
 * Defines a factory for logging channels.
 */
class LoggerChannelFactory implements LoggerChannelFactoryInterface {

  /**
   * Array of all instantiated logger channels keyed by channel name.
   *
   * @var \OpenlayersDrupal\Core\Logger\LoggerChannelInterface[]
   */
  protected $channels = array();

  /**
   * An array of arrays of \OpenlayersPsr\Log\LoggerInterface keyed by priority.
   *
   * @var array
   */
  protected $loggers = array();

  /**
   * {@inheritdoc}
   */
  public function get($channel) {
    if (!isset($this->channels[$channel])) {
      $instance = new LoggerChannel($channel);

      // Pass the loggers to the channel.
      $instance->setLoggers($this->loggers);
      $this->channels[$channel] = $instance;
    }

    return $this->channels[$channel];
  }

  /**
   * {@inheritdoc}
   */
  public function addLogger(LoggerInterface $logger, $priority = 0) {
    // Store it so we can pass it to potential new logger instances.
    $this->loggers[$priority][] = $logger;
    // Add the logger to already instantiated channels.
    foreach ($this->channels as $channel) {
      $channel->addLogger($logger, $priority);
    }
  }

}
