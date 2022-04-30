<?php

namespace OpenlayersDoctrine\Common\Reflection;

/**
 * Finds a class in a PSR-0 structure.
 *
 * @author Karoly Negyesi <karoly@negyesi.net>
 */
class Psr0FindFile implements ClassFinderInterface {

  /**
   * The PSR-0 prefixes.
   *
   * @var array
   */
  protected $prefixes;

  /**
   * FIX - insert comment here.
   *
   * @param array $prefixes
   *   An array of prefixes. Each key is a PHP namespace and each value is
   *   a list of directories.
   */
  public function __construct(array $prefixes) {
    $this->prefixes = $prefixes;
  }

  /**
   * FIX - insert comment here.
   */
  public function findFile($class) {
    $lastNsPos = strrpos($class, '\\');
    if ('\\' == $class[0]) {
      $class = substr($class, 1);
    }

    if (FALSE !== $lastNsPos) {
      // Namespaced class name.
      $classPath = str_replace('\\', DIRECTORY_SEPARATOR, substr($class, 0, $lastNsPos)) . DIRECTORY_SEPARATOR;
      $className = substr($class, $lastNsPos + 1);
    }
    else {
      // PEAR-like class name.
      $classPath = NULL;
      $className = $class;
    }

    $classPath .= str_replace('_', DIRECTORY_SEPARATOR, $className) . '.php';

    foreach ($this->prefixes as $prefix => $dirs) {
      if (0 === strpos($class, $prefix)) {
        foreach ($dirs as $dir) {
          if (is_file($dir . DIRECTORY_SEPARATOR . $classPath)) {
            return $dir . DIRECTORY_SEPARATOR . $classPath;
          }
        }
      }
    }

    return NULL;
  }

}
