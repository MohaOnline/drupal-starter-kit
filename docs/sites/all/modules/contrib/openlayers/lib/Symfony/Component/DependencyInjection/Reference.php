<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * Reference represents a service reference.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
class Reference {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $id;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $invalidBehavior;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $strict;

  /**
   * Constructor.
   *
   * @param string $id
   *   The service identifier.
   * @param int $invalidBehavior
   *   The behavior when the service does not exist.
   * @param bool $strict
   *   Sets how this reference is validated.
   */
  public function __construct($id, $invalidBehavior = ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE, $strict = TRUE) {
    $this->id = strtolower($id);
    $this->invalidBehavior = $invalidBehavior;
    $this->strict = $strict;
  }

  /**
   * Creates __toString.
   *
   * @return string
   *   The service identifier.
   */
  public function __toString() {
    return $this->id;
  }

  /**
   * Returns the behavior to be used when the service does not exist.
   *
   * @return int
   *   FIX - inset comment here.
   */
  public function getInvalidBehavior() {
    return $this->invalidBehavior;
  }

  /**
   * Returns true when this Reference is strict.
   *
   * @return bool
   *   FIX - inset comment here.
   */
  public function isStrict() {
    return $this->strict;
  }

}
