<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Extension;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * FIX - insert comment here.
 */
interface PrependExtensionInterface {

  /**
   * Allow an extension to prepend the extension configurations.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function prepend(ContainerBuilder $container);

}
