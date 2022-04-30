<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * FIX - insert comment here.
 *
 * IntrospectableContainerInterface defines additional introspection
 * functionality for containers, allowing logic to be implemented based
 * on a Container's state.
 *
 * @author Evan Villemez <evillemez@gmail.com>
 */
interface IntrospectableContainerInterface extends ContainerInterface {

  /**
   * Check for whether or not a service has been initialized.
   *
   * @param string $id
   *   FIX - insert comment here.
   *
   * @return bool
   *   True if the service has been initialized, false otherwise.
   */
  public function initialized($id);

}
