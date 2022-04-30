<?php

namespace OpenlayersDrupal\Component\Annotation\Reflection;

use OpenlayersDoctrine\Common\Reflection\ClassFinderInterface;

/**
 * Defines a mock file finder that only returns a single filename.
 *
 * This can be used with
 * OpenlayersDoctrine\Common\Reflection\StaticReflectionParser if
 * the filename is known and inheritance is not a concern (for example, if
 * only the class annotation is needed).
 */
class MockFileFinder implements ClassFinderInterface {

  /**
   * The only filename this finder ever returns.
   *
   * @var string
   */
  protected $filename;

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDoctrine\Common\Reflection\ClassFinderInterface::findFile().
   */
  public function findFile($class) {
    return $this->filename;
  }

  /**
   * Creates new mock file finder objects.
   */
  public static function create($filename) {
    $object = new static();
    $object->filename = $filename;
    return $object;
  }

}
