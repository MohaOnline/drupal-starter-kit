<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * FIX - insert comment here.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class Scope implements ScopeInterface {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $name;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $parentName;

  /**
   * FIX - insert comment here.
   */
  public function __construct($name, $parentName = ContainerInterface::SCOPE_CONTAINER) {
    $this->name = $name;
    $this->parentName = $parentName;
  }

  /**
   * FIX - insert comment here.
   */
  public function getName() {
    return $this->name;
  }

  /**
   * FIX - insert comment here.
   */
  public function getParentName() {
    return $this->parentName;
  }

}
