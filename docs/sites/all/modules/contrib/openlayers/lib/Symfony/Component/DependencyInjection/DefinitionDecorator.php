<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

use OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException;
use OpenlayersSymfony\Component\DependencyInjection\Exception\OutOfBoundsException;

/**
 * This definition decorates another definition.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class DefinitionDecorator extends Definition {

  /**
   * FIX - insert comment here.
   *
   * @var object
   */
  private $parent;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $changes = array();

  /**
   * Constructor.
   *
   * @param string $parent
   *   The id of Definition instance to decorate.
   */
  public function __construct($parent) {
    parent::__construct();

    $this->parent = $parent;
  }

  /**
   * Returns the Definition being decorated.
   *
   * @return string
   *   FIX - insert comment here
   */
  public function getParent() {
    return $this->parent;
  }

  /**
   * Returns all changes tracked for the Definition object.
   *
   * @return array
   *   An array of changes for this Definition.
   */
  public function getChanges() {
    return $this->changes;
  }

  /**
   * FIX - insert comment here.
   */
  public function setClass($class) {
    $this->changes['class'] = TRUE;

    return parent::setClass($class);
  }

  /**
   * FIX - insert comment here.
   */
  public function setFactory($callable) {
    $this->changes['factory'] = TRUE;

    return parent::setFactory($callable);
  }

  /**
   * FIX - insert comment here.
   */
  public function setFactoryClass($class) {
    $this->changes['factory_class'] = TRUE;

    return parent::setFactoryClass($class);
  }

  /**
   * FIX - insert comment here.
   */
  public function setFactoryMethod($method) {
    $this->changes['factory_method'] = TRUE;

    return parent::setFactoryMethod($method);
  }

  /**
   * FIX - insert comment here.
   */
  public function setFactoryService($service) {
    $this->changes['factory_service'] = TRUE;

    return parent::setFactoryService($service);
  }

  /**
   * FIX - insert comment here.
   */
  public function setConfigurator($callable) {
    $this->changes['configurator'] = TRUE;

    return parent::setConfigurator($callable);
  }

  /**
   * FIX - insert comment here.
   */
  public function setFile($file) {
    $this->changes['file'] = TRUE;

    return parent::setFile($file);
  }

  /**
   * FIX - insert comment here.
   */
  public function setPublic($boolean) {
    $this->changes['public'] = TRUE;

    return parent::setPublic($boolean);
  }

  /**
   * FIX - insert comment here.
   */
  public function setLazy($boolean) {
    $this->changes['lazy'] = TRUE;

    return parent::setLazy($boolean);
  }

  /**
   * Gets an argument to pass to the service constructor/factory method.
   *
   * If replaceArgument() has been used to replace an argument, this method
   * will return the replacement value.
   *
   * @param int $index
   *   FIX - insert comment here.
   *
   * @return mixed
   *   The argument value.
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\OutOfBoundsException
   *   When the argument does not exist.
   */
  public function getArgument($index) {
    if (array_key_exists('index_' . $index, $this->arguments)) {
      return $this->arguments['index_' . $index];
    }

    $lastIndex = count(array_filter(array_keys($this->arguments), 'is_int')) - 1;

    if ($index < 0 || $index > $lastIndex) {
      throw new OutOfBoundsException(sprintf('The index "%d" is not in the range [0, %d].', $index, $lastIndex));
    }

    return $this->arguments[$index];
  }

  /**
   * FIX - insert comment here.
   *
   * You should always use this method when overwriting existing arguments
   * of the parent definition.
   *
   * If you directly call setArguments() keep in mind that you must follow
   * certain conventions when you want to overwrite the arguments of the
   * parent definition, otherwise your arguments will only be appended.
   *
   * @param int $index
   *   FIX - insert comment here.
   * @param mixed $value
   *   FIX - insert comment here.
   *
   * @return DefinitionDecorator
   *   The current instance
   *
   * @throws \OpenlayersSymfony\Component\DependencyInjection\Exception\InvalidArgumentException
   *   When $index isn't an integer.
   */
  public function replaceArgument($index, $value) {
    if (!is_int($index)) {
      throw new InvalidArgumentException('$index must be an integer.');
    }

    $this->arguments['index_' . $index] = $value;

    return $this;
  }

}
