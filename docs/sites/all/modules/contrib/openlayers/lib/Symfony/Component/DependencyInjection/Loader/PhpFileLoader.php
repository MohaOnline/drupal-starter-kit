<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Loader;

use OpenlayersSymfony\Component\Config\Resource\FileResource;

/**
 * PhpFileLoader loads service definitions from a PHP file.
 *
 * The PHP file is required and the $container variable can be
 * used within the file to change the container.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class PhpFileLoader extends FileLoader {

  /**
   * {@inheritdoc}
   */
  public function load($resource, $type = NULL) {
    // The container and loader variables are exposed to the included file
    // below.
    $container = $this->container;
    $loader = $this;

    $path = $this->locator->locate($resource);
    $this->setCurrentDir(dirname($path));
    $this->container->addResource(new FileResource($path));

    include $path;
  }

  /**
   * {@inheritdoc}
   */
  public function supports($resource, $type = NULL) {
    return is_string($resource) && 'php' === pathinfo($resource, PATHINFO_EXTENSION);
  }

}
