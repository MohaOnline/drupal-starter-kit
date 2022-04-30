<?php

namespace OpenlayersDrupal\Component\Discovery;

/**
 * Interface for classes providing a type of discovery.
 */
interface DiscoverableInterface {

  /**
   * Returns an array of discoverable items.
   *
   * @return array
   *   An array of discovered data keyed by provider.
   */
  public function findAll();

}
