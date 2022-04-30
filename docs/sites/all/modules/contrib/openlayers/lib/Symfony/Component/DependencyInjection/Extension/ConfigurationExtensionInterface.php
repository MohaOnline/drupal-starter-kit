<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Extension;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * The interface implemented by container extension classes.
 *
 * @author Kevin Bond <kevinbond@gmail.com>
 */
interface ConfigurationExtensionInterface {

  /**
   * Returns extension configuration.
   *
   * @param array $config
   *   An array of configuration values.
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   A ContainerBuilder instance.
   *
   * @return ConfigurationInterface|null
   *   The configuration or null
   */
  public function getConfiguration(array $config, ContainerBuilder $container);

}
