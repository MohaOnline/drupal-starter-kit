<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Dumper;

use OpenlayersSymfony\Component\DependencyInjection\Variable;
use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Container;
use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\Parameter;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException;
use OpenlayersSymfony\Component\DependencyInjection\LazyProxy\PhpDumper\DumperInterface as ProxyDumper;
use OpenlayersSymfony\Component\DependencyInjection\LazyProxy\PhpDumper\NullDumper;
use OpenlayersSymfony\Component\DependencyInjection\ExpressionLanguage;
use OpenlayersSymfony\Component\ExpressionLanguage\Expression;
use OpenlayersSymfony\Component\ExpressionLanguage\ExpressionFunctionProviderInterface;

/**
 * PhpDumper dumps a service container as a PHP class.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class PhpDumper extends Dumper {

  /**
   * FIX - insert comment here.
   *
   * Characters that might appear in the generated variable name as first
   * character.
   *
   * @var string
   */
  const FIRST_CHARS = 'abcdefghijklmnopqrstuvwxyz';

  /**
   * FIX - insert comment here.
   *
   * Characters that might appear in the generated variable name as any but
   * the first character.
   *
   * @var string
   */
  const NON_FIRST_CHARS = 'abcdefghijklmnopqrstuvwxyz0123456789_';

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $inlinedDefinitions;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $definitionVariables;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $referenceVariables;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $variableCount;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $reservedVariables = array('instance', 'class');

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $expressionLanguage;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $targetDirRegex;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $targetDirMaxMatches;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $expressionLanguageProviders = array();

  /**
   * FIX - insert comment here.
   *
   * @var \OpenlayersSymfony\Component\DependencyInjection\LazyProxy\PhpDumper\DumperInterface
   */
  private $proxyDumper;

  /**
   * FIX - insert comment here.
   */
  public function __construct(ContainerBuilder $container) {
    parent::__construct($container);

    $this->inlinedDefinitions = new \SplObjectStorage();
  }

  /**
   * Sets the dumper to be used when dumping proxies in the generated container.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\LazyProxy\PhpDumper\DumperInterface $proxyDumper
   *   FIX - insert comment here.
   */
  public function setProxyDumper(ProxyDumper $proxyDumper) {
    $this->proxyDumper = $proxyDumper;
  }

  /**
   * Dumps the service container as a PHP class.
   *
   * Available options:
   *
   *  * class:      The class name
   *  * base_class: The base class name
   *  * namespace:  The class namespace
   *
   * @param array $options
   *   An array of options.
   *
   * @return string
   *   A PHP class representing of the service container.
   */
  public function dump(array $options = array()) {
    $this->targetDirRegex = NULL;
    $options = array_merge(array(
      'class' => 'ProjectServiceContainer',
      'base_class' => 'Container',
      'namespace' => '',
    ), $options);

    if (!empty($options['file']) && is_dir($dir = dirname($options['file']))) {
      // Build a regexp where the first root dirs are mandatory,
      // but every other sub-dir is optional up to the full path in $dir
      // Mandate at least 2 root dirs and not more that 5 optional dirs.
      $dir = explode(DIRECTORY_SEPARATOR, realpath($dir));
      $i = count($dir);

      if (3 <= $i) {
        $regex = '';
        $lastOptionalDir = $i > 8 ? $i - 5 : 3;
        $this->targetDirMaxMatches = $i - $lastOptionalDir;

        while (--$i >= $lastOptionalDir) {
          $regex = sprintf('(%s%s)?', preg_quote(DIRECTORY_SEPARATOR . $dir[$i], '#'), $regex);
        }

        do {
          $regex = preg_quote(DIRECTORY_SEPARATOR . $dir[$i], '#') . $regex;
        } while (0 < --$i);

        $this->targetDirRegex = '#' . preg_quote($dir[0], '#') . $regex . '#';
      }
    }

    $code = $this->startClass($options['class'], $options['base_class'], $options['namespace']);

    if ($this->container->isFrozen()) {
      $code .= $this->addFrozenConstructor();
      $code .= $this->addFrozenCompile();
    }
    else {
      $code .= $this->addConstructor();
    }

    $code .=
            $this->addServices() .
            $this->addDefaultParametersMethod() .
            $this->endClass() .
            $this->addProxyClasses();
    $this->targetDirRegex = NULL;

    return $code;
  }

  /**
   * Retrieves the currently set proxy dumper or instantiates one.
   *
   * @return OpenlayersSymfony\Component\DependencyInjection\LazyProxy\PhpDumper\DumperInterface
   *   FIX - insert comment here.
   */
  private function getProxyDumper() {
    if (!$this->proxyDumper) {
      $this->proxyDumper = new NullDumper();
    }

    return $this->proxyDumper;
  }

  /**
   * Generates Service local temp variables.
   *
   * @param string $cId
   *   FIX - insert comment here.
   * @param string $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServiceLocalTempVariables($cId, $definition) {
    static $template = "        \$%s = %s;\n";

    $localDefinitions = array_merge(
          array($definition),
          $this->getInlinedDefinitions($definition)
      );

    $calls = $behavior = array();
    foreach ($localDefinitions as $iDefinition) {
      $this->getServiceCallsFromArguments($iDefinition->getArguments(), $calls, $behavior);
      $this->getServiceCallsFromArguments($iDefinition->getMethodCalls(), $calls, $behavior);
      $this->getServiceCallsFromArguments($iDefinition->getProperties(), $calls, $behavior);
      $this->getServiceCallsFromArguments(array($iDefinition->getConfigurator()), $calls, $behavior);
      $this->getServiceCallsFromArguments(array($iDefinition->getFactory()), $calls, $behavior);
    }

    $code = '';
    foreach ($calls as $id => $callCount) {
      if ('service_container' === $id || $id === $cId) {
        continue;
      }

      if ($callCount > 1) {
        $name = $this->getNextVariableName();
        $this->referenceVariables[$id] = new Variable($name);

        if (ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE === $behavior[$id]) {
          $code .= sprintf($template, $name, $this->getServiceCall($id));
        }
        else {
          $code .= sprintf($template, $name, $this->getServiceCall($id, new Reference($id, ContainerInterface::NULL_ON_INVALID_REFERENCE)));
        }
      }
    }

    if ('' !== $code) {
      $code .= "\n";
    }

    return $code;
  }

  /**
   * Generates code for the proxies to be attached after the container class.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addProxyClasses() {
    /** @var \Definition[] $definitions */
    $definitions = array_filter(
          $this->container->getDefinitions(),
          array($this->getProxyDumper(), 'isProxyCandidate')
      );
    $code = '';

    foreach ($definitions as $definition) {
      $code .= "\n" . $this->getProxyDumper()->getProxyCode($definition);
    }

    return $code;
  }

  /**
   * Generates the require_once statement for service includes.
   *
   * @param string $id
   *   The service id.
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServiceInclude($id, Definition $definition) {
    $template = "        require_once %s;\n";
    $code = '';

    if (NULL !== $file = $definition->getFile()) {
      $code .= sprintf($template, $this->dumpValue($file));
    }

    foreach ($this->getInlinedDefinitions($definition) as $definition) {
      if (NULL !== $file = $definition->getFile()) {
        $code .= sprintf($template, $this->dumpValue($file));
      }
    }

    if ('' !== $code) {
      $code .= "\n";
    }

    return $code;
  }

  /**
   * Generates the inline definition of a service.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When the factory definition is incomplete.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException
   *   When a circular reference is detected.
   */
  private function addServiceInlinedDefinitions($id, Definition $definition) {
    $code = '';
    $variableMap = $this->definitionVariables;
    $nbOccurrences = new \SplObjectStorage();
    $processed = new \SplObjectStorage();
    $inlinedDefinitions = $this->getInlinedDefinitions($definition);

    foreach ($inlinedDefinitions as $definition) {
      if (FALSE === $nbOccurrences->contains($definition)) {
        $nbOccurrences->offsetSet($definition, 1);
      }
      else {
        $i = $nbOccurrences->offsetGet($definition);
        $nbOccurrences->offsetSet($definition, $i + 1);
      }
    }

    foreach ($inlinedDefinitions as $sDefinition) {
      if ($processed->contains($sDefinition)) {
        continue;
      }
      $processed->offsetSet($sDefinition);

      $class = $this->dumpValue($sDefinition->getClass());
      if ($nbOccurrences->offsetGet($sDefinition) > 1 || $sDefinition->getMethodCalls() || $sDefinition->getProperties() || NULL !== $sDefinition->getConfigurator() || FALSE !== strpos($class, '$')) {
        $name = $this->getNextVariableName();
        $variableMap->offsetSet($sDefinition, new Variable($name));

        // A construct like:
        // $a = new ServiceA(ServiceB $b); $b = new ServiceB(ServiceA $a);
        // this is an indication for a wrong implementation, you can circumvent
        // this problem
        // by setting up your service structure like this:
        // $b = new ServiceB();
        // $a = new ServiceA(ServiceB $b);
        // $b->setServiceA(ServiceA $a);.
        if ($this->hasReference($id, $sDefinition->getArguments())) {
          throw new ServiceCircularReferenceException($id, array($id));
        }

        $code .= $this->addNewInstance($id, $sDefinition, '$' . $name, ' = ');

        if (!$this->hasReference($id, $sDefinition->getMethodCalls(), TRUE) && !$this->hasReference($id, $sDefinition->getProperties(), TRUE)) {
          $code .= $this->addServiceMethodCalls(NULL, $sDefinition, $name);
          $code .= $this->addServiceProperties(NULL, $sDefinition, $name);
          $code .= $this->addServiceConfigurator(NULL, $sDefinition, $name);
        }

        $code .= "\n";
      }
    }

    return $code;
  }

  /**
   * Adds the service return statement.
   *
   * @param string $id
   *   Service id.
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServiceReturn($id, Definition $definition) {
    if ($this->isSimpleInstance($id, $definition)) {
      return "    }\n";
    }

    return "\n        return \$instance;\n    }\n";
  }

  /**
   * Generates the service instance.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   FIX - insert comment here.
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   FIX - insert comment here.
   */
  private function addServiceInstance($id, Definition $definition) {
    $class = $this->dumpValue($definition->getClass());

    if (0 === strpos($class, "'") && !preg_match('/^\'[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*(\\\{2}[a-zA-Z_\x7f-\xff][a-zA-Z0-9_\x7f-\xff]*)*\'$/', $class)) {
      throw new InvalidArgumentException(sprintf('"%s" is not a valid class name for the "%s" service.', $class, $id));
    }

    $simple = $this->isSimpleInstance($id, $definition);
    $isProxyCandidate = $this->getProxyDumper()->isProxyCandidate($definition);
    $instantiation = '';

    if (!$isProxyCandidate && ContainerInterface::SCOPE_CONTAINER === $definition->getScope()) {
      $instantiation = "\$this->services['$id'] = " . ($simple ? '' : '$instance');
    }
    elseif (!$isProxyCandidate && ContainerInterface::SCOPE_PROTOTYPE !== $scope = $definition->getScope()) {
      $instantiation = "\$this->services['$id'] = \$this->scopedServices['$scope']['$id'] = " . ($simple ? '' : '$instance');
    }
    elseif (!$simple) {
      $instantiation = '$instance';
    }

    $return = '';
    if ($simple) {
      $return = 'return ';
    }
    else {
      $instantiation .= ' = ';
    }

    $code = $this->addNewInstance($id, $definition, $return, $instantiation);

    if (!$simple) {
      $code .= "\n";
    }

    return $code;
  }

  /**
   * Checks if the definition is a simple instance.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  private function isSimpleInstance($id, Definition $definition) {
    foreach (array_merge(array($definition), $this->getInlinedDefinitions($definition)) as $sDefinition) {
      if ($definition !== $sDefinition && !$this->hasReference($id, $sDefinition->getMethodCalls())) {
        continue;
      }

      if ($sDefinition->getMethodCalls() || $sDefinition->getProperties() || $sDefinition->getConfigurator()) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Adds method calls to a service definition.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \Definition $definition
   *   FIX - insert comment here.
   * @param string $variableName
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServiceMethodCalls($id, Definition $definition, $variableName = 'instance') {
    $calls = '';
    foreach ($definition->getMethodCalls() as $call) {
      $arguments = array();
      foreach ($call[1] as $value) {
        $arguments[] = $this->dumpValue($value);
      }

      $calls .= $this->wrapServiceConditionals($call[1], sprintf("        \$%s->%s(%s);\n", $variableName, $call[0], implode(', ', $arguments)));
    }

    return $calls;
  }

  /**
   * FIX - insert comment here.
   */
  private function addServiceProperties($id, $definition, $variableName = 'instance') {
    $code = '';
    foreach ($definition->getProperties() as $name => $value) {
      $code .= sprintf("        \$%s->%s = %s;\n", $variableName, $name, $this->dumpValue($value));
    }

    return $code;
  }

  /**
   * Generates the inline definition setup.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ServiceCircularReferenceException
   *   When the container contains a circular reference.
   */
  private function addServiceInlinedDefinitionsSetup($id, Definition $definition) {
    $this->referenceVariables[$id] = new Variable('instance');

    $code = '';
    $processed = new \SplObjectStorage();
    foreach ($this->getInlinedDefinitions($definition) as $iDefinition) {
      if ($processed->contains($iDefinition)) {
        continue;
      }
      $processed->offsetSet($iDefinition);

      if (!$this->hasReference($id, $iDefinition->getMethodCalls(), TRUE) && !$this->hasReference($id, $iDefinition->getProperties(), TRUE)) {
        continue;
      }

      // If the instance is simple, the return statement has already been
      // generated so, the only possible way to get there is because of a
      // circular reference.
      if ($this->isSimpleInstance($id, $definition)) {
        throw new ServiceCircularReferenceException($id, array($id));
      }

      $name = (string) $this->definitionVariables->offsetGet($iDefinition);
      $code .= $this->addServiceMethodCalls(NULL, $iDefinition, $name);
      $code .= $this->addServiceProperties(NULL, $iDefinition, $name);
      $code .= $this->addServiceConfigurator(NULL, $iDefinition, $name);
    }

    if ('' !== $code) {
      $code .= "\n";
    }

    return $code;
  }

  /**
   * Adds configurator definition.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \Definition $definition
   *   FIX - insert comment here.
   * @param string $variableName
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServiceConfigurator($id, Definition $definition, $variableName = 'instance') {
    if (!$callable = $definition->getConfigurator()) {
      return '';
    }

    if (is_array($callable)) {
      if ($callable[0] instanceof Reference
            || ($callable[0] instanceof Definition && $this->definitionVariables->contains($callable[0]))) {
        return sprintf("        %s->%s(\$%s);\n", $this->dumpValue($callable[0]), $callable[1], $variableName);
      }

      $class = $this->dumpValue($callable[0]);
      // If the class is a string we can optimize call_user_func away.
      if (strpos($class, "'") === 0) {
        return sprintf("        %s::%s(\$%s);\n", $this->dumpLiteralClass($class), $callable[1], $variableName);
      }

      return sprintf("        call_user_func(array(%s, '%s'), \$%s);\n", $this->dumpValue($callable[0]), $callable[1], $variableName);
    }

    return sprintf("        %s(\$%s);\n", $callable, $variableName);
  }

  /**
   * Adds a service.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addService($id, Definition $definition) {
    $this->definitionVariables = new \SplObjectStorage();
    $this->referenceVariables = array();
    $this->variableCount = 0;

    $return = array();

    if ($definition->isSynthetic()) {
      $return[] = '@throws RuntimeException always since this service is expected to be injected dynamically';
    }
    elseif ($class = $definition->getClass()) {
      $return[] = sprintf('@return %s A %s instance.', 0 === strpos($class, '%') ? 'object' : '\\' . $class, $class);
    }
    elseif ($definition->getFactory()) {
      $factory = $definition->getFactory();
      if (is_string($factory)) {
        $return[] = sprintf('@return object An instance returned by %s().', $factory);
      }
      elseif (is_array($factory) && (is_string($factory[0]) || $factory[0] instanceof Definition || $factory[0] instanceof Reference)) {
        if (is_string($factory[0]) || $factory[0] instanceof Reference) {
          $return[] = sprintf('@return object An instance returned by %s::%s().', (string) $factory[0], $factory[1]);
        }
        elseif ($factory[0] instanceof Definition) {
          $return[] = sprintf('@return object An instance returned by %s::%s().', $factory[0]->getClass(), $factory[1]);
        }
      }
    }
    elseif ($definition->getFactoryClass(FALSE)) {
      $return[] = sprintf('@return object An instance returned by %s::%s().', $definition->getFactoryClass(FALSE), $definition->getFactoryMethod(FALSE));
    }
    elseif ($definition->getFactoryService(FALSE)) {
      $return[] = sprintf('@return object An instance returned by %s::%s().', $definition->getFactoryService(FALSE), $definition->getFactoryMethod(FALSE));
    }

    $scope = $definition->getScope();
    if (!in_array($scope, array(
      ContainerInterface::SCOPE_CONTAINER,
      ContainerInterface::SCOPE_PROTOTYPE,
    ))) {
      if ($return && 0 === strpos($return[count($return) - 1], '@return')) {
        $return[] = '';
      }
      $return[] = sprintf("@throws InactiveScopeException when the '%s' service is requested while the '%s' scope is not active", $id, $scope);
    }

    $return = implode("\n     * ", $return);

    $doc = '';
    if (ContainerInterface::SCOPE_PROTOTYPE !== $scope) {
      $doc .= <<<EOF

     *
     * This service is shared.
     * This method always returns the same instance of the service.
EOF;
    }

    if (!$definition->isPublic()) {
      $doc .= <<<EOF

     *
     * This service is private.
     * If you want to be able to request this service from the container directly,
     * make it public, otherwise you might end up with broken code.
EOF;
    }

    if ($definition->isLazy()) {
      $lazyInitialization = '$lazyLoad = true';
      $lazyInitializationDoc = "\n     * @param bool    \$lazyLoad whether to try lazy-loading the service with a proxy\n     *";
    }
    else {
      $lazyInitialization = '';
      $lazyInitializationDoc = '';
    }

    // With proxies, for 5.3.3 compatibility, the getter must be public to be
    // accessible to the initializer.
    $isProxyCandidate = $this->getProxyDumper()->isProxyCandidate($definition);
    $visibility = $isProxyCandidate ? 'public' : 'protected';
    $code = <<<EOF

    /**
     * Gets the '$id' service.$doc
     *$lazyInitializationDoc
     * $return
     */
    {$visibility} function get{$this->camelize($id)}Service($lazyInitialization)
    {

EOF;

    $code .= $isProxyCandidate ? $this->getProxyDumper()->getProxyFactoryCode($definition, $id) : '';

    if (!in_array(
      $scope,
      array(
        ContainerInterface::SCOPE_CONTAINER,
        ContainerInterface::SCOPE_PROTOTYPE,
      )
    )) {
      $code .= <<<EOF
        if (!isset(\$this->scopedServices['$scope'])) {
            throw new InactiveScopeException('$id', '$scope');
        }


EOF;
    }

    if ($definition->isSynthetic()) {
      $code .= sprintf("        throw new RuntimeException('You have requested a synthetic service (\"%s\"). The DIC does not know how to construct this service.');\n    }\n", $id);
    }
    else {
      $code .=
                $this->addServiceInclude($id, $definition) .
                $this->addServiceLocalTempVariables($id, $definition) .
                $this->addServiceInlinedDefinitions($id, $definition) .
                $this->addServiceInstance($id, $definition) .
                $this->addServiceInlinedDefinitionsSetup($id, $definition) .
                $this->addServiceMethodCalls($id, $definition) .
                $this->addServiceProperties($id, $definition) .
                $this->addServiceConfigurator($id, $definition) .
                $this->addServiceReturn($id, $definition);
    }

    $this->definitionVariables = NULL;
    $this->referenceVariables = NULL;

    return $code;
  }

  /**
   * Adds multiple services.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addServices() {
    $publicServices = $privateServices = $synchronizers = '';
    $definitions = $this->container->getDefinitions();
    ksort($definitions);
    foreach ($definitions as $id => $definition) {
      if ($definition->isPublic()) {
        $publicServices .= $this->addService($id, $definition);
      }
      else {
        $privateServices .= $this->addService($id, $definition);
      }

      $synchronizers .= $this->addServiceSynchronizer($id, $definition);
    }

    return $publicServices . $synchronizers . $privateServices;
  }

  /**
   * Adds synchronizer methods.
   *
   * @param string $id
   *   A service identifier.
   * @param \Definition $definition
   *   A Definition instance.
   *
   * @return string|null
   *   FIX - insert comment here.
   */
  private function addServiceSynchronizer($id, Definition $definition) {
    if (!$definition->isSynchronized(FALSE)) {
      return;
    }

    if ('request' !== $id) {
      trigger_error('Synchronized services were deprecated in version 2.7 and won\'t work anymore in 3.0.', E_USER_DEPRECATED);
    }

    $code = '';
    foreach ($this->container->getDefinitions() as $definitionId => $definition) {
      foreach ($definition->getMethodCalls() as $call) {
        foreach ($call[1] as $argument) {
          if ($argument instanceof Reference && $id == (string) $argument) {
            $arguments = array();
            foreach ($call[1] as $value) {
              $arguments[] = $this->dumpValue($value);
            }

            $call = $this->wrapServiceConditionals($call[1], sprintf("\$this->get('%s')->%s(%s);", $definitionId, $call[0], implode(', ', $arguments)));

            $code .= <<<EOF
        if (\$this->initialized('$definitionId')) {
            $call
        }

EOF;
          }
        }
      }
    }

    if (!$code) {
      return;
    }

    return <<<EOF

    /**
     * Updates the '$id' service.
     */
    protected function synchronize{$this->camelize($id)}Service()
    {
$code    }

EOF;
  }

  /**
   * FIX - insert comment here.
   */
  private function addNewInstance($id, Definition $definition, $return, $instantiation) {
    $class = $this->dumpValue($definition->getClass());

    $arguments = array();
    foreach ($definition->getArguments() as $value) {
      $arguments[] = $this->dumpValue($value);
    }

    if (NULL !== $definition->getFactory()) {
      $callable = $definition->getFactory();
      if (is_array($callable)) {
        if ($callable[0] instanceof Reference
              || ($callable[0] instanceof Definition && $this->definitionVariables->contains($callable[0]))) {
          return sprintf("        $return{$instantiation}%s->%s(%s);\n", $this->dumpValue($callable[0]), $callable[1], $arguments ? implode(', ', $arguments) : '');
        }

        $class = $this->dumpValue($callable[0]);
        // If the class is a string we can optimize call_user_func away.
        if (strpos($class, "'") === 0) {
          return sprintf("        $return{$instantiation}%s::%s(%s);\n", $this->dumpLiteralClass($class), $callable[1], $arguments ? implode(', ', $arguments) : '');
        }

        return sprintf("        $return{$instantiation}call_user_func(array(%s, '%s')%s);\n", $this->dumpValue($callable[0]), $callable[1], $arguments ? ', ' . implode(', ', $arguments) : '');
      }

      return sprintf("        $return{$instantiation}\\%s(%s);\n", $callable, $arguments ? implode(', ', $arguments) : '');
    }
    elseif (NULL !== $definition->getFactoryMethod(FALSE)) {
      if (NULL !== $definition->getFactoryClass(FALSE)) {
        $class = $this->dumpValue($definition->getFactoryClass(FALSE));

        // If the class is a string we can optimize call_user_func away.
        if (strpos($class, "'") === 0) {
          return sprintf("        $return{$instantiation}%s::%s(%s);\n", $this->dumpLiteralClass($class), $definition->getFactoryMethod(FALSE), $arguments ? implode(', ', $arguments) : '');
        }

        return sprintf("        $return{$instantiation}call_user_func(array(%s, '%s')%s);\n", $this->dumpValue($definition->getFactoryClass(FALSE)), $definition->getFactoryMethod(FALSE), $arguments ? ', ' . implode(', ', $arguments) : '');
      }

      if (NULL !== $definition->getFactoryService(FALSE)) {
        return sprintf("        $return{$instantiation}%s->%s(%s);\n", $this->getServiceCall($definition->getFactoryService(FALSE)), $definition->getFactoryMethod(FALSE), implode(', ', $arguments));
      }

      throw new RuntimeException(sprintf('Factory method requires a factory service or factory class in service definition for %s', $id));
    }

    if (FALSE !== strpos($class, '$')) {
      return sprintf("        \$class = %s;\n\n        $return{$instantiation}new \$class(%s);\n", $class, implode(', ', $arguments));
    }

    return sprintf("        $return{$instantiation}new %s(%s);\n", $this->dumpLiteralClass($class), implode(', ', $arguments));
  }

  /**
   * Adds the class headers.
   *
   * @param string $class
   *   Class name.
   * @param string $baseClass
   *   The name of the base class.
   * @param string $namespace
   *   The class namespace.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function startClass($class, $baseClass, $namespace) {
    $bagClass = $this->container->isFrozen() ? 'use OpenlayersSymfony\Component\DependencyInjection\ParameterBag\FrozenParameterBag;' : 'use OpenlayersSymfony\Component\DependencyInjection\ParameterBag\\ParameterBag;';
    $namespaceLine = $namespace ? "namespace $namespace;\n" : '';

    return <<<EOF
<?php
$namespaceLine
use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Container;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InactiveScopeException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\LogicException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;
$bagClass

/**
 * $class.
 *
 * This class has been auto-generated
 * by the Symfony Dependency Injection Component.
 */
class $class extends $baseClass
{
    private \$parameters;
    private \$targetDirs = array();

EOF;
  }

  /**
   * Adds the constructor.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addConstructor() {
    $targetDirs = $this->exportTargetDirs();
    $arguments = $this->container->getParameterBag()->all() ? 'new ParameterBag($this->getDefaultParameters())' : NULL;

    $code = <<<EOF

    /**
     * Constructor.
     */
    public function __construct()
    {{$targetDirs}
        parent::__construct($arguments);

EOF;

    if (count($scopes = $this->container->getScopes()) > 0) {
      $code .= "\n";
      $code .= "        \$this->scopes = " . $this->dumpValue($scopes) . ";\n";
      $code .= "        \$this->scopeChildren = " . $this->dumpValue($this->container->getScopeChildren()) . ";\n";
    }

    $code .= $this->addMethodMap();
    $code .= $this->addAliases();

    $code .= <<<EOF
    }

EOF;

    return $code;
  }

  /**
   * Adds the constructor for a frozen container.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addFrozenConstructor() {
    $targetDirs = $this->exportTargetDirs();

    $code = <<<EOF

    /**
     * Constructor.
     */
    public function __construct()
    {{$targetDirs}
EOF;

    if ($this->container->getParameterBag()->all()) {
      $code .= "\n        \$this->parameters = \$this->getDefaultParameters();\n";
    }

    $code .= <<<EOF

        \$this->services =
        \$this->scopedServices =
        \$this->scopeStacks = array();
EOF;

    $code .= "\n";
    if (count($scopes = $this->container->getScopes()) > 0) {
      $code .= "        \$this->scopes = " . $this->dumpValue($scopes) . ";\n";
      $code .= "        \$this->scopeChildren = " . $this->dumpValue($this->container->getScopeChildren()) . ";\n";
    }
    else {
      $code .= "        \$this->scopes = array();\n";
      $code .= "        \$this->scopeChildren = array();\n";
    }

    $code .= $this->addMethodMap();
    $code .= $this->addAliases();

    $code .= <<<EOF
    }

EOF;

    return $code;
  }

  /**
   * Adds the constructor for a frozen container.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addFrozenCompile() {
    return <<<EOF

    /**
     * {@inheritdoc}
     */
    public function compile()
    {
        throw new LogicException('You cannot compile a dumped frozen container.');
    }

EOF;
  }

  /**
   * Adds the methodMap property definition.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addMethodMap() {
    if (!$definitions = $this->container->getDefinitions()) {
      return '';
    }

    $code = "        \$this->methodMap = array(\n";
    ksort($definitions);
    foreach ($definitions as $id => $definition) {
      $code .= '            ' . var_export($id, TRUE) . ' => ' . var_export('get' . $this->camelize($id) . 'Service', TRUE) . ",\n";
    }

    return $code . "        );\n";
  }

  /**
   * Adds the aliases property definition.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addAliases() {
    if (!$aliases = $this->container->getAliases()) {
      if ($this->container->isFrozen()) {
        return "\n        \$this->aliases = array();\n";
      }
      else {
        return '';
      }
    }

    $code = "        \$this->aliases = array(\n";
    ksort($aliases);
    foreach ($aliases as $alias => $id) {
      $id = (string) $id;
      while (isset($aliases[$id])) {
        $id = (string) $aliases[$id];
      }
      $code .= '            ' . var_export($alias, TRUE) . ' => ' . var_export($id, TRUE) . ",\n";
    }

    return $code . "        );\n";
  }

  /**
   * Adds default parameters method.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function addDefaultParametersMethod() {
    if (!$this->container->getParameterBag()->all()) {
      return '';
    }

    $parameters = $this->exportParameters($this->container->getParameterBag()->all());

    $code = '';
    if ($this->container->isFrozen()) {
      $code .= <<<EOF

    /**
     * {@inheritdoc}
     */
    public function getParameter(\$name)
    {
        \$name = strtolower(\$name);

        if (!(isset(\$this->parameters[\$name]) || array_key_exists(\$name, \$this->parameters))) {
            throw new InvalidArgumentException(sprintf('The parameter "%s" must be defined.', \$name));
        }

        return \$this->parameters[\$name];
    }

    /**
     * {@inheritdoc}
     */
    public function hasParameter(\$name)
    {
        \$name = strtolower(\$name);

        return isset(\$this->parameters[\$name]) || array_key_exists(\$name, \$this->parameters);
    }

    /**
     * {@inheritdoc}
     */
    public function setParameter(\$name, \$value)
    {
        throw new LogicException('Impossible to call set() on a frozen ParameterBag.');
    }

    /**
     * {@inheritdoc}
     */
    public function getParameterBag()
    {
        if (null === \$this->parameterBag) {
            \$this->parameterBag = new FrozenParameterBag(\$this->parameters);
        }

        return \$this->parameterBag;
    }

EOF;
    }

    $code .= <<<EOF

    /**
     * Gets the default parameters.
     *
     * @return array An array of the default parameters
     */
    protected function getDefaultParameters()
    {
        return $parameters;
    }

EOF;

    return $code;
  }

  /**
   * Exports parameters.
   *
   * @param array $parameters
   *   FIX - insert comment here.
   * @param string $path
   *   FIX - insert comment here.
   * @param int $indent
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   FIX - insert comment here.
   */
  private function exportParameters(array $parameters, $path = '', $indent = 12) {
    $php = array();
    foreach ($parameters as $key => $value) {
      if (is_array($value)) {
        $value = $this->exportParameters($value, $path . '/' . $key, $indent + 4);
      }
      elseif ($value instanceof Variable) {
        throw new InvalidArgumentException(sprintf('You cannot dump a container with parameters that contain variable references. Variable "%s" found in "%s".', $value, $path . '/' . $key));
      }
      elseif ($value instanceof Definition) {
        throw new InvalidArgumentException(sprintf('You cannot dump a container with parameters that contain service definitions. Definition for "%s" found in "%s".', $value->getClass(), $path . '/' . $key));
      }
      elseif ($value instanceof Reference) {
        throw new InvalidArgumentException(sprintf('You cannot dump a container with parameters that contain references to other services (reference to service "%s" found in "%s").', $value, $path . '/' . $key));
      }
      elseif ($value instanceof Expression) {
        throw new InvalidArgumentException(sprintf('You cannot dump a container with parameters that contain expressions. Expression "%s" found in "%s".', $value, $path . '/' . $key));
      }
      else {
        $value = $this->export($value);
      }

      $php[] = sprintf('%s%s => %s,', str_repeat(' ', $indent), var_export($key, TRUE), $value);
    }

    return sprintf("array(\n%s\n%s)", implode("\n", $php), str_repeat(' ', $indent - 4));
  }

  /**
   * Ends the class definition.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function endClass() {
    return <<<EOF
}

EOF;
  }

  /**
   * Wraps the service conditionals.
   *
   * @param string $value
   *   FIX - insert comment here.
   * @param string $code
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function wrapServiceConditionals($value, $code) {
    if (!$services = ContainerBuilder::getServiceConditionals($value)) {
      return $code;
    }

    $conditions = array();
    foreach ($services as $service) {
      $conditions[] = sprintf("\$this->has('%s')", $service);
    }

    // re-indent the wrapped code.
    $code = implode("\n", array_map(function ($line) {
      return $line ? '    ' . $line : $line;
    }, explode("\n", $code)));

    return sprintf("        if (%s) {\n%s        }\n", implode(' && ', $conditions), $code);
  }

  /**
   * Builds service calls from arguments.
   *
   * @param array $arguments
   *   FIX - insert comment here.
   * @param array &$calls
   *   By reference.
   * @param array &$behavior
   *   By reference.
   */
  private function getServiceCallsFromArguments(array $arguments, array &$calls, array &$behavior) {
    foreach ($arguments as $argument) {
      if (is_array($argument)) {
        $this->getServiceCallsFromArguments($argument, $calls, $behavior);
      }
      elseif ($argument instanceof Reference) {
        $id = (string) $argument;

        if (!isset($calls[$id])) {
          $calls[$id] = 0;
        }
        if (!isset($behavior[$id])) {
          $behavior[$id] = $argument->getInvalidBehavior();
        }
        elseif (ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE !== $behavior[$id]) {
          $behavior[$id] = $argument->getInvalidBehavior();
        }

        ++$calls[$id];
      }
    }
  }

  /**
   * Returns the inline definition.
   *
   * @param \Definition $definition
   *   FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function getInlinedDefinitions(Definition $definition) {
    if (FALSE === $this->inlinedDefinitions->contains($definition)) {
      $definitions = array_merge(
            $this->getDefinitionsFromArguments($definition->getArguments()),
            $this->getDefinitionsFromArguments($definition->getMethodCalls()),
            $this->getDefinitionsFromArguments($definition->getProperties()),
            $this->getDefinitionsFromArguments(array($definition->getConfigurator())),
            $this->getDefinitionsFromArguments(array($definition->getFactory()))
        );

      $this->inlinedDefinitions->offsetSet($definition, $definitions);

      return $definitions;
    }

    return $this->inlinedDefinitions->offsetGet($definition);
  }

  /**
   * Gets the definition from arguments.
   *
   * @param array $arguments
   *   FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function getDefinitionsFromArguments(array $arguments) {
    $definitions = array();
    foreach ($arguments as $argument) {
      if (is_array($argument)) {
        $definitions = array_merge($definitions, $this->getDefinitionsFromArguments($argument));
      }
      elseif ($argument instanceof Definition) {
        $definitions = array_merge(
              $definitions,
              $this->getInlinedDefinitions($argument),
              array($argument)
          );
      }
    }

    return $definitions;
  }

  /**
   * Checks if a service id has a reference.
   *
   * @param string $id
   *   FIX - insert comment here.
   * @param array $arguments
   *   FIX - insert comment here.
   * @param bool $deep
   *   FIX - insert comment here.
   * @param array $visited
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  private function hasReference($id, array $arguments, $deep = FALSE, array &$visited = array()) {
    foreach ($arguments as $argument) {
      if (is_array($argument)) {
        if ($this->hasReference($id, $argument, $deep, $visited)) {
          return TRUE;
        }
      }
      elseif ($argument instanceof Reference) {
        $argumentId = (string) $argument;
        if ($id === $argumentId) {
          return TRUE;
        }

        if ($deep && !isset($visited[$argumentId])) {
          $visited[$argumentId] = TRUE;

          $service = $this->container->getDefinition($argumentId);
          $arguments = array_merge($service->getMethodCalls(), $service->getArguments(), $service->getProperties());

          if ($this->hasReference($id, $arguments, $deep, $visited)) {
            return TRUE;
          }
        }
      }
    }

    return FALSE;
  }

  /**
   * Dumps values.
   *
   * @param mixed $value
   *   FIX - insert comment here.
   * @param bool $interpolate
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   FIX - insert comment here.
   */
  private function dumpValue($value, $interpolate = TRUE) {
    if (is_array($value)) {
      $code = array();
      foreach ($value as $k => $v) {
        $code[] = sprintf('%s => %s', $this->dumpValue($k, $interpolate), $this->dumpValue($v, $interpolate));
      }

      return sprintf('array(%s)', implode(', ', $code));
    }
    elseif ($value instanceof Definition) {
      if (NULL !== $this->definitionVariables && $this->definitionVariables->contains($value)) {
        return $this->dumpValue($this->definitionVariables->offsetGet($value), $interpolate);
      }
      if (count($value->getMethodCalls()) > 0) {
        throw new RuntimeException('Cannot dump definitions which have method calls.');
      }
      if (NULL !== $value->getConfigurator()) {
        throw new RuntimeException('Cannot dump definitions which have a configurator.');
      }

      $arguments = array();
      foreach ($value->getArguments() as $argument) {
        $arguments[] = $this->dumpValue($argument);
      }
      $class = $this->dumpValue($value->getClass());

      if (FALSE !== strpos($class, '$')) {
        throw new RuntimeException('Cannot dump definitions which have a variable class name.');
      }

      if (NULL !== $value->getFactory()) {
        $factory = $value->getFactory();

        if (is_string($factory)) {
          return sprintf('\\%s(%s)', $factory, implode(', ', $arguments));
        }

        if (is_array($factory)) {
          if (is_string($factory[0])) {
            return sprintf('\\%s::%s(%s)', $factory[0], $factory[1], implode(', ', $arguments));
          }

          if ($factory[0] instanceof Definition) {
            return sprintf("call_user_func(array(%s, '%s')%s)", $this->dumpValue($factory[0]), $factory[1], count($arguments) > 0 ? ', ' . implode(', ', $arguments) : '');
          }

          if ($factory[0] instanceof Reference) {
            return sprintf('%s->%s(%s)', $this->dumpValue($factory[0]), $factory[1], implode(', ', $arguments));
          }
        }

        throw new RuntimeException('Cannot dump definition because of invalid factory');
      }

      if (NULL !== $value->getFactoryMethod(FALSE)) {
        if (NULL !== $value->getFactoryClass(FALSE)) {
          return sprintf("call_user_func(array(%s, '%s')%s)", $this->dumpValue($value->getFactoryClass(FALSE)), $value->getFactoryMethod(FALSE), count($arguments) > 0 ? ', ' . implode(', ', $arguments) : '');
        }
        elseif (NULL !== $value->getFactoryService(FALSE)) {
          $service = $this->dumpValue($value->getFactoryService(FALSE));

          return sprintf('%s->%s(%s)', 0 === strpos($service, '$') ? sprintf('$this->get(%s)', $service) : $this->getServiceCall($value->getFactoryService(FALSE)), $value->getFactoryMethod(FALSE), implode(', ', $arguments));
        }
        else {
          throw new RuntimeException('Cannot dump definitions which have factory method without factory service or factory class.');
        }
      }

      return sprintf('new \\%s(%s)', substr(str_replace('\\\\', '\\', $class), 1, -1), implode(', ', $arguments));
    }
    elseif ($value instanceof Variable) {
      return '$' . $value;
    }
    elseif ($value instanceof Reference) {
      if (NULL !== $this->referenceVariables && isset($this->referenceVariables[$id = (string) $value])) {
        return $this->dumpValue($this->referenceVariables[$id], $interpolate);
      }

      return $this->getServiceCall((string) $value, $value);
    }
    elseif ($value instanceof Expression) {
      return $this->getExpressionLanguage()->compile((string) $value, array('this' => 'container'));
    }
    elseif ($value instanceof Parameter) {
      return $this->dumpParameter($value);
    }
    elseif (TRUE === $interpolate && is_string($value)) {
      if (preg_match('/^%([^%]+)%$/', $value, $match)) {
        // We do this to deal with non string values (Boolean, integer, ...)
        // the preg_replace_callback converts them to strings.
        return $this->dumpParameter(strtolower($match[1]));
      }
      else {
        $that = $this;
        $replaceParameters = function ($match) use ($that) {
          return "'." . $that->dumpParameter(strtolower($match[2])) . ".'";
        };

        $code = str_replace('%%', '%', preg_replace_callback('/(?<!%)(%)([^%]+)\1/', $replaceParameters, $this->export($value)));

        return $code;
      }
    }
    elseif (is_object($value) || is_resource($value)) {
      throw new RuntimeException('Unable to dump a service container if a parameter is an object or a resource.');
    }
    else {
      return $this->export($value);
    }
  }

  /**
   * Dumps a string to a literal (aka PHP Code) class value.
   *
   * @param string $class
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function dumpLiteralClass($class) {
    return '\\' . substr(str_replace('\\\\', '\\', $class), 1, -1);
  }

  /**
   * Dumps a parameter.
   *
   * @param string $name
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function dumpParameter($name) {
    if ($this->container->isFrozen() && $this->container->hasParameter($name)) {
      return $this->dumpValue($this->container->getParameter($name), FALSE);
    }

    return sprintf("\$this->getParameter('%s')", strtolower($name));
  }

  /**
   * FIX - insert comment here.
   *
   * @param \OpenlayersSymfony\Component\ExpressionLanguage\ExpressionFunctionProviderInterface $provider
   *   FIX - insert comment here.
   */
  public function addExpressionLanguageProvider(ExpressionFunctionProviderInterface $provider) {
    trigger_error('The ' . __METHOD__ . ' method is deprecated since version 2.6.2 and will be removed in 3.0. Use the OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder::addExpressionLanguageProvider method instead.', E_USER_DEPRECATED);

    $this->expressionLanguageProviders[] = $provider;
  }

  /**
   * Gets a service call.
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
    if ('service_container' === $id) {
      return '$this';
    }

    if (NULL !== $reference && ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE !== $reference->getInvalidBehavior()) {
      return sprintf('$this->get(\'%s\', ContainerInterface::NULL_ON_INVALID_REFERENCE)', $id);
    }
    else {
      if ($this->container->hasAlias($id)) {
        $id = (string) $this->container->getAlias($id);
      }

      return sprintf('$this->get(\'%s\')', $id);
    }
  }

  /**
   * Convert a service id to a valid PHP method name.
   *
   * @param string $id
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   FIX - insert comment here.
   */
  private function camelize($id) {
    $name = Container::camelize($id);

    if (!preg_match('/^[a-zA-Z0-9_\x7f-\xff]+$/', $name)) {
      throw new InvalidArgumentException(sprintf('Service id "%s" cannot be converted to a valid PHP method name.', $id));
    }

    return $name;
  }

  /**
   * Returns the next name to use.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function getNextVariableName() {
    $firstChars = self::FIRST_CHARS;
    $firstCharsLength = strlen($firstChars);
    $nonFirstChars = self::NON_FIRST_CHARS;
    $nonFirstCharsLength = strlen($nonFirstChars);

    while (TRUE) {
      $name = '';
      $i = $this->variableCount;

      if ('' === $name) {
        $name .= $firstChars[$i % $firstCharsLength];
        $i = (int) ($i / $firstCharsLength);
      }

      while ($i > 0) {
        --$i;
        $name .= $nonFirstChars[$i % $nonFirstCharsLength];
        $i = (int) ($i / $nonFirstCharsLength);
      }

      ++$this->variableCount;

      // Check that the name is not reserved.
      if (in_array($name, $this->reservedVariables, TRUE)) {
        continue;
      }

      return $name;
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
      $providers = array_merge($this->container->getExpressionLanguageProviders(), $this->expressionLanguageProviders);
      $this->expressionLanguage = new ExpressionLanguage(NULL, $providers);

      if ($this->container->isTrackingResources()) {
        foreach ($providers as $provider) {
          $this->container->addObjectResource($provider);
        }
      }
    }

    return $this->expressionLanguage;
  }

  /**
   * FIX - insert comment here.
   */
  private function exportTargetDirs() {
    return NULL === $this->targetDirRegex ? '' : <<<EOF

        \$dir = __DIR__;
        for (\$i = 1; \$i <= {$this->targetDirMaxMatches}; ++\$i) {
            \$this->targetDirs[\$i] = \$dir = dirname(\$dir);
        }
EOF;
  }

  /**
   * FIX - insert comment here.
   */
  private function export($value) {
    if (NULL !== $this->targetDirRegex && is_string($value) && preg_match($this->targetDirRegex, $value, $matches, PREG_OFFSET_CAPTURE)) {
      $prefix = $matches[0][1] ? var_export(substr($value, 0, $matches[0][1]), TRUE) . '.' : '';
      $suffix = $matches[0][1] + strlen($matches[0][0]);
      $suffix = isset($value[$suffix]) ? '.' . var_export(substr($value, $suffix), TRUE) : '';
      $dirname = '__DIR__';

      if (0 < $offset = 1 + $this->targetDirMaxMatches - count($matches)) {
        $dirname = sprintf('$this->targetDirs[%d]', $offset);
      }

      if ($prefix || $suffix) {
        return sprintf('(%s%s%s)', $prefix, $dirname, $suffix);
      }

      return $dirname;
    }

    return var_export($value, TRUE);
  }

}
