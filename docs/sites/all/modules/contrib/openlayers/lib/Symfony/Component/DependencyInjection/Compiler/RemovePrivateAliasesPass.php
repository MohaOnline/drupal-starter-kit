<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * FIX - insert comment here.
 *
 * Remove private aliases from the container. They were only used to establish
 * dependencies between services, and these dependencies have been resolved in
 * one of the previous passes.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class RemovePrivateAliasesPass implements CompilerPassInterface {

  /**
   * Removes private aliases from the ContainerBuilder.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $compiler = $container->getCompiler();
    $formatter = $compiler->getLoggingFormatter();

    foreach ($container->getAliases() as $id => $alias) {
      if ($alias->isPublic()) {
        continue;
      }

      $container->removeAlias($id);
      $compiler->addLogMessage($formatter->formatRemoveService($this, $id, 'private alias'));
    }
  }

}
