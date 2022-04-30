<?php

namespace OpenlayersSymfony\Component\DependencyInjection;

trigger_error('The ' . __NAMESPACE__ . '\SimpleXMLElement method is deprecated since version 2.5 and will be removed in 3.0.', E_USER_DEPRECATED);

use OpenlayersSymfony\Component\Config\Util\XmlUtils;
use OpenlayersSymfony\Component\ExpressionLanguage\Expression;

/**
 * FIX - insert comment here.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class SimpleXMLElement extends \SimpleXMLElement {

  /**
   * Converts an attribute as a PHP type.
   *
   * @param string $name
   *   FIX - insert comment here.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  public function getAttributeAsPhp($name) {
    return self::phpize($this[$name]);
  }

  /**
   * Returns arguments as valid PHP types.
   *
   * @param string $name
   *   FIX - insert comment here.
   * @param bool $lowercase
   *   FIX - insert comment here.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  public function getArgumentsAsPhp($name, $lowercase = TRUE) {
    $arguments = array();
    foreach ($this->$name as $arg) {
      if (isset($arg['name'])) {
        $arg['key'] = (string) $arg['name'];
      }
      $key = isset($arg['key']) ? (string) $arg['key'] : (!$arguments ? 0 : max(array_keys($arguments)) + 1);

      // Parameter keys are case insensitive.
      if ('parameter' == $name && $lowercase) {
        $key = strtolower($key);
      }

      // This is used by DefinitionDecorator to overwrite a specific
      // argument of the parent definition.
      if (isset($arg['index'])) {
        $key = 'index_' . $arg['index'];
      }

      switch ($arg['type']) {
        case 'service':
          $invalidBehavior = ContainerInterface::EXCEPTION_ON_INVALID_REFERENCE;
          if (isset($arg['on-invalid']) && 'ignore' == $arg['on-invalid']) {
            $invalidBehavior = ContainerInterface::IGNORE_ON_INVALID_REFERENCE;
          }
          elseif (isset($arg['on-invalid']) && 'null' == $arg['on-invalid']) {
            $invalidBehavior = ContainerInterface::NULL_ON_INVALID_REFERENCE;
          }

          if (isset($arg['strict'])) {
            $strict = self::phpize($arg['strict']);
          }
          else {
            $strict = TRUE;
          }

          $arguments[$key] = new Reference((string) $arg['id'], $invalidBehavior, $strict);
          break;

        case 'expression':
          $arguments[$key] = new Expression((string) $arg);
          break;

        case 'collection':
          $arguments[$key] = $arg->getArgumentsAsPhp($name, FALSE);
          break;

        case 'string':
          $arguments[$key] = (string) $arg;
          break;

        case 'constant':
          $arguments[$key] = constant((string) $arg);
          break;

        default:
          $arguments[$key] = self::phpize($arg);
      }
    }

    return $arguments;
  }

  /**
   * Converts an xml value to a PHP type.
   *
   * @param mixed $value
   *   FIX - insert comment here.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  public static function phpize($value) {
    return XmlUtils::phpize($value);
  }

}
