<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Compiler;

/**
 * FIX - insert comment here.
 *
 * Interface that must be implemented by passes that are run as part of an
 * RepeatedPass.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
interface RepeatablePassInterface extends CompilerPassInterface {

  /**
   * Sets the RepeatedPass interface.
   *
   * @param RepeatedPass $repeatedPass
   *   FIX - insert comment here.
   */
  public function setRepeatedPass(RepeatedPass $repeatedPass);

}
