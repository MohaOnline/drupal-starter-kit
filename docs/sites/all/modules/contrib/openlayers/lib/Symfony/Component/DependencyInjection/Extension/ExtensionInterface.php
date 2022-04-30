<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Extension;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * FIX - insert comment here.
 *
 * ExtensionInterface is the interface implemented by container
 * extension classes.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
interface ExtensionInterface {

  /**
   * Loads a specific configuration.
   *
   * @param array $config
   *   An array of configuration values.
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   A ContainerBuilder instance.
   *
   * @throws InvalidArgumentException
   *   When provided tag is not defined in this extension.
   */
  public function load(array $config, ContainerBuilder $container);

  /**
   * Returns the namespace to be used for this extension (XML namespace).
   *
   * @return string
   *   The XML namespace.
   */
  public function getNamespace();

  /**
   * Returns the base path for the XSD files.
   *
   * @return string
   *   The XSD base path.
   */
  public function getXsdValidationBasePath();

  /**
   * Returns the recommended alias to use in XML.
   *
   * This alias is also the mandatory prefix to use when using YAML.
   *
   * @return string
   *   The alias.
   */
  public function getAlias();

}
