<?php

namespace OpenlayersDoctrine\Common\Reflection;

use OpenlayersDoctrine\Common\Annotations\TokenParser;

/**
 * Parses a file for namespaces/use/class declarations.
 *
 * @author Karoly Negyesi <karoly@negyesi.net>
 */
class StaticReflectionParser implements ReflectionProviderInterface {

  /**
   * The fully qualified class name.
   *
   * @var string
   */
  protected $className;

  /**
   * The short class name.
   *
   * @var string
   */
  protected $shortClassName;

  /**
   * Whether the caller only wants class annotations.
   *
   * @var bool
   */
  protected $classAnnotationOptimize;

  /**
   * Whether the parser has run.
   *
   * @var bool
   */
  protected $parsed = FALSE;

  /**
   * The namespace of the class.
   *
   * @var string
   */
  protected $namespace = '';

  /**
   * The use statements of the class.
   *
   * @var array
   */
  protected $useStatements = array();

  /**
   * The docComment of the class.
   *
   * @var string
   */
  protected $docComment = array(
    'class' => '',
    'property' => array(),
    'method' => array(),
  );

  /**
   * The name of the class this class extends, if any.
   *
   * @var string
   */
  protected $parentClassName = '';

  /**
   * The parent PSR-0 Parser.
   *
   * @var \OpenlayersDoctrine\Common\Reflection\StaticReflectionParser
   */
  protected $parentStaticReflectionParser;

  /**
   * Parses a class residing in a PSR-0 hierarchy.
   *
   * @param string $className
   *   The full, namespaced class name.
   * @param \OpenlayersDoctrine\Common\Reflection\ClassFinderInterface $finder
   *   A ClassFinder object which finds the class.
   * @param bool $classAnnotationOptimize
   *   Only retrieve the class docComment.
   *   Presumes there is only one statement per line.
   */
  public function __construct($className, ClassFinderInterface $finder, $classAnnotationOptimize = FALSE) {
    $this->className = ltrim($className, '\\');
    $lastNsPos = strrpos($this->className, '\\');

    if ($lastNsPos !== FALSE) {
      $this->namespace = substr($this->className, 0, $lastNsPos);
      $this->shortClassName = substr($this->className, $lastNsPos + 1);
    }
    else {
      $this->shortClassName = $this->className;
    }

    $this->finder = $finder;
    $this->classAnnotationOptimize = $classAnnotationOptimize;
  }

  /**
   * FIX - insert comment here.
   */
  protected function parse() {
    if ($this->parsed || !$fileName = $this->finder->findFile($this->className)) {
      return;
    }
    $this->parsed = TRUE;
    $contents = file_get_contents($fileName);
    if ($this->classAnnotationOptimize) {
      if (preg_match("/\A.*^\s*((abstract|final)\s+)?class\s+{$this->shortClassName}\s+/sm", $contents, $matches)) {
        $contents = $matches[0];
      }
    }
    $tokenParser = new TokenParser($contents);
    $docComment = '';
    while ($token = $tokenParser->next(FALSE)) {
      if (is_array($token)) {
        switch ($token[0]) {
          case T_USE:
            $this->useStatements = array_merge($this->useStatements, $tokenParser->parseUseStatement());
            break;

          case T_DOC_COMMENT:
            $docComment = $token[1];
            break;

          case T_CLASS:
            $this->docComment['class'] = $docComment;
            $docComment = '';
            break;

          case T_VAR:
          case T_PRIVATE:
          case T_PROTECTED:
          case T_PUBLIC:
            $token = $tokenParser->next();
            if ($token[0] === T_VARIABLE) {
              $propertyName = substr($token[1], 1);
              $this->docComment['property'][$propertyName] = $docComment;
              continue 2;
            }
            if ($token[0] !== T_FUNCTION) {
              // For example, it can be T_FINAL.
              continue 2;
            }
            // No break.
          case T_FUNCTION:
            // The next string after function is the name, but
            // there can be & before the function name so find the
            // string.
            while (($token = $tokenParser->next()) && $token[0] !== T_STRING) {
            }
            $methodName = $token[1];
            $this->docComment['method'][$methodName] = $docComment;
            $docComment = '';
            break;

          case T_EXTENDS:
            $this->parentClassName = $tokenParser->parseClass();
            $nsPos = strpos($this->parentClassName, '\\');
            $fullySpecified = FALSE;
            if ($nsPos === 0) {
              $fullySpecified = TRUE;
            }
            else {
              if ($nsPos) {
                $prefix = strtolower(substr($this->parentClassName, 0, $nsPos));
                $postfix = substr($this->parentClassName, $nsPos);
              }
              else {
                $prefix = strtolower($this->parentClassName);
                $postfix = '';
              }
              foreach ($this->useStatements as $alias => $use) {
                if ($alias == $prefix) {
                  $this->parentClassName = '\\' . $use . $postfix;
                  $fullySpecified = TRUE;
                }
              }
            }
            if (!$fullySpecified) {
              $this->parentClassName = '\\' . $this->namespace . '\\' . $this->parentClassName;
            }
            break;
        }
      }
    }
  }

  /**
   * FIX - insert comment here.
   *
   * @return StaticReflectionParser
   *   FIX - insert comment here.
   */
  protected function getParentStaticReflectionParser() {
    if (empty($this->parentStaticReflectionParser)) {
      $this->parentStaticReflectionParser = new static($this->parentClassName, $this->finder);
    }

    return $this->parentStaticReflectionParser;
  }

  /**
   * FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getClassName() {
    return $this->className;
  }

  /**
   * FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getNamespaceName() {
    return $this->namespace;
  }

  /**
   * FIX - insert comment here.
   *
   * @return \OpenlayersDoctrine\Common\Reflection\StaticReflectionClass
   *   FIX - insert comment here.
   */
  public function getReflectionClass() {
    return new StaticReflectionClass($this);
  }

  /**
   * FIX - insert comment here.
   *
   * @return object
   *   FIX - insert comment here.
   */
  public function getReflectionMethod($methodName) {
    return new StaticReflectionMethod($this, $methodName);
  }

  /**
   * FIX - insert comment here.
   *
   * @return object
   *   FIX - insert comment here.
   */
  public function getReflectionProperty($propertyName) {
    return new StaticReflectionProperty($this, $propertyName);
  }

  /**
   * Gets the use statements from this file.
   *
   * @return array
   *   FIX - insert comment here.
   */
  public function getUseStatements() {
    $this->parse();

    return $this->useStatements;
  }

  /**
   * Gets the doc comment.
   *
   * @param string $type
   *   The type: 'class', 'property' or 'method'.
   * @param string $name
   *   The name of the property or method, not needed for 'class'.
   *
   * @return string
   *   The doc comment, empty string if none.
   */
  public function getDocComment($type = 'class', $name = '') {
    $this->parse();

    return $name ? $this->docComment[$type][$name] : $this->docComment[$type];
  }

  /**
   * Gets the PSR-0 parser for the declaring class.
   *
   * @param string $type
   *   The type: 'property' or 'method'.
   * @param string $name
   *   The name of the property or method.
   *
   * @return StaticReflectionParser
   *   A static reflection parser for the declaring class.
   *
   * @throws ReflectionException
   *   FIX - insert comment here.
   */
  public function getStaticReflectionParserForDeclaringClass($type, $name) {
    $this->parse();
    if (isset($this->docComment[$type][$name])) {
      return $this;
    }
    if (!empty($this->parentClassName)) {
      return $this->getParentStaticReflectionParser()->getStaticReflectionParserForDeclaringClass($type, $name);
    }
    throw new ReflectionException('Invalid ' . $type . ' "' . $name . '"');
  }

}
