<?php

namespace OpenlayersDrupal\Core\DependencyInjection;

use OpenlayersDrupal\Component\FileCache\FileCacheFactory;
use OpenlayersDrupal\Component\Serialization\Yaml;
use OpenlayersSymfony\Component\DependencyInjection\Alias;
use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\DefinitionDecorator;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;

/**
 * YamlFileLoader loads YAML files service definitions.
 *
 * Drupal does not use Symfony's Config component, and Symfony's dependency on
 * it cannot be removed easily. Therefore, this is a partial but mostly literal
 * copy of upstream, which does not depend on the Config component.
 *
 * @see \OpenlayersSymfony\Component\DependencyInjection\Loader\YamlFileLoader
 * @see https://github.com/symfony/symfony/pull/10920
 *
 * NOTE: 98% of this code is a literal copy of Symfony's YamlFileLoader.
 *
 * This file does NOT follow Drupal coding standards, so as to simplify future
 * synchronizations.
 */
class YamlFileLoader {

  /**
   * FIX - insert comment here.
   *
   * @var \OpenlayersDrupal\Core\DependencyInjection\ContainerBuilder
   */
  protected $container;

  /**
   * File cache object.
   *
   * @var \OpenlayersDrupal\Component\FileCache\FileCacheInterface
   */
  protected $fileCache;

  /**
   * FIX - insert comment here.
   */
  public function __construct(ContainerBuilder $container) {
    $this->container = $container;
    $this->fileCache = FileCacheFactory::get('container_yaml_loader');
  }

  /**
   * Loads a Yaml file.
   *
   * @param mixed $file
   *   The resource.
   */
  public function load($file) {
    // Load from the file cache, fall back to loading the file.
    // FIX Refactor this to cache parsed definition objects in
    // https://www.drupal.org/node/2464053
    $content = $this->fileCache->get($file);
    if (!$content) {
      $content = $this->loadFile($file);
      $this->fileCache->set($file, $content);
    }

    // Not supported.
    // $this->container->addResource(new FileResource($path));
    // Empty file.
    if (NULL === $content) {
      return;
    }

    // imports
    // Not supported.
    // $this->parseImports($content, $file);
    // Parameters.
    if (isset($content['parameters'])) {
      if (!is_array($content['parameters'])) {
        throw new InvalidArgumentException(sprintf('The "parameters" key should contain an array in %s. Check your YAML syntax.', $file));
      }

      foreach ($content['parameters'] as $key => $value) {
        $this->container->setParameter($key, $this->resolveServices($value));
      }
    }

    // extensions
    // Not supported.
    // $this->loadFromExtensions($content);
    // Services
    $this->parseDefinitions($content, $file);
  }

  /**
   * Parses definitions.
   *
   * @param array $content
   *   FIX - insert comment here.
   * @param string $file
   *   FIX - insert comment here.
   */
  private function parseDefinitions(array $content, $file) {
    if (!isset($content['services'])) {
      return;
    }

    if (!is_array($content['services'])) {
      throw new InvalidArgumentException(sprintf('The "services" key should contain an array in %s. Check your YAML syntax.', $file));
    }

    foreach ($content['services'] as $id => $service) {
      $this->parseDefinition($id, $service, $file);
    }
  }

  /**
   * Parses a definition.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param array $service
   *   FIX - insert comment here.
   * @param string $file
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When tags are invalid.
   */
  private function parseDefinition($id, array $service, $file) {
    if (is_string($service) && 0 === strpos($service, '@')) {
      $this->container->setAlias($id, substr($service, 1));

      return;
    }

    if (!is_array($service)) {
      throw new InvalidArgumentException(sprintf('A service definition must be an array or a string starting with "@" but %s found for service "%s" in %s. Check your YAML syntax.', gettype($service), $id, $file));
    }

    if (isset($service['alias'])) {
      $public = !array_key_exists('public', $service) || (bool) $service['public'];
      $this->container->setAlias($id, new Alias($service['alias'], $public));

      return;
    }

    if (isset($service['parent'])) {
      $definition = new DefinitionDecorator($service['parent']);
    }
    else {
      $definition = new Definition();
    }

    if (isset($service['class'])) {
      $definition->setClass($service['class']);
    }

    if (isset($service['scope'])) {
      $definition->setScope($service['scope']);
    }

    if (isset($service['synthetic'])) {
      $definition->setSynthetic($service['synthetic']);
    }

    if (isset($service['synchronized'])) {
      $definition->setSynchronized($service['synchronized'], 'request' !== $id);
    }

    if (isset($service['lazy'])) {
      $definition->setLazy($service['lazy']);
    }

    if (isset($service['public'])) {
      $definition->setPublic($service['public']);
    }

    if (isset($service['abstract'])) {
      $definition->setAbstract($service['abstract']);
    }

    if (isset($service['factory'])) {
      if (is_string($service['factory'])) {
        if (strpos($service['factory'], ':') !== FALSE && strpos($service['factory'], '::') === FALSE) {
          $parts = explode(':', $service['factory']);
          $definition->setFactory(
            array($this->resolveServices('@' . $parts[0]), $parts[1])
          );
        }
        else {
          $definition->setFactory($service['factory']);
        }
      }
      else {
        $definition->setFactory(
          array(
            $this->resolveServices($service['factory'][0]),
            $service['factory'][1],
          )
        );
      }
    }

    if (isset($service['factory_class'])) {
      $definition->setFactoryClass($service['factory_class']);
    }

    if (isset($service['factory_method'])) {
      $definition->setFactoryMethod($service['factory_method']);
    }

    if (isset($service['factory_service'])) {
      $definition->setFactoryService($service['factory_service']);
    }

    if (isset($service['file'])) {
      $definition->setFile($service['file']);
    }

    if (isset($service['arguments'])) {
      $definition->setArguments($this->resolveServices($service['arguments']));
    }

    if (isset($service['properties'])) {
      $definition->setProperties($this->resolveServices($service['properties']));
    }

    if (isset($service['configurator'])) {
      if (is_string($service['configurator'])) {
        $definition->setConfigurator($service['configurator']);
      }
      else {
        $definition->setConfigurator(array(
          $this->resolveServices($service['configurator'][0]),
          $service['configurator'][1],
        ));
      }
    }

    if (isset($service['calls'])) {
      if (!is_array($service['calls'])) {
        throw new InvalidArgumentException(sprintf('Parameter "calls" must be an array for service "%s" in %s. Check your YAML syntax.', $id, $file));
      }

      foreach ($service['calls'] as $call) {
        if (isset($call['method'])) {
          $method = $call['method'];
          $args = isset($call['arguments']) ? $this->resolveServices($call['arguments']) : array();
        }
        else {
          $method = $call[0];
          $args = isset($call[1]) ? $this->resolveServices($call[1]) : array();
        }

        $definition->addMethodCall($method, $args);
      }
    }

    if (isset($service['tags'])) {
      if (!is_array($service['tags'])) {
        throw new InvalidArgumentException(sprintf('Parameter "tags" must be an array for service "%s" in %s. Check your YAML syntax.', $id, $file));
      }

      foreach ($service['tags'] as $tag) {
        if (!is_array($tag)) {
          throw new InvalidArgumentException(sprintf('A "tags" entry must be an array for service "%s" in %s. Check your YAML syntax.', $id, $file));
        }

        if (!isset($tag['name'])) {
          throw new InvalidArgumentException(sprintf('A "tags" entry is missing a "name" key for service "%s" in %s.', $id, $file));
        }

        $name = $tag['name'];
        unset($tag['name']);

        foreach ($tag as $attribute => $value) {
          if (!is_scalar($value) && NULL !== $value) {
            throw new InvalidArgumentException(sprintf('A "tags" attribute must be of a scalar-type for service "%s", tag "%s", attribute "%s" in %s. Check your YAML syntax.', $id, $name, $attribute, $file));
          }
        }

        $definition->addTag($name, $tag);
      }
    }

    if (isset($service['decorates'])) {
      $renameId = isset($service['decoration_inner_name']) ? $service['decoration_inner_name'] : NULL;
      $definition->setDecoratedService($service['decorates'], $renameId);
    }

    $this->container->setDefinition($id, $definition);
  }

  /**
   * Loads a YAML file.
   *
   * @param string $file
   *   FIX - insert comment here.
   *
   * @return array
   *   The file content.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When the given file is not a local file or when it does not exist.
   */
  protected function loadFile($file) {
    if (!stream_is_local($file)) {
      throw new InvalidArgumentException(sprintf('This is not a local file "%s".', $file));
    }

    if (!file_exists($file)) {
      throw new InvalidArgumentException(sprintf('The service file "%s" is not valid.', $file));
    }

    return $this->validate(Yaml::decode(file_get_contents($file)), $file);
  }

  /**
   * Validates a YAML file.
   *
   * @param mixed $content
   *   FIX - insert comment here.
   * @param string $file
   *   FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When service file is not valid.
   */
  private function validate($content, $file) {
    if (NULL === $content) {
      return $content;
    }

    if (!is_array($content)) {
      throw new InvalidArgumentException(sprintf('The service file "%s" is not valid. It should contain an array. Check your YAML syntax.', $file));
    }

    if ($invalid_keys = array_diff_key(
      $content,
      array('parameters' => 1, 'services' => 1)
    )) {
      throw new InvalidArgumentException(sprintf('The service file "%s" is not valid: it contains invalid keys %s. Services have to be added under "services" and Parameters under "parameters".', $file, $invalid_keys));
    }

    return $content;
  }

  /**
   * Resolves services.
   *
   * @param string|array $value
   *   FIX - insert comment here.
   *
   * @return array|string|Reference
   *   FIX - insert comment here.
   */
  private function resolveServices($value) {
    if (is_array($value)) {
      $value = array_map(array($this, 'resolveServices'), $value);
    }
    elseif (is_string($value) &&  0 === strpos($value, '@=')) {
      // Not supported.
      // return new Expression(substr($value, 2));.
      throw new InvalidArgumentException(sprintf("'%s' is an Expression, but expressions are not supported.", $value));
    }
    elseif (is_string($value) &&  0 === strpos($value, '@')) {
      if (0 === strpos($value, '@@')) {
        $value = substr($value, 1);
        $invalidBehavior = NULL;
      }
      elseif (0 === strpos($value, '@?')) {
        $value = substr($value, 2);
        $invalidBehavior = ContainerInterface::IGNORE_ON_INVALID_REFERENCE;
      }
      else {
        $value = substr($value, 1);
        $invalidBehavior = ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE;
      }

      if ('=' === substr($value, -1)) {
        $value = substr($value, 0, -1);
        $strict = FALSE;
      }
      else {
        $strict = TRUE;
      }

      if (NULL !== $invalidBehavior) {
        $value = new Reference($value, $invalidBehavior, $strict);
      }
    }

    return $value;
  }

}
