<?php

namespace OpenlayersDrupal\Core\DependencyInjection;

use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides dependency injection friendly methods for serialization.
 */
trait DependencySerializationTrait {

  /**
   * An array of service IDs keyed by property name used for serialization.
   *
   * @var array
   */
  protected $serviceIds = array();

  /**
   * {@inheritdoc}
   */
  public function __sleep() {
    $this->serviceIds = array();
    $vars = get_object_vars($this);
    foreach ($vars as $key => $value) {
      if (is_object($value) && isset($value->_serviceId)) {
        // If a class member was instantiated by the dependency injection
        // container, only store its ID so it can be used to get a fresh object
        // on unserialization.
        $this->serviceIds[$key] = $value->_serviceId;
        unset($vars[$key]);
      }
      // Special case the container, which might not have a service ID.
      elseif ($value instanceof ContainerInterface) {
        $this->serviceIds[$key] = 'service_container';
        unset($vars[$key]);
      }
    }

    return array_keys($vars);
  }

  /**
   * {@inheritdoc}
   */
  public function __wakeup() {
    $container = \Drupal::getContainer();
    foreach ($this->serviceIds as $key => $service_id) {
      $this->$key = $container->get($service_id);
    }
    $this->serviceIds = array();
  }

}
