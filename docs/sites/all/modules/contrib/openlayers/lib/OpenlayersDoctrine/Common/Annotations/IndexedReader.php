<?php

namespace OpenlayersDoctrine\Common\Annotations;

/**
 * Allows the reader to be used in-place of Doctrine's reader.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class IndexedReader implements Reader {

  /**
   * FIX - insert comment here.
   *
   * @var Reader
   */
  private $delegate;

  /**
   * Constructor.
   *
   * @param Reader $reader
   *   FIX - insert comment here.
   */
  public function __construct(Reader $reader) {
    $this->delegate = $reader;
  }

  /**
   * FIX - insert comment here.
   */
  public function getClassAnnotations(\ReflectionClass $class) {
    $annotations = array();
    foreach ($this->delegate->getClassAnnotations($class) as $annot) {
      $annotations[get_class($annot)] = $annot;
    }

    return $annotations;
  }

  /**
   * FIX - insert comment here.
   */
  public function getClassAnnotation(\ReflectionClass $class, $annotation) {
    return $this->delegate->getClassAnnotation($class, $annotation);
  }

  /**
   * FIX - insert comment here.
   */
  public function getMethodAnnotations(\ReflectionMethod $method) {
    $annotations = array();
    foreach ($this->delegate->getMethodAnnotations($method) as $annot) {
      $annotations[get_class($annot)] = $annot;
    }

    return $annotations;
  }

  /**
   * FIX - insert comment here.
   */
  public function getMethodAnnotation(\ReflectionMethod $method, $annotation) {
    return $this->delegate->getMethodAnnotation($method, $annotation);
  }

  /**
   * FIX - insert comment here.
   */
  public function getPropertyAnnotations(\ReflectionProperty $property) {
    $annotations = array();
    foreach ($this->delegate->getPropertyAnnotations($property) as $annot) {
      $annotations[get_class($annot)] = $annot;
    }

    return $annotations;
  }

  /**
   * FIX - insert comment here.
   */
  public function getPropertyAnnotation(\ReflectionProperty $property, $annotation) {
    return $this->delegate->getPropertyAnnotation($property, $annotation);
  }

  /**
   * Proxies all methods to the delegate.
   *
   * @param string $method
   *   FIX - insert comment here.
   * @param array $args
   *   FIX - insert comment here.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  public function __call($method, array $args) {
    return call_user_func_array(array($this->delegate, $method), $args);
  }

}
