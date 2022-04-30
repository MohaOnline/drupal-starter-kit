<?php

namespace OpenlayersDoctrine\Common\Reflection;

use OpenlayersDoctrine\Common\Proxy\Proxy;

/**
 * FIX - insert comment here.
 *
 * PHP Runtime Reflection Public Property - special overrides for public
 * properties.
 *
 * @author Marco Pivetta <ocramius@gmail.com>
 *
 * @since 2.4
 */
class RuntimePublicReflectionProperty extends ReflectionProperty {

  /**
   * FIX - insert comment here.
   *
   * Checks is the value actually exist before fetching it.
   * This is to avoid calling `__get` on the provided $object if it
   * is a {@see \OpenlayersDoctrine\Common\Proxy\Proxy}.
   */
  public function getValue($object = NULL) {
    $name = $this->getName();

    if ($object instanceof Proxy && !$object->__isInitialized()) {
      $originalInitializer = $object->__getInitializer();
      $object->__setInitializer(NULL);
      $val = isset($object->$name) ? $object->$name : NULL;
      $object->__setInitializer($originalInitializer);

      return $val;
    }

    return isset($object->$name) ? parent::getValue($object) : NULL;
  }

  /**
   * FIX - insert comment here.
   *
   * Avoids triggering lazy loading via `__set` if the provided object
   * is a {@see \OpenlayersDoctrine\Common\Proxy\Proxy}.
   * @link https://bugs.php.net/bug.php?id=63463
   */
  public function setValue($object, $value = NULL) {
    if (!($object instanceof Proxy && !$object->__isInitialized())) {
      parent::setValue($object, $value);

      return;
    }

    $originalInitializer = $object->__getInitializer();
    $object->__setInitializer(NULL);
    parent::setValue($object, $value);
    $object->__setInitializer($originalInitializer);
  }

}
