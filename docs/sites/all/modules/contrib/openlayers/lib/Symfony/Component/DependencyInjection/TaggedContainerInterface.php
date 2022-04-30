<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * The interface implemented when a container knows how to deals with tags.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
interface TaggedContainerInterface extends ContainerInterface {

  /**
   * Returns service ids for a given tag.
   *
   * @param string $name
   *   The tag name.
   *
   * @return array
   *   An array of tags.
   */
  public function findTaggedServiceIds($name);

}
