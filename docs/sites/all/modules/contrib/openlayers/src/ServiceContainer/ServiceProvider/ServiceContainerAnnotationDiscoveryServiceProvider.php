<?php

namespace Drupal\openlayers\ServiceContainer\ServiceProvider;

/**
 * Provides render cache service definitions.
 *
 * @codeCoverageIgnore
 */
class ServiceContainerAnnotationDiscoveryServiceProvider extends ServiceContainerServiceProvider {

  /**
   * {@inheritdoc}
   */
  public function getContainerDefinition() {
    $services = array();
    $parameters['service_container.plugin_managers'] = array();
    $parameters['service_container.plugin_manager_types'] = array(
      'annotated' => '\Drupal\openlayers\Plugin\Discovery\AnnotatedClassDiscovery',
    );

    return array(
      'parameters' => $parameters,
      'services' => $services,
    );
  }

}
