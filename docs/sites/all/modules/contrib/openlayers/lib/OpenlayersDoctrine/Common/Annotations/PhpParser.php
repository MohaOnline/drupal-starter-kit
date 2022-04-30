<?php

namespace OpenlayersDoctrine\Common\Annotations;

use SplFileObject;

/**
 * Parses a file for namespaces/use/class declarations.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 * @author Christian Kaps <christian.kaps@mohiva.com>
 */
final class PhpParser {

  /**
   * Parses a class.
   *
   * @param \ReflectionClass $class
   *   A <code>ReflectionClass</code> object.
   *
   * @return array
   *   A list with use statements in the form (Alias => FQN).
   */
  public function parseClass(\ReflectionClass $class) {
    if (method_exists($class, 'getUseStatements')) {
      return $class->getUseStatements();
    }

    if (FALSE === $filename = $class->getFilename()) {
      return array();
    }

    $content = $this->getFileContent($filename, $class->getStartLine());

    if (NULL === $content) {
      return array();
    }

    $namespace = preg_quote($class->getNamespaceName());
    $content = preg_replace('/^.*?(\bnamespace\s+' . $namespace . '\s*[;{].*)$/s', '\\1', $content);
    $tokenizer = new TokenParser('<?php ' . $content);

    $statements = $tokenizer->parseUseStatements($class->getNamespaceName());

    return $statements;
  }

  /**
   * Gets the content of the file right up to the given line number.
   *
   * @param string $filename
   *   The name of the file to load.
   * @param int $lineNumber
   *   The number of lines to read from file.
   *
   * @return string
   *   The content of the file.
   */
  private function getFileContent($filename, $lineNumber) {
    if (!is_file($filename)) {
      return NULL;
    }

    $content = '';
    $lineCnt = 0;
    $file = new SplFileObject($filename);
    while (!$file->eof()) {
      if ($lineCnt++ == $lineNumber) {
        break;
      }

      $content .= $file->fgets();
    }

    return $content;
  }

}
