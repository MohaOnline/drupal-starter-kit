<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Dumper;

/**
 * The interface implemented by service container dumper classes.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
interface DumperInterface {

  /**
   * Dumps the service container.
   *
   * @param array $options
   *   An array of options.
   *
   * @return string
   *   The representation of the service container.
   */
  public function dump(array $options = array());

}
