<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\Alias;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;

/**
 * Sets a service to be an alias of another one, given a format pattern.
 */
class AutoAliasServicePass implements CompilerPassInterface {

  /**
   * {@inheritdoc}
   */
  public function process(ContainerBuilder $container) {
    foreach ($container->findTaggedServiceIds('auto_alias') as $serviceId => $tags) {
      foreach ($tags as $tag) {
        if (!isset($tag['format'])) {
          throw new InvalidArgumentException(sprintf('Missing tag information "format" on auto_alias service "%s".', $serviceId));
        }

        $aliasId = $container->getParameterBag()->resolveValue($tag['format']);
        if ($container->hasDefinition($aliasId) || $container->hasAlias($aliasId)) {
          $container->setAlias($serviceId, new Alias($aliasId));
        }
      }
    }
  }

}
