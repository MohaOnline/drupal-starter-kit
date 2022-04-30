<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Loader;

use OpenlayersSymfony\Component\Config\Resource\FileResource;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;

/**
 * IniFileLoader loads parameters from INI files.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class IniFileLoader extends FileLoader {

  /**
   * {@inheritdoc}
   */
  public function load($resource, $type = NULL) {
    $path = $this->locator->locate($resource);

    $this->container->addResource(new FileResource($path));

    $result = parse_ini_file($path, TRUE);
    if (FALSE === $result || array() === $result) {
      throw new InvalidArgumentException(sprintf('The "%s" file is not valid.', $resource));
    }

    if (isset($result['parameters']) && is_array($result['parameters'])) {
      foreach ($result['parameters'] as $key => $value) {
        $this->container->setParameter($key, $value);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function supports($resource, $type = NULL) {
    return is_string($resource) && 'ini' === pathinfo($resource, PATHINFO_EXTENSION);
  }

}
