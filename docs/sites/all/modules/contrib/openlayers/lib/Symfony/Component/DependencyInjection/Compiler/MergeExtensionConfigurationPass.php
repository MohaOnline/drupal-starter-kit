<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Extension\PrependExtensionInterface;

/**
 * Merges extension configs into the container builder.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class MergeExtensionConfigurationPass implements CompilerPassInterface {

  /**
   * {@inheritdoc}
   */
  public function process(ContainerBuilder $container) {
    $parameters = $container->getParameterBag()->all();
    $definitions = $container->getDefinitions();
    $aliases = $container->getAliases();
    $exprLangProviders = $container->getExpressionLanguageProviders();

    foreach ($container->getExtensions() as $extension) {
      if ($extension instanceof PrependExtensionInterface) {
        $extension->prepend($container);
      }
    }

    foreach ($container->getExtensions() as $name => $extension) {
      if (!$config = $container->getExtensionConfig($name)) {
        // This extension was not called.
        continue;
      }
      $config = $container->getParameterBag()->resolveValue($config);

      $tmpContainer = new ContainerBuilder($container->getParameterBag());
      $tmpContainer->setResourceTracking($container->isTrackingResources());
      $tmpContainer->addObjectResource($extension);

      foreach ($exprLangProviders as $provider) {
        $tmpContainer->addExpressionLanguageProvider($provider);
      }

      $extension->load($config, $tmpContainer);

      $container->merge($tmpContainer);
      $container->getParameterBag()->add($parameters);
    }

    $container->addDefinitions($definitions);
    $container->addAliases($aliases);
  }

}
