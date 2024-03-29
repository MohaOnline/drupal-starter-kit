<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\OutOfBoundsException;

/**
 * Definition represents a service definition.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class Definition {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $class;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $file;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $factory;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $factoryClass;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $factoryMethod;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $factoryService;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $scope = ContainerInterface::SCOPE_CONTAINER;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $properties = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $calls = array();

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $configurator;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $tags = array();

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $public = TRUE;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $synthetic = FALSE;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $abstract = FALSE;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $synchronized = FALSE;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $lazy = FALSE;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $decoratedService;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $arguments;

  /**
   * Constructor.
   *
   * @param string|null $class
   *   The service class.
   * @param array $arguments
   *   An array of arguments to pass to the service constructor.
   */
  public function __construct($class = NULL, array $arguments = array()) {
    $this->class = $class;
    $this->arguments = $arguments;
  }

  /**
   * Sets a factory.
   *
   * @param string|array $factory
   *   A PHP function or an array containing a class/Reference and a
   *   method to call.
   *
   * @return Definition
   *   The current instance.
   */
  public function setFactory($factory) {
    if (is_string($factory) && strpos($factory, '::') !== FALSE) {
      $factory = explode('::', $factory, 2);
    }

    $this->factory = $factory;

    return $this;
  }

  /**
   * Gets the factory.
   *
   * @return string|array
   *   The PHP function or an array containing a class/Reference and a
   *   method to call.
   */
  public function getFactory() {
    return $this->factory;
  }

  /**
   * FIX - insert comment here.
   *
   * Sets the name of the class that acts as a factory using the factory method,
   * which will be invoked statically.
   *
   * @param string $factoryClass
   *   The factory class name.
   *
   * @return Definition
   *   The current instance.
   */
  public function setFactoryClass($factoryClass) {
    trigger_error(sprintf('%s(%s) is deprecated since version 2.6 and will be removed in 3.0. Use Definition::setFactory() instead.', __METHOD__, $factoryClass), E_USER_DEPRECATED);

    $this->factoryClass = $factoryClass;

    return $this;
  }

  /**
   * Gets the factory class.
   *
   * @return string|null
   *   The factory class name.
   */
  public function getFactoryClass($triggerDeprecationError = TRUE) {
    if ($triggerDeprecationError) {
      trigger_error('The ' . __METHOD__ . ' method is deprecated since version 2.6 and will be removed in 3.0.', E_USER_DEPRECATED);
    }

    return $this->factoryClass;
  }

  /**
   * Sets the factory method able to create an instance of this class.
   *
   * @param string $factoryMethod
   *   The factory method name.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setFactoryMethod($factoryMethod) {
    trigger_error(sprintf('%s(%s) is deprecated since version 2.6 and will be removed in 3.0. Use Definition::setFactory() instead.', __METHOD__, $factoryMethod), E_USER_DEPRECATED);

    $this->factoryMethod = $factoryMethod;

    return $this;
  }

  /**
   * Sets the service that this service is decorating.
   *
   * @param null|string $id
   *   The decorated service id, use null to remove decoration.
   * @param null|string $renamedId
   *   The new decorated service id.
   *
   * @return \Definition
   *   The current instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   In case the decorated service id and the new decorated service id are
   *   equals.
   */
  public function setDecoratedService($id, $renamedId = NULL) {
    if ($renamedId && $id == $renamedId) {
      throw new \InvalidArgumentException(sprintf('The decorated service inner name for "%s" must be different than the service name itself.', $id));
    }

    if (NULL === $id) {
      $this->decoratedService = NULL;
    }
    else {
      $this->decoratedService = array($id, $renamedId);
    }

    return $this;
  }

  /**
   * Gets the service that decorates this service.
   *
   * @return null|array
   *   An array composed of the decorated service id and the new id for it,
   *   null if no service is decorated.
   */
  public function getDecoratedService() {
    return $this->decoratedService;
  }

  /**
   * Gets the factory method.
   *
   * @return string|null
   *   The factory method name.
   */
  public function getFactoryMethod($triggerDeprecationError = TRUE) {
    if ($triggerDeprecationError) {
      trigger_error('The ' . __METHOD__ . ' method is deprecated since version 2.6 and will be removed in 3.0.', E_USER_DEPRECATED);
    }

    return $this->factoryMethod;
  }

  /**
   * FIX - insert comment here.
   *
   * Sets the name of the service that acts as a factory using the factory
   * method.
   *
   * @param string $factoryService
   *   The factory service id.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setFactoryService($factoryService) {
    trigger_error(sprintf('%s(%s) is deprecated since version 2.6 and will be removed in 3.0. Use Definition::setFactory() instead.', __METHOD__, $factoryService), E_USER_DEPRECATED);

    $this->factoryService = $factoryService;

    return $this;
  }

  /**
   * Gets the factory service id.
   *
   * @return string|null
   *   The factory service id.
   */
  public function getFactoryService($triggerDeprecationError = TRUE) {
    if ($triggerDeprecationError) {
      trigger_error('The ' . __METHOD__ . ' method is deprecated since version 2.6 and will be removed in 3.0.', E_USER_DEPRECATED);
    }

    return $this->factoryService;
  }

  /**
   * Sets the service class.
   *
   * @param string $class
   *   The service class.
   *
   * @return Definition
   *   The current instance.
   */
  public function setClass($class) {
    $this->class = $class;

    return $this;
  }

  /**
   * Gets the service class.
   *
   * @return string|null
   *   The service class.
   */
  public function getClass() {
    return $this->class;
  }

  /**
   * Sets the arguments to pass to the service constructor/factory method.
   *
   * @param array $arguments
   *   An array of arguments.
   *
   * @return Definition
   *   The current instance.
   *
   * @api
   */
  public function setArguments(array $arguments) {
    $this->arguments = $arguments;

    return $this;
  }

  /**
   * FIX - insert comment here.
   */
  public function setProperties(array $properties) {
    $this->properties = $properties;

    return $this;
  }

  /**
   * FIX - insert comment here.
   */
  public function getProperties() {
    return $this->properties;
  }

  /**
   * FIX - insert comment here.
   */
  public function setProperty($name, $value) {
    $this->properties[$name] = $value;

    return $this;
  }

  /**
   * Adds an argument to pass to the service constructor/factory method.
   *
   * @param mixed $argument
   *   An argument.
   *
   * @return Definition
   *   The current instance
   */
  public function addArgument($argument) {
    $this->arguments[] = $argument;

    return $this;
  }

  /**
   * Sets a specific argument.
   *
   * @param int $index
   *   FIX - insert comment here.
   * @param mixed $argument
   *   FIX - insert comment here.
   *
   * @return \Definition
   *   The current instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\OutOfBoundsException
   *   When the replaced argument does not exist.
   */
  public function replaceArgument($index, $argument) {
    if ($index < 0 || $index > count($this->arguments) - 1) {
      throw new OutOfBoundsException(sprintf('The index "%d" is not in the range [0, %d].', $index, count($this->arguments) - 1));
    }

    $this->arguments[$index] = $argument;

    return $this;
  }

  /**
   * Gets the arguments to pass to the service constructor/factory method.
   *
   * @return array
   *   The array of arguments.
   */
  public function getArguments() {
    return $this->arguments;
  }

  /**
   * Gets an argument to pass to the service constructor/factory method.
   *
   * @param int $index
   *   FIX - insert comment here.
   *
   * @return mixed
   *   The argument value.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\OutOfBoundsException
   *   When the argument does not exist.
   */
  public function getArgument($index) {
    if ($index < 0 || $index > count($this->arguments) - 1) {
      throw new OutOfBoundsException(sprintf('The index "%d" is not in the range [0, %d].', $index, count($this->arguments) - 1));
    }

    return $this->arguments[$index];
  }

  /**
   * Sets the methods to call after service initialization.
   *
   * @param array $calls
   *   An array of method calls.
   *
   * @return Definition
   *   The current instance.
   */
  public function setMethodCalls(array $calls = array()) {
    $this->calls = array();
    foreach ($calls as $call) {
      $this->addMethodCall($call[0], $call[1]);
    }

    return $this;
  }

  /**
   * Adds a method to call after service initialization.
   *
   * @param string $method
   *   The method name to call.
   * @param array $arguments
   *   An array of arguments to pass to the method call.
   *
   * @return \Definition
   *   The current instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   On empty $method param.
   */
  public function addMethodCall($method, array $arguments = array()) {
    if (empty($method)) {
      throw new InvalidArgumentException(sprintf('Method name cannot be empty.'));
    }
    $this->calls[] = array($method, $arguments);

    return $this;
  }

  /**
   * Removes a method to call after service initialization.
   *
   * @param string $method
   *   The method name to remove.
   *
   * @return \Definition
   *   The current instance.
   */
  public function removeMethodCall($method) {
    foreach ($this->calls as $i => $call) {
      if ($call[0] === $method) {
        unset($this->calls[$i]);
        break;
      }
    }

    return $this;
  }

  /**
   * FIX - insert comment here.
   *
   * Check if the current definition has a given method to call after service
   * initialization.
   *
   * @param string $method
   *   The method name to search for.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function hasMethodCall($method) {
    foreach ($this->calls as $call) {
      if ($call[0] === $method) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Gets the methods to call after service initialization.
   *
   * @return array
   *   An array of method calls.
   */
  public function getMethodCalls() {
    return $this->calls;
  }

  /**
   * Sets tags for this definition.
   *
   * @param array $tags
   *   FIX - insert comment here.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setTags(array $tags) {
    $this->tags = $tags;

    return $this;
  }

  /**
   * Returns all tags.
   *
   * @return array
   *   An array of tags.
   */
  public function getTags() {
    return $this->tags;
  }

  /**
   * Gets a tag by name.
   *
   * @param string $name
   *   The tag name.
   *
   * @return array
   *   An array of attributes.
   */
  public function getTag($name) {
    return isset($this->tags[$name]) ? $this->tags[$name] : array();
  }

  /**
   * Adds a tag for this definition.
   *
   * @param string $name
   *   The tag name.
   * @param array $attributes
   *   An array of attributes.
   *
   * @return \Definition
   *   The current instance.
   */
  public function addTag($name, array $attributes = array()) {
    $this->tags[$name][] = $attributes;

    return $this;
  }

  /**
   * Whether this definition has a tag with the given name.
   *
   * @param string $name
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function hasTag($name) {
    return isset($this->tags[$name]);
  }

  /**
   * Clears all tags for a given name.
   *
   * @param string $name
   *   The tag name.
   *
   * @return \Definition
   *   FIX - insert comment here.
   */
  public function clearTag($name) {
    if (isset($this->tags[$name])) {
      unset($this->tags[$name]);
    }

    return $this;
  }

  /**
   * Clears the tags for this definition.
   *
   * @return Definition
   *   The current instance.
   */
  public function clearTags() {
    $this->tags = array();

    return $this;
  }

  /**
   * Sets a file to require before creating the service.
   *
   * @param string $file
   *   A full pathname to include.
   *
   * @return Definition
   *   The current instance.
   */
  public function setFile($file) {
    $this->file = $file;

    return $this;
  }

  /**
   * Gets the file to require before creating the service.
   *
   * @return string|null
   *   The full pathname to include.
   */
  public function getFile() {
    return $this->file;
  }

  /**
   * Sets the scope of the service.
   *
   * @param string $scope
   *   Whether the service must be shared or not.
   *
   * @return Definition
   *   The current instance.
   */
  public function setScope($scope) {
    $this->scope = $scope;

    return $this;
  }

  /**
   * Returns the scope of the service.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getScope() {
    return $this->scope;
  }

  /**
   * Sets the visibility of this service.
   *
   * @param bool $boolean
   *   FIX - insert comment here.
   *
   * @return Definition
   *   The current instance.
   */
  public function setPublic($boolean) {
    $this->public = (bool) $boolean;

    return $this;
  }

  /**
   * Whether this service is public facing.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isPublic() {
    return $this->public;
  }

  /**
   * Sets the synchronized flag of this service.
   *
   * @param bool $boolean
   *   FIX - insert comment here.
   * @param bool $triggerDeprecationError
   *   FIX - insert comment here.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setSynchronized($boolean, $triggerDeprecationError = TRUE) {
    if ($triggerDeprecationError) {
      trigger_error('The ' . __METHOD__ . ' method is deprecated since version 2.7 and will be removed in 3.0.', E_USER_DEPRECATED);
    }

    $this->synchronized = (bool) $boolean;

    return $this;
  }

  /**
   * Whether this service is synchronized.
   *
   * @param bool $triggerDeprecationError
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isSynchronized($triggerDeprecationError = TRUE) {
    if ($triggerDeprecationError) {
      trigger_error('The ' . __METHOD__ . ' method is deprecated since version 2.7 and will be removed in 3.0.', E_USER_DEPRECATED);
    }

    return $this->synchronized;
  }

  /**
   * Sets the lazy flag of this service.
   *
   * @param bool $lazy
   *   FIX - insert comment here.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setLazy($lazy) {
    $this->lazy = (bool) $lazy;

    return $this;
  }

  /**
   * Whether this service is lazy.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isLazy() {
    return $this->lazy;
  }

  /**
   * FIX - insert comment here.
   *
   * Sets whether this definition is synthetic, that is not constructed by the
   * container, but dynamically injected.
   *
   * @param bool $boolean
   *   FIX - insert comment here.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setSynthetic($boolean) {
    $this->synthetic = (bool) $boolean;

    return $this;
  }

  /**
   * FIX - insert comment here.
   *
   * Whether this definition is synthetic, that is not constructed by the
   * container, but dynamically injected.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isSynthetic() {
    return $this->synthetic;
  }

  /**
   * FIX - insert comment here.
   *
   * Whether this definition is abstract, that means it merely serves as a
   * template for other definitions.
   *
   * @param bool $boolean
   *   FIX - insert comment here.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setAbstract($boolean) {
    $this->abstract = (bool) $boolean;

    return $this;
  }

  /**
   * FIX - insert comment here.
   *
   * Whether this definition is abstract, that means it merely serves as a
   * template for other definitions.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isAbstract() {
    return $this->abstract;
  }

  /**
   * Sets a configurator to call after the service is fully initialized.
   *
   * @param callable $callable
   *   A PHP callable.
   *
   * @return \Definition
   *   The current instance.
   */
  public function setConfigurator(callable $callable) {
    $this->configurator = $callable;

    return $this;
  }

  /**
   * Gets the configurator to call after the service is fully initialized.
   *
   * @return callable|null
   *   The PHP callable to call.
   */
  public function getConfigurator() {
    return $this->configurator;
  }

}
