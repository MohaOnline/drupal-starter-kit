<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * ContainerAware trait.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
trait ContainerAwareTrait {

  /**
   * FIX - insert comment here.
   *
   * @var ContainerInterface
   */
  protected $container;

  /**
   * Sets the Container associated with this Controller.
   *
   * @param ContainerInterface $container
   *   A ContainerInterface instance.
   */
  public function setContainer(ContainerInterface $container = NULL) {
    $this->container = $container;
  }

}
