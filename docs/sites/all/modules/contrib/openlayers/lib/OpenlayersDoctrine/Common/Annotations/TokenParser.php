<?php

namespace OpenlayersDoctrine\Common\Annotations;

/**
 * Parses a file for namespaces/use/class declarations.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 * @author Christian Kaps <christian.kaps@mohiva.com>
 */
class TokenParser {

  /**
   * The token list.
   *
   * @var array
   */
  private $tokens;

  /**
   * The number of tokens.
   *
   * @var int
   */
  private $numTokens;

  /**
   * The current array pointer.
   *
   * @var int
   */
  private $pointer = 0;

  /**
   * FIX - insert comment here.
   *
   * @param string $contents
   *   FIX - insert comment here.
   */
  public function __construct($contents) {
    $this->tokens = token_get_all($contents);

    // The PHP parser sets internal compiler globals for certain things.
    // Annoyingly, the last docblock comment it saw gets stored in doc_comment.
    // When it comes to compile the next thing to be include()d this stored
    // doc_comment becomes owned by the first thing the compiler sees in the
    // file that it considers might have a
    // docblock. If the first thing in the file is a class without a doc block
    // this would cause calls to getDocBlock() on said class to return our
    // long lost doc_comment. Argh.
    // To workaround, cause the parser to parse an empty docblock. Sure
    // getDocBlock() will return this, but at least
    // it's harmless to us.
    token_get_all("<?php\n/**\n *\n */");

    $this->numTokens = count($this->tokens);
  }

  /**
   * Gets the next non whitespace and non comment token.
   *
   * @param bool $docCommentIsComment
   *   If TRUE then a doc comment is considered a comment and skipped.
   *   If FALSE then only whitespace and normal comments are skipped.
   *
   * @return array|null
   *   The token if exists, null otherwise.
   */
  public function next($docCommentIsComment = TRUE) {
    for ($i = $this->pointer; $i < $this->numTokens; $i++) {
      $this->pointer++;
      if ($this->tokens[$i][0] === T_WHITESPACE ||
        $this->tokens[$i][0] === T_COMMENT ||
        ($docCommentIsComment && $this->tokens[$i][0] === T_DOC_COMMENT)) {

        continue;
      }

      return $this->tokens[$i];
    }

    return NULL;
  }

  /**
   * Parses a single use statement.
   *
   * @return array
   *   A list with all found class names for a use statement.
   */
  public function parseUseStatement() {
    $class = '';
    $alias = '';
    $statements = array();
    $explicitAlias = FALSE;
    while (($token = $this->next())) {
      $isNameToken = $token[0] === T_STRING || $token[0] === T_NS_SEPARATOR;
      if (!$explicitAlias && $isNameToken) {
        $class .= $token[1];
        $alias = $token[1];
      }
      elseif ($explicitAlias && $isNameToken) {
        $alias .= $token[1];
      }
      elseif ($token[0] === T_AS) {
        $explicitAlias = TRUE;
        $alias = '';
      }
      elseif ($token === ',') {
        $statements[strtolower($alias)] = $class;
        $class = '';
        $alias = '';
        $explicitAlias = FALSE;
      }
      elseif ($token === ';') {
        $statements[strtolower($alias)] = $class;
        break;
      }
      else {
        break;
      }
    }

    return $statements;
  }

  /**
   * Gets all use statements.
   *
   * @param string $namespaceName
   *   The namespace name of the reflected class.
   *
   * @return array
   *   A list with all found use statements.
   */
  public function parseUseStatements($namespaceName) {
    $statements = array();
    while (($token = $this->next())) {
      if ($token[0] === T_USE) {
        $statements = array_merge($statements, $this->parseUseStatement());
        continue;
      }
      if ($token[0] !== T_NAMESPACE || $this->parseNamespace() != $namespaceName) {
        continue;
      }

      // Get fresh array for new namespace. This is to prevent the parser to
      // collect the use statements for a previous namespace with the same
      // name. This is the case if a namespace is defined twice
      // or if a namespace with the same name is commented out.
      $statements = array();
    }

    return $statements;
  }

  /**
   * Gets the namespace.
   *
   * @return string
   *   The found namespace.
   */
  public function parseNamespace() {
    $name = '';
    while (($token = $this->next()) && ($token[0] === T_STRING || $token[0] === T_NS_SEPARATOR)) {
      $name .= $token[1];
    }

    return $name;
  }

  /**
   * Gets the class name.
   *
   * @return string
   *   The found class name.
   */
  public function parseClass() {
    // Namespaces and class names are tokenized the same: T_STRINGs
    // separated by T_NS_SEPARATOR so we can use one function to provide
    // both.
    return $this->parseNamespace();
  }

}
