<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Extension;

use OpenlayersSymfony\Component\DependencyInjection\Container;
use OpenlayersSymfony\Component\DependencyInjection\Exception\BadMethodCallException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\Config\Resource\FileResource;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\Config\Definition\Processor;
use OpenlayersSymfony\Component\Config\Definition\ConfigurationInterface;

/**
 * Provides useful features shared by many extensions.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
abstract class Extension implements ExtensionInterface, ConfigurationExtensionInterface {

  /**
   * Returns the base path for the XSD files.
   *
   * @return string
   *   The XSD base path.
   */
  public function getXsdValidationBasePath() {
    return FALSE;
  }

  /**
   * Returns the namespace to be used for this extension (XML namespace).
   *
   * @return string
   *   The XML namespace.
   */
  public function getNamespace() {
    return 'http://example.org/schema/dic/' . $this->getAlias();
  }

  /**
   * Returns the recommended alias to use in XML.
   *
   * This alias is also the mandatory prefix to use when using YAML.
   *
   * This convention is to remove the "Extension" postfix from the class
   * name and then lowercase and underscore the result. So:
   *
   *     AcmeHelloExtension
   *
   * becomes
   *
   *     acme_hello
   *
   * This can be overridden in a sub-class to specify the alias manually.
   *
   * @return string
   *   The alias.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\BadMethodCallException
   *   When the extension name does not follow conventions.
   */
  public function getAlias() {
    $className = get_class($this);
    if (substr($className, -9) != 'Extension') {
      throw new BadMethodCallException('This extension does not follow the naming convention; you must overwrite the getAlias() method.');
    }
    $classBaseName = substr(strrchr($className, '\\'), 1, -9);

    return Container::underscore($classBaseName);
  }

  /**
   * {@inheritdoc}
   */
  public function getConfiguration(array $config, ContainerBuilder $container) {
    $reflected = new \ReflectionClass($this);
    $namespace = $reflected->getNamespaceName();

    $class = $namespace . '\\Configuration';
    if (class_exists($class)) {
      $r = new \ReflectionClass($class);
      $container->addResource(new FileResource($r->getFileName()));

      if (!method_exists($class, '__construct')) {
        $configuration = new $class();

        return $configuration;
      }
    }
  }

  /**
   * FIX - insert comment here.
   */
  final protected function processConfiguration(ConfigurationInterface $configuration, array $configs) {
    $processor = new Processor();

    return $processor->processConfiguration($configuration, $configs);
  }

  /**
   * FIX - insert comment here.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   * @param array $config
   *   FIX - insert comment here.
   *
   * @return bool
   *   Whether the configuration is enabled.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When the config is not enableable.
   */
  protected function isConfigEnabled(ContainerBuilder $container, array $config) {
    if (!array_key_exists('enabled', $config)) {
      throw new InvalidArgumentException("The config array has no 'enabled' key.");
    }

    return (bool) $container->getParameterBag()->resolveValue($config['enabled']);
  }

}
