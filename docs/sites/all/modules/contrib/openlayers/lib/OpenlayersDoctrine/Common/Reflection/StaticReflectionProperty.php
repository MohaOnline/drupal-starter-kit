<?php

namespace OpenlayersDoctrine\Common\Reflection;

/**
 * FIX - insert comment here.
 */
class StaticReflectionProperty extends ReflectionProperty {

  /**
   * The PSR-0 parser object.
   *
   * @var StaticReflectionParser
   */
  protected $staticReflectionParser;

  /**
   * The name of the property.
   *
   * @var string|null
   */
  protected $propertyName;

  /**
   * FIX - insert comment here.
   *
   * @param StaticReflectionParser $staticReflectionParser
   *   FIX - insert comment here.
   * @param string|null $propertyName
   *   FIX - insert comment here.
   */
  public function __construct(StaticReflectionParser $staticReflectionParser, $propertyName) {
    $this->staticReflectionParser = $staticReflectionParser;
    $this->propertyName = $propertyName;
  }

  /**
   * FIX - insert comment here.
   */
  public function getName() {
    return $this->propertyName;
  }

  /**
   * FIX - insert comment here.
   *
   * @return StaticReflectionParser
   *   FIX - insert comment here.
   */
  protected function getStaticReflectionParser() {
    return $this->staticReflectionParser->getStaticReflectionParserForDeclaringClass('property', $this->propertyName);
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
  public function getDocComment() {
    return $this->getStaticReflectionParser()->getDocComment('property', $this->propertyName);
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
  public function getModifiers() {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function getValue($object = NULL) {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function isDefault() {
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
  public function setValue($object, $value = NULL) {
    throw new ReflectionException('Method not implemented');
  }

  /**
   * FIX - insert comment here.
   */
  public function __toString() {
    throw new ReflectionException('Method not implemented');
  }

}
