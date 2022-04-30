<?php

namespace OpenlayersDoctrine\Common\Reflection;

/**
 * FIX - insert comment here.
 */
interface ReflectionProviderInterface {

  /**
   * Gets the ReflectionClass equivalent for this class.
   *
   * @return \ReflectionClass
   *   FIX - insert comment here.
   */
  public function getReflectionClass();

  /**
   * Gets the ReflectionMethod equivalent for this class.
   *
   * @param string $name
   *   FIX - insert comment here.
   *
   * @return \ReflectionMethod
   *   FIX - insert comment here.
   */
  public function getReflectionMethod($name);

  /**
   * Gets the ReflectionProperty equivalent for this class.
   *
   * @param string $name
   *   FIX - insert comment here.
   *
   * @return \ReflectionProperty
   *   FIX - insert comment here.
   */
  public function getReflectionProperty($name);

}
