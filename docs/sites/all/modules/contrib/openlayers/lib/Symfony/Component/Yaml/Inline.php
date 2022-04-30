<?php

namespace OpenlayersSymfony\Component\Yaml;

use OpenlayersSymfony\Component\Yaml\Exception\ParseException;
use OpenlayersSymfony\Component\Yaml\Exception\DumpException;

/**
 * Inline implements a YAML parser/dumper for the YAML inline syntax.
 *
 * This file is part of the Symfony package.
 *
 * (c) Fabien Potencier <fabien@symfony.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class Inline {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  const REGEX_QUOTED_STRING = '(?:"([^"\\\\]*(?:\\\\.[^"\\\\]*)*)"|\'([^\']*(?:\'\'[^\']*)*)\')';

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private static $exceptionOnInvalidType = FALSE;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private static $objectSupport = FALSE;

  /**
   * FIX - insert comment here.
   *
   * @var bool
   */
  private static $objectForMap = FALSE;

  /**
   * Converts a YAML string to a PHP array.
   *
   * @param string $value
   *   A YAML string.
   * @param bool $exceptionOnInvalidType
   *   True if an exception must be thrown on invalid types (a PHP resource
   *   or object), false otherwise.
   * @param bool $objectSupport
   *   True if object support is enabled, false otherwise.
   * @param bool $objectForMap
   *   True if maps should return a stdClass instead of array().
   * @param array $references
   *   Mapping of variable names to values.
   *
   * @return array
   *   A PHP array representing the YAML string.
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\ParseException
   */
  public static function parse($value, $exceptionOnInvalidType = FALSE, $objectSupport = FALSE, $objectForMap = FALSE, array $references = array()) {
    self::$exceptionOnInvalidType = $exceptionOnInvalidType;
    self::$objectSupport = $objectSupport;
    self::$objectForMap = $objectForMap;

    $value = trim($value . ' ');

    if (0 == strlen($value)) {
      return '';
    }

    if (function_exists('mb_internal_encoding') && ((int) ini_get('mbstring.func_overload')) & 2) {
      $mbEncoding = mb_internal_encoding();
      mb_internal_encoding('ASCII');
    }

    $i = 0;
    switch ($value[0]) {
      case '[':
        $result = self::parseSequence($value, $i, $references);
        ++$i;
        break;

      case '{':
        $result = self::parseMapping($value, $i, $references);
        ++$i;
        break;

      default:
        $result = self::parseScalar($value, NULL, array('"', "'"), $i, TRUE, $references);
    }

    // Some comments are allowed at the end.
    if (preg_replace('/\s+#.*$/A', '', substr($value, $i))) {
      throw new ParseException(sprintf('Unexpected characters near "%s".', substr($value, $i)));
    }

    if (isset($mbEncoding)) {
      mb_internal_encoding($mbEncoding);
    }

    return $result;
  }

  /**
   * Dumps a given PHP variable to a YAML string.
   *
   * @param mixed $value
   *   The PHP variable to convert.
   * @param bool $exceptionOnInvalidType
   *   True if an exception must be thrown on invalid types (a PHP resource or
   *   object), false otherwise.
   * @param bool $objectSupport
   *   True if object support is enabled, false otherwise.
   *
   * @return string
   *   The YAML string representing the PHP array.
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\DumpException
   *   When trying to dump PHP resource.
   */
  public static function dump($value, $exceptionOnInvalidType = FALSE, $objectSupport = FALSE) {
    switch (TRUE) {
      case is_resource($value):
        if ($exceptionOnInvalidType) {
          throw new DumpException(sprintf('Unable to dump PHP resources in a YAML file ("%s").', get_resource_type($value)));
        }
        return 'null';

      case is_object($value):
        if ($objectSupport) {
          return '!!php/object:' . serialize($value);
        }

        if ($exceptionOnInvalidType) {
          throw new DumpException('Object support when dumping a YAML file has been disabled.');
        }

        return 'null';

      case is_array($value):
        return self::dumpArray($value, $exceptionOnInvalidType, $objectSupport);

      case NULL === $value:
        return 'null';

      case TRUE === $value:
        return 'true';

      case FALSE === $value:
        return 'false';

      case ctype_digit($value):
        return is_string($value) ? "'$value'" : (int) $value;

      case is_numeric($value):
        $locale = setlocale(LC_NUMERIC, 0);
        if (FALSE !== $locale) {
          setlocale(LC_NUMERIC, 'C');
        }
        if (is_float($value)) {
          $repr = (string) $value;
          if (is_infinite($value)) {
            $repr = str_ireplace('INF', '.Inf', $repr);
          }
          elseif (floor($value) == $value && $repr == $value) {
            // Preserve float data type since storing a whole number will
            // result in integer value.
            $repr = '!!float ' . $repr;
          }
        }
        else {
          $repr = is_string($value) ? "'$value'" : (string) $value;
        }
        if (FALSE !== $locale) {
          setlocale(LC_NUMERIC, $locale);
        }

        return $repr;

      case '' == $value:
        return "''";

      case Escaper::requiresDoubleQuoting($value):
        return Escaper::escapeWithDoubleQuotes($value);

      case Escaper::requiresSingleQuoting($value):
      case preg_match(self::getHexRegex(), $value):
      case preg_match(self::getTimestampRegex(), $value):
        return Escaper::escapeWithSingleQuotes($value);

      default:
        return $value;
    }
  }

  /**
   * Dumps a PHP array to a YAML string.
   *
   * @param array $value
   *   The PHP array to dump.
   * @param bool $exceptionOnInvalidType
   *   True if an exception must be thrown on invalid types (a PHP resource
   *   or object), false otherwise.
   * @param bool $objectSupport
   *   True if object support is enabled, false otherwise.
   *
   * @return string
   *   The YAML string representing the PHP array.
   */
  private static function dumpArray(array $value, $exceptionOnInvalidType, $objectSupport) {
    // Array.
    $keys = array_keys($value);
    $keysCount = count($keys);
    if ((1 === $keysCount && '0' == $keys[0])
          || ($keysCount > 1 && array_reduce($keys, function ($v, $w) {
            return (int) $v + $w;
          }, 0) === $keysCount * ($keysCount - 1) / 2)
      ) {
      $output = array();
      foreach ($value as $val) {
        $output[] = self::dump($val, $exceptionOnInvalidType, $objectSupport);
      }

      return sprintf('[%s]', implode(', ', $output));
    }

    // Mapping.
    $output = array();
    foreach ($value as $key => $val) {
      $output[] = sprintf('%s: %s', self::dump($key, $exceptionOnInvalidType, $objectSupport), self::dump($val, $exceptionOnInvalidType, $objectSupport));
    }

    return sprintf('{ %s }', implode(', ', $output));
  }

  /**
   * Parses a scalar to a YAML string.
   *
   * @param string $scalar
   *   FIX - insert comment here.
   * @param string $delimiters
   *   FIX - insert comment here.
   * @param array $stringDelimiters
   *   FIX - insert comment here.
   * @param int &$i
   *   FIX - insert comment here.
   * @param bool $evaluate
   *   FIX - insert comment here.
   * @param array $references
   *   FIX - insert comment here.
   *
   * @return string
   *   A YAML string.
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\ParseException
   *   When malformed inline YAML string is parsed.
   */
  public static function parseScalar(
    $scalar,
    $delimiters = NULL,
    array $stringDelimiters = array('"', "'"),
    &$i = 0,
    $evaluate = TRUE,
    array $references = array()
  ) {
    if (in_array($scalar[$i], $stringDelimiters)) {
      // Quoted scalar.
      $output = self::parseQuotedScalar($scalar, $i);

      if (NULL !== $delimiters) {
        $tmp = ltrim(substr($scalar, $i), ' ');
        if (!in_array($tmp[0], $delimiters)) {
          throw new ParseException(sprintf('Unexpected characters (%s).', substr($scalar, $i)));
        }
      }
    }
    else {
      // "normal" string.
      if (!$delimiters) {
        $output = substr($scalar, $i);
        $i += strlen($output);

        // Remove comments.
        if (FALSE !== $strpos = strpos($output, ' #')) {
          $output = rtrim(substr($output, 0, $strpos));
        }
      }
      elseif (preg_match('/^(.+?)(' . implode('|', $delimiters) . ')/', substr($scalar, $i), $match)) {
        $output = $match[1];
        $i += strlen($output);
      }
      else {
        throw new ParseException(sprintf('Malformed inline YAML string (%s).', $scalar));
      }

      if ($evaluate) {
        $output = self::evaluateScalar($output, $references);
      }
    }

    return $output;
  }

  /**
   * Parses a quoted scalar to YAML.
   *
   * @param string $scalar
   *   FIX - insert comment here.
   * @param int &$i
   *   FIX - insert comment here.
   *
   * @return string
   *   A YAML string.
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\ParseException
   *   When malformed inline YAML string is parsed.
   */
  private static function parseQuotedScalar($scalar, &$i) {
    if (!preg_match('/' . self::REGEX_QUOTED_STRING . '/Au', substr($scalar, $i), $match)) {
      throw new ParseException(sprintf('Malformed inline YAML string (%s).', substr($scalar, $i)));
    }

    $output = substr($match[0], 1, strlen($match[0]) - 2);

    $unescaper = new Unescaper();
    if ('"' == $scalar[$i]) {
      $output = $unescaper->unescapeDoubleQuotedString($output);
    }
    else {
      $output = $unescaper->unescapeSingleQuotedString($output);
    }

    $i += strlen($match[0]);

    return $output;
  }

  /**
   * Parses a sequence to a YAML string.
   *
   * @param string $sequence
   *   FIX - insert comment here.
   * @param int &$i
   *   FIX - insert comment here.
   * @param array $references
   *   FIX - insert comment here.
   *
   * @return string
   *   A YAML string.
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\ParseException
   *   When malformed inline YAML string is parsed.
   */
  private static function parseSequence($sequence, &$i = 0, array $references = array()) {
    $output = array();
    $len = strlen($sequence);
    ++$i;

    // [foo, bar, ...].
    while ($i < $len) {
      switch ($sequence[$i]) {
        case '[':
          // Nested sequence.
          $output[] = self::parseSequence($sequence, $i, $references);
          break;

        case '{':
          // Nested mapping.
          $output[] = self::parseMapping($sequence, $i, $references);
          break;

        case ']':
          return $output;

        case ',':
        case ' ':
          break;

        default:
          $isQuoted = in_array($sequence[$i], array('"', "'"));
          $value = self::parseScalar($sequence, array(',', ']'), array('"', "'"), $i, TRUE, $references);

          // The value can be an array if a reference has been resolved to
          // an array var.
          if (!is_array($value) && !$isQuoted && FALSE !== strpos($value, ': ')) {
            // Embedded mapping?
            try {
              $pos = 0;
              $value = self::parseMapping('{' . $value . '}', $pos, $references);
            }
            catch (\InvalidArgumentException $e) {
              // no, it's not.
            }
          }

          $output[] = $value;

          --$i;
      }

      ++$i;
    }

    throw new ParseException(sprintf('Malformed inline YAML string %s', $sequence));
  }

  /**
   * Parses a mapping to a YAML string.
   *
   * @param string $mapping
   *   FIX - insert comment here.
   * @param int &$i
   *   FIX - insert comment here.
   * @param array $references
   *   FIX - insert comment here.
   *
   * @return string
   *   A YAML string.
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\ParseException
   *   When malformed inline YAML string is parsed.
   */
  private static function parseMapping($mapping, &$i = 0, array $references = array()) {
    $output = array();
    $len = strlen($mapping);
    ++$i;

    // {foo: bar, bar:foo, ...}.
    while ($i < $len) {
      switch ($mapping[$i]) {
        case ' ':
        case ',':
          ++$i;
          continue 2;

        case '}':
          if (self::$objectForMap) {
            return (object) $output;
          }

          return $output;
      }

      // Key.
      $key = self::parseScalar($mapping, array(':', ' '), array('"', "'"), $i, FALSE);

      // Value.
      $done = FALSE;

      while ($i < $len) {
        switch ($mapping[$i]) {
          case '[':
            // Nested sequence.
            $value = self::parseSequence($mapping, $i, $references);
            // Spec: Keys MUST be unique; first one wins.
            // Parser cannot abort this mapping earlier, since lines
            // are processed sequentially.
            if (!isset($output[$key])) {
              $output[$key] = $value;
            }
            $done = TRUE;
            break;

          case '{':
            // Nested mapping.
            $value = self::parseMapping($mapping, $i, $references);
            // Spec: Keys MUST be unique; first one wins.
            // Parser cannot abort this mapping earlier, since lines
            // are processed sequentially.
            if (!isset($output[$key])) {
              $output[$key] = $value;
            }
            $done = TRUE;
            break;

          case ':':
          case ' ':
            break;

          default:
            $value = self::parseScalar(
              $mapping,
              array(',', '}'),
              array('"', "'"),
              $i,
              TRUE,
              $references
            );
            // Spec: Keys MUST be unique; first one wins.
            // Parser cannot abort this mapping earlier, since lines
            // are processed sequentially.
            if (!isset($output[$key])) {
              $output[$key] = $value;
            }
            $done = TRUE;
            --$i;
        }

        ++$i;

        if ($done) {
          continue 2;
        }
      }
    }

    throw new ParseException(sprintf('Malformed inline YAML string %s', $mapping));
  }

  /**
   * Evaluates scalars and replaces magic values.
   *
   * @param string $scalar
   *   FIX - insert comment here.
   * @param array $references
   *   FIX - insert comment here.
   *
   * @return string|null
   *   A YAML string.
   *
   * @throws \OpenlayersSymfony\Component\Yaml\Exception\ParseException
   *   When object parsing support was disabled and the parser detected a PHP
   *   object or when a reference could not be resolved.
   */
  private static function evaluateScalar($scalar, array $references = array()) {
    $scalar = trim($scalar);
    $scalarLower = strtolower($scalar);

    if (0 === strpos($scalar, '*')) {
      if (FALSE !== $pos = strpos($scalar, '#')) {
        $value = substr($scalar, 1, $pos - 2);
      }
      else {
        $value = substr($scalar, 1);
      }

      // An unquoted *.
      if (FALSE === $value || '' === $value) {
        throw new ParseException('A reference must contain at least one character.');
      }

      if (!array_key_exists($value, $references)) {
        throw new ParseException(sprintf('Reference "%s" does not exist.', $value));
      }

      return $references[$value];
    }

    switch (TRUE) {
      case 'null' === $scalarLower:
      case '' === $scalar:
      case '~' === $scalar:
        return;

      case 'true' === $scalarLower:
        return TRUE;

      case 'false' === $scalarLower:
        return FALSE;

      // Optimise for returning strings.
      case $scalar[0] === '+' || $scalar[0] === '-' || $scalar[0] === '.' || $scalar[0] === '!' || is_numeric($scalar[0]):
        switch (TRUE) {
          case 0 === strpos($scalar, '!str'):
            return (string) substr($scalar, 5);

          case 0 === strpos($scalar, '! '):
            return (int) self::parseScalar(substr($scalar, 2));

          case 0 === strpos($scalar, '!!php/object:'):
            if (self::$objectSupport) {
              return unserialize(substr($scalar, 13));
            }

            if (self::$exceptionOnInvalidType) {
              throw new ParseException('Object support when parsing a YAML file has been disabled.');
            }

            return;

          case 0 === strpos($scalar, '!!float '):
            return (float) substr($scalar, 8);

          case ctype_digit($scalar):
            $raw = $scalar;
            $cast = (int) $scalar;

            return '0' == $scalar[0] ? octdec($scalar) : (((string) $raw == (string) $cast) ? $cast : $raw);

          case '-' === $scalar[0] && ctype_digit(substr($scalar, 1)):
            $raw = $scalar;
            $cast = (int) $scalar;

            return '0' == $scalar[1] ? octdec($scalar) : (((string) $raw === (string) $cast) ? $cast : $raw);

          case is_numeric($scalar):
          case preg_match(self::getHexRegex(), $scalar):
            return '0x' === $scalar[0] . $scalar[1] ? hexdec($scalar) : (float) $scalar;

          case '.inf' === $scalarLower:
          case '.nan' === $scalarLower:
            return -log(0);

          case '-.inf' === $scalarLower:
            return log(0);

          case preg_match('/^(-|\+)?[0-9,]+(\.[0-9]+)?$/', $scalar):
            return (float) str_replace(',', '', $scalar);

          case preg_match(self::getTimestampRegex(), $scalar):
            return strtotime($scalar);
        }
      default:
        return (string) $scalar;
    }
  }

  /**
   * Gets a regex that matches a YAML date.
   *
   * @return string
   *   The regular expression.
   */
  private static function getTimestampRegex() {
    return <<<EOF
        ~^
        (?P<year>[0-9][0-9][0-9][0-9])
        -(?P<month>[0-9][0-9]?)
        -(?P<day>[0-9][0-9]?)
        (?:(?:[Tt]|[ \t]+)
        (?P<hour>[0-9][0-9]?)
        :(?P<minute>[0-9][0-9])
        :(?P<second>[0-9][0-9])
        (?:\.(?P<fraction>[0-9]*))?
        (?:[ \t]*(?P<tz>Z|(?P<tz_sign>[-+])(?P<tz_hour>[0-9][0-9]?)
        (?::(?P<tz_minute>[0-9][0-9]))?))?)?
        $~x
EOF;
  }

  /**
   * Gets a regex that matches a YAML number in hexadecimal notation.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private static function getHexRegex() {
    return '~^0x[0-9a-f]++$~i';
  }

}
