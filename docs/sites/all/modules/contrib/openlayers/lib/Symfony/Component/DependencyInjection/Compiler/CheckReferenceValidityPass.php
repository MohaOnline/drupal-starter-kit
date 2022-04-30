<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\Reference;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ScopeCrossingInjectionException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\ScopeWideningInjectionException;

/**
 * Checks the validity of references.
 *
 * The following checks are performed by this pass:
 * - target definitions are not abstract
 * - target definitions are of equal or wider scope
 * - target definitions are in the same scope hierarchy.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class CheckReferenceValidityPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $container;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $currentId;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $currentScope;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $currentScopeAncestors;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $currentScopeChildren;

  /**
   * Processes the ContainerBuilder to validate References.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $this->container = $container;

    $children = $this->container->getScopeChildren();
    $ancestors = array();

    $scopes = $this->container->getScopes();
    foreach ($scopes as $name => $parent) {
      $ancestors[$name] = array($parent);

      while (isset($scopes[$parent])) {
        $ancestors[$name][] = $parent = $scopes[$parent];
      }
    }

    foreach ($container->getDefinitions() as $id => $definition) {
      if ($definition->isSynthetic() || $definition->isAbstract()) {
        continue;
      }

      $this->currentId = $id;
      $this->currentDefinition = $definition;
      $this->currentScope = $scope = $definition->getScope();

      if (ContainerInterface::SCOPE_CONTAINER === $scope) {
        $this->currentScopeChildren = array_keys($scopes);
        $this->currentScopeAncestors = array();
      }
      elseif (ContainerInterface::SCOPE_PROTOTYPE !== $scope) {
        $this->currentScopeChildren = isset($children[$scope]) ? $children[$scope] : array();
        $this->currentScopeAncestors = isset($ancestors[$scope]) ? $ancestors[$scope] : array();
      }

      $this->validateReferences($definition->getArguments());
      $this->validateReferences($definition->getMethodCalls());
      $this->validateReferences($definition->getProperties());
    }
  }

  /**
   * Validates an array of References.
   *
   * @param array $arguments
   *   An array of Reference objects.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When there is a reference to an abstract definition.
   */
  private function validateReferences(array $arguments) {
    foreach ($arguments as $argument) {
      if (is_array($argument)) {
        $this->validateReferences($argument);
      }
      elseif ($argument instanceof Reference) {
        $targetDefinition = $this->getDefinition((string) $argument);

        if (NULL !== $targetDefinition && $targetDefinition->isAbstract()) {
          throw new RuntimeException(sprintf(
                'The definition "%s" has a reference to an abstract definition "%s". '
               . 'Abstract definitions cannot be the target of references.',
               $this->currentId,
               $argument
            ));
        }

        $this->validateScope($argument, $targetDefinition);
      }
    }
  }

  /**
   * Validates the scope of a single Reference.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\Reference $reference
   *   FIX - insert comment here.
   * @param \OpenlayersSymfony\Component\DependencyInjection\Definition $definition
   *   FIX - insert comment here.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ScopeWideningInjectionException
   *   When the definition references a service of a narrower scope.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\ScopeWideningInjectionException
   *   When the definition references a service of another scope hierarchy.
   */
  private function validateScope(Reference $reference, Definition $definition = NULL) {
    if (ContainerInterface::SCOPE_PROTOTYPE === $this->currentScope) {
      return;
    }

    if (!$reference->isStrict()) {
      return;
    }

    if (NULL === $definition) {
      return;
    }

    if ($this->currentScope === $scope = $definition->getScope()) {
      return;
    }

    $id = (string) $reference;

    if (in_array($scope, $this->currentScopeChildren, TRUE)) {
      throw new ScopeWideningInjectionException($this->currentId, $this->currentScope, $id, $scope);
    }

    if (!in_array($scope, $this->currentScopeAncestors, TRUE)) {
      throw new ScopeCrossingInjectionException($this->currentId, $this->currentScope, $id, $scope);
    }
  }

  /**
   * Returns the Definition given an id.
   *
   * @param string $id
   *   Definition identifier.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\Definition|null
   *   FIX - insert comment here.
   */
  private function getDefinition($id) {
    if (!$this->container->hasDefinition($id)) {
      return;
    }

    return $this->container->getDefinition($id);
  }

}
