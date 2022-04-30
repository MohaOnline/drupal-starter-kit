<?php

namespace OpenlayersDrupal\Component\Plugin;

use OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryTrait;
use OpenlayersDrupal\Component\Plugin\Exception\PluginNotFoundException;

/**
 * Base class for plugin managers.
 */
abstract class PluginManagerBase implements PluginManagerInterface {

  use DiscoveryTrait;

  /**
   * The object that discovers plugins managed by this manager.
   *
   * @var \OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface
   */
  protected $discovery;

  /**
   * The object that instantiates plugins managed by this manager.
   *
   * @var \OpenlayersDrupal\Component\Plugin\Factory\FactoryInterface
   */
  protected $factory;

  /**
   * FIX - insert comment here.
   *
   * The object that returns the preconfigured plugin instance appropriate for
   * a particular runtime condition.
   *
   * @var \OpenlayersDrupal\Component\Plugin\Mapper\MapperInterface
   */
  protected $mapper;

  /**
   * {@inheritdoc}
   */
  public function getDefinition($plugin_id, $exception_on_invalid = TRUE) {
    return $this->discovery->getDefinition($plugin_id, $exception_on_invalid);
  }

  /**
   * {@inheritdoc}
   */
  public function getDefinitions() {
    return $this->discovery->getDefinitions();
  }

  /**
   * {@inheritdoc}
   */
  public function createInstance($plugin_id, array $configuration = array()) {
    // If this PluginManager has fallback capabilities catch
    // PluginNotFoundExceptions.
    if ($this instanceof FallbackPluginManagerInterface) {
      try {
        return $this->factory->createInstance($plugin_id, $configuration);
      }
      catch (PluginNotFoundException $e) {
        $fallback_id = $this->getFallbackPluginId($plugin_id, $configuration);
        return $this->factory->createInstance($fallback_id, $configuration);
      }
    }
    else {
      return $this->factory->createInstance($plugin_id, $configuration);
    }
  }

  /**
   * {@inheritdoc}
   */
  public function getInstance(array $options) {
    return $this->mapper->getInstance($options);
  }

}
