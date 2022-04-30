<?php

namespace OpenlayersSymfony\Component\DependencyInjection\ParameterBag;

/**
 * FIX - inset comment here.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
interface ParameterBagInterface {

  /**
   * Clears all parameters.
   */
  public function clear();

  /**
   * Adds parameters to the service container parameters.
   *
   * @param array $parameters
   *   An array of parameters.
   */
  public function add(array $parameters);

  /**
   * Gets the service container parameters.
   *
   * @return array
   *   An array of parameters.
   */
  public function all();

  /**
   * Gets a service container parameter.
   *
   * @param string $name
   *   The parameter name.
   *
   * @return mixed
   *   The parameter value.
   *
   * @throws ParameterNotFoundException
   *   If the parameter is not defined.
   */
  public function get($name);

  /**
   * Sets a service container parameter.
   *
   * @param string $name
   *   The parameter name.
   * @param mixed $value
   *   The parameter value.
   */
  public function set($name, $value);

  /**
   * Returns true if a parameter name is defined.
   *
   * @param string $name
   *   The parameter name.
   *
   * @return bool
   *   True if the parameter name is defined, false otherwise.
   */
  public function has($name);

  /**
   * FIX - insert comment here.
   *
   * Replaces parameter placeholders (%name%) by their values for all
   * parameters.
   */
  public function resolve();

  /**
   * Replaces parameter placeholders (%name%) by their values.
   *
   * @param mixed $value
   *   A value.
   *
   * @throws ParameterNotFoundException
   *   If a placeholder references a parameter that does not exist.
   */
  public function resolveValue($value);

  /**
   * Escape parameter placeholders %.
   *
   * @param mixed $value
   *   FIX - insert comment here.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  public function escapeValue($value);

  /**
   * Unescape parameter placeholders %.
   *
   * @param mixed $value
   *   FIX - insert comment here.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  public function unescapeValue($value);

}
