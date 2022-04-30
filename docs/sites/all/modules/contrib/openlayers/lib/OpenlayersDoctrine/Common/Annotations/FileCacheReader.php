<?php

namespace OpenlayersDoctrine\Common\Annotations;

/**
 * File cache reader for annotations.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 * @author Benjamin Eberlei <kontakt@beberlei.de>
 */
class FileCacheReader implements Reader {

  /**
   * FIX - insert comment here.
   *
   * @var Reader
   */
  private $reader;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $dir;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private $debug;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $loadedAnnotations = array();

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $classNameHashes = array();

  /**
   * Constructor.
   *
   * @param Reader $reader
   *   FIX - insert comment here.
   * @param string $cacheDir
   *   FIX - insert comment here.
   * @param bool $debug
   *   FIX - insert comment here.
   *
   * @throws \InvalidArgumentException
   *   FIX - insert comment here.
   */
  public function __construct(Reader $reader, $cacheDir, $debug = FALSE) {
    $this->reader = $reader;
    if (!is_dir($cacheDir) && !@mkdir($cacheDir, 0777, TRUE)) {
      throw new \InvalidArgumentException(sprintf('The directory "%s" does not exist and could not be created.', $cacheDir));
    }

    $this->dir   = rtrim($cacheDir, '\\/');
    $this->debug = $debug;
  }

  /**
   * FIX - insert comment here.
   */
  public function getClassAnnotations(\ReflectionClass $class) {
    if (!isset($this->classNameHashes[$class->name])) {
      $this->classNameHashes[$class->name] = sha1($class->name);
    }
    $key = $this->classNameHashes[$class->name];

    if (isset($this->loadedAnnotations[$key])) {
      return $this->loadedAnnotations[$key];
    }

    $path = $this->dir . '/' . strtr($key, '\\', '-') . '.cache.php';
    if (!is_file($path)) {
      $annot = $this->reader->getClassAnnotations($class);
      $this->saveCacheFile($path, $annot);
      return $this->loadedAnnotations[$key] = $annot;
    }

    if ($this->debug
        && (FALSE !== $filename = $class->getFilename())
        && filemtime($path) < filemtime($filename)) {
      @unlink($path);

      $annot = $this->reader->getClassAnnotations($class);
      $this->saveCacheFile($path, $annot);
      return $this->loadedAnnotations[$key] = $annot;
    }

    return $this->loadedAnnotations[$key] = include $path;
  }

  /**
   * FIX - insert comment here.
   */
  public function getPropertyAnnotations(\ReflectionProperty $property) {
    $class = $property->getDeclaringClass();
    if (!isset($this->classNameHashes[$class->name])) {
      $this->classNameHashes[$class->name] = sha1($class->name);
    }
    $key = $this->classNameHashes[$class->name] . '$' . $property->getName();

    if (isset($this->loadedAnnotations[$key])) {
      return $this->loadedAnnotations[$key];
    }

    $path = $this->dir . '/' . strtr($key, '\\', '-') . '.cache.php';
    if (!is_file($path)) {
      $annot = $this->reader->getPropertyAnnotations($property);
      $this->saveCacheFile($path, $annot);
      return $this->loadedAnnotations[$key] = $annot;
    }

    if ($this->debug
        && (FALSE !== $filename = $class->getFilename())
        && filemtime($path) < filemtime($filename)) {
      @unlink($path);

      $annot = $this->reader->getPropertyAnnotations($property);
      $this->saveCacheFile($path, $annot);
      return $this->loadedAnnotations[$key] = $annot;
    }

    return $this->loadedAnnotations[$key] = include $path;
  }

  /**
   * FIX - insert comment here.
   */
  public function getMethodAnnotations(\ReflectionMethod $method) {
    $class = $method->getDeclaringClass();
    if (!isset($this->classNameHashes[$class->name])) {
      $this->classNameHashes[$class->name] = sha1($class->name);
    }
    $key = $this->classNameHashes[$class->name] . '#' . $method->getName();

    if (isset($this->loadedAnnotations[$key])) {
      return $this->loadedAnnotations[$key];
    }

    $path = $this->dir . '/' . strtr($key, '\\', '-') . '.cache.php';
    if (!is_file($path)) {
      $annot = $this->reader->getMethodAnnotations($method);
      $this->saveCacheFile($path, $annot);
      return $this->loadedAnnotations[$key] = $annot;
    }

    if ($this->debug
        && (FALSE !== $filename = $class->getFilename())
        && filemtime($path) < filemtime($filename)) {
      @unlink($path);

      $annot = $this->reader->getMethodAnnotations($method);
      $this->saveCacheFile($path, $annot);
      return $this->loadedAnnotations[$key] = $annot;
    }

    return $this->loadedAnnotations[$key] = include $path;
  }

  /**
   * Saves the cache file.
   *
   * @param string $path
   *   FIX - insert comment here.
   * @param mixed $data
   *   FIX - insert comment here.
   */
  private function saveCacheFile($path, $data) {
    if (!is_writable($this->dir)) {
      throw new \InvalidArgumentException(sprintf('The directory "%s" is not writable. Both, the webserver and the console user need access. You can manage access rights for multiple users with "chmod +a". If your system does not support this, check out the acl package.', $this->dir));
    }
    file_put_contents($path, '<?php return unserialize(' . var_export(serialize($data), TRUE) . ');');
  }

  /**
   * FIX - insert comment here.
   */
  public function getClassAnnotation(\ReflectionClass $class, $annotationName) {
    $annotations = $this->getClassAnnotations($class);

    foreach ($annotations as $annotation) {
      if ($annotation instanceof $annotationName) {
        return $annotation;
      }
    }

    return NULL;
  }

  /**
   * FIX - insert comment here.
   */
  public function getMethodAnnotation(\ReflectionMethod $method, $annotationName) {
    $annotations = $this->getMethodAnnotations($method);

    foreach ($annotations as $annotation) {
      if ($annotation instanceof $annotationName) {
        return $annotation;
      }
    }

    return NULL;
  }

  /**
   * FIX - insert comment here.
   */
  public function getPropertyAnnotation(\ReflectionProperty $property, $annotationName) {
    $annotations = $this->getPropertyAnnotations($property);

    foreach ($annotations as $annotation) {
      if ($annotation instanceof $annotationName) {
        return $annotation;
      }
    }

    return NULL;
  }

  /**
   * Clears loaded annotations.
   */
  public function clearLoadedAnnotations() {
    $this->loadedAnnotations = array();
  }

}
