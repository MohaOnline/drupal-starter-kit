<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

use OpenlayersSymfony\Component\DependencyInjection\Exception\InactiveScopeException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceNotFoundException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException;
use OpenlayersSymfony\Component\DependencyInjection\ParameterBag\ParameterBagInterface;
use OpenlayersSymfony\Component\DependencyInjection\ParameterBag\ParameterBag;
use OpenlayersSymfony\Component\DependencyInjection\ParameterBag\FrozenParameterBag;

/**
 * Container is a dependency injection container.
 *
 * It gives access to object instances (services).
 *
 * Services and parameters are simple key/pair stores.
 *
 * Parameter and service keys are case insensitive.
 *
 * A service id can contain lowercased letters, digits, underscores, and dots.
 * Underscores are used to separate words, and dots to group services
 * under namespaces:
 *
 * <ul>
 *   <li>request</li>
 *   <li>mysql_session_storage</li>
 *   <li>symfony.mysql_session_storage</li>
 * </ul>
 *
 * A service can also be defined by creating a method named
 * getXXXService(), where XXX is the camelized version of the id:
 *
 * <ul>
 *   <li>request -> getRequestService()</li>
 *   <li>mysql_session_storage -> getMysqlSessionStorageService()</li>
 *   <li>symfony.mysql_session_storage ->
 *     getSymfony_MysqlSessionStorageService()</li>
 * </ul>
 *
 * The container can have three possible behaviors when a service does not
 * exist:
 *
 *  * EXCEPTION_ON_INVALID_REFERENCE: Throws an exception (the default)
 *  * NULL_ON_INVALID_REFERENCE:      Returns null
 *  * IGNORE_ON_INVALID_REFERENCE:    Ignores the wrapping command asking for
 *                                    the reference (for instance, ignore a
 *                                    setter if the service does not exist)
 *
 * @author Fabien Potencier <fabien@symfony.com>
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class Container implements IntrospectableContainerInterface {

  /**
   * FIX - insert comment here.
   *
   * @var \OpenlayersSymfony\Component\DependencyInjection\ParameterBag\ParameterBagInterface
   */
  protected $parameterBag;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $services = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $methodMap = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $aliases = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $scopes = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $scopeChildren = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $scopedServices = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $scopeStacks = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  protected $loading = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $underscoreMap = array('_' => '', '.' => '_', '\\' => '_');

  /**
   * Constructor.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ParameterBag\ParameterBagInterface $parameterBag
   *   A ParameterBagInterface instance.
   */
  public function __construct(ParameterBagInterface $parameterBag = NULL) {
    $this->parameterBag = $parameterBag ?: new ParameterBag();
  }

  /**
   * Compiles the container.
   *
   * This method does two things:
   *
   *  * Parameter values are resolved;
   *  * The parameter bag is frozen.
   *
   * @api
   */
  public function compile() {
    $this->parameterBag->resolve();

    $this->parameterBag = new FrozenParameterBag($this->parameterBag->all());
  }

  /**
   * Returns true if the container parameter bag are frozen.
   *
   * @return bool
   *   true if the container parameter bag are frozen, false otherwise.
   */
  public function isFrozen() {
    return $this->parameterBag instanceof FrozenParameterBag;
  }

  /**
   * Gets the service container parameter bag.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\ParameterBag\ParameterBagInterface
   *   A ParameterBagInterface instance.
   */
  public function getParameterBag() {
    return $this->parameterBag;
  }

  /**
   * Gets a parameter.
   *
   * @param string $name
   *   The parameter name.
   *
   * @return mixed
   *   The parameter value
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the parameter is not defined.
   */
  public function getParameter($name) {
    return $this->parameterBag->get($name);
  }

  /**
   * Checks if a parameter exists.
   *
   * @param string $name
   *   The parameter name.
   *
   * @return bool
   *   The presence of parameter in container.
   */
  public function hasParameter($name) {
    return $this->parameterBag->has($name);
  }

  /**
   * Sets a parameter.
   *
   * @param string $name
   *   The parameter name.
   * @param mixed $value
   *   The parameter value.
   */
  public function setParameter($name, $value) {
    $this->parameterBag->set($name, $value);
  }

  /**
   * Sets a service.
   *
   * Setting a service to null resets the service: has() returns false and get()
   * behaves in the same way as if the service was never created.
   *
   * @param string $id
   *   The service identifier.
   * @param object $service
   *   The service instance.
   * @param string $scope
   *   The scope of the service.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException@throws
   *   When trying to set a service in an inactive scope.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When trying to set a service in the prototype scope.
   */
  public function set($id, $service, $scope = self::SCOPE_CONTAINER) {
    if (self::SCOPE_PROTOTYPE === $scope) {
      throw new InvalidArgumentException(sprintf('You cannot set service "%s" of scope "prototype".', $id));
    }

    $id = strtolower($id);

    if ('service_container' === $id) {
      // BC: 'service_container' is no longer a self-reference but always
      // $this, so ignore this call.
      // FIX Throw InvalidArgumentException in next major release.
      return;
    }
    if (self::SCOPE_CONTAINER !== $scope) {
      if (!isset($this->scopedServices[$scope])) {
        throw new RuntimeException(sprintf('You cannot set service "%s" of inactive scope.', $id));
      }

      $this->scopedServices[$scope][$id] = $service;
    }

    $this->services[$id] = $service;

    if (method_exists($this, $method = 'synchronize' . strtr($id, $this->underscoreMap) . 'Service')) {
      $this->$method();
    }

    if (NULL === $service) {
      if (self::SCOPE_CONTAINER !== $scope) {
        unset($this->scopedServices[$scope][$id]);
      }

      unset($this->services[$id]);
    }
  }

  /**
   * Returns true if the given service is defined.
   *
   * @param string $id
   *   The service identifier.
   *
   * @return bool
   *   true if the service is defined, false otherwise
   */
  public function has($id) {
    for ($i = 2;;) {
      if ('service_container' === $id
            || isset($this->aliases[$id])
            || isset($this->services[$id])
            || array_key_exists($id, $this->services)
        ) {
        return TRUE;
      }
      if (--$i && $id !== $lcId = strtolower($id)) {
        $id = $lcId;
      }
      else {
        return method_exists($this, 'get' . strtr($id, $this->underscoreMap) . 'Service');
      }
    }
  }

  /**
   * Gets a service.
   *
   * If a service is defined both through a set() method and
   * with a get{$id}Service() method, the former has always precedence.
   *
   * @param string $id
   *   The service identifier.
   * @param int $invalidBehavior
   *   The behavior when the service does not exist.
   *
   * @return object|null
   *   The associated service.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException
   *   When a circular reference is detected.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceNotFoundException
   *   When the service is not defined.
   * @throws \Exception
   *   If an exception has been thrown when the service has been resolved.
   *
   * @see Reference
   */
  public function get($id, $invalidBehavior = self::EXCEPTION_ON_INVALID_REFERENCE) {
    // Attempt to retrieve the service by checking first aliases then
    // available services. Service IDs are case insensitive, however since
    // this method can be called thousands of times during a request, avoid
    // calling strtolower() unless necessary.
    for ($i = 2;;) {
      if ('service_container' === $id) {
        return $this;
      }
      if (isset($this->aliases[$id])) {
        $id = $this->aliases[$id];
      }
      // Re-use shared service instance if it exists.
      if (isset($this->services[$id]) || array_key_exists($id, $this->services)) {
        return $this->services[$id];
      }

      if (isset($this->loading[$id])) {
        throw new ServiceCircularReferenceException($id, array_keys($this->loading));
      }

      if (isset($this->methodMap[$id])) {
        $method = $this->methodMap[$id];
      }
      elseif (--$i && $id !== $lcId = strtolower($id)) {
        $id = $lcId;
        continue;
      }
      elseif (method_exists($this, $method = 'get' . strtr($id, $this->underscoreMap) . 'Service')) {
        // $method is set to the right value, proceed.
      }
      else {
        if (self::EXCEPTION_ON_INVALID_REFERENCE === $invalidBehavior) {
          if (!$id) {
            throw new ServiceNotFoundException($id);
          }

          $alternatives = array();
          foreach ($this->services as $key => $associatedService) {
            $lev = levenshtein($id, $key);
            if ($lev <= strlen($id) / 3 || FALSE !== strpos($key, $id)) {
              $alternatives[] = $key;
            }
          }

          throw new ServiceNotFoundException($id, NULL, NULL, $alternatives);
        }

        return;
      }

      $this->loading[$id] = TRUE;

      try {
        $service = $this->$method();
      }
      catch (\Exception $e) {
        unset($this->loading[$id]);

        if (array_key_exists($id, $this->services)) {
          unset($this->services[$id]);
        }

        if ($e instanceof InactiveScopeException && self::EXCEPTION_ON_INVALID_REFERENCE !== $invalidBehavior) {
          return;
        }

        throw $e;
      }

      unset($this->loading[$id]);

      return $service;
    }
  }

  /**
   * Returns true if the given service has actually been initialized.
   *
   * @param string $id
   *   The service identifier.
   *
   * @return bool
   *   true if service has already been initialized, false otherwise.
   */
  public function initialized($id) {
    $id = strtolower($id);

    if ('service_container' === $id) {
      // BC: 'service_container' was a synthetic service previously.
      // FIX Change to false in next major release.
      return TRUE;
    }

    if (isset($this->aliases[$id])) {
      $id = $this->aliases[$id];
    }

    return isset($this->services[$id]) || array_key_exists($id, $this->services);
  }

  /**
   * Gets all service ids.
   *
   * @return array
   *   An array of all defined service ids.
   */
  public function getServiceIds() {
    $ids = array();
    $r = new \ReflectionClass($this);
    foreach ($r->getMethods() as $method) {
      if (preg_match('/^get(.+)Service$/', $method->name, $match)) {
        $ids[] = self::underscore($match[1]);
      }
    }
    $ids[] = 'service_container';

    return array_unique(array_merge($ids, array_keys($this->services)));
  }

  /**
   * This is called when you enter a scope.
   *
   * @param string $name
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When the parent scope is inactive.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When the scope does not exist.
   */
  public function enterScope($name) {
    if (!isset($this->scopes[$name])) {
      throw new InvalidArgumentException(sprintf('The scope "%s" does not exist.', $name));
    }

    if (self::SCOPE_CONTAINER !== $this->scopes[$name] && !isset($this->scopedServices[$this->scopes[$name]])) {
      throw new RuntimeException(sprintf('The parent scope "%s" must be active when entering this scope.', $this->scopes[$name]));
    }

    // Check if a scope of this name is already active, if so we need to
    // remove all services of this scope, and those of any of its child
    // scopes from the global services map.
    if (isset($this->scopedServices[$name])) {
      $services = array($this->services, $name => $this->scopedServices[$name]);
      unset($this->scopedServices[$name]);

      foreach ($this->scopeChildren[$name] as $child) {
        if (isset($this->scopedServices[$child])) {
          $services[$child] = $this->scopedServices[$child];
          unset($this->scopedServices[$child]);
        }
      }

      // Update global map.
      $this->services = call_user_func_array('array_diff_key', $services);
      array_shift($services);

      // Add stack entry for this scope so we can restore the removed
      // services later.
      if (!isset($this->scopeStacks[$name])) {
        $this->scopeStacks[$name] = new \SplStack();
      }
      $this->scopeStacks[$name]->push($services);
    }

    $this->scopedServices[$name] = array();
  }

  /**
   * FIX - insert comment here.
   *
   * This is called to leave the current scope, and move back to the parent
   * scope.
   *
   * @param string $name
   *   The name of the scope to leave.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   If the scope is not active.
   */
  public function leaveScope($name) {
    if (!isset($this->scopedServices[$name])) {
      throw new InvalidArgumentException(sprintf('The scope "%s" is not active.', $name));
    }

    // Remove all services of this scope, or any of its child scopes from
    // the global service map.
    $services = array($this->services, $this->scopedServices[$name]);
    unset($this->scopedServices[$name]);

    foreach ($this->scopeChildren[$name] as $child) {
      if (isset($this->scopedServices[$child])) {
        $services[] = $this->scopedServices[$child];
        unset($this->scopedServices[$child]);
      }
    }

    // Update global map.
    $this->services = call_user_func_array('array_diff_key', $services);

    // Check if we need to restore services of a previous scope of this type.
    if (isset($this->scopeStacks[$name]) && count($this->scopeStacks[$name]) > 0) {
      $services = $this->scopeStacks[$name]->pop();
      $this->scopedServices += $services;

      if ($this->scopeStacks[$name]->isEmpty()) {
        unset($this->scopeStacks[$name]);
      }

      foreach ($services as $array) {
        foreach ($array as $id => $service) {
          $this->set($id, $service, $name);
        }
      }
    }
  }

  /**
   * Adds a scope to the container.
   *
   * @param ScopeInterface $scope
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   FIX - insert comment here.
   */
  public function addScope(ScopeInterface $scope) {
    $name = $scope->getName();
    $parentScope = $scope->getParentName();

    if (self::SCOPE_CONTAINER === $name || self::SCOPE_PROTOTYPE === $name) {
      throw new InvalidArgumentException(sprintf('The scope "%s" is reserved.', $name));
    }
    if (isset($this->scopes[$name])) {
      throw new InvalidArgumentException(sprintf('A scope with name "%s" already exists.', $name));
    }
    if (self::SCOPE_CONTAINER !== $parentScope && !isset($this->scopes[$parentScope])) {
      throw new InvalidArgumentException(sprintf('The parent scope "%s" does not exist, or is invalid.', $parentScope));
    }

    $this->scopes[$name] = $parentScope;
    $this->scopeChildren[$name] = array();

    // Normalize the child relations.
    while ($parentScope !== self::SCOPE_CONTAINER) {
      $this->scopeChildren[$parentScope][] = $name;
      $parentScope = $this->scopes[$parentScope];
    }
  }

  /**
   * Returns whether this container has a certain scope.
   *
   * @param string $name
   *   The name of the scope.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function hasScope($name) {
    return isset($this->scopes[$name]);
  }

  /**
   * Returns whether this scope is currently active.
   *
   * This does not actually check if the passed scope actually exists.
   *
   * @param string $name
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isScopeActive($name) {
    return isset($this->scopedServices[$name]);
  }

  /**
   * Camelizes a string.
   *
   * @param string $id
   *   A string to camelize.
   *
   * @return string
   *   The camelized string.
   */
  public static function camelize($id) {
    return strtr(ucwords(strtr($id, array('_' => ' ', '.' => '_ ', '\\' => '_ '))), array(' ' => ''));
  }

  /**
   * A string to underscore.
   *
   * @param string $id
   *   The string to underscore.
   *
   * @return string
   *   The underscored string.
   */
  public static function underscore($id) {
    return strtolower(preg_replace(
      array('/([A-Z]+)([A-Z][a-z])/', '/([a-z\d])([A-Z])/'),
      array('\\1_\\2', '\\1_\\2'),
      strtr($id, '_', '.')
    ));
  }

}
