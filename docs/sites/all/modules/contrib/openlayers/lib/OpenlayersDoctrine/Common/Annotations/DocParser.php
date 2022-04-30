<?php

namespace OpenlayersDoctrine\Common\Annotations;

use OpenlayersDoctrine\Common\Annotations\Annotation\Attribute;
use OpenlayersDoctrine\Common\Annotations\Annotation\Enum;
use OpenlayersDoctrine\Common\Annotations\Annotation\Target;
use OpenlayersDoctrine\Common\Annotations\Annotation\Attributes;

/**
 * A parser for docblock annotations.
 *
 * It is strongly discouraged to change the default annotation parsing process.
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
 * @author Fabio B. Silva <fabio.bat.silva@gmail.com>
 */
final class DocParser {

  /**
   * An array of all valid tokens for a class name.
   *
   * @var array
   */
  private static $classIdentifiers = array(
    DocLexer::T_IDENTIFIER,
    DocLexer::T_TRUE,
    DocLexer::T_FALSE,
    DocLexer::T_NULL,
  );

  /**
   * The lexer.
   *
   * @var \OpenlayersDoctrine\Common\Annotations\DocLexer
   */
  private $lexer;

  /**
   * Current target context.
   *
   * @var string
   */
  private $target;

  /**
   * Doc parser used to collect annotation target.
   *
   * @var \OpenlayersDoctrine\Common\Annotations\DocParser
   */
  private static $metadataParser;

  /**
   * Flag to control if the current annotation is nested or not.
   *
   * @var bool
   */
  private $isNestedAnnotation = FALSE;

  /**
   * FIX - insert comment here.
   *
   * Hashmap containing all use-statements that are to be used when parsing
   * the given doc block.
   *
   * @var array
   */
  private $imports = array();

  /**
   * FIX - insert comment here.
   *
   * This hashmap is used internally to cache results of class_exists()
   * look-ups.
   *
   * @var array
   */
  private $classExists = array();

  /**
   * Whether annotations that have not been imported should be ignored.
   *
   * @var bool
   */
  private $ignoreNotImportedAnnotations = FALSE;

  /**
   * An array of default namespaces if operating in simple mode.
   *
   * @var array
   */
  private $namespaces = array();

  /**
   * FIX - insert comment here.
   *
   * A list with annotations that are not causing exceptions when not
   * resolved to an annotation class.
   *
   * The names must be the raw names as used in the class, not the fully
   * qualified class names.
   *
   * @var array
   */
  private $ignoredAnnotationNames = array();

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $context = '';

  /**
   * Hash-map for caching annotation metadata.
   *
   * @var array
   */
  private static $annotationMetadata = array(
    'OpenlayersDoctrine\Common\Annotations\Annotation\Target' => array(
      'is_annotation'    => TRUE,
      'has_constructor'  => TRUE,
      'properties'       => array(),
      'targets_literal'  => 'ANNOTATION_CLASS',
      'targets'          => Target::TARGET_CLASS,
      'default_property' => 'value',
      'attribute_types'  => array(
        'value'  => array(
          'required'  => FALSE,
          'type'      => 'array',
          'array_type' => 'string',
          'value'     => 'array<string>',
        ),
      ),
    ),
    'OpenlayersDoctrine\Common\Annotations\Annotation\Attribute' => array(
      'is_annotation'    => TRUE,
      'has_constructor'  => FALSE,
      'targets_literal'  => 'ANNOTATION_ANNOTATION',
      'targets'          => Target::TARGET_ANNOTATION,
      'default_property' => 'name',
      'properties'       => array(
        'name'      => 'name',
        'type'      => 'type',
        'required'  => 'required',
      ),
      'attribute_types'  => array(
        'value'  => array(
          'required'  => TRUE,
          'type'      => 'string',
          'value'     => 'string',
        ),
        'type'  => array(
          'required'  => TRUE,
          'type'      => 'string',
          'value'     => 'string',
        ),
        'required'  => array(
          'required'  => FALSE,
          'type'      => 'boolean',
          'value'     => 'boolean',
        ),
      ),
    ),
    'OpenlayersDoctrine\Common\Annotations\Annotation\Attributes' => array(
      'is_annotation'    => TRUE,
      'has_constructor'  => FALSE,
      'targets_literal'  => 'ANNOTATION_CLASS',
      'targets'          => Target::TARGET_CLASS,
      'default_property' => 'value',
      'properties'       => array(
        'value' => 'value',
      ),
      'attribute_types'  => array(
        'value' => array(
          'type'      => 'array',
          'required'  => TRUE,
          'array_type' => 'OpenlayersDoctrine\Common\Annotations\Annotation\Attribute',
          'value'     => 'array<OpenlayersDoctrine\Common\Annotations\Annotation\Attribute>',
        ),
      ),
    ),
    'OpenlayersDoctrine\Common\Annotations\Annotation\Enum' => array(
      'is_annotation'    => TRUE,
      'has_constructor'  => TRUE,
      'targets_literal'  => 'ANNOTATION_PROPERTY',
      'targets'          => Target::TARGET_PROPERTY,
      'default_property' => 'value',
      'properties'       => array(
        'value' => 'value',
      ),
      'attribute_types'  => array(
        'value' => array(
          'type'      => 'array',
          'required'  => TRUE,
        ),
        'literal' => array(
          'type'      => 'array',
          'required'  => FALSE,
        ),
      ),
    ),
  );

  /**
   * Hash-map for handle types declaration.
   *
   * @var array
   */
  private static $typeMap = array(
    'float'     => 'double',
    'bool'      => 'boolean',
    // Allow uppercase Boolean in honor of George Boole.
    'Boolean'   => 'boolean',
    'int'       => 'integer',
  );

  /**
   * Constructs a new DocParser.
   */
  public function __construct() {
    $this->lexer = new DocLexer();
  }

  /**
   * Sets the annotation names that are ignored during the parsing process.
   *
   * The names are supposed to be the raw names as used in the class, not the
   * fully qualified class names.
   *
   * @param array $names
   *   FIX - insert comment here.
   */
  public function setIgnoredAnnotationNames(array $names) {
    $this->ignoredAnnotationNames = $names;
  }

  /**
   * Sets ignore on not-imported annotations.
   *
   * @param bool $bool
   *   FIX - insert comment here.
   */
  public function setIgnoreNotImportedAnnotations($bool) {
    $this->ignoreNotImportedAnnotations = (boolean) $bool;
  }

  /**
   * Sets the default namespaces.
   *
   * @param string $namespace
   *   FIX - insert comment here.
   *
   * @throws \RuntimeException
   *   FIX - insert comment here.
   */
  public function addNamespace($namespace) {
    if ($this->imports) {
      throw new \RuntimeException('You must either use addNamespace(), or setImports(), but not both.');
    }

    $this->namespaces[] = $namespace;
  }

  /**
   * Sets the imports.
   *
   * @param array $imports
   *   FIX - insert comment here.
   *
   * @throws \RuntimeException
   *   FIX - insert comment here.
   */
  public function setImports(array $imports) {
    if ($this->namespaces) {
      throw new \RuntimeException('You must either use addNamespace(), or setImports(), but not both.');
    }

    $this->imports = $imports;
  }

  /**
   * Sets current target context as bitmask.
   *
   * @param int $target
   *   FIX - insert comment here.
   */
  public function setTarget($target) {
    $this->target = $target;
  }

  /**
   * Parses the given docblock string for annotations.
   *
   * @param string $input
   *   The docblock string to parse.
   * @param string $context
   *   The parsing context.
   *
   * @return array
   *   Array of annotations. If no annotations are found, an empty array
   *   is returned.
   */
  public function parse($input, $context = '') {
    $pos = $this->findInitialTokenPosition($input);
    if ($pos === NULL) {
      return array();
    }

    $this->context = $context;

    $this->lexer->setInput(trim(substr($input, $pos), '* /'));
    $this->lexer->moveNext();

    return $this->Annotations();
  }

  /**
   * Finds the first valid annotation.
   *
   * @param string $input
   *   The docblock string to parse.
   *
   * @return int|null
   *   FIX - insert comment here.
   */
  private function findInitialTokenPosition($input) {
    $pos = 0;

    // Search for first valid annotation.
    while (($pos = strpos($input, '@', $pos)) !== FALSE) {
      // If the @ is preceded by a space or * it is valid.
      if ($pos === 0 || $input[$pos - 1] === ' ' || $input[$pos - 1] === '*') {
        return $pos;
      }

      $pos++;
    }

    return NULL;
  }

  /**
   * FIX - insert comment here.
   *
   * Attempts to match the given token with the current lookahead token.
   * If they match, updates the lookahead token; otherwise raises a syntax
   * error.
   *
   * @param int $token
   *   Type of token.
   *
   * @return bool
   *   True if tokens match; false otherwise.
   */
  private function match($token) {
    if (!$this->lexer->isNextToken($token)) {
      $this->syntaxError($this->lexer->getLiteral($token));
    }

    return $this->lexer->moveNext();
  }

  /**
   * Attempts to match the current lookahead token with any of the given tokens.
   *
   * If any of them matches, this method updates the lookahead token; otherwise
   * a syntax error is raised.
   *
   * @param array $tokens
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  private function matchAny(array $tokens) {
    if (!$this->lexer->isNextTokenAny($tokens)) {
      $this->syntaxError(
        implode(' or ', array_map(array($this->lexer, 'getLiteral'), $tokens))
      );
    }

    return $this->lexer->moveNext();
  }

  /**
   * Generates a new syntax error.
   *
   * @param string $expected
   *   Expected string.
   * @param array|null $token
   *   Optional token.
   */
  private function syntaxError($expected, $token = NULL) {
    if ($token === NULL) {
      $token = $this->lexer->lookahead;
    }

    $message  = sprintf('Expected %s, got ', $expected);
    $message .= ($this->lexer->lookahead === NULL)
        ? 'end of string'
        : sprintf("'%s' at position %s", $token['value'], $token['position']);

    if (strlen($this->context)) {
      $message .= ' in ' . $this->context;
    }

    $message .= '.';

    throw AnnotationException::syntaxError($message);
  }

  /**
   * FIX - insert comment here.
   *
   * Attempts to check if a class exists or not. This never goes through
   * the PHP autoloading mechanism
   * but uses the {@link AnnotationRegistry} to load classes.
   *
   * @param string $fqcn
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  private function classExists($fqcn) {
    if (isset($this->classExists[$fqcn])) {
      return $this->classExists[$fqcn];
    }

    // First check if the class already exists, maybe loaded through another
    // AnnotationReader.
    if (class_exists($fqcn, FALSE)) {
      return $this->classExists[$fqcn] = TRUE;
    }

    // Final check, does this class exist?
    return $this->classExists[$fqcn] = AnnotationRegistry::loadAnnotationClass($fqcn);
  }

  /**
   * Collects parsing metadata for a given annotation class.
   *
   * @param string $name
   *   The annotation name.
   */
  private function collectAnnotationMetadata($name) {
    if (self::$metadataParser === NULL) {
      self::$metadataParser = new self();

      self::$metadataParser->setIgnoreNotImportedAnnotations(TRUE);
      self::$metadataParser->setIgnoredAnnotationNames($this->ignoredAnnotationNames);
      self::$metadataParser->setImports(array(
        'enum'          => 'OpenlayersDoctrine\Common\Annotations\Annotation\Enum',
        'target'        => 'OpenlayersDoctrine\Common\Annotations\Annotation\Target',
        'attribute'     => 'OpenlayersDoctrine\Common\Annotations\Annotation\Attribute',
        'attributes'    => 'OpenlayersDoctrine\Common\Annotations\Annotation\Attributes',
      ));

      AnnotationRegistry::registerFile(__DIR__ . '/Annotation/Enum.php');
      AnnotationRegistry::registerFile(__DIR__ . '/Annotation/Target.php');
      AnnotationRegistry::registerFile(__DIR__ . '/Annotation/Attribute.php');
      AnnotationRegistry::registerFile(__DIR__ . '/Annotation/Attributes.php');
    }

    $class      = new \ReflectionClass($name);
    $docComment = $class->getDocComment();

    // Sets default values for annotation metadata.
    $metadata = array(
      'default_property' => NULL,
      'has_constructor'  => (NULL !== $constructor = $class->getConstructor()) && $constructor->getNumberOfParameters() > 0,
      'properties'       => array(),
      'property_types'   => array(),
      'attribute_types'  => array(),
      'targets_literal'  => NULL,
      'targets'          => Target::TARGET_ALL,
      'is_annotation'    => FALSE !== strpos($docComment, '@Annotation'),
    );

    // Verify that the class is really meant to be an annotation.
    if ($metadata['is_annotation']) {
      self::$metadataParser->setTarget(Target::TARGET_CLASS);

      foreach (self::$metadataParser->parse($docComment, 'class @' . $name) as $annotation) {
        if ($annotation instanceof Target) {
          $metadata['targets']         = $annotation->targets;
          $metadata['targets_literal'] = $annotation->literal;

          continue;
        }

        if ($annotation instanceof Attributes) {
          foreach ($annotation->value as $attribute) {
            $this->collectAttributeTypeMetadata($metadata, $attribute);
          }
        }
      }

      // If not has a constructor will inject values into public properties.
      if (FALSE === $metadata['has_constructor']) {
        // Collect all public properties.
        foreach ($class->getProperties(\ReflectionProperty::IS_PUBLIC) as $property) {
          $metadata['properties'][$property->name] = $property->name;

          if (FALSE === ($propertyComment = $property->getDocComment())) {
            continue;
          }

          $attribute = new Attribute();

          $attribute->required = (FALSE !== strpos($propertyComment, '@Required'));
          $attribute->name     = $property->name;
          $attribute->type     = (FALSE !== strpos($propertyComment, '@var') && preg_match('/@var\s+([^\s]+)/', $propertyComment, $matches))
                ? $matches[1]
                : 'mixed';

          $this->collectAttributeTypeMetadata($metadata, $attribute);

          // Checks if the property has @Enum.
          if (FALSE !== strpos($propertyComment, '@Enum')) {
            $context = 'property ' . $class->name . "::\$" . $property->name;

            self::$metadataParser->setTarget(Target::TARGET_PROPERTY);

            foreach (self::$metadataParser->parse($propertyComment, $context) as $annotation) {
              if (!$annotation instanceof Enum) {
                continue;
              }

              $metadata['enum'][$property->name]['value']   = $annotation->value;
              $metadata['enum'][$property->name]['literal'] = (!empty($annotation->literal))
                        ? $annotation->literal
                        : $annotation->value;
            }
          }
        }

        // Choose the first property as default property.
        $metadata['default_property'] = reset($metadata['properties']);
      }
    }

    self::$annotationMetadata[$name] = $metadata;
  }

  /**
   * Collects parsing metadata for a given attribute.
   *
   * @param array $metadata
   *   FIX - insert comment here.
   * @param OpenlayersDoctrine\Common\Annotations\Annotation\Attribute $attribute
   *   FIX - insert comment here.
   */
  private function collectAttributeTypeMetadata(array &$metadata, Attribute $attribute) {
    // Handle internal type declaration.
    $type = isset(self::$typeMap[$attribute->type])
        ? self::$typeMap[$attribute->type]
        : $attribute->type;

    // Handle the case if the property type is mixed.
    if ('mixed' === $type) {
      return;
    }

    // Evaluate type.
    switch (TRUE) {
      // Checks if the property has array<type>.
      case (FALSE !== $pos = strpos($type, '<')):
        $arrayType = substr($type, $pos + 1, -1);
        $type      = 'array';

        if (isset(self::$typeMap[$arrayType])) {
          $arrayType = self::$typeMap[$arrayType];
        }

        $metadata['attribute_types'][$attribute->name]['array_type'] = $arrayType;
        break;

      // Checks if the property has type[].
      case (FALSE !== $pos = strrpos($type, '[')):
        $arrayType = substr($type, 0, $pos);
        $type      = 'array';

        if (isset(self::$typeMap[$arrayType])) {
          $arrayType = self::$typeMap[$arrayType];
        }

        $metadata['attribute_types'][$attribute->name]['array_type'] = $arrayType;
        break;
    }

    $metadata['attribute_types'][$attribute->name]['type']     = $type;
    $metadata['attribute_types'][$attribute->name]['value']    = $attribute->type;
    $metadata['attribute_types'][$attribute->name]['required'] = $attribute->required;
  }

  /**
   * Annotations ::= Annotation {[ "*" ]* [Annotation]}*.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function annotations() {
    $annotations = array();

    while (NULL !== $this->lexer->lookahead) {
      if (DocLexer::T_AT !== $this->lexer->lookahead['type']) {
        $this->lexer->moveNext();
        continue;
      }

      // Make sure the @ is preceded by non-catchable pattern.
      if (NULL !== $this->lexer->token && $this->lexer->lookahead['position'] === $this->lexer->token['position'] + strlen($this->lexer->token['value'])) {
        $this->lexer->moveNext();
        continue;
      }

      // Make sure the @ is followed by either a namespace separator, or
      // an identifier token.
      if ((NULL === $peek = $this->lexer->glimpse())
        || (DocLexer::T_NAMESPACE_SEPARATOR !== $peek['type'] && !in_array($peek['type'], self::$classIdentifiers, TRUE))
        || $peek['position'] !== $this->lexer->lookahead['position'] + 1) {
        $this->lexer->moveNext();
        continue;
      }

      $this->isNestedAnnotation = FALSE;
      if (FALSE !== $annot = $this->annotation()) {
        $annotations[] = $annot;
      }
    }

    return $annotations;
  }

  /**
   * FIX - insert comment here.
   *
   * Annotation     ::= "@" AnnotationName MethodCall
   * AnnotationName ::= QualifiedName | SimpleName
   * QualifiedName  ::= NameSpacePart "\" {NameSpacePart "\"}* SimpleName
   * NameSpacePart  ::= identifier | null | false | true
   * SimpleName     ::= identifier | null | false | true.
   *
   * @return mixed
   *   False if it is not a valid annotation.
   *
   * @throws AnnotationException
   */
  private function annotation() {
    $this->match(DocLexer::T_AT);

    // Check if we have an annotation.
    $name = $this->identifier();

    // Only process names which are not fully qualified, yet
    // fully qualified names must start with a \.
    $originalName = $name;

    if ('\\' !== $name[0]) {
      $alias = (FALSE === $pos = strpos($name, '\\')) ? $name : substr($name, 0, $pos);
      $found = FALSE;

      if ($this->namespaces) {
        foreach ($this->namespaces as $namespace) {
          if ($this->classExists($namespace . '\\' . $name)) {
            $name = $namespace . '\\' . $name;
            $found = TRUE;
            break;
          }
        }
      }
      elseif (isset($this->imports[$loweredAlias = strtolower($alias)])) {
        $found = TRUE;
        $name  = (FALSE !== $pos)
              ? $this->imports[$loweredAlias] . substr($name, $pos)
              : $this->imports[$loweredAlias];
      }
      elseif (!isset($this->ignoredAnnotationNames[$name])
          && isset($this->imports['__NAMESPACE__'])
          && $this->classExists($this->imports['__NAMESPACE__'] . '\\' . $name)
      ) {
        $name  = $this->imports['__NAMESPACE__'] . '\\' . $name;
        $found = TRUE;
      }
      elseif (!isset($this->ignoredAnnotationNames[$name]) && $this->classExists($name)) {
        $found = TRUE;
      }

      if (!$found) {
        if ($this->ignoreNotImportedAnnotations || isset($this->ignoredAnnotationNames[$name])) {
          return FALSE;
        }

        throw AnnotationException::semanticalError(sprintf('The annotation "@%s" in %s was never imported. Did you maybe forget to add a "use" statement for this annotation?', $name, $this->context));
      }
    }

    if (!$this->classExists($name)) {
      throw AnnotationException::semanticalError(sprintf('The annotation "@%s" in %s does not exist, or could not be auto-loaded.', $name, $this->context));
    }

    // At this point, $name contains the fully qualified class name of the
    // annotation, and it is also guaranteed that this class exists, and
    // that it is loaded.
    // Collects the metadata annotation only if there is not yet.
    if (!isset(self::$annotationMetadata[$name])) {
      $this->collectAnnotationMetadata($name);
    }

    // Verify that the class is really meant to be an annotation and not
    // just any ordinary class.
    if (self::$annotationMetadata[$name]['is_annotation'] === FALSE) {
      if (isset($this->ignoredAnnotationNames[$originalName])) {
        return FALSE;
      }

      throw AnnotationException::semanticalError(sprintf('The class "%s" is not annotated with @Annotation. Are you sure this class can be used as annotation? If so, then you need to add @Annotation to the _class_ doc comment of "%s". If it is indeed no annotation, then you need to add @Ignoreannotation("%s") to the _class_ doc comment of %s.', $name, $name, $originalName, $this->context));
    }

    // If target is nested annotation.
    $target = $this->isNestedAnnotation ? Target::TARGET_ANNOTATION : $this->target;

    // Next will be nested.
    $this->isNestedAnnotation = TRUE;

    // If annotation does not support current target.
    if (0 === (self::$annotationMetadata[$name]['targets'] & $target) && $target) {
      throw AnnotationException::semanticalError(
            sprintf('Annotation @%s is not allowed to be declared on %s. You may only use this annotation on these code elements: %s.',
                 $originalName, $this->context, self::$annotationMetadata[$name]['targets_literal'])
        );
    }

    $values = $this->methodCall();

    if (isset(self::$annotationMetadata[$name]['enum'])) {
      // Checks all declared attributes.
      foreach (self::$annotationMetadata[$name]['enum'] as $property => $enum) {
        // Checks if the attribute is a valid enumerator.
        if (isset($values[$property]) && !in_array($values[$property], $enum['value'])) {
          throw AnnotationException::enumeratorError($property, $name, $this->context, $enum['literal'], $values[$property]);
        }
      }
    }

    // Checks all declared attributes.
    foreach (self::$annotationMetadata[$name]['attribute_types'] as $property => $type) {
      if ($property === self::$annotationMetadata[$name]['default_property']
          && !isset($values[$property]) && isset($values['value'])) {
        $property = 'value';
      }

      // Handle a not given attribute or null value.
      if (!isset($values[$property])) {
        if ($type['required']) {
          throw AnnotationException::requiredError($property, $originalName, $this->context, 'a(n) ' . $type['value']);
        }

        continue;
      }

      if ($type['type'] === 'array') {
        // Handle the case of a single value.
        if (!is_array($values[$property])) {
          $values[$property] = array($values[$property]);
        }

        // Checks if the attribute has array type declaration, such
        // as "array<string>".
        if (isset($type['array_type'])) {
          foreach ($values[$property] as $item) {
            if (gettype($item) !== $type['array_type'] && !$item instanceof $type['array_type']) {
              throw AnnotationException::attributeTypeError($property, $originalName, $this->context, 'either a(n) ' . $type['array_type'] . ', or an array of ' . $type['array_type'] . 's', $item);
            }
          }
        }
      }
      elseif (gettype($values[$property]) !== $type['type'] && !$values[$property] instanceof $type['type']) {
        throw AnnotationException::attributeTypeError($property, $originalName, $this->context, 'a(n) ' . $type['value'], $values[$property]);
      }
    }

    // Check if the annotation expects values via the constructor,
    // or directly injected into public properties.
    if (self::$annotationMetadata[$name]['has_constructor'] === TRUE) {
      return new $name($values);
    }

    $instance = new $name();

    foreach ($values as $property => $value) {
      if (!isset(self::$annotationMetadata[$name]['properties'][$property])) {
        if ('value' !== $property) {
          throw AnnotationException::creationError(sprintf('The annotation @%s declared on %s does not have a property named "%s". Available properties: %s', $originalName, $this->context, $property, implode(', ', self::$annotationMetadata[$name]['properties'])));
        }

        // Handle the case if the property has no annotations.
        if (!$property = self::$annotationMetadata[$name]['default_property']) {
          throw AnnotationException::creationError(sprintf('The annotation @%s declared on %s does not accept any values, but got %s.', $originalName, $this->context, json_encode($values)));
        }
      }

      $instance->{$property} = $value;
    }

    return $instance;
  }

  /**
   * MethodCall ::= ["(" [Values] ")"].
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function methodCall() {
    $values = array();

    if (!$this->lexer->isNextToken(DocLexer::T_OPEN_PARENTHESIS)) {
      return $values;
    }

    $this->match(DocLexer::T_OPEN_PARENTHESIS);

    if (!$this->lexer->isNextToken(DocLexer::T_CLOSE_PARENTHESIS)) {
      $values = $this->values();
    }

    $this->match(DocLexer::T_CLOSE_PARENTHESIS);

    return $values;
  }

  /**
   * Values ::= Array | Value {"," Value}* [","].
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function values() {
    $values = array($this->xValue());

    while ($this->lexer->isNextToken(DocLexer::T_COMMA)) {
      $this->match(DocLexer::T_COMMA);

      if ($this->lexer->isNextToken(DocLexer::T_CLOSE_PARENTHESIS)) {
        break;
      }

      $token = $this->lexer->lookahead;
      $value = $this->xValue();

      if (!is_object($value) && !is_array($value)) {
        $this->syntaxError('Value', $token);
      }

      $values[] = $value;
    }

    foreach ($values as $k => $value) {
      if (is_object($value) && $value instanceof \stdClass) {
        $values[$value->name] = $value->value;
      }
      elseif (!isset($values['value'])) {
        $values['value'] = $value;
      }
      else {
        if (!is_array($values['value'])) {
          $values['value'] = array($values['value']);
        }

        $values['value'][] = $value;
      }

      unset($values[$k]);
    }

    return $values;
  }

  /**
   * Constant ::= integer | string | float | boolean.
   *
   * @return mixed
   *   FIX - insert comment here.
   *
   * @throws AnnotationException
   */
  private function xConstant() {
    $identifier = $this->identifier();

    if (!defined($identifier) && FALSE !== strpos($identifier, '::') && '\\' !== $identifier[0]) {
      list($className, $const) = explode('::', $identifier);

      $alias = (FALSE === $pos = strpos($className, '\\')) ? $className : substr($className, 0, $pos);
      $found = FALSE;

      switch (TRUE) {
        case !empty($this->namespaces):
          foreach ($this->namespaces as $ns) {
            if (class_exists($ns . '\\' . $className) || interface_exists($ns . '\\' . $className)) {
              $className = $ns . '\\' . $className;
              $found = TRUE;
              break;
            }
          }
          break;

        case isset($this->imports[$loweredAlias = strtolower($alias)]):
          $found     = TRUE;
          $className = (FALSE !== $pos)
            ? $this->imports[$loweredAlias] . substr($className, $pos)
            : $this->imports[$loweredAlias];
          break;

        default:
          if (isset($this->imports['__NAMESPACE__'])) {
            $ns = $this->imports['__NAMESPACE__'];

            if (class_exists($ns . '\\' . $className) || interface_exists($ns . '\\' . $className)) {
              $className = $ns . '\\' . $className;
              $found = TRUE;
            }
          }
          break;
      }

      if ($found) {
        $identifier = $className . '::' . $const;
      }
    }

    // Checks if identifier ends with ::class, \strlen('::class') === 7.
    $classPos = stripos($identifier, '::class');
    if ($classPos === strlen($identifier) - 7) {
      return substr($identifier, 0, $classPos);
    }

    if (!defined($identifier)) {
      throw AnnotationException::semanticalErrorConstants($identifier, $this->context);
    }

    return constant($identifier);
  }

  /**
   * Identifier ::= string.
   *
   * @return string
   *   FIX - insert comment here.
   */
  private function identifier() {
    // Check if we have an annotation.
    if (!$this->lexer->isNextTokenAny(self::$classIdentifiers)) {
      $this->syntaxError('namespace separator or identifier');
    }

    $this->lexer->moveNext();

    $className = $this->lexer->token['value'];

    while (isset($this->lexer->lookahead['position']) && $this->lexer->lookahead['position'] === ($this->lexer->token['position'] + strlen($this->lexer->token['value']))
            && $this->lexer->isNextToken(DocLexer::T_NAMESPACE_SEPARATOR)) {
      $this->match(DocLexer::T_NAMESPACE_SEPARATOR);
      $this->matchAny(self::$classIdentifiers);

      $className .= '\\' . $this->lexer->token['value'];
    }

    return $className;
  }

  /**
   * Value ::= plainValue | fieldAssignment.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  private function xValue() {
    $peek = $this->lexer->glimpse();

    if (DocLexer::T_EQUALS === $peek['type']) {
      return $this->fieldAssignment();
    }

    return $this->plainValue();
  }

  /**
   * A plainValue ::= integer | string | float | boolean | Array | Annotation.
   *
   * @return mixed
   *   FIX - insert comment here.
   */
  private function plainValue() {
    if ($this->lexer->isNextToken(DocLexer::T_OPEN_CURLY_BRACES)) {
      return $this->arrayX();
    }

    if ($this->lexer->isNextToken(DocLexer::T_AT)) {
      return $this->annotation();
    }

    if ($this->lexer->isNextToken(DocLexer::T_IDENTIFIER)) {
      return $this->xConstant();
    }

    switch ($this->lexer->lookahead['type']) {
      case DocLexer::T_STRING:
        $this->match(DocLexer::T_STRING);
        return $this->lexer->token['value'];

      case DocLexer::T_INTEGER:
        $this->match(DocLexer::T_INTEGER);
        return (int) $this->lexer->token['value'];

      case DocLexer::T_FLOAT:
        $this->match(DocLexer::T_FLOAT);
        return (float) $this->lexer->token['value'];

      case DocLexer::T_TRUE:
        $this->match(DocLexer::T_TRUE);
        return TRUE;

      case DocLexer::T_FALSE:
        $this->match(DocLexer::T_FALSE);
        return FALSE;

      case DocLexer::T_NULL:
        $this->match(DocLexer::T_NULL);
        return NULL;

      default:
        $this->syntaxError('plainValue');
    }
  }

  /**
   * FIX - insert comment here.
   *
   * A fieldAssignment ::= FieldName "=" plainValue
   * FieldName ::= identifier.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function fieldAssignment() {
    $this->match(DocLexer::T_IDENTIFIER);
    $fieldName = $this->lexer->token['value'];

    $this->match(DocLexer::T_EQUALS);

    $item = new \stdClass();
    $item->name  = $fieldName;
    $item->value = $this->plainValue();

    return $item;
  }

  /**
   * Array ::= "{" arrayEntry {"," arrayEntry}* [","] "}".
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function arrayX() {
    $array = $values = array();

    $this->match(DocLexer::T_OPEN_CURLY_BRACES);

    // If the array is empty, stop parsing and return.
    if ($this->lexer->isNextToken(DocLexer::T_CLOSE_CURLY_BRACES)) {
      $this->match(DocLexer::T_CLOSE_CURLY_BRACES);

      return $array;
    }

    $values[] = $this->arrayEntry();

    while ($this->lexer->isNextToken(DocLexer::T_COMMA)) {
      $this->match(DocLexer::T_COMMA);

      // Optional trailing comma.
      if ($this->lexer->isNextToken(DocLexer::T_CLOSE_CURLY_BRACES)) {
        break;
      }

      $values[] = $this->arrayEntry();
    }

    $this->match(DocLexer::T_CLOSE_CURLY_BRACES);

    foreach ($values as $value) {
      list ($key, $val) = $value;

      if ($key !== NULL) {
        $array[$key] = $val;
      }
      else {
        $array[] = $val;
      }
    }

    return $array;
  }

  /**
   * FIX - insert comment here.
   *
   * A arrayEntry ::= Value | KeyValuePair
   * KeyValuePair ::= Key ("=" | ":") plainValue | Constant
   * Key ::= string | integer | Constant.
   *
   * @return array
   *   FIX - insert comment here.
   */
  private function arrayEntry() {
    $peek = $this->lexer->glimpse();

    if (DocLexer::T_EQUALS === $peek['type']
      || DocLexer::T_COLON === $peek['type']) {

      if ($this->lexer->isNextToken(DocLexer::T_IDENTIFIER)) {
        $key = $this->xConstant();
      }
      else {
        $this->matchAny(array(DocLexer::T_INTEGER, DocLexer::T_STRING));
        $key = $this->lexer->token['value'];
      }

      $this->matchAny(array(DocLexer::T_EQUALS, DocLexer::T_COLON));

      return array($key, $this->plainValue());
    }

    return array(NULL, $this->xValue());
  }

}
