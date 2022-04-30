<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Definition;
use OpenlayersSymfony\Component\DependencyInjection\DefinitionDecorator;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;

/**
 * FIX - insert comment here.
 *
 * This replaces all DefinitionDecorator instances with their equivalent fully
 * merged Definition instance.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ResolveDefinitionTemplatesPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $container;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $compiler;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $formatter;

  /**
   * FIX - insert comment here.
   *
   * Process the ContainerBuilder to replace DefinitionDecorator instances with
   * their real Definition instances.
   *
   * @param OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $this->container = $container;
    $this->compiler = $container->getCompiler();
    $this->formatter = $this->compiler->getLoggingFormatter();

    foreach ($container->getDefinitions() as $id => $definition) {
      // yes, we are specifically fetching the definition from the
      // container to ensure we are not operating on stale data.
      $definition = $container->getDefinition($id);
      if (!$definition instanceof DefinitionDecorator || $definition->isAbstract()) {
        continue;
      }

      $this->resolveDefinition($id, $definition);
    }
  }

  /**
   * Resolves the definition.
   *
   * @param string $id
   *   The definition identifier.
   * @param \OpenlayersSymfony\Component\DependencyInjection\DefinitionDecorator $definition
   *   FIX - insert comment here.
   *
   * @return \OpenlayersSymfony\Component\DependencyInjection\Definition
   *   FIX - insert comment here.
   *
   * @throws \RuntimeException
   *   When the definition is invalid.
   */
  private function resolveDefinition($id, DefinitionDecorator $definition) {
    if (!$this->container->hasDefinition($parent = $definition->getParent())) {
      throw new RuntimeException(sprintf('The parent definition "%s" defined for definition "%s" does not exist.', $parent, $id));
    }

    $parentDef = $this->container->getDefinition($parent);
    if ($parentDef instanceof DefinitionDecorator) {
      $parentDef = $this->resolveDefinition($parent, $parentDef);
    }

    $this->compiler->addLogMessage($this->formatter->formatResolveInheritance($this, $id, $parent));
    $def = new Definition();

    // Merge in parent definition
    // purposely ignored attributes: scope, abstract, tags.
    $def->setClass($parentDef->getClass());
    $def->setArguments($parentDef->getArguments());
    $def->setMethodCalls($parentDef->getMethodCalls());
    $def->setProperties($parentDef->getProperties());
    if ($parentDef->getFactoryClass(FALSE)) {
      $def->setFactoryClass($parentDef->getFactoryClass(FALSE));
    }
    if ($parentDef->getFactoryMethod(FALSE)) {
      $def->setFactoryMethod($parentDef->getFactoryMethod(FALSE));
    }
    if ($parentDef->getFactoryService(FALSE)) {
      $def->setFactoryService($parentDef->getFactoryService(FALSE));
    }
    $def->setFactory($parentDef->getFactory());
    $def->setConfigurator($parentDef->getConfigurator());
    $def->setFile($parentDef->getFile());
    $def->setPublic($parentDef->isPublic());
    $def->setLazy($parentDef->isLazy());

    // Overwrite with values specified in the decorator.
    $changes = $definition->getChanges();
    if (isset($changes['class'])) {
      $def->setClass($definition->getClass());
    }
    if (isset($changes['factory_class'])) {
      $def->setFactoryClass($definition->getFactoryClass(FALSE));
    }
    if (isset($changes['factory_method'])) {
      $def->setFactoryMethod($definition->getFactoryMethod(FALSE));
    }
    if (isset($changes['factory_service'])) {
      $def->setFactoryService($definition->getFactoryService(FALSE));
    }
    if (isset($changes['factory'])) {
      $def->setFactory($definition->getFactory());
    }
    if (isset($changes['configurator'])) {
      $def->setConfigurator($definition->getConfigurator());
    }
    if (isset($changes['file'])) {
      $def->setFile($definition->getFile());
    }
    if (isset($changes['public'])) {
      $def->setPublic($definition->isPublic());
    }
    if (isset($changes['lazy'])) {
      $def->setLazy($definition->isLazy());
    }

    // Merge arguments.
    foreach ($definition->getArguments() as $k => $v) {
      if (is_numeric($k)) {
        $def->addArgument($v);
        continue;
      }

      if (0 !== strpos($k, 'index_')) {
        throw new RuntimeException(sprintf('Invalid argument key "%s" found.', $k));
      }

      $index = (int) substr($k, strlen('index_'));
      $def->replaceArgument($index, $v);
    }

    // Merge properties.
    foreach ($definition->getProperties() as $k => $v) {
      $def->setProperty($k, $v);
    }

    // Append method calls.
    if (count($calls = $definition->getMethodCalls()) > 0) {
      $def->setMethodCalls(array_merge($def->getMethodCalls(), $calls));
    }

    // These attributes are always taken from the child.
    $def->setAbstract($definition->isAbstract());
    $def->setScope($definition->getScope());
    $def->setTags($definition->getTags());

    // Set new definition on container.
    $this->container->setDefinition($id, $def);

    return $def;
  }

}
