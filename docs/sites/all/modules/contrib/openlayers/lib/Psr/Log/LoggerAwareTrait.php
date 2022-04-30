<?php

namespace OpenlayersPsr\Log;

/**
 * Basic Implementation of LoggerAwareInterface.
 */
trait LoggerAwareTrait {

  /**
   * FIX - insert comment here.
   *
   * @var LoggerInterface
   */
  protected $logger;

  /**
   * Sets a logger.
   *
   * @param LoggerInterface $logger
   *   FIX - insert comment here.
   */
  public function setLogger(LoggerInterface $logger) {
    $this->logger = $logger;
  }

}
