<?php

namespace Drupal\openlayers\Plugin;

use OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface;
use OpenlayersDrupal\Component\Plugin\Factory\DefaultFactory;

/**
 * Defines a plugin manager used for discovering generic plugins.
 */
class DefaultPluginManager extends PluginManagerBase {

  /**
   * Constructs a DefaultPluginManager object.
   *
   * @param OpenlayersDrupal\Component\Plugin\Discovery\DiscoveryInterface $discovery
   *   The discovery object used to find plugins.
   */
  public function __construct(DiscoveryInterface $discovery) {
    $this->discovery = $discovery;
    // Use a generic factory.
    $this->factory = new DefaultFactory($this->discovery);
  }

}
