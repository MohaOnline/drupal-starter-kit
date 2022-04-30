<?php

namespace OpenlayersDoctrine\Common\Annotations;

/**
 * Description of AnnotationException.
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
 */
class AnnotationException extends \Exception {

  /**
   * Creates a new AnnotationException describing a Syntax error.
   *
   * @param string $message
   *   Exception message.
   *
   * @return AnnotationException
   *   FIX.
   */
  public static function syntaxError($message) {
    return new self('[Syntax Error] ' . $message);
  }

  /**
   * Creates a new AnnotationException describing a Semantical error.
   *
   * @param string $message
   *   Exception message.
   *
   * @return AnnotationException
   *   FIX.
   */
  public static function semanticalError($message) {
    return new self('[Semantical Error] ' . $message);
  }

  /**
   * Creates a new AnnotationException.
   *
   * Creates a new AnnotationException describing an error which occurred during
   * the creation of the annotation.
   *
   * @param string $message
   *   FIX.
   *
   * @return AnnotationException
   *   FIX.
   */
  public static function creationError($message) {
    return new self('[Creation Error] ' . $message);
  }

  /**
   * Creates a new AnnotationException describing a type error.
   *
   * @param string $message
   *   FIX.
   *
   * @return AnnotationException
   *   FIX.
   */
  public static function typeError($message) {
    return new self('[Type Error] ' . $message);
  }

  /**
   * Creates a new AnnotationException describing a constant semantical error.
   *
   * @param string $identifier
   *   FIX.
   * @param string $context
   *   FIX.
   *
   * @return AnnotationException
   *   FIX.
   *
   * @since 2.3
   */
  public static function semanticalErrorConstants($identifier, $context = NULL) {
    return self::semanticalError(sprintf(
      "Couldn't find constant %s%s.",
      $identifier,
      $context ? ', ' . $context : ''
    ));
  }

  /**
   * Creates a new AnnotationException describing an type error of an attribute.
   *
   * @param string $attributeName
   *   FIX.
   * @param string $annotationName
   *   FIX.
   * @param string $context
   *   FIX.
   * @param string $expected
   *   FIX.
   * @param mixed $actual
   *   FIX.
   *
   * @return AnnotationException
   *   FIX.
   *
   * @since 2.2
   */
  public static function attributeTypeError($attributeName, $annotationName, $context, $expected, $actual) {
    return self::typeError(sprintf(
      'Attribute "%s" of @%s declared on %s expects %s, but got %s.',
      $attributeName,
      $annotationName,
      $context,
      $expected,
      is_object($actual) ? 'an instance of ' . get_class($actual) : gettype($actual)
    ));
  }

  /**
   * Creates a new AnnotationException.
   *
   * Creates a new AnnotationException describing an required error of an
   * attribute.
   *
   * @param string $attributeName
   *   FIX.
   * @param string $annotationName
   *   FIX.
   * @param string $context
   *   FIX.
   * @param string $expected
   *   FIX.
   *
   * @return AnnotationException
   *   FIX.
   *
   * @since 2.2
   */
  public static function requiredError($attributeName, $annotationName, $context, $expected) {
    return self::typeError(sprintf(
      'Attribute "%s" of @%s declared on %s expects %s. This value should not be null.',
      $attributeName,
      $annotationName,
      $context,
      $expected
    ));
  }

  /**
   * Creates a new AnnotationException describing a invalid enummerator.
   *
   * @param string $attributeName
   *   FIX.
   * @param string $annotationName
   *   FIX.
   * @param string $context
   *   FIX.
   * @param array $available
   *   FIX.
   * @param mixed $given
   *   FIX.
   *
   * @return AnnotationException
   *   FIX.
   *
   * @since 2.4
   */
  public static function enumeratorError($attributeName, $annotationName, $context, array $available, $given) {
    return new self(sprintf(
      '[Enum Error] Attribute "%s" of @%s declared on %s accept only [%s], but got %s.',
      $attributeName,
      $annotationName,
      $context,
      implode(', ', $available),
      is_object($given) ? get_class($given) : $given
    ));
  }

  /**
   * FIX.
   *
   * @return AnnotationException
   *   FIX.
   */
  public static function optimizerPlusSaveComments() {
    return new self(
      "You have to enable opcache.save_comments=1 or zend_optimizerplus.save_comments=1."
    );
  }

  /**
   * FIX.
   *
   * @return AnnotationException
   *   FIX.
   */
  public static function optimizerPlusLoadComments() {
    return new self(
      "You have to enable opcache.load_comments=1 or zend_optimizerplus.load_comments=1."
    );
  }

}
