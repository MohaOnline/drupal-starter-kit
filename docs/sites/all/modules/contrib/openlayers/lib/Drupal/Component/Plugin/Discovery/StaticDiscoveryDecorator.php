<?php

namespace OpenlayersDrupal\Component\Plugin\Discovery;

/**
 * A decorator that allows manual registration of undiscoverable definitions.
 */
class StaticDiscoveryDecorator extends StaticDiscovery {

  /**
   * The Discovery object being decorated.
   *
   * @var \OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface
   */
  protected $decorated;

  /**
   * A callback or closure used for registering additional definitions.
   *
   * @var \Callable
   */
  protected $registerDefinitions;

  /**
   * FIX - insert comment here.
   *
   * Constructs a
   * \OpenlayersDrupal\Component\Plugin\Discovery\StaticDiscoveryDecorator
   * object.
   *
   * @param \OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface $decorated
   *   The discovery object that is being decorated.
   * @param callable $registerDefinitions
   *   (optional) A callback or closure used for registering additional
   *   definitions.
   */
  public function __construct(DiscoveryInterface $decorated, callable $registerDefinitions = NULL) {
    $this->decorated = $decorated;
    $this->registerDefinitions = $registerDefinitions;
  }

  /**
   * {@inheritdoc}
   */
  public function getDefinition($base_plugin_id, $exception_on_invalid = TRUE) {
    if (isset($this->registerDefinitions)) {
      call_user_func($this->registerDefinitions);
    }
    $this->definitions += $this->decorated->getDefinitions();
    return parent::getDefinition($base_plugin_id, $exception_on_invalid);
  }

  /**
   * Implements OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface::getDefinitions().
   */
  public function getDefinitions() {
    if (isset($this->registerDefinitions)) {
      call_user_func($this->registerDefinitions);
    }
    $this->definitions += $this->decorated->getDefinitions();
    return parent::getDefinitions();
  }

  /**
   * Passes through all unknown calls onto the decorated object.
   */
  public function __call($method, $args) {
    return call_user_func_array(array($this->decorated, $method), $args);
  }

}
