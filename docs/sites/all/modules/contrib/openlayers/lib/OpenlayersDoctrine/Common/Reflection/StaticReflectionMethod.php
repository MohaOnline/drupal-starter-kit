<?php

namespace OpenlayersDoctrine\Common\Reflection;

/**
 * FIX - insert comment here.
 */
class StaticReflectionMethod extends ReflectionMethod {

  /**
   * The PSR-0 parser object.
   *
   * @var StaticReflectionParser
   */
  protected $staticReflectionParser;

  /**
   * The name of the method.
   *
   * @var string
   */
  protected $methodName;

  /**
   * FIX - insert comment here.
   *
   * @param StaticReflectionParser $staticReflectionParser
   *   FIX - insert comment here.
   * @param string $methodName
   *   FIX - insert comment here.
   */
  public function __construct(StaticReflectionParser $staticReflectionParser, $methodName) {
    $this->staticReflectionParser = $staticReflectionParser;
    $this->methodName = $methodName;
  }

  /**
   * FIX - insert comment here.
   */
  public function getName() {
    return $this->methodName;
  }

  /**
   * FIX - insert comment here.
   *
   * @return StaticReflectionParser
   *   FIX - insert comment here.
   */
  protected function getStaticReflectionParser() {
    return $this->staticReflectionParser->getStaticReflectionParserForDeclaringClass('method', $this->methodName);
  }

  /**
   * FIX - insert comment here.
   */
  public function getDeclaringClass() {
    return $this->getStaticReflectionParser()->getReflectionClass();
  }

  /**
   * FIX - insert comment here.
   */
  public function getNamespaceName() {
    return $this->getStaticReflectionParser()->getNamespaceName();
  }

  /**
   * FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getDocComment() {
    return $this->getStaticReflectionParser()->getDocComment('method', $this->methodName);
  }

  /**
   * FIX - insert comment here.
   *
   * @return array
   *   FIX - insert comment here.
   */
  public function getUseStatements() {
    return $this->getStaticReflectionParser()->getUseStatements();
  }

  /**
   * FIX - insert comment here.
   */
  public static function export($class, $name, $return = FALSE) {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getClosure($object) {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getModifiers() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getPrototype() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function invoke($object, $parameter = NULL) {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function invokeArgs($object, array $args) {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isAbstract() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isConstructor() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isDestructor() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isFinal() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isPrivate() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isProtected() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isPublic() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isStatic() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function setAccessible($accessible) {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function __toString() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getClosureThis() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getEndLine() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getExtension() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getExtensionName() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getFileName() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getNumberOfParameters() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getNumberOfRequiredParameters() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getParameters() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getShortName() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getStartLine() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getStaticVariables() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function inNamespace() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isClosure() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isDeprecated() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isInternal() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isUserDefined() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function returnsReference() {
    throw new ReflectionException('Method not implemented');
  }

}
