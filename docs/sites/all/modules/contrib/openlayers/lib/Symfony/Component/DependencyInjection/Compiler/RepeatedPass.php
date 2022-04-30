<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;
use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;

/**
 * A pass that might be run repeatedly.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class RepeatedPass implements CompilerPassInterface {

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $repeat = FALSE;

  /**
   * FIX - insert comment here.
   *
   * @var RepeatablePassInterface[]
   */
  private $passes;

  /**
   * Constructor.
   *
   * @param RepeatablePassInterface[] $passes
   *   An array of RepeatablePassInterface objects.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When the passes don't implement RepeatablePassInterface.
   */
  public function __construct(array $passes) {
    foreach ($passes as $pass) {
      if (!$pass instanceof RepeatablePassInterface) {
        throw new InvalidArgumentException('$passes must be an array of RepeatablePassInterface.');
      }

      $pass->setRepeatedPass($this);
    }

    $this->passes = $passes;
  }

  /**
   * Process the repeatable passes that run more than once.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container) {
    $this->repeat = FALSE;
    foreach ($this->passes as $pass) {
      $pass->process($container);
    }

    if ($this->repeat) {
      $this->process($container);
    }
  }

  /**
   * Sets if the pass should repeat.
   */
  public function setRepeat() {
    $this->repeat = TRUE;
  }

  /**
   * Returns the passes.
   *
   * @return RepeatablePassInterface[]
   *   An array of RepeatablePassInterface objects.
   */
  public function getPasses() {
    return $this->passes;
  }

}
