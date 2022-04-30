<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Loader;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\Config\Loader\FileLoader as BaseFileLoader;
use OpenlayersSymfony\Component\Config\FileLocatorInterface;

/**
 * FIX - insert comment here.
 *
 * FileLoader is the abstract class used by all built-in loaders that are file
 * based.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
abstract class FileLoader extends BaseFileLoader {

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
   *   A ContainerBuilder instance.
   * @param \OpenlayersSymfony\Component\Config\FileLocatorInterface $locator
   *   A FileLocator instance.
   */
  public function __construct(ContainerBuilder $container, FileLocatorInterface $locator) {
    $this->container = $container;

    parent::__construct($locator);
  }

}
