<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * FIX - insert column here.
 *
 * ContainerAwareInterface should be implemented by classes that depends
 * on a Container.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
interface ContainerAwareInterface {

  /**
   * Sets the Container.
   *
   * @param ContainerInterface|null $container
   *   A ContainerInterface instance or null.
   */
  public function setContainer(ContainerInterface $container = NULL);

}
