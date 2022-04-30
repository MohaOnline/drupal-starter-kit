<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Dumper;

use OpenlayersSymfony\Component\Yaml\Dumper as YmlDumper;
use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Parameter;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;
use OpenlayersSymfony\Component\ExpressionLanguage\Expression;

/**
 * YamlDumper dumps a service container as a YAML string.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
class YamlDumper extends Dumper {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $dumper;

  /**
   * Dumps the service container as an YAML string.
   *
   * @param array $options
   *   An array of options.
   *
   * @return string
   *   A YAML string representing of the service container.
   */
  public function dump(array $options = array()) {
    if (!class_exists('OpenlayersSymfony\Component\Yaml\Dumper')) {
      throw new RuntimeException('Unable to dump the container as the Symfony Yaml Component is not installed.');
    }

    if (NULL === $this->dumper) {
      $this->dumper = new YmlDumper();
    }

    return $this->addParameters() . "\n" . $this->addServices();
  }

  /**
   * Adds a service.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addService($id, Definition $definition) {
    $code = "    $id:\n";
    if ($definition->getClass()) {
      $code .= sprintf("        class: %s\n", $definition->getClass());
    }

    if (!$definition->isPublic()) {
      $code .= "        public: false\n";
    }

    $tagsCode = '';
    foreach ($definition->getTags() as $name => $tags) {
      foreach ($tags as $attributes) {
        $att = array();
        foreach ($attributes as $key => $value) {
          $att[] = sprintf('%s: %s', $this->dumper->dump($key), $this->dumper->dump($value));
        }
        $att = $att ? ', ' . implode(', ', $att) : '';

        $tagsCode .= sprintf("            - { name: %s%s }\n", $this->dumper->dump($name), $att);
      }
    }
    if ($tagsCode) {
      $code .= "        tags:\n" . $tagsCode;
    }

    if ($definition->getFile()) {
      $code .= sprintf("        file: %s\n", $definition->getFile());
    }

    if ($definition->isSynthetic()) {
      $code .= sprintf("        synthetic: true\n");
    }

    if ($definition->isSynchronized(FALSE)) {
      $code .= sprintf("        synchronized: true\n");
    }

    if ($definition->getFactoryClass(FALSE)) {
      $code .= sprintf("        factory_class: %s\n", $definition->getFactoryClass(FALSE));
    }

    if ($definition->isLazy()) {
      $code .= sprintf("        lazy: true\n");
    }

    if ($definition->getFactoryMethod(FALSE)) {
      $code .= sprintf("        factory_method: %s\n", $definition->getFactoryMethod(FALSE));
    }

    if ($definition->getFactoryService(FALSE)) {
      $code .= sprintf("        factory_service: %s\n", $definition->getFactoryService(FALSE));
    }

    if ($definition->getArguments()) {
      $code .= sprintf("        arguments: %s\n", $this->dumper->dump($this->dumpValue($definition->getArguments()), 0));
    }

    if ($definition->getProperties()) {
      $code .= sprintf("        properties: %s\n", $this->dumper->dump($this->dumpValue($definition->getProperties()), 0));
    }

    if ($definition->getMethodCalls()) {
      $code .= sprintf("        calls:\n%s\n", $this->dumper->dump($this->dumpValue($definition->getMethodCalls()), 1, 12));
    }

    if (ContainerInterface::SCOPE_CONTAINER !== $scope = $definition->getScope()) {
      $code .= sprintf("        scope: %s\n", $scope);
    }

    if (NULL !== $decorated = $definition->getDecoratedService()) {
      list($decorated, $renamedId) = $decorated;
      $code .= sprintf("        decorates: %s\n", $decorated);
      if (NULL !== $renamedId) {
        $code .= sprintf("        decoration_inner_name: %s\n", $renamedId);
      }
    }

    if ($callable = $definition->getFactory()) {
      $code .= sprintf("        factory: %s\n", $this->dumper->dump($this->dumpCallable($callable), 0));
    }

    if ($callable = $definition->getConfigurator()) {
      $code .= sprintf("        configurator: %s\n", $this->dumper->dump($this->dumpCallable($callable), 0));
    }

    return $code;
  }

  /**
   * Adds a service alias.
   *
   * @param string $alias
   *   FIX - insert comment here.
   * @param Alias $id
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServiceAlias($alias, Alias $id) {
    if ($id->isPublic()) {
      return sprintf("    %s: @%s\n", $alias, $id);
    }
    else {
      return sprintf("    %s:\n        alias: %s\n        public: false", $alias, $id);
    }
  }

  /**
   * Adds services.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServices() {
    if (!$this->container->getDefinitions()) {
      return '';
    }

    $code = "services:\n";
    foreach ($this->container->getDefinitions() as $id => $definition) {
      $code .= $this->addService($id, $definition);
    }

    $aliases = $this->container->getAliases();
    foreach ($aliases as $alias => $id) {
      while (isset($aliases[(string) $id])) {
        $id = $aliases[(string) $id];
      }
      $code .= $this->addServiceAlias($alias, $id);
    }

    return $code;
  }

  /**
   * Adds parameters.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addParameters() {
    if (!$this->container->getParameterBag()->all()) {
      return '';
    }

    $parameters = $this->prepareParameters($this->container->getParameterBag()->all(), $this->container->isFrozen());

    return $this->dumper->dump(array('parameters' => $parameters), 2);
  }

  /**
   * Dumps callable to YAML format.
   *
   * @param callable $callable
   *   FIX - insert comment here.
   *
   * @return callable
   *   FIX - insert comment here.
   */
  private function dumpCallable(callable $callable) {
    if (is_array($callable)) {
      if ($callable[0] instanceof Reference) {
        $callable = array(
          $this->getServiceCall((string) $callable[0], $callable[0]),
          $callable[1],
        );
      }
      else {
        $callable = array($callable[0], $callable[1]);
      }
    }

    return $callable;
  }

  /**
   * Dumps the value to YAML format.
   *
   * @param mixed $value
   *   FIX - insert comment here.
   *
   * @return mixed
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When trying to dump object or resource.
   */
  private function dumpValue($value) {
    if (is_array($value)) {
      $code = array();
      foreach ($value as $k => $v) {
        $code[$k] = $this->dumpValue($v);
      }

      return $code;
    }
    elseif ($value instanceof Reference) {
      return $this->getServiceCall((string) $value, $value);
    }
    elseif ($value instanceof Parameter) {
      return $this->getParameterCall((string) $value);
    }
    elseif ($value instanceof Expression) {
      return $this->getExpressionCall((string) $value);
    }
    elseif (is_object($value) || is_resource($value)) {
      throw new RuntimeException('Unable to dump a service container if a parameter is an object or a resource.');
    }

    return $value;
  }

  /**
   * Gets the service call.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \OpenlayersSymfony\Component\DependencyInjection\Reference $reference
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function getServiceCall($id, Reference $reference = NULL) {
    if (NULL !== $reference && ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE !== $reference->getInvalidBehavior()) {
      return sprintf('@?%s', $id);
    }

    return sprintf('@%s', $id);
  }

  /**
   * Gets parameter call.
   *
   * @param string $id
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function getParameterCall($id) {
    return sprintf('%%%s%%', $id);
  }

  /**
   * FIX - insert comment here.
   */
  private function getExpressionCall($expression) {
    return sprintf('@=%s', $expression);
  }

  /**
   * Prepares parameters.
   *
   * @param array $parameters
   *   FIX - insert comment here.
   * @param bool $escape
   *   FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function prepareParameters(array $parameters, $escape = TRUE) {
    $filtered = array();
    foreach ($parameters as $key => $value) {
      if (is_array($value)) {
        $value = $this->prepareParameters($value, $escape);
      }
      elseif ($value instanceof Reference || is_string($value) && 0 === strpos($value, '@')) {
        $value = '@' . $value;
      }

      $filtered[$key] = $value;
    }

    return $escape ? $this->escape($filtered) : $filtered;
  }

  /**
   * Escapes arguments.
   *
   * @param array $arguments
   *   FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function escape(array $arguments) {
    $args = array();
    foreach ($arguments as $k => $v) {
      if (is_array($v)) {
        $args[$k] = $this->escape($v);
      }
      elseif (is_string($v)) {
        $args[$k] = str_replace('%', '%%', $v);
      }
      else {
        $args[$k] = $v;
      }
    }

    return $args;
  }

}
