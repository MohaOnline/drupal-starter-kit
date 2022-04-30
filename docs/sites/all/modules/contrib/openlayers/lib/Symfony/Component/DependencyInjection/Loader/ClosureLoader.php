<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Loader;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\Config\Loader\Loader;

/**
 * ClosureLoader loads service definitions from a PHP closure.
 *
 * The Closure has access to the container as its first argument.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class ClosureLoader extends Loader {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $container;

  /**
   * Constructor.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   A ContainerBuilder instance.
   */
  public function __construct(ContainerBuilder $container) {
    $this->container = $container;
  }

  /**
   * {@inheritdoc}
   */
  public function load($resource, $type = NULL) {
    call_user_func($resource, $this->container);
  }

  /**
   * {@inheritdoc}
   */
  public function supports($resource, $type = NULL) {
    return $resource instanceof \Closure;
  }

}
