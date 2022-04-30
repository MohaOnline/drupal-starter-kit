<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Exception;

/**
 * This exception is thrown when the a scope crossing injection is detected.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ScopeCrossingInjectionException extends RuntimeException {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $sourceServiceId;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $sourceScope;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $destServiceId;

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $destScope;

  /**
   * FIX - insert comment here.
   */
  public function __construct($sourceServiceId, $sourceScope, $destServiceId, $destScope, \Exception $previous = NULL) {
    parent::__construct(sprintf(
          'Scope Crossing Injection detected: The definition "%s" references the service "%s" which belongs to another scope hierarchy. '
         . 'This service might not be available consistently. Generally, it is safer to either move the definition "%s" to scope "%s", or '
         . 'declare "%s" as a child scope of "%s". If you can be sure that the other scope is always active, you can set the reference to strict=false to get rid of this error.',
         $sourceServiceId,
         $destServiceId,
         $sourceServiceId,
         $destScope,
         $sourceScope,
         $destScope
      ), 0, $previous);

    $this->sourceServiceId = $sourceServiceId;
    $this->sourceScope = $sourceScope;
    $this->destServiceId = $destServiceId;
    $this->destScope = $destScope;
  }

  /**
   * FIX - insert comment here.
   */
  public function getSourceServiceId() {
    return $this->sourceServiceId;
  }

  /**
   * FIX - insert comment here.
   */
  public function getSourceScope() {
    return $this->sourceScope;
  }

  /**
   * FIX - insert comment here.
   */
  public function getDestServiceId() {
    return $this->destServiceId;
  }

  /**
   * FIX - insert comment here.
   */
  public function getDestScope() {
    return $this->destScope;
  }

}
