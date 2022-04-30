<?php

namespace Drupal\openlayers\DependencyInjection;

/**
 * Interface to build container objects.
 *
 * @ingroup dic
 */
interface ContainerBuilderInterface {

  /**
   * Returns the fully build container definition.
   *
   * @return array
   *   An associative array with the following keys:
   *     - parameters: The parameters of the container, simple k/v
   *     - services: The services of the container.
   *
   * @see \Drupal\openlayers\DependencyInjection\ServiceProviderInterface
   */
  public function getContainerDefinition();

  /**
   * Compiles the container builder to a new container.
   *
   * @return \Drupal\openlayers\DependencyInjection\ContainerInterface
   *   The newly constructed container.
   */
  public function compile();

}
