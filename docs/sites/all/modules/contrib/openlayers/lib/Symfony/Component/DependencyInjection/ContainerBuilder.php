<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

use OpenlayersSymfony\Component\DependencyInjection\Compiler\Compiler;
use OpenlayersSymfony\Component\DependencyInjection\Compiler\CompilerPassInterface;
use OpenlayersSymfony\Component\DependencyInjection\Compiler\PassConfig;
use OpenlayersSymfony\Component\DependencyInjection\Exception\BadMethodCallException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InactiveScopeException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\LogicException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;
use OpenlayersSymfony\Component\DependencyInjection\Extension\ExtensionInterface;
use OpenlayersSymfony\Component\Config\Resource\FileResource;
use OpenlayersSymfony\Component\Config\Resource\ResourceInterface;
use OpenlayersSymfony\Component\DependencyInjection\LazyProxy\Instantiator\InstantiatorInterface;
use OpenlayersSymfony\Component\DependencyInjection\LazyProxy\Instantiator\RealServiceInstantiator;
use OpenlayersSymfony\Component\ExpressionLanguage\Expression;
use OpenlayersSymfony\Component\ExpressionLanguage\ExpressionFunctionProviderInterface;

/**
 * FIX - insert comment here.
 *
 * ContainerBuilder is a DI container that provides an API to easily describe
 * services.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class ContainerBuilder extends Container implements TaggedContainerInterface {

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $extensions = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $extensionsByNs = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $definitions = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $obsoleteDefinitions = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $aliasDefinitions = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $resources = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $extensionConfigs = array();

  /**
   * FIX - insert comment here.
   *
   * @var \OpenlayersSymfony\Component\DependencyInjection\Compiler\Compiler
   */
  private $compiler;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $trackResources = TRUE;

  /**
   * FIX - insert comment here.
   *
   * @var \OpenlayersSymfony\Component\DependencyInjection\LazyProxy\Instantiator\InstantiatorInterface|null
   */
  private $proxyInstantiator;

  /**
   * FIX - insert comment here.
   *
   * @var string|null
   */
  private $expressionLanguage;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $expressionLanguageProviders = array();

  /**
   * Sets the track resources flag.
   *
   * If you are not using the loaders and therefore don't want
   * to depend on the Config component, set this flag to false.
   *
   * @param bool $track
   *   True if you want to track resources, false otherwise.
   */
  public function setResourceTracking($track) {
    $this->trackResources = (bool) $track;
  }

  /**
   * Checks if resources are tracked.
   *
   * @return bool
   *   True if resources are tracked, false otherwise.
   */
  public function isTrackingResources() {
    return $this->trackResources;
  }

  /**
   * Sets the instantiator to be used when fetching proxies.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\LazyProxy\Instantiator\InstantiatorInterface $proxyInstantiator
   *   FIX - insert comment here.
   */
  public function setProxyInstantiator(InstantiatorInterface $proxyInstantiator) {
    $this->proxyInstantiator = $proxyInstantiator;
  }

  /**
   * Registers an extension.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\Extension\ExtensionInterface $extension
   *   An extension instance.
   *
   * @api
   */
  public function registerExtension(ExtensionInterface $extension) {
    $this->extensions[$extension->getAlias()] = $extension;

    if (FALSE !== $extension->getNamespace()) {
      $this->extensionsByNs[$extension->getNamespace()] = $extension;
    }
  }

  /**
   * Returns an extension by alias or namespace.
   *
   * @param string $name
   *   An alias or a namespace.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\Extension\ExtensionInterface
   *   An extension instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\LogicException
   *   If the extension is not registered.
   */
  public function getExtension($name) {
    if (isset($this->extensions[$name])) {
      return $this->extensions[$name];
    }

    if (isset($this->extensionsByNs[$name])) {
      return $this->extensionsByNs[$name];
    }

    throw new LogicException(sprintf('Container extension "%s" is not registered', $name));
  }

  /**
   * Returns all registered extensions.
   *
   * @return array
   *   An array of ExtensionInterface.
   */
  public function getExtensions() {
    return $this->extensions;
  }

  /**
   * Checks if we have an extension.
   *
   * @param string $name
   *   The name of the extension.
   *
   * @return bool
   *   If the extension exists.
   */
  public function hasExtension($name) {
    return isset($this->extensions[$name]) || isset($this->extensionsByNs[$name]);
  }

  /**
   * Returns an array of resources loaded to build this configuration.
   *
   * @return \OpenlayersSymfony\Component\Config\Resource\ResourceInterface[]
   *   An array of resources.
   *
   * @api
   */
  public function getResources() {
    return array_unique($this->resources);
  }

  /**
   * Adds a resource for this configuration.
   *
   * @param \OpenlayersSymfony\Component\Config\Resource\ResourceInterface $resource
   *   A resource instance.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder
   *   The current instance.
   *
   * @api
   */
  public function addResource(ResourceInterface $resource) {
    if (!$this->trackResources) {
      return $this;
    }

    $this->resources[] = $resource;

    return $this;
  }

  /**
   * Sets the resources for this configuration.
   *
   * @param \OpenlayersSymfony\Component\Config\Resource\ResourceInterface[] $resources
   *   An array of resources.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder
   *   The current instance.
   */
  public function setResources(array $resources) {
    if (!$this->trackResources) {
      return $this;
    }

    $this->resources = $resources;

    return $this;
  }

  /**
   * Adds the object class hierarchy as resources.
   *
   * @param object $object
   *   An object instance.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder
   *   The current instance.
   */
  public function addObjectResource($object) {
    if ($this->trackResources) {
      $this->addClassResource(new \ReflectionClass($object));
    }

    return $this;
  }

  /**
   * Adds the given class hierarchy as resources.
   *
   * @param \ReflectionClass $class
   *   FIX - insert comment here.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder
   *   The current instance.
   */
  public function addClassResource(\ReflectionClass $class) {
    if (!$this->trackResources) {
      return $this;
    }

    do {
      $this->addResource(new FileResource($class->getFileName()));
    } while ($class = $class->getParentClass());

    return $this;
  }

  /**
   * Loads the configuration for an extension.
   *
   * @param string $extension
   *   The extension alias or namespace.
   * @param array $values
   *   An array of values that customizes the extension.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder
   *   The current instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\BadMethodCallException
   *   When this ContainerBuilder is frozen.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\LogicException
   *   If the container is frozen.
   */
  public function loadFromExtension($extension, array $values = array()) {
    if ($this->isFrozen()) {
      throw new BadMethodCallException('Cannot load from an extension on a frozen container.');
    }

    $namespace = $this->getExtension($extension)->getAlias();

    $this->extensionConfigs[$namespace][] = $values;

    return $this;
  }

  /**
   * Adds a compiler pass.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\Compiler\CompilerPassInterface $pass
   *   A compiler pass.
   * @param string $type
   *   The type of compiler pass.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder
   *   The current instance.
   */
  public function addCompilerPass(CompilerPassInterface $pass, $type = PassConfig::TYPE_BEFORE_OPTIMIZATION) {
    $this->getCompiler()->addPass($pass, $type);

    $this->addObjectResource($pass);

    return $this;
  }

  /**
   * Returns the compiler pass config which can then be modified.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\Compiler\PassConfig
   *   The compiler pass config.
   */
  public function getCompilerPassConfig() {
    return $this->getCompiler()->getPassConfig();
  }

  /**
   * Returns the compiler.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\Compiler\Compiler
   *   The compiler.
   */
  public function getCompiler() {
    if (NULL === $this->compiler) {
      $this->compiler = new Compiler();
    }

    return $this->compiler;
  }

  /**
   * Returns all Scopes.
   *
   * @return array
   *   An array of scopes
   */
  public function getScopes() {
    return $this->scopes;
  }

  /**
   * Returns all Scope children.
   *
   * @return array
   *   An array of scope children.
   */
  public function getScopeChildren() {
    return $this->scopeChildren;
  }

  /**
   * Sets a service.
   *
   * @param string $id
   *   The service identifier.
   * @param object $service
   *   The service instance.
   * @param string $scope
   *   The scope.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\BadMethodCallException
   *   When this ContainerBuilder is frozen.
   */
  public function set($id, $service, $scope = self::SCOPE_CONTAINER) {
    $id = strtolower($id);

    if ($this->isFrozen()) {
      // Setting a synthetic service on a frozen container is alright.
      if (
            (!isset($this->definitions[$id]) && !isset($this->obsoleteDefinitions[$id]))
                ||
            (isset($this->definitions[$id]) && !$this->definitions[$id]->isSynthetic())
                ||
            (isset($this->obsoleteDefinitions[$id]) && !$this->obsoleteDefinitions[$id]->isSynthetic())
        ) {
        throw new BadMethodCallException(sprintf('Setting service "%s" on a frozen container is not allowed.', $id));
      }
    }

    if (isset($this->definitions[$id])) {
      $this->obsoleteDefinitions[$id] = $this->definitions[$id];
    }

    unset($this->definitions[$id], $this->aliasDefinitions[$id]);

    parent::set($id, $service, $scope);

    if (isset($this->obsoleteDefinitions[$id]) && $this->obsoleteDefinitions[$id]->isSynchronized(FALSE)) {
      $this->synchronize($id);
    }
  }

  /**
   * Removes a service definition.
   *
   * @param string $id
   *   The service.
   */
  public function removeDefinition($id) {
    unset($this->definitions[strtolower($id)]);
  }

  /**
   * Returns true if the given service is defined.
   *
   * @param string $id
   *   The service identifier.
   *
   * @return bool
   *   true if the service is defined, false otherwise.
   */
  public function has($id) {
    $id = strtolower($id);

    return isset($this->definitions[$id]) || isset($this->aliasDefinitions[$id]) || parent::has($id);
  }

  /**
   * Gets a service.
   *
   * @param string $id
   *   The service identifier.
   * @param int $invalidBehavior
   *   The behavior when the service does not exist.
   *
   * @return object|null
   *   The associated service.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When no definitions are available.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InactiveScopeException
   *   When the current scope is not active.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\LogicException
   *   When a circular dependency is detected.
   * @throws \Exception
   *   FIX - insert comment here.
   */
  public function get($id, $invalidBehavior = ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE) {
    $id = strtolower($id);

    if ($service = parent::get($id, ContainerInterface::NULL_ON_INVALID_REFERENCE)) {
      return $service;
    }

    if (!array_key_exists($id, $this->definitions) && isset($this->aliasDefinitions[$id])) {
      return $this->get($this->aliasDefinitions[$id]);
    }

    try {
      $definition = $this->getDefinition($id);
    }
    catch (InvalidArgumentException $e) {
      if (ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE !== $invalidBehavior) {
        return;
      }

      throw $e;
    }

    $this->loading[$id] = TRUE;

    try {
      $service = $this->createService($definition, $id);
    }
    catch (\Exception $e) {
      unset($this->loading[$id]);

      if ($e instanceof InactiveScopeException && self::EXCEPTION_ON_INVALID_REFERENCE !== $invalidBehavior) {
        return;
      }

      throw $e;
    }

    unset($this->loading[$id]);

    return $service;
  }

  /**
   * Merges a ContainerBuilder with the current ContainerBuilder configuration.
   *
   * Service definitions overrides the current defined ones.
   *
   * But for parameters, they are overridden by the current ones. It allows
   * the parameters passed to the container constructor to have precedence
   * over the loaded ones.
   *
   * $container = new ContainerBuilder(array('foo' => 'bar'));
   * $loader = new LoaderXXX($container);
   * $loader->load('resource_name');
   * $container->register('foo', new stdClass());
   *
   * In the above example, even if the loaded resource defines a foo
   * parameter, the value will still be 'bar' as defined in the ContainerBuilder
   * constructor.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   The ContainerBuilder instance to merge.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\BadMethodCallException
   *   When this ContainerBuilder is frozen.
   */
  public function merge(ContainerBuilder $container) {
    if ($this->isFrozen()) {
      throw new BadMethodCallException('Cannot merge on a frozen container.');
    }

    $this->addDefinitions($container->getDefinitions());
    $this->addAliases($container->getAliases());
    $this->getParameterBag()->add($container->getParameterBag()->all());

    if ($this->trackResources) {
      foreach ($container->getResources() as $resource) {
        $this->addResource($resource);
      }
    }

    foreach ($this->extensions as $name => $extension) {
      if (!isset($this->extensionConfigs[$name])) {
        $this->extensionConfigs[$name] = array();
      }

      $this->extensionConfigs[$name] = array_merge($this->extensionConfigs[$name], $container->getExtensionConfig($name));
    }
  }

  /**
   * Returns the configuration array for the given extension.
   *
   * @param string $name
   *   The name of the extension.
   *
   * @return array
   *   An array of configuration.
   */
  public function getExtensionConfig($name) {
    if (!isset($this->extensionConfigs[$name])) {
      $this->extensionConfigs[$name] = array();
    }

    return $this->extensionConfigs[$name];
  }

  /**
   * Prepends a config array to the configs of the given extension.
   *
   * @param string $name
   *   The name of the extension.
   * @param array $config
   *   The config to set.
   */
  public function prependExtensionConfig($name, array $config) {
    if (!isset($this->extensionConfigs[$name])) {
      $this->extensionConfigs[$name] = array();
    }

    array_unshift($this->extensionConfigs[$name], $config);
  }

  /**
   * Compiles the container.
   *
   * This method passes the container to compiler
   * passes whose job is to manipulate and optimize
   * the container.
   *
   * The main compiler passes roughly do four things:
   *
   *  * The extension configurations are merged;
   *  * Parameter values are resolved;
   *  * The parameter bag is frozen;
   *  * Extension loading is disabled.
   */
  public function compile() {
    $compiler = $this->getCompiler();

    if ($this->trackResources) {
      foreach ($compiler->getPassConfig()->getPasses() as $pass) {
        $this->addObjectResource($pass);
      }
    }

    $compiler->compile($this);

    if ($this->trackResources) {
      foreach ($this->definitions as $definition) {
        if ($definition->isLazy() && ($class = $definition->getClass()) && class_exists($class)) {
          $this->addClassResource(new \ReflectionClass($class));
        }
      }
    }

    $this->extensionConfigs = array();

    parent::compile();
  }

  /**
   * Gets all service ids.
   *
   * @return array
   *   An array of all defined service ids.
   */
  public function getServiceIds() {
    return array_unique(array_merge(array_keys($this->getDefinitions()), array_keys($this->aliasDefinitions), parent::getServiceIds()));
  }

  /**
   * Adds the service aliases.
   *
   * @param array $aliases
   *   An array of aliases.
   */
  public function addAliases(array $aliases) {
    foreach ($aliases as $alias => $id) {
      $this->setAlias($alias, $id);
    }
  }

  /**
   * Sets the service aliases.
   *
   * @param array $aliases
   *   An array of aliases.
   */
  public function setAliases(array $aliases) {
    $this->aliasDefinitions = array();
    $this->addAliases($aliases);
  }

  /**
   * Sets an alias for an existing service.
   *
   * @param string $alias
   *   The alias to create.
   * @param string|Alias $id
   *   The service to alias.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the id is not a string or an Alias.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the alias is for itself.
   */
  public function setAlias($alias, $id) {
    $alias = strtolower($alias);

    if (is_string($id)) {
      $id = new Alias($id);
    }
    elseif (!$id instanceof Alias) {
      throw new InvalidArgumentException('$id must be a string, or an Alias object.');
    }

    if ($alias === (string) $id) {
      throw new InvalidArgumentException(sprintf('An alias can not reference itself, got a circular reference on "%s".', $alias));
    }

    unset($this->definitions[$alias]);

    $this->aliasDefinitions[$alias] = $id;
  }

  /**
   * Removes an alias.
   *
   * @param string $alias
   *   The alias to remove.
   *
   * @api
   */
  public function removeAlias($alias) {
    unset($this->aliasDefinitions[strtolower($alias)]);
  }

  /**
   * Returns true if an alias exists under the given identifier.
   *
   * @param string $id
   *   The service identifier.
   *
   * @return bool
   *   True if the alias exists, false otherwise.
   *
   * @api
   */
  public function hasAlias($id) {
    return isset($this->aliasDefinitions[strtolower($id)]);
  }

  /**
   * Gets all defined aliases.
   *
   * @return array
   *   An array of aliases.
   */
  public function getAliases() {
    return $this->aliasDefinitions;
  }

  /**
   * Gets an alias.
   *
   * @param string $id
   *   The service identifier.
   *
   * @return Alias
   *   An Alias instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the alias does not exist.
   */
  public function getAlias($id) {
    $id = strtolower($id);

    if (!isset($this->aliasDefinitions[$id])) {
      throw new InvalidArgumentException(sprintf('The service alias "%s" does not exist.', $id));
    }

    return $this->aliasDefinitions[$id];
  }

  /**
   * Registers a service definition.
   *
   * This methods allows for simple registration of service definition
   * with a fluid interface.
   *
   * @param string $id
   *   The service identifier.
   * @param string $class
   *   The service class.
   *
   * @return Definition
   *   A Definition instance.
   *
   * @api
   */
  public function register($id, $class = NULL) {
    return $this->setDefinition($id, new Definition($class));
  }

  /**
   * Adds the service definitions.
   *
   * @param Definition[] $definitions
   *   An array of service definitions.
   */
  public function addDefinitions(array $definitions) {
    foreach ($definitions as $id => $definition) {
      $this->setDefinition($id, $definition);
    }
  }

  /**
   * Sets the service definitions.
   *
   * @param Definition[] $definitions
   *   An array of service definitions.
   */
  public function setDefinitions(array $definitions) {
    $this->definitions = array();
    $this->addDefinitions($definitions);
  }

  /**
   * Gets all service definitions.
   *
   * @return Definition[]
   *   An array of Definition instances'
   */
  public function getDefinitions() {
    return $this->definitions;
  }

  /**
   * Sets a service definition.
   *
   * @param string $id
   *   The service identifier.
   * @param Definition $definition
   *   A Definition instance.
   *
   * @return Definition
   *   The service definition.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\BadMethodCallException
   *   When this ContainerBuilder is frozen.
   */
  public function setDefinition($id, Definition $definition) {
    if ($this->isFrozen()) {
      throw new BadMethodCallException('Adding definition to a frozen container is not allowed');
    }

    $id = strtolower($id);

    unset($this->aliasDefinitions[$id]);

    return $this->definitions[$id] = $definition;
  }

  /**
   * Returns true if a service definition exists under the given identifier.
   *
   * @param string $id
   *   The service identifier.
   *
   * @return bool
   *   True if the service definition exists, false otherwise.
   */
  public function hasDefinition($id) {
    return array_key_exists(strtolower($id), $this->definitions);
  }

  /**
   * Gets a service definition.
   *
   * @param string $id
   *   The service identifier.
   *
   * @return Definition
   *   A Definition instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the service definition does not exist.
   */
  public function getDefinition($id) {
    $id = strtolower($id);

    if (!array_key_exists($id, $this->definitions)) {
      throw new InvalidArgumentException(sprintf('The service definition "%s" does not exist.', $id));
    }

    return $this->definitions[$id];
  }

  /**
   * Gets a service definition by id or alias.
   *
   * The method "unaliases" recursively to return a Definition instance.
   *
   * @param string $id
   *   The service identifier or alias.
   *
   * @return Definition
   *   A Definition instance.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the service definition does not exist.
   */
  public function findDefinition($id) {
    $id = strtolower($id);

    while (isset($this->aliasDefinitions[$id])) {
      $id = (string) $this->aliasDefinitions[$id];
    }

    return $this->getDefinition($id);
  }

  /**
   * Creates a service for a service definition.
   *
   * @param Definition $definition
   *   A service definition instance.
   * @param string $id
   *   The service identifier.
   * @param bool $tryProxy
   *   Whether to try proxying the service with a lazy proxy.
   *
   * @return object
   *   The service described by the service definition.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When the scope is inactive.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When the factory definition is incomplete.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When the service is a synthetic service.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When configure callable is not callable.
   *
   * @internal this method is public because of PHP 5.3 limitations, do not use it explicitly in your code
   */
  public function createService(Definition $definition, $id, $tryProxy = TRUE) {
    if ($definition->isSynthetic()) {
      throw new RuntimeException(sprintf('You have requested a synthetic service ("%s"). The DIC does not know how to construct this service.', $id));
    }

    if ($tryProxy && $definition->isLazy()) {
      $container = $this;

      $proxy = $this
        ->getProxyInstantiator()
        ->instantiateProxy(
          $container,
          $definition,
          $id, function () use ($definition, $id, $container) {
            return $container->createService($definition, $id, FALSE);
          }
        );
      $this->shareService($definition, $proxy, $id);

      return $proxy;
    }

    $parameterBag = $this->getParameterBag();

    if (NULL !== $definition->getFile()) {
      require_once $parameterBag->resolveValue($definition->getFile());
    }

    $arguments = $this->resolveServices($parameterBag->unescapeValue($parameterBag->resolveValue($definition->getArguments())));

    if (NULL !== $factory = $definition->getFactory()) {
      if (is_array($factory)) {
        $factory = array(
          $this->resolveServices($parameterBag->resolveValue($factory[0])),
          $factory[1],
        );
      }
      elseif (!is_string($factory)) {
        throw new RuntimeException(sprintf('Cannot create service "%s" because of invalid factory', $id));
      }

      $service = call_user_func_array($factory, $arguments);
    }
    elseif (NULL !== $definition->getFactoryMethod(FALSE)) {
      if (NULL !== $definition->getFactoryClass(FALSE)) {
        $factory = $parameterBag->resolveValue($definition->getFactoryClass(FALSE));
      }
      elseif (NULL !== $definition->getFactoryService(FALSE)) {
        $factory = $this->get($parameterBag->resolveValue($definition->getFactoryService(FALSE)));
      }
      else {
        throw new RuntimeException(sprintf('Cannot create service "%s" from factory method without a factory service or factory class.', $id));
      }

      $service = call_user_func_array(
        array($factory, $definition->getFactoryMethod(FALSE)),
        $arguments
      );
    }
    else {
      $r = new \ReflectionClass($parameterBag->resolveValue($definition->getClass()));

      $service = NULL === $r->getConstructor() ? $r->newInstance() : $r->newInstanceArgs($arguments);
    }

    if ($tryProxy || !$definition->isLazy()) {
      // Share only if proxying failed, or if not a proxy.
      $this->shareService($definition, $service, $id);
    }

    foreach ($definition->getMethodCalls() as $call) {
      $this->callMethod($service, $call);
    }

    $properties = $this->resolveServices($parameterBag->resolveValue($definition->getProperties()));
    foreach ($properties as $name => $value) {
      $service->$name = $value;
    }

    if ($callable = $definition->getConfigurator()) {
      if (is_array($callable)) {
        $callable[0] = $callable[0] instanceof Reference ? $this->get((string) $callable[0]) : $parameterBag->resolveValue($callable[0]);
      }

      if (!is_callable($callable)) {
        throw new InvalidArgumentException(sprintf('The configure callable for class "%s" is not a callable.', get_class($service)));
      }

      call_user_func($callable, $service);
    }

    return $service;
  }

  /**
   * FIX - insert comment here.
   *
   * Replaces service references by the real service instance and evaluates
   * expressions.
   *
   * @param mixed $value
   *   A value.
   *
   * @return mixed
   *   The same value with all service references replaced by
   *   the real service instances and all expressions evaluated.
   */
  public function resolveServices($value) {
    if (is_array($value)) {
      $value = array_map(array($this, 'resolveServices'), $value);
    }
    elseif ($value instanceof Reference) {
      $value = $this->get((string) $value, $value->getInvalidBehavior());
    }
    elseif ($value instanceof Definition) {
      $value = $this->createService($value, NULL);
    }
    elseif ($value instanceof Expression) {
      $value = $this->getExpressionLanguage()->evaluate($value, array('container' => $this));
    }

    return $value;
  }

  /**
   * Returns service ids for a given tag.
   *
   * Example:
   *
   * $container->register('foo')->addTag('my.tag', array('hello' => 'world'));
   *
   * $serviceIds = $container->findTaggedServiceIds('my.tag');
   * foreach ($serviceIds as $serviceId => $tags) {
   *     foreach ($tags as $tag) {
   *         echo $tag['hello'];
   *     }
   * }
   *
   * @param string $name
   *   The tag name.
   *
   * @return array
   *   An array of tags with the tagged service as key, holding a list
   *   of attribute arrays.
   */
  public function findTaggedServiceIds($name) {
    $tags = array();
    foreach ($this->getDefinitions() as $id => $definition) {
      if ($definition->hasTag($name)) {
        $tags[$id] = $definition->getTag($name);
      }
    }

    return $tags;
  }

  /**
   * Returns all tags the defined services use.
   *
   * @return array
   *   An array of tags.
   */
  public function findTags() {
    $tags = array();
    foreach ($this->getDefinitions() as $id => $definition) {
      $tags = array_merge(array_keys($definition->getTags()), $tags);
    }

    return array_unique($tags);
  }

  /**
   * FIX - insert comment here.
   */
  public function addExpressionLanguageProvider(ExpressionFunctionProviderInterface $provider) {
    $this->expressionLanguageProviders[] = $provider;
  }

  /**
   * FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   */
  public function getExpressionLanguageProviders() {
    return $this->expressionLanguageProviders;
  }

  /**
   * Returns the Service Conditionals.
   *
   * @param mixed $value
   *   An array of conditionals to return.
   *
   * @return array
   *   An array of Service conditionals.
   */
  public static function getServiceConditionals($value) {
    $services = array();

    if (is_array($value)) {
      foreach ($value as $v) {
        $services = array_unique(array_merge($services, self::getServiceConditionals($v)));
      }
    }
    elseif ($value instanceof Reference && $value->getInvalidBehavior() === ContainerInterface::IGNORE_ON_INVALID_REFERENCE) {
      $services[] = (string) $value;
    }

    return $services;
  }

  /**
   * Retrieves the currently set proxy instantiator or instantiates one.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\LazyProxy\Instantiator\InstantiatorInterface
   *   FIX - insert comment here.
   */
  private function getProxyInstantiator() {
    if (!$this->proxyInstantiator) {
      $this->proxyInstantiator = new RealServiceInstantiator();
    }

    return $this->proxyInstantiator;
  }

  /**
   * Synchronizes a service change.
   *
   * This method updates all services that depend on the given
   * service by calling all methods referencing it.
   *
   * @param string $id
   *   A service id.
   */
  private function synchronize($id) {
    if ('request' !== $id) {
      trigger_error('The ' . __METHOD__ . ' method is deprecated in version 2.7 and will be removed in version 3.0.', E_USER_DEPRECATED);
    }

    foreach ($this->definitions as $definitionId => $definition) {
      // Only check initialized services.
      if (!$this->initialized($definitionId)) {
        continue;
      }

      foreach ($definition->getMethodCalls() as $call) {
        foreach ($call[1] as $argument) {
          if ($argument instanceof Reference && $id == (string) $argument) {
            $this->callMethod($this->get($definitionId), $call);
          }
        }
      }
    }
  }

  /**
   * FIX - insert comment here.
   */
  private function callMethod($service, $call) {
    $services = self::getServiceConditionals($call[1]);

    foreach ($services as $s) {
      if (!$this->has($s)) {
        return;
      }
    }

    call_user_func_array(array($service, $call[0]), $this->resolveServices($this->getParameterBag()->resolveValue($call[1])));
  }

  /**
   * Shares a given service in the container.
   *
   * @param \Definition $definition
   *   FIX - insert comment here.
   * @param mixed $service
   *   FIX - insert comment here.
   * @param string $id
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InactiveScopeException
   *   FIX - insert comment here.
   */
  private function shareService(Definition $definition, $service, $id) {
    if (self::SCOPE_PROTOTYPE !== $scope = $definition->getScope()) {
      if (self::SCOPE_CONTAINER !== $scope && !isset($this->scopedServices[$scope])) {
        throw new InactiveScopeException($id, $scope);
      }

      $this->services[$lowerId = strtolower($id)] = $service;

      if (self::SCOPE_CONTAINER !== $scope) {
        $this->scopedServices[$scope][$lowerId] = $service;
      }
    }
  }

  /**
   * FIX - insert comment here.
   */
  private function getExpressionLanguage() {
    if (NULL === $this->expressionLanguage) {
      if (!class_exists('OpenlayersSymfony\Component\ExpressionLanguage\ExpressionLanguage')) {
        throw new RuntimeException('Unable to use expressions as the Symfony ExpressionLanguage component is not installed.');
      }
      $this->expressionLanguage = new ExpressionLanguage(NULL, $this->expressionLanguageProviders);
    }

    return $this->expressionLanguage;
  }

}
