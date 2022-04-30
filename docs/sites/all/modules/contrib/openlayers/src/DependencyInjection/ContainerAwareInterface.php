<?php

namespace Drupal\openlayers\DependencyInjection;

/**
 * FIX - insert short comment here.
 *
 * ContainerAwareInterface should be implemented by classes that depend on a
 * Container.
 *
 * @ingroup dic
 */
interface ContainerAwareInterface {

  /**
   * Sets the Container associated with this service.
   *
   * @param ContainerInterface|null $container
   *   A ContainerInterface instance or NULL to be injected in the service.
   */
  public function setContainer(ContainerInterface $container = NULL);

}
