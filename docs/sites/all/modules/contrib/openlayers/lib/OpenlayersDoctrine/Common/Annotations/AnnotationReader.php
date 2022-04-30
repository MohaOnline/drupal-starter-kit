<?php

namespace OpenlayersDoctrine\Common\Annotations;

use OpenlayersDoctrine\Common\Annotations\Annotation\IgnoreAnnotation;
use OpenlayersDoctrine\Common\Annotations\Annotation\Target;

/**
 * A reader for docblock annotations.
 *
 * COPYRIGHT NOTICE.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This software consists of voluntary contributions made by many individuals
 * and is licensed under the MIT license. For more information, see
 * <http://www.doctrine-project.org>.
 *
 * @author Benjamin Eberlei <kontakt@beberlei.de>
 * @author Guilherme Blanco <guilhermeblanco@hotmail.com>
 * @author Jonathan Wage <jonwage@gmail.com>
 * @author Roman Borschel <roman@code-factory.org>
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class AnnotationReader implements Reader {

  /**
   * Global map for imports.
   *
   * @var array
   */
  private static $globalImports = array(
    'ignoreannotation' => 'OpenlayersDoctrine\Common\Annotations\Annotation\IgnoreAnnotation',
  );

  /**
   * FIX.
   *
   * A list with annotations that are not causing exceptions when not
   * resolved to an annotation class.
   *
   * The names are case sensitive.
   *
   * @var array
   *  FIX.
   */
  private static $globalIgnoredNames = array(
    // Annotation tags.
    'Annotation' => TRUE,
    'Attribute' => TRUE,
    'Attributes' => TRUE,
    /* Can we enable this? 'Enum' => true, */
    'Required' => TRUE,
    'Target' => TRUE,
    // Widely used tags (but not existent in phpdoc).
    'fix' => TRUE ,
    'fixme' => TRUE,
    'override' => TRUE,
    // PHPDocumentor 1 tags.
    'abstract' => TRUE,
    'access' => TRUE,
    'code' => TRUE,
    'deprec' => TRUE,
    'endcode' => TRUE,
    'exception' => TRUE,
    'final' => TRUE,
    'ingroup' => TRUE,
    'inheritdoc' => TRUE,
    'inheritDoc' => TRUE,
    'magic' => TRUE,
    'name' => TRUE,
    'toc' => TRUE,
    'tutorial' => TRUE,
    'private' => TRUE,
    'static' => TRUE,
    'staticvar' => TRUE,
    'staticVar' => TRUE,
    'throw' => TRUE,
    // PHPDocumentor 2 tags.
    'api' => TRUE,
    'author' => TRUE,
    'category' => TRUE,
    'copyright' => TRUE,
    'deprecated' => TRUE,
    'example' => TRUE,
    'filesource' => TRUE,
    'global' => TRUE,
    'ignore' => TRUE, /* Can we enable this? 'index' => true, */
    'internal' => TRUE,
    'license' => TRUE,
    'link' => TRUE,
    'method' => TRUE,
    'package' => TRUE,
    'param' => TRUE,
    'property' => TRUE,
    'property-read' => TRUE,
    'property-write' => TRUE,
    'return' => TRUE,
    'see' => TRUE,
    'since' => TRUE,
    'source' => TRUE,
    'subpackage' => TRUE,
    'throws' => TRUE,
    'todo' => TRUE,
    'TODO' => TRUE,
    'usedby' => TRUE,
    'uses' => TRUE,
    'var' => TRUE,
    'version' => TRUE,
    // PHPUnit tags.
    'codeCoverageIgnore' => TRUE,
    'codeCoverageIgnoreStart' => TRUE,
    'codeCoverageIgnoreEnd' => TRUE,
    // PHPCheckStyle.
    'SuppressWarnings' => TRUE,
    // PHPStorm.
    'noinspection' => TRUE,
    // PEAR.
    'package_version' => TRUE,
    // PlantUML.
    'startuml' => TRUE,
    'enduml' => TRUE,
  );

  /**
   * Add a new annotation.
   *
   * Add a new annotation to the globally ignored annotation names with regard
   * to exception handling.
   *
   * @param string $name
   *   FIX.
   */
  public static function addGlobalIgnoredName($name) {
    self::$globalIgnoredNames[$name] = TRUE;
  }

  /**
   * Annotations parser.
   *
   * @var \OpenlayersDoctrine\Common\Annotations\DocParser
   */
  private $parser;

  /**
   * Annotations parser used to collect parsing metadata.
   *
   * @var \OpenlayersDoctrine\Common\Annotations\DocParser
   */
  private $preParser;

  /**
   * PHP parser used to collect imports.
   *
   * @var \OpenlayersDoctrine\Common\Annotations\PhpParser
   */
  private $phpParser;

  /**
   * In-memory cache mechanism to store imported annotations per class.
   *
   * @var array
   */
  private $imports = array();

  /**
   * In-memory cache mechanism to store ignored annotations per class.
   *
   * @var array
   */
  private $ignoredAnnotationNames = array();

  /**
   * Constructor.
   *
   * Initializes a new AnnotationReader.
   */
  public function __construct() {
    if (extension_loaded('Zend Optimizer+') && (ini_get('zend_optimizerplus.save_comments') === "0" || ini_get('opcache.save_comments') === "0")) {
      throw AnnotationException::optimizerPlusSaveComments();
    }

    if (extension_loaded('Zend OPcache') && ini_get('opcache.save_comments') == 0) {
      throw AnnotationException::optimizerPlusSaveComments();
    }

    if (extension_loaded('Zend Optimizer+') && (ini_get('zend_optimizerplus.load_comments') === "0" || ini_get('opcache.load_comments') === "0")) {
      throw AnnotationException::optimizerPlusLoadComments();
    }

    if (extension_loaded('Zend OPcache') && ini_get('opcache.load_comments') == 0) {
      throw AnnotationException::optimizerPlusLoadComments();
    }

    AnnotationRegistry::registerFile(__DIR__ . '/Annotation/IgnoreAnnotation.php');

    $this->parser    = new DocParser();
    $this->preParser = new DocParser();

    $this->preParser->setImports(self::$globalImports);
    $this->preParser->setIgnoreNotImportedAnnotations(TRUE);

    $this->phpParser = new PhpParser();
  }

  /**
   * FIX - insert comment here.
   */
  public function getClassAnnotations(ReflectionClass $class) {
    $this->parser->setTarget(Target::TARGET_CLASS);
    $this->parser->setImports($this->getClassImports($class));
    $this->parser->setIgnoredAnnotationNames($this->getIgnoredAnnotationNames($class));

    return $this->parser->parse($class->getDocComment(), 'class ' . $class->getName());
  }

  /**
   * FIX - insert comment here.
   */
  public function getClassAnnotation(ReflectionClass $class, $annotationName) {
    $annotations = $this->getClassAnnotations($class);

    foreach ($annotations as $annotation) {
      if ($annotation instanceof $annotationName) {
        return $annotation;
      }
    }

    return NULL;
  }

  /**
   * FIX - insert comment here.
   */
  public function getPropertyAnnotations(ReflectionProperty $property) {
    $class   = $property->getDeclaringClass();
    $context = 'property ' . $class->getName() . "::\$" . $property->getName();

    $this->parser->setTarget(Target::TARGET_PROPERTY);
    $this->parser->setImports($this->getPropertyImports($property));
    $this->parser->setIgnoredAnnotationNames($this->getIgnoredAnnotationNames($class));

    return $this->parser->parse($property->getDocComment(), $context);
  }

  /**
   * FIX - insert comment here.
   */
  public function getPropertyAnnotation(ReflectionProperty $property, $annotationName) {
    $annotations = $this->getPropertyAnnotations($property);

    foreach ($annotations as $annotation) {
      if ($annotation instanceof $annotationName) {
        return $annotation;
      }
    }

    return NULL;
  }

  /**
   * FIX - insert comment here.
   */
  public function getMethodAnnotations(ReflectionMethod $method) {
    $class   = $method->getDeclaringClass();
    $context = 'method ' . $class->getName() . '::' . $method->getName() . '()';

    $this->parser->setTarget(Target::TARGET_METHOD);
    $this->parser->setImports($this->getMethodImports($method));
    $this->parser->setIgnoredAnnotationNames($this->getIgnoredAnnotationNames($class));

    return $this->parser->parse($method->getDocComment(), $context);
  }

  /**
   * FIX - insert comment here.
   */
  public function getMethodAnnotation(ReflectionMethod $method, $annotationName) {
    $annotations = $this->getMethodAnnotations($method);

    foreach ($annotations as $annotation) {
      if ($annotation instanceof $annotationName) {
        return $annotation;
      }
    }

    return NULL;
  }

  /**
   * Returns the ignored annotations for the given class.
   *
   * @param \ReflectionClass $class
   *   FIX.
   *
   * @return array
   *   FIX.
   */
  private function getIgnoredAnnotationNames(ReflectionClass $class) {
    if (isset($this->ignoredAnnotationNames[$name = $class->getName()])) {
      return $this->ignoredAnnotationNames[$name];
    }

    $this->collectParsingMetadata($class);

    return $this->ignoredAnnotationNames[$name];
  }

  /**
   * Retrieves imports.
   *
   * @param \ReflectionClass $class
   *   FIX.
   *
   * @return array
   *   FIX.
   */
  private function getClassImports(ReflectionClass $class) {
    if (isset($this->imports[$name = $class->getName()])) {
      return $this->imports[$name];
    }

    $this->collectParsingMetadata($class);

    return $this->imports[$name];
  }

  /**
   * Retrieves imports for methods.
   *
   * @param \ReflectionMethod $method
   *   FIX.
   *
   * @return array
   *   FIX.
   */
  private function getMethodImports(ReflectionMethod $method) {
    $class = $method->getDeclaringClass();
    $classImports = $this->getClassImports($class);
    if (!method_exists($class, 'getTraits')) {
      return $classImports;
    }

    $traitImports = array();

    foreach ($class->getTraits() as $trait) {
      if ($trait->hasMethod($method->getName())
        && $trait->getFileName() === $method->getFileName()
      ) {
        $traitImports = array_merge($traitImports, $this->phpParser->parseClass($trait));
      }
    }

    return array_merge($classImports, $traitImports);
  }

  /**
   * Retrieves imports for properties.
   *
   * @param \ReflectionProperty $property
   *   FIX.
   *
   * @return array
   *   FIX.
   */
  private function getPropertyImports(ReflectionProperty $property) {
    $class = $property->getDeclaringClass();
    $classImports = $this->getClassImports($class);
    if (!method_exists($class, 'getTraits')) {
      return $classImports;
    }

    $traitImports = array();

    foreach ($class->getTraits() as $trait) {
      if ($trait->hasProperty($property->getName())) {
        $traitImports = array_merge($traitImports, $this->phpParser->parseClass($trait));
      }
    }

    return array_merge($classImports, $traitImports);
  }

  /**
   * Collects parsing metadata for a given class.
   *
   * @param \ReflectionClass $class
   *   FIX.
   */
  private function collectParsingMetadata(ReflectionClass $class) {
    $ignoredAnnotationNames = self::$globalIgnoredNames;
    $annotations            = $this->preParser->parse($class->getDocComment(), 'class ' . $class->name);

    foreach ($annotations as $annotation) {
      if ($annotation instanceof IgnoreAnnotation) {
        foreach ($annotation->names as $annot) {
          $ignoredAnnotationNames[$annot] = TRUE;
        }
      }
    }

    $name = $class->getName();

    $this->imports[$name] = array_merge(
      self::$globalImports,
      $this->phpParser->parseClass($class),
      array('__NAMESPACE__' => $class->getNamespaceName())
    );

    $this->ignoredAnnotationNames[$name] = $ignoredAnnotationNames;
  }

}
