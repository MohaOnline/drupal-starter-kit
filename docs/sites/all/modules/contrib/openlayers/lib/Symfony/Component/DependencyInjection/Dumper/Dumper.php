<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Dumper;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Dumper is the abstract class for all built-in dumpers.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
abstract class Dumper implements DumperInterface {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  protected $container;

  /**
   * Constructor.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   The service container to dump.
   *
   * @api
   */
  public function __construct(ContainerBuilder $container) {
    $this->container = $container;
  }

}
