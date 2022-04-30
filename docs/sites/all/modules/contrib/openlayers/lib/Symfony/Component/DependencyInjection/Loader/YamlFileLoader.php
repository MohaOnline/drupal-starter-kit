<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Loader;

use OpenlayersSymfony\Component\DependencyInjection\DefinitionDecorator;
use OpenlayersSymfony\Component\DependencyInjection\Alias;
use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;
use OpenlayersSymfony\Component\Config\Resource\FileResource;
use OpenlayersSymfony\Component\Yaml\Parser as YamlParser;
use OpenlayersSymfony\Component\ExpressionLanguage\Expression;

/**
 * YamlFileLoader loads YAML files service definitions.
 *
 * The YAML format does not support anonymous services (cf. the XML loader).
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class YamlFileLoader extends FileLoader {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $yamlParser;

  /**
   * {@inheritdoc}
   */
  public function load($resource, $type = NULL) {
    $path = $this->locator->locate($resource);

    $content = $this->loadFile($path);

    $this->container->addResource(new FileResource($path));

    // Empty file.
    if (NULL === $content) {
      return;
    }

    // Imports.
    $this->parseImports($content, $path);

    // Parameters.
    if (isset($content['parameters'])) {
      if (!is_array($content['parameters'])) {
        throw new InvalidArgumentException(sprintf('The "parameters" key should contain an array in %s. Check your YAML syntax.', $resource));
      }

      foreach ($content['parameters'] as $key => $value) {
        $this->container->setParameter($key, $this->resolveServices($value));
      }
    }

    // Extensions.
    $this->loadFromExtensions($content);

    // Services.
    $this->parseDefinitions($content, $resource);
  }

  /**
   * {@inheritdoc}
   */
  public function supports($resource, $type = NULL) {
    return is_string($resource) &&
      in_array(
        pathinfo($resource, PATHINFO_EXTENSION),
        array('yml', 'yaml'),
        TRUE
      );
  }

  /**
   * Parses all imports.
   *
   * @param array $content
   *   FIX - insert comment here.
   * @param string $file
   *   FIX - insert comment here.
   */
  private function parseImports(array $content, $file) {
    if (!isset($content['imports'])) {
      return;
    }

    if (!is_array($content['imports'])) {
      throw new InvalidArgumentException(sprintf('The "imports" key should contain an array in %s. Check your YAML syntax.', $file));
    }

    foreach ($content['imports'] as $import) {
      if (!is_array($import)) {
        throw new InvalidArgumentException(sprintf('The values in the "imports" key should be arrays in %s. Check your YAML syntax.', $file));
      }

      $this->setCurrentDir(dirname($file));
      $this->import($import['resource'], NULL, isset($import['ignore_errors']) ? (bool) $import['ignore_errors'] : FALSE, $file);
    }
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
      trigger_error(sprintf('The "synchronized" key in file "%s" is deprecated since version 2.7 and will be removed in 3.0.', $file), E_USER_DEPRECATED);
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
          $definition->setFactory(array(
            $this->resolveServices('@' . $parts[0]),
            $parts[1],
          ));
        }
        else {
          $definition->setFactory($service['factory']);
        }
      }
      else {
        $definition->setFactory(array(
          $this->resolveServices($service['factory'][0]),
          $service['factory'][1],
        ));
      }
    }

    if (isset($service['factory_class'])) {
      trigger_error(sprintf('The "factory_class" key in file "%s" is deprecated since version 2.6 and will be removed in 3.0. Use "factory" instead.', $file), E_USER_DEPRECATED);
      $definition->setFactoryClass($service['factory_class']);
    }

    if (isset($service['factory_method'])) {
      trigger_error(sprintf('The "factory_method" key in file "%s" is deprecated since version 2.6 and will be removed in 3.0. Use "factory" instead.', $file), E_USER_DEPRECATED);
      $definition->setFactoryMethod($service['factory_method']);
    }

    if (isset($service['factory_service'])) {
      trigger_error(sprintf('The "factory_service" key in file "%s" is deprecated since version 2.6 and will be removed in 3.0. Use "factory" instead.', $file), E_USER_DEPRECATED);
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
    if (!class_exists('OpenlayersSymfony\Component\Yaml\Parser')) {
      throw new RuntimeException('Unable to load YAML config files as the Symfony Yaml Component is not installed.');
    }

    if (!stream_is_local($file)) {
      throw new InvalidArgumentException(sprintf('This is not a local file "%s".', $file));
    }

    if (!file_exists($file)) {
      throw new InvalidArgumentException(sprintf('The service file "%s" is not valid.', $file));
    }

    if (NULL === $this->yamlParser) {
      $this->yamlParser = new YamlParser();
    }

    return $this->validate($this->yamlParser->parse(file_get_contents($file)), $file);
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

    foreach ($content as $namespace => $data) {
      if (in_array($namespace, array('imports', 'parameters', 'services'))) {
        continue;
      }

      if (!$this->container->hasExtension($namespace)) {
        $extensionNamespaces = array_filter(array_map(function ($ext) {
          return $ext->getAlias();
        }, $this->container->getExtensions()));
        throw new InvalidArgumentException(sprintf(
              'There is no extension able to load the configuration for "%s" (in %s). Looked for namespace "%s", found %s',
              $namespace,
              $file,
              $namespace,
              $extensionNamespaces ? sprintf('"%s"', implode('", "', $extensionNamespaces)) : 'none'
          ));
      }
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
      return new Expression(substr($value, 2));
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

  /**
   * Loads from Extensions.
   *
   * @param array $content
   *   FIX - insert comment here.
   */
  private function loadFromExtensions(array $content) {
    foreach ($content as $namespace => $values) {
      if (in_array($namespace, array('imports', 'parameters', 'services'))) {
        continue;
      }

      if (!is_array($values)) {
        $values = array();
      }

      $this->container->loadFromExtension($namespace, $values);
    }
  }

}
