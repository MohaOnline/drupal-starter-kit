<?php

namespace OpenlayersDrupal\Component\Uuid;

/**
 * Interface that defines a UUID backend.
 */
interface UuidInterface {

  /**
   * Generates a Universally Unique IDentifier (UUID).
   */
  public function generate();

}
