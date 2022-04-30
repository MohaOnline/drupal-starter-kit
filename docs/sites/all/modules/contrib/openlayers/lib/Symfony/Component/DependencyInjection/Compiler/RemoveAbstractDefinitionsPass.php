<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Removes abstract Definitions.
 */
class RemoveAbstractDefinitionsPass implements CompilerPassInterface {

  /**
   * Removes abstract definitions from the ContainerBuilder.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $compiler = $container->getCompiler();
    $formatter = $compiler->getLoggingFormatter();

    foreach ($container->getDefinitions() as $id => $definition) {
      if ($definition->isAbstract()) {
        $container->removeDefinition($id);
        $compiler->addLogMessage($formatter->formatRemoveService($this, $id, 'abstract'));
      }
    }
  }

}
