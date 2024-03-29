<?php

namespace OpenlayersSymfony\Component\Yaml;

use OpenlayersSymfony\Component\Yaml\Exception\ParseException;

/**
 * Yaml offers convenience methods to load and dump YAML.
 *
 * This file is part of the Symfony package.
 *
 * (c) Fabien Potencier <fabien@symfony.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 *
 * @api
 */
class Yaml {

  /**
   * Parses YAML into a PHP array.
   *
   * The parse method, when supplied with a YAML stream (string or file),
   * will do its best to convert YAML in a file into a PHP array.
   *
   *  Usage:
   *  <code>
   *   $array = Yaml::parse(file_get_contents('config.yml'));
   *   print_r($array);
   *  </code>
   *
   * As this method accepts both plain strings and file names as an input,
   * you must validate the input before calling this method. Passing a file
   * as an input is a deprecated feature and will be removed in 3.0.
   *
   * Note: the ability to pass file names to the Yaml::parse method is
   * deprecated since version 2.2 and will be removed in 3.0. Pass the
   * YAML contents of the file instead.
   *
   * @param string $input
   *   Path to a YAML file or a string containing YAML.
   * @param bool $exceptionOnInvalidType
   *   True if an exception must be thrown on invalid types false otherwise.
   * @param bool $objectSupport
   *   True if object support is enabled, false otherwise.
   * @param bool $objectForMap
   *   True if maps should return a stdClass instead of array.
   *
   * @return array
   *   The YAML converted to a PHP array
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\ParseException
   *   If the YAML is not valid.
   */
  public static function parse($input, $exceptionOnInvalidType = FALSE, $objectSupport = FALSE, $objectForMap = FALSE) {
    // If input is a file, process it.
    $file = '';
    if (strpos($input, "\n") === FALSE && is_file($input)) {
      trigger_error('The ability to pass file names to the ' . __METHOD__ . ' method is deprecated since version 2.2 and will be removed in 3.0. Pass the YAML contents of the file instead.', E_USER_DEPRECATED);

      if (FALSE === is_readable($input)) {
        throw new ParseException(sprintf('Unable to parse "%s" as the file is not readable.', $input));
      }

      $file = $input;
      $input = file_get_contents($file);
    }

    $yaml = new Parser();

    try {
      return $yaml->parse($input, $exceptionOnInvalidType, $objectSupport, $objectForMap);
    }
    catch (ParseException $e) {
      if ($file) {
        $e->setParsedFile($file);
      }

      throw $e;
    }
  }

  /**
   * Dumps a PHP array to a YAML string.
   *
   * The dump method, when supplied with an array, will do its best
   * to convert the array into friendly YAML.
   *
   * @param array $array
   *   PHP array.
   * @param int $inline
   *   The level where you switch to inline YAML.
   * @param int $indent
   *   The amount of spaces to use for indentation of nested nodes.
   * @param bool $exceptionOnInvalidType
   *   True if an exception must be thrown on invalid types (a PHP resource
   *   or object), false otherwise.
   * @param bool $objectSupport
   *   True if object support is enabled, false otherwise.
   *
   * @return string
   *   A YAML string representing the original PHP array
   */
  public static function dump(array $array, $inline = 2, $indent = 4, $exceptionOnInvalidType = FALSE, $objectSupport = FALSE) {
    $yaml = new Dumper();
    $yaml->setIndentation($indent);

    return $yaml->dump($array, $inline, 0, $exceptionOnInvalidType, $objectSupport);
  }

}
