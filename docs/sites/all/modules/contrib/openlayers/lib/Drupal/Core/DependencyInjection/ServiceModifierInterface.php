<?php

namespace OpenlayersDrupal\Core\DependencyInjection;

/**
 * Interface that service providers can implement to modify services.
 *
 * @ingroup container
 */
interface ServiceModifierInterface {

  /**
   * Modifies existing service definitions.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   The ContainerBuilder whose service definitions can be altered.
   */
  public function alter(ContainerBuilder $container);

}
