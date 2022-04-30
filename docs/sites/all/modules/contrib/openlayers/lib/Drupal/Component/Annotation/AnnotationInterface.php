<?php

namespace OpenlayersDrupal\Component\Annotation;

/**
 * Defines a common interface for classed annotations.
 */
interface AnnotationInterface {

  /**
   * Gets the value of an annotation.
   */
  public function get();

  /**
   * Gets the name of the provider of the annotated class.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getProvider();

  /**
   * Sets the name of the provider of the annotated class.
   *
   * @param string $provider
   *   FIX - insert comment here.
   */
  public function setProvider($provider);

  /**
   * Gets the unique ID for this annotated class.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getId();

  /**
   * Gets the class of the annotated class.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getClass();

  /**
   * Sets the class of the annotated class.
   *
   * @param string $class
   *   FIX - insert comment here.
   */
  public function setClass($class);

}
