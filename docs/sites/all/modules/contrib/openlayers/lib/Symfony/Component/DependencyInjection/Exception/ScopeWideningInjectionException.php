<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Exception;

/**
 * Thrown when a scope widening injection is detected.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ScopeWideningInjectionException extends RuntimeException {

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
          'Scope Widening Injection detected: The definition "%s" references the service "%s" which belongs to a narrower scope. '
         . 'Generally, it is safer to either move "%s" to scope "%s" or alternatively rely on the provider pattern by injecting the container itself, and requesting the service "%s" each time it is needed. '
         . 'In rare, special cases however that might not be necessary, then you can set the reference to strict=false to get rid of this error.',
         $sourceServiceId,
         $destServiceId,
         $sourceServiceId,
         $destScope,
         $destServiceId
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
