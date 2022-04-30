<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

use OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder;

/**
 * Interface that must be implemented by compilation passes.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
interface CompilerPassInterface {

  /**
   * You can modify the container here before it is dumped to PHP code.
   *
   * @param \OpenlayersSymfony\Component\DependencyInjection\ContainerBuilder $container
   *   FIX - insert comment here.
   */
  public function process(ContainerBuilder $container);

}
