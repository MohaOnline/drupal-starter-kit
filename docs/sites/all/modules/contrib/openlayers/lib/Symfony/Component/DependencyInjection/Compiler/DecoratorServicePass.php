<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Alias;

/**
 * Overwrites a service but keeps the overridden one.
 *
 * @author Christophe Coevoet <stof@notk.org>
 * @author Fabien Potencier <fabien@symfony.com>
 */
class DecoratorServicePass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    foreach ($container->getDefinitions() as $id => $definition) {
      if (!$decorated = $definition->getDecoratedService()) {
        continue;
      }
      $definition->setDecoratedService(NULL);

      list($inner, $renamedId) = $decorated;
      if (!$renamedId) {
        $renamedId = $id . '.inner';
      }

      // We create a new alias/service for the service we are replacing
      // to be able to reference it in the new one.
      if ($container->hasAlias($inner)) {
        $alias = $container->getAlias($inner);
        $public = $alias->isPublic();
        $container->setAlias($renamedId, new Alias((string) $alias, FALSE));
      }
      else {
        $definition = $container->getDefinition($inner);
        $public = $definition->isPublic();
        $definition->setPublic(FALSE);
        $container->setDefinition($renamedId, $definition);
      }

      $container->setAlias($inner, new Alias($id, $public));
    }
  }

}
