<?php

namespace OpenlayersPsr\Log;

/**
 * Describes a logger-aware instance.
 */
interface LoggerAwareInterface {

  /**
   * Sets a logger instance on the object.
   *
   * @param LoggerInterface $logger
   *   FIX - insert comment here.
   *
   * @return null
   *   FIX - insert comment here.
   */
  public function setLogger(LoggerInterface $logger);

}
