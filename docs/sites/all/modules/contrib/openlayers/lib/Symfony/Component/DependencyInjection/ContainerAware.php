<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * A simple implementation of ContainerAwareInterface.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
abstract class ContainerAware implements ContainerAwareInterface {

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
