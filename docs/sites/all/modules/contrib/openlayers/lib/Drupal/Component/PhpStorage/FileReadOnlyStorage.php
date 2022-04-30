<?php

namespace OpenlayersDrupal\Component\PhpStorage;

/**
 * Reads code as regular PHP files, but won't write them.
 */
class FileReadOnlyStorage implements PhpStorageInterface {

  /**
   * The directory where the files should be stored.
   *
   * @var string
   */
  protected $directory;

  /**
   * Constructs this FileStorage object.
   *
   * @param array $configuration
   *   An associative array, containing at least two keys (the rest are
   *   ignored):
   *   - directory: The directory where the files should be stored.
   *   - bin: The storage bin. Multiple storage objects can be instantiated with
   *   the same configuration, but for different bins.
   */
  public function __construct(array $configuration) {

    $this->directory = $configuration['directory'] . '/' . $configuration['bin'];
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Component\PhpStorage\PhpStorageInterface::exists().
   */
  public function exists($name) {
    return file_exists($this->getFullPath($name));
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Component\PhpStorage\PhpStorageInterface::load().
   */
  public function load($name) {
    // The FALSE returned on failure is enough for the caller to handle this,
    // we do not want a warning too.
    return (@include_once $this->getFullPath($name)) !== FALSE;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Component\PhpStorage\PhpStorageInterface::save().
   */
  public function save($name, $code) {
    return FALSE;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Component\PhpStorage\PhpStorageInterface::delete().
   */
  public function delete($name) {
    return FALSE;
  }

  /**
   * FIX - insert comment here.
   */
  public function getFullPath($name) {
    return $this->directory . '/' . $name;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Component\PhpStorage\PhpStorageInterface::writeable().
   */
  public function writeable() {
    return FALSE;
  }

  /**
   * FIX - insert comment here.
   *
   * Implements
   * OpenlayersDrupal\Component\PhpStorage\PhpStorageInterface::deleteAll().
   */
  public function deleteAll() {
    return FALSE;
  }

  /**
   * FIX - insert comment here.
   */
  public function listAll() {
    $names = array();
    if (file_exists($this->directory)) {
      foreach (new \DirectoryIterator($this->directory) as $fileinfo) {
        if (!$fileinfo->isDot()) {
          $name = $fileinfo->getFilename();
          if ($name != '.htaccess') {
            $names[] = $name;
          }
        }
      }
    }
    return $names;
  }

}
