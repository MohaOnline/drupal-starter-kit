<?php

namespace Drupal\openlayers\DependencyInjection;

use Drupal\openlayers\Plugin\DefaultPluginManager;
use Drupal\openlayers\Plugin\Discovery\CToolsPluginDiscovery;

/**
 * Defines a plugin manager used for discovering container service definitions.
 */
class ServiceProviderPluginManager extends DefaultPluginManager {

  /**
   * Constructs a ServiceProviderPluginManager object.
   *
   * This uses ctools for discovery of openlayers ServiceProvider objects.
   *
   * @codeCoverageIgnore
   */
  public function __construct() {
    $discovery = new CToolsPluginDiscovery(array(
      'owner' => 'openlayers',
      'type' => 'ServiceProvider',
    ));
    parent::__construct($discovery);
  }

}
