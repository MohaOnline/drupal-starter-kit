<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

/**
 * This file is part of the Symfony package.
 *
 * (c) Fabien Potencier <fabien@symfony.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */
class Alias {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $id;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $public;

  /**
   * Constructor.
   *
   * @param string $id
   *   Alias identifier.
   * @param bool $public
   *   If this alias is public.
   */
  public function __construct($id, $public = TRUE) {
    $this->id = strtolower($id);
    $this->public = $public;
  }

  /**
   * Checks if this DI Alias should be public or not.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isPublic() {
    return $this->public;
  }

  /**
   * Sets if this Alias is public.
   *
   * @param bool $boolean
   *   If this Alias should be public.
   */
  public function setPublic($boolean) {
    $this->public = (bool) $boolean;
  }

  /**
   * Returns the Id of this alias.
   *
   * @return string
   *   The alias id.
   */
  public function __toString() {
    return $this->id;
  }

}
