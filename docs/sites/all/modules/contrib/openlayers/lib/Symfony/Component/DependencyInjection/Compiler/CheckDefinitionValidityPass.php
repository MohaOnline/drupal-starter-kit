<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerInterface;
use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException;

/**
 * FIX - insert comment here.
 *
 * This pass validates each definition individually only taking the information
 * into account which is contained in the definition itself.
 *
 * Later passes can rely on the following, and specifically do not need to
 * perform these checks themselves:
 *
 * - non synthetic, non abstract services always have a class set
 * - synthetic services are always public
 * - synthetic services are always of non-prototype scope
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class CheckDefinitionValidityPass implements CompilerPassInterface {

  /**
   * Processes the ContainerBuilder to validate the Definition.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   *
   * @throws OpenlayersSymfony\Component\DependencyInjection\Exception\RuntimeException
   *   When the Definition is invalid.
   */
  public function process(ContainerBuilder $container) {
    foreach ($container->getDefinitions() as $id => $definition) {
      // Synthetic service is public.
      if ($definition->isSynthetic() && !$definition->isPublic()) {
        throw new RuntimeException(sprintf('A synthetic service ("%s") must be public.', $id));
      }

      // Synthetic service has non-prototype scope.
      if ($definition->isSynthetic() && ContainerInterface::SCOPE_PROTOTYPE === $definition->getScope()) {
        throw new RuntimeException(sprintf('A synthetic service ("%s") cannot be of scope "prototype".', $id));
      }

      if ($definition->getFactory() && ($definition->getFactoryClass(FALSE) || $definition->getFactoryService(FALSE) || $definition->getFactoryMethod(FALSE))) {
        throw new RuntimeException(sprintf('A service ("%s") can use either the old or the new factory syntax, not both.', $id));
      }

      // non-synthetic, non-abstract service has class.
      if (!$definition->isAbstract() && !$definition->isSynthetic() && !$definition->getClass()) {
        if ($definition->getFactory() || $definition->getFactoryClass(FALSE) || $definition->getFactoryService(FALSE)) {
          throw new RuntimeException(sprintf('Please add the class to service "%s" even if it is constructed by a factory since we might need to add method calls based on compile-time checks.', $id));
        }

        throw new RuntimeException(sprintf(
              'The definition for "%s" has no class. If you intend to inject '
             . 'this service dynamically at runtime, please mark it as synthetic=true. '
             . 'If this is an abstract definition solely used by child definitions, '
             . 'please add abstract=true, otherwise specify a class to get rid of this error.',
             $id
          ));
      }

      // Tag attribute values must be scalars.
      foreach ($definition->getTags() as $name => $tags) {
        foreach ($tags as $attributes) {
          foreach ($attributes as $attribute => $value) {
            if (!is_scalar($value) && NULL !== $value) {
              throw new RuntimeException(sprintf('A "tags" attribute must be of a scalar-type for service "%s", tag "%s", attribute "%s".', $id, $name, $attribute));
            }
          }
        }
      }
    }
  }

}
