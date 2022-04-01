<?php

/**
 * @file
 * The Pretty Printer class for PHP code.
 */

use PhpParser\Comment;
use PhpParser\Node;
use PhpParser\Node\Scalar;
use PhpParser\Node\Expr;
use PhpParser\Node\Stmt;
use PhpParser\PrettyPrinter\Standard;

module_load_include('inc', 'api', 'api.utilities');

/**
 * Pretty-prints code for Drupal coding standards and HTML output.
 *
 * This class overrides the standard PrettyPrinter class from the PhpParser
 * project, so that the output conforms more closely with the Drupal project
 * coding standards. Modifications:
 * - Indentation is 2 spaces instead of 4 spaces by default, with an option
 *   to set it to a different amount.
 * - In class and function declarations, the { is on the same line instead of
 *   the next line.
 * - There is a blank line before the closing } in class declarations.
 * - There is a line of vertical whitespace before each comment block.
 * - Individual single-line '//' comments are combined into blocks.
 * - Arrays are printed out multi-line instead of single-line, and comments are
 *   printed inside arrays. Exception: empty arrays on one line.
 * - HTML spans are added for highlighting and linking.
 * - Chained methods are split into lines.
 * - Space at ends of lines is removed.
 */
class ApiPrettyPrinter extends Standard {

  /**
   * Current state, to keep track of certain function calls and the like.
   *
   * This is only tracked if HTML spans are turned on.
   *
   * Array elements:
   * - last_string: The most recent literal '' or "" string to be printed.
   * - function_calls: Stack of names of method and function calls we are
   *   currently in.
   * - first_argument: TRUE if printing the first argument of the deepest-level
   *   function in the function_calls stack.
   * - in_array: TRUE if we are currently printing an array element.
   * - in_string: TRUE if we are currently printing the content of a literal
   *   string.
   * - array_key: Key of the array element we are printing, if it is a string.
   *
   * @var array
   */
  protected $state = array(
    'function_calls' => array(),
    'in_array' => FALSE,
    'in_string' => FALSE,
  );

  /**
   * Constructs an ApiPrettyPrinter object.
   *
   * @param array $options
   *   Array of options, including:
   *   - shortArraySyntax: TRUE to use [] when printing arrays instead of
   *     array(), for unspecified arrays. Default is TRUE, unlike the base
   *     class.
   *   - indentation: String to use for indentation. Defaults to two spaces,
   *     unlike the base class, which has no option but indents 4 spaces.
   *   - html: TRUE (default) to add HTML spans to the code.
   *   - isDrupal: TRUE (default) if this is Drupal code.
   */
  public function __construct(array $options = array()) {
    $options += array(
      'shortArraySyntax' => TRUE,
      'indentation' => '  ',
      'html' => TRUE,
      'isDrupal' => TRUE,
    );

    parent::__construct($options);
  }

  /**
   * Returns a list of invoke function names for Drupal code.
   *
   * @return string[]
   *   Associative array whose keys are function or method names, and whose
   *   values are the type of Drupal hook invocation they represent, either
   *   'hook', 'theme', or 'alter'.
   *
   * @deprecated in api:7.x-2.2 and is removed from api:8.x-1.0. Use
   *   ApiPrettyPrinter::invokeFunctions() instead.
   *
   * @see https://www.drupal.org/project/api/issues/3059981
   */
  public static function invokeFunctionsList() {
    return array(
      '_field_invoke' => 'hook',
      '_field_invoke_default' => 'hook',
      '_field_invoke_multiple' => 'hook',
      '_field_invoke_multiple_default' => 'hook',
      'bootstrap_invoke_all' => 'hook',
      'entity_invoke_bundle_hook' => 'hook',
      'getImplementations' => 'hook',
      'implementsHook' => 'hook',
      'invoke' => 'hook',
      'invokeAll' => 'hook',
      'invokeHook' => 'hook',
      'module_hook' => 'hook',
      'module_implements' => 'hook',
      'module_invoke' => 'hook',
      'module_invoke_all' => 'hook',
      'node_invoke' => 'hook',
      'user_module_invoke' => 'hook',
      'theme' => 'theme',
      'drupal_alter' => 'alter',
      'alter' => 'alter',
      'alterInfo' => 'alter',
    );
  }

  /**
   * Returns information about invoke function names for Drupal code.
   *
   * @return string[]
   *   Associative array whose keys are function or method names, and whose
   *   values are an associative array of information:
   *   - type: The type of Drupal hook invocation they represent, such as
   *     'hook', 'fieldhook', 'entityhook', 'userhook', 'theme', or 'alter'.
   *   - position: The position of the hook name parameter in the function
   *     parameter list, counting the first parameter as 0.
   */
  public static function invokeFunctions() {
    return array(
      '_field_invoke' => array('fieldhook', 1),
      '_field_invoke_default' => array('fieldhook', 1),
      '_field_invoke_multiple' => array('fieldhook', 1),
      '_field_invoke_multiple_default' => array('fieldhook', 1),
      'bootstrap_invoke_all' => array('hook', 1),
      'getImplementations' => array('hook', 1),
      'implementsHook' => array('hook', 2),
      'invoke' => array('hook', 2),
      'invokeAll' => array('hook', 1),
      'invokeHook' => array('entityhook', 1),
      'module_hook' => array('hook', 2),
      'module_implements' => array('hook', 1),
      'module_invoke' => array('hook', 2),
      'module_invoke_all' => array('hook', 1),
      'node_invoke' => array('hook', 1),
      'user_module_invoke' => array('userhook', 1),
      'theme' => array('theme', 1),
      'drupal_alter' => array('alter', 1),
      'alter' => array('alter', 1),
      'alterInfo' => array('alter', 1),
    );
  }

  /**
   * {@inheritdoc}
   */
  public function prettyPrintFile(array $stmts) {
    if (!$this->options['html']) {
      return parent::prettyPrintFile($stmts);
    }

    // Override to use HTML entities in output.
    $phptag = '<span class="php-boundary">' . htmlentities('<?php') . '</span>';
    $phpendtag = '<span class="php-boundary">' . htmlentities('?>') . '</span>';

    if (!$stmts) {
      return $phptag . "\n\n";
    }

    $p = $phptag . "\n\n" . $this->prettyPrint($stmts);

    if ($stmts[0] instanceof Stmt\InlineHTML) {
      $p = preg_replace('/^' . preg_quote($phptag, '/') . '\s+' . preg_quote($phpendtag, '/') . '\n?/', '', $p);
    }

    if ($stmts[count($stmts) - 1] instanceof Stmt\InlineHTML) {
      $p = preg_replace('/' . preg_quote($phptag, '/') . '$/', '', rtrim($p));
    }

    return $p;
  }

  /**
   * Overrides basic statement printing.
   *
   * Modifications:
   * - Indentation is from the option.
   * - Extra vertical whitespace.
   */
  protected function pStmts(array $nodes, $indent = TRUE) {
    $result = '';
    foreach ($nodes as $node) {
      $comments = $node->getAttribute('comments', array());
      if ($comments) {
        $result .= "\n\n" . $this->pComments($comments);
        if ($node instanceof Stmt\Nop) {
          continue;
        }
      }

      $result .= "\n" . $this->p($node) . ($node instanceof Expr ? ';' : '');
    }
    if ($indent) {
      // Indent lines.
      $result = preg_replace('~\n(?!$|' . $this->noIndentToken . ')~', "\n" . $this->options['indentation'], $result);
      // Remove trailing spaces at ends of lines (usually from indenting).
      $result = preg_replace('| +$|m', '', $result);
      return $result;
    }
    else {
      return $result;
    }
  }

  /**
   * Overrides pretty-printing of nodes to add HTML in some cases.
   *
   * @param \PhpParser\Node $node
   *   Node to be pretty printed.
   *
   * @return string
   *   Pretty printed node.
   */
  protected function p(Node $node) {
    $type = $node->getType();
    $type_pieces = explode('_', $type);

    if ($type == 'Stmt_If' || $type == 'Stmt_ElseIf' || $type == 'Stmt_Else') {
      // Override of if-type statements even if it is not HTML.
      $keyword = strtolower(array_pop($type_pieces));
      return $this->printIfLike($node, $keyword);
    }

    $easy_types = array('Expr_Isset', 'Expr_List', 'Expr_Clone',
      'Expr_Include', 'Expr_Exit', 'Expr_Empty', 'Expr_Eval',
      'Stmt_For', 'Stmt_Foreach', 'Stmt_While', 'Stmt_Do',
      'Stmt_Switch', 'Stmt_Case',
      'Stmt_TryCatch', 'Stmt_Catch', 'Stmt_Throw',
      'Stmt_Finally', 'Stmt_Break', 'Stmt_Continue',
      'Stmt_Return', 'Stmt_Goto', 'Stmt_Echo', 'Stmt_Static', 'Stmt_Global',
      'Stmt_Unset',
    );

    if ($this->options['html'] && !$this->state['in_string']) {
      // Overrides of certain simple statements if we are adding HTML and
      // not currently printing a string.
      if ($node instanceof Scalar\MagicConst) {
        $output = parent::p($node);
        return '<span class="php-keyword">' . $output . '</span>';
      }
      elseif ($type == 'Scalar_LNumber' || $type == 'Scalar_DNumber') {
        $output = parent::p($node);
        return '<span class="php-constant">' . $output . '</span>';
      }
      elseif ($node instanceof Expr\Cast) {
        $cast_type = strtolower(array_pop($type_pieces));
        return $this->pPrefixOp($type, '(<span class="php-keyword">' . $cast_type . '</span>) ', $node->expr);
      }
      elseif ($type == 'Expr_ConstFetch') {
        $output = parent::p($node);
        return '<span class="php-function-or-constant">' . $output . '</span>';
      }
      elseif (in_array($type, $easy_types)) {
        // In all of these types, the parent class output starts with a PHP
        // keyword, possibly preceded by a space. Wrap the keyword in a span.
        $output = parent::p($node);
        $output = preg_replace('|^( *)([a-z]+)|', '$1<span class="php-keyword">$2</span>', $output);
        return $output;
      }

    }

    // If we have not overridden anything and returned already, use the parent.
    return parent::p($node);
  }

  /**
   * {@inheritdoc}
   */
  protected function pPrec(Node $node, $parentPrecedence, $parentAssociativity, $childPosition) {
    // Overrides pPrec function so that it calls our p() instead of
    // shortcutting. Note: This has been fixed in the latest development code
    // in the project, but the fix isn't in a release yet.
    $type = $node->getType();
    if (isset($this->precedenceMap[$type])) {
      $childPrecedence = $this->precedenceMap[$type][0];
      if ($childPrecedence > $parentPrecedence
        || ($parentPrecedence == $childPrecedence && $parentAssociativity != $childPosition)
      ) {
        return '(' . $this->p($node) . ')';
      }
    }
    return $this->p($node);
  }

  /**
   * Overrides constant printing to add HTML.
   */
  protected function pConst(Node\Const_ $node) {
    if (!$this->options['html']) {
      return parent::pConst($node);
    }

    return '<span class="php-function-or-constant-declared">' . $node->name . '</span> = ' . $this->p($node->value);
  }

  /**
   * Overrides comment printing.
   *
   * Adjoining single-line '//' comments are combined into blocks together,
   * and there are empty lines between comment blocks. HTML spans are also
   * added.
   *
   * @param \PhpParser\Comment[] $comments
   *   List of comments.
   *
   * @return string
   *   Reformatted text of comments.
   */
  protected function pComments(array $comments) {
    $formattedComments = [];
    $prevType = 'not a comment';
    foreach ($comments as $comment) {
      $thisType = $this->findCommentPrefix($comment);
      $comment_text = $comment->getReformattedText();
      if ($this->options['html']) {
        $comment_text = htmlentities($comment_text);
      }

      if ($thisType == '//' && $prevType == '//') {
        // Combine this single-line comment with the previous one.
        $formattedComments[count($formattedComments) - 1] .= "\n" . $comment_text;
      }
      else {
        $formattedComments[] = $comment_text;
      }
      $prevType = $thisType;
    }

    if ($this->options['html']) {
      $span = '<span class="php-comment">';
      $endspan = '</span>';
    }
    else {
      $span = '';
      $endspan = '';
    }

    return $span . implode($endspan . "\n\n" . $span, $formattedComments) . $endspan;
  }

  /**
   * Figures out the comment prefix.
   *
   * @param \PhpParser\Comment $comment
   *   Comment object.
   *
   * @return string
   *   '//' if this is a single-line comment, or /** or /* if it is a multi-
   *   line comment.
   */
  protected function findCommentPrefix(Comment $comment) {
    $text = trim(preg_replace('|\s|', ' ', $comment->getText()));
    if (!$text) {
      return '';
    }
    $tokens = explode(' ', $text);
    return $tokens[0];
  }

  /**
   * Overrides string printing to add HTML spans and keep track of state.
   */
  protected function pScalar_String(Scalar\String_ $node) {
    if (!$this->options['html'] || $this->state['in_string']) {
      return parent::pScalar_String($node);
    }

    unset($this->state['last_string']);

    $span = $this->getStringSpan($node->value);
    $endspan = '</span>';

    $kind = $node->getAttribute('kind', Scalar\String_::KIND_SINGLE_QUOTED);
    switch ($kind) {
      case Scalar\String_::KIND_SINGLE_QUOTED:
        $this->state['last_string'] = $node->value;
        return $span . "'" . $this->pNoIndent(htmlentities(addcslashes($node->value, '\'\\'))) . "'" . $endspan;

      case Scalar\String_::KIND_DOUBLE_QUOTED:
        $this->state['last_string'] = $node->value;
        return $span . '"' . htmlentities($this->escapeString($node->value, '"')) . '"' . $endspan;

      default:
        return htmlentities(parent::pScalar_String($node));
    }
  }

  /**
   * Overrides string printing to add HTML spans.
   */
  protected function pScalar_Encapsed(Scalar\Encapsed $node) {
    if (!$this->options['html']) {
      return parent::pScalar_Encapsed($node);
    }

    unset($this->state['last_string']);

    if ($node->getAttribute('kind') === Scalar\String_::KIND_HEREDOC) {
      return htmlentities(parent::pScalar_Encapsed($node));
    }

    // Change the state to record we are inside a string.
    $this->state['in_string'] = TRUE;

    $string = $this->pEncapsList($node->parts, '"');
    $span = $this->getStringSpan($string);
    $endspan = '</span>';
    $this->state['last_string'] = $string;
    $this->state['in_string'] = FALSE;
    return $span . '"' . htmlentities($string) . '"' . $endspan;
  }

  /**
   * Overrides variable printing to add HTML spans.
   */
  protected function pExpr_Variable(Expr\Variable $node) {
    $output = parent::pExpr_Variable($node);

    // Add the HTML spans only if we aren't inside a string.
    if ($this->options['html'] && !$this->state['in_string']) {
      $output = '<span class="php-variable">' . $output . '</span>';
    }

    return $output;
  }

  /**
   * Calculates the string span to use, depending on the current state.
   *
   * @param string $string
   *   String to be spanned.
   *
   * @return string
   *   Span open tag for this string and the current state.
   */
  protected function getStringSpan($string) {
    $class = 'php-string';

    // If the string consists only of legal function characters, it could
    // be a theme hook name, element name, or hook name, if this is Drupal
    // code.
    if ($string && $this->options['isDrupal'] &&
      preg_match('|^' . API_RE_FUNCTION_CHARACTERS . '$|', $string)) {
      if ($this->state['in_array'] && isset($this->state['array_key'])) {
        if ($this->state['array_key'] == '#theme') {
          $class .= ' potential-theme';
        }
        elseif ($this->state['array_key'] == '#type') {
          $class .= ' potential-element';
        }
      }
      elseif (count($this->state['function_calls'])) {
        $last = $this->state['function_calls'][count($this->state['function_calls']) - 1];
        if (isset(static::invokeFunctions()[$last])) {
          $class .= ' potential-' . static::invokeFunctions()[$last][0];
        }
      }
    }

    return '<span class="' . $class . '">';
  }

  /**
   * Overrides function call printing to add HTML spans and keep track of state.
   */
  protected function pExpr_FuncCall(Expr\FuncCall $node) {
    if (!$this->options['html'] || $this->state['in_string']) {
      return parent::pExpr_FuncCall($node);
    }

    $name = $this->pCallLhs($node->name);

    $this->state['function_calls'][] = $name;
    $this->state['first_argument'] = TRUE;
    $args = $this->pCommaSeparated($node->args);
    array_pop($this->state['function_calls']);

    return '<span class="php-function-or-constant">' . $name . '</span>(' .
      $args . ')';
  }

  /**
   * Overrides method call printing to add HTML spans and split into lines.
   */
  protected function pExpr_MethodCall(Expr\MethodCall $node) {
    if (!$this->options['html'] || $this->state['in_string']) {
      // Only split into lines if we're not inside a string.
      $newline = ($this->state['in_string']) ? '' : "\n" . $this->options['indentation'];
      return $this->pDereferenceLhs($node->var) .
        $newline . '->' .
        $this->pObjectProperty($node->name) .
        '(' . $this->pOurMaybeMultiline($node->args) . ')';
    }

    $variable = $this->pDereferenceLhs($node->var);
    $variable_name = strip_tags($variable);

    $method_name = $this->pObjectProperty($node->name);
    $span_class = 'php-function-or-constant function';
    if ($variable_name == '$this') {
      $span_class .= ' member-of-self';
    }
    else {
      $span_class .= ' member-of-variable';
    }

    $this->state['function_calls'][] = $method_name;
    $this->state['first_argument'] = TRUE;
    $args = $this->pCommaSeparated($node->args);
    array_pop($this->state['function_calls']);

    return $variable . "\n" . $this->options['indentation'] .
      htmlentities('->') .
      '<span class="' . $span_class . '">' .
      $method_name . '</span>(' . $args . ')';
  }

  /**
   * Overrides static method call printing to add HTML spans.
   */
  protected function pExpr_StaticCall(Expr\StaticCall $node) {
    if (!$this->options['html'] || $this->state['in_string']) {
      return parent::pExpr_StaticCall($node);
    }

    $class_name = strip_tags($this->pDereferenceLhs($node->class));

    $method_name = ($node->name instanceof Expr ?
      ($node->name instanceof Expr\Variable ?
        $this->p($node->name) : '{' . $this->p($node->name) . '}')
      : $node->name);
    $span_class = 'php-function-or-constant function';
    if ($class_name == 'static' || $class_name == 'self') {
      $span_class .= ' member-of-self';
    }
    elseif ($class_name == 'parent') {
      $span_class .= ' member-of-parent';
    }
    else {
      $span_class .= ' member-of-class-' . $class_name;
    }

    $this->state['function_calls'][] = $method_name;
    $this->state['first_argument'] = TRUE;
    $args = $this->pCommaSeparated($node->args);
    array_pop($this->state['function_calls']);

    return '<span class="php-function-or-constant">' . $class_name .
      '</span>::<span class="' . $span_class . '">' . $method_name .
      '</span>(' . $args . ')';
  }

  /**
   * Overrides function argument printing to keep track of state.
   */
  protected function pArg(Node\Arg $node) {
    $output = parent::pArg($node);
    $this->state['first_argument'] = FALSE;
    return $output;
  }

  /**
   * Overrides function parameter printing to add HTML.
   */
  protected function pParam(Node\Param $node) {
    if (!$this->options['html'] || $this->state['in_string']) {
      return parent::pParam($node);
    }

    return ($node->type ? $this->pType($node->type) . ' ' : '') .
      ($node->byRef ? htmlentities('&') : '') .
      ($node->variadic ? '...' : '') .
      '<span class="php-variable">$' . $node->name . '</span>' .
      ($node->default ? ' = ' . $this->p($node->default) : '');
  }

  /**
   * Overrides property fetch printing to add HTML.
   */
  protected function pExpr_PropertyFetch(Expr\PropertyFetch $node) {
    if (!$this->options['html'] || $this->state['in_string']) {
      return parent::pExpr_PropertyFetch($node);
    }

    $variable = $this->pDereferenceLhs($node->var);
    $variable_name = strip_tags($variable);

    $property_name = $this->pObjectProperty($node->name);
    $span_class = 'php-function-or-constant property';
    if ($variable_name == '$this') {
      $span_class .= ' member-of-self';
    }
    else {
      $span_class .= ' member-of-variable';
    }

    return $variable . htmlentities('->') .
      '<span class="' . $span_class . '">' . $property_name . '</span>';
  }

  /**
   * Overrides static property fetch printing to add HTML.
   */
  protected function pExpr_StaticPropertyFetch(Expr\StaticPropertyFetch $node) {
    if (!$this->options['html'] || $this->state['in_string']) {
      return parent::pExpr_StaticPropertyFetch($node);
    }

    $class_name = strip_tags($this->pDereferenceLhs($node->class));

    $property_name = $this->pObjectProperty($node->name);
    $span_class = 'php-function-or-constant property';
    if ($class_name == 'static') {
      $span_class .= ' member-of-self';
    }
    elseif ($class_name == 'parent') {
      $span_class .= ' member-of-parent';
    }
    else {
      $span_class .= ' member-of-class-' . $class_name;
    }

    return '<span class="php-function-or-constant">' . $class_name .
      '</span>::$<span class="' . $span_class . '">' . $property_name .
      '</span>';
  }

  /**
   * Pretty-prints an array of nodes, comma separated, on separate lines.
   *
   * @param \PhpParser\Node[] $nodes
   *   Array of Nodes to be printed.
   * @param bool $comma_at_end
   *   (optional) TRUE (default) if there should be a comma at the end of the
   *   last element too.
   *
   * @return string
   *   Comma separated pretty printed nodes, with each on its own line,
   *   indented.
   */
  protected function pCommaSeparatedMultiLine(array $nodes, $comma_at_end = TRUE) {
    // Implode with commas and newlines.
    $result = $this->pImplode($nodes, ",\n");
    if ($comma_at_end && count($nodes)) {
      $result .= ',';
    }

    // Indent the whole thing.
    return $this->options['indentation'] . preg_replace('~\n(?!$|' . $this->noIndentToken . ')~', "\n" . $this->options['indentation'], $result) . "\n";
  }

  /**
   * Overrides array printing to use multiple lines.
   *
   * @see self::pExprArrayItem
   */
  protected function pExpr_Array(Expr\Array_ $node) {
    $syntax = $node->getAttribute('kind',
      $this->options['shortArraySyntax'] ? Expr\Array_::KIND_SHORT : Expr\Array_::KIND_LONG);
    if (empty($node->items)) {
      $items = '';
    }
    else {
      $items = "\n" . $this->pCommaSeparatedMultiLine($node->items);
    }

    if ($syntax === Expr\Array_::KIND_SHORT) {
      return '[' . $items . ']';
    }
    else {
      if ($this->options['html'] && !$this->state['in_string']) {
        return '<span class="php-keyword">array</span>(' . $items . ')';
      }
      else {
        return 'array(' . $items . ")";
      }
    }
  }

  /**
   * Overrides printing of array items to include comments and track state.
   */
  protected function pExpr_ArrayItem(Expr\ArrayItem $node) {
    $result = '';
    $comments = $node->getAttribute('comments', array());
    if ($comments) {
      $result .= $this->pComments($comments) . "\n";
    }

    unset($this->state['last_string']);
    $key = (is_null($node->key)) ? NULL : $this->p($node->key);
    if (isset($this->state['last_string'])) {
      $this->state['array_key'] = $this->state['last_string'];
    }
    $this->state['in_array'] = TRUE;

    $amp = (($this->options['html'] && !$this->state['in_string']) ? htmlentities('&') : '&');

    $result .= (NULL !== $key ? $key . ' => ' : '')
      . ($node->byRef ? $amp : '') . $this->p($node->value);

    $this->state['in_array'] = FALSE;
    unset($this->state['array_key']);

    return $result;
  }

  /**
   * Prints out an if/else/elseif statement.
   *
   * Overrides the default to have the { on the same line, and add HTML.
   *
   * @param \PhpParser\Node $node
   *   If, else, or elseif statement to print.
   * @param string $keyword
   *   Keyword to print, 'if', 'else', or 'elseif'.
   *
   * @return string
   *   Formatted statement.
   */
  protected function printIfLike(Node $node, $keyword) {
    $output = '';
    if ($keyword != 'if') {
      $output .= "\n";
    }

    if ($this->options['html']) {
      $output .= '<span class="php-keyword">' . $keyword . '</span>';
    }
    else {
      $output .= $keyword;
    }

    if ($keyword != 'else') {
      $output .= ' (' . $this->p($node->cond) . ')';
    }

    $output .= ' {' . $this->pStmts($node->stmts) . "\n" . '}';

    if ($keyword == 'if') {
      if (!is_null($node->elseifs)) {
        $output .= $this->pImplode($node->elseifs);
      }
      if (!is_null($node->else)) {
        $output .= $this->printIfLike($node->else, 'else');
      }
    }

    return $output;
  }

  /**
   * Overrides new printing to add HTML.
   */
  protected function pExpr_New(Expr\New_ $node) {
    if ($this->options['html']) {
      $new = '<span class="php-keyword">new</span> ';
      $class_span = '<span class="php-function-or-constant">';
      $class_end_span = '</span>';
    }
    else {
      $new = 'new ';
      $class_span = '';
      $class_end_span = '';
    }

    if ($node->class instanceof Stmt\Class_) {
      $args = $node->args ? '(' . $this->pCommaSeparated($node->args) . ')' : '';
      return $new . $this->pClassCommon($node->class, $args);
    }

    return $new . $class_span . $this->p($node->class) . $class_end_span . '(' . $this->pCommaSeparated($node->args) . ')';
  }

  /**
   * Overrides interface printing.
   *
   * Modifications:
   * - Put the { on the same line.
   * - Leave a blank line before the }.
   * - Add HTML.
   */
  protected function pStmt_Interface(Stmt\Interface_ $node) {
    if ($this->options['html']) {
      return '<span class="php-keyword">interface</span> <span class="php-function-or-constant">' . $node->name . '</span>' .
        (!empty($node->extends) ? ' <span class="php-keyword">extends</span> ' . $this->pCommaSeparated($node->extends) : '') .
        ' {' . $this->pStmts($node->stmts) . "\n\n" . '}';
    }
    else {
      return 'interface ' . $node->name .
        (!empty($node->extends) ? ' extends ' . $this->pCommaSeparated($node->extends) : '') .
        ' {' . $this->pStmts($node->stmts) . "\n\n" . '}';
    }
  }

  /**
   * Overrides trait printing.
   *
   * Modifications:
   * - Put the { on the same line.
   * - Leave a blank line before the }.
   * - Add HTML.
   */
  protected function pStmt_Trait(Stmt\Trait_ $node) {
    if ($this->options['html']) {
      return '<span class="php-keyword">trait</span> <span class="php-function-or-constant">' . $node->name . '</span>' .
        ' {' . $this->pStmts($node->stmts) . "\n\n" . '}';
    }
    else {
      return 'trait ' . $node->name . ' {' . $this->pStmts($node->stmts) . "\n\n" . '}';
    }
  }

  /**
   * Overrides class printing.
   *
   * Modifications:
   * - Put the { on the same line.
   * - Leave a blank line before the }.
   * - Add HTML.
   */
  protected function pClassCommon(Stmt\Class_ $node, $afterClassToken) {
    if ($this->options['html']) {
      return $this->pModifiers($node->flags) .
        '<span class="php-keyword">class</span><span class="php-function-or-constant">' . $afterClassToken . '</span>' .
        (NULL !== $node->extends ? ' <span class="php-keyword">extends</span> ' . $this->p($node->extends) : '') .
        (!empty($node->implements) ? ' <span class="php-keyword">implements</span> ' . $this->pCommaSeparated($node->implements) : '') .
        ' {' . $this->pStmts($node->stmts) . "\n\n" . '}';
    }
    else {
      return $this->pModifiers($node->flags) .
        'class' . $afterClassToken .
        (NULL !== $node->extends ? ' extends ' . $this->p($node->extends) : '') .
        (!empty($node->implements) ? ' implements ' . $this->pCommaSeparated($node->implements) : '') .
        ' {' . $this->pStmts($node->stmts) . "\n\n" . '}';
    }
  }

  /**
   * Overrides function printing to put the { on the same line and add HTML.
   */
  protected function pStmt_Function(Stmt\Function_ $node) {
    if ($this->options['html']) {
      $function = '<span class="php-keyword">function</span> ';
      $name_span = '<span class="php-function-or-constant-declared">';
      $name_end_span = '</span>';
      $amp = htmlentities('&');
    }
    else {
      $function = 'function ';
      $name_span = '';
      $name_end_span = '';
      $amp = '&';
    }

    return $function . ($node->byRef ? $amp : '') .
      $name_span . $node->name . $name_end_span .
      '(' . $this->pCommaSeparated($node->params) . ')' .
      (NULL !== $node->returnType ? ' : ' . $this->pType($node->returnType) : '') .
      ' {' . $this->pStmts($node->stmts) . "\n" . '}';
  }

  /**
   * Overrides class method printing to put the { on the same line.
   */
  protected function pStmt_ClassMethod(Stmt\ClassMethod $node) {
    if ($this->options['html']) {
      $function = '<span class="php-keyword">function</span> ';
      $name_span = '<span class="php-function-or-constant">';
      $name_end_span = '</span>';
      $amp = htmlentities('&');
    }
    else {
      $function = 'function ';
      $name_span = '';
      $name_end_span = '';
      $amp = '&';
    }

    return $this->pModifiers($node->flags) .
      $function . ($node->byRef ? $amp : '') .
      $name_span . $node->name . $name_end_span .
      '(' . $this->pCommaSeparated($node->params) . ')' .
      (NULL !== $node->returnType ? ' : ' . $this->pType($node->returnType) : '') .
      (NULL !== $node->stmts
        ? ' {' . $this->pStmts($node->stmts) . "\n" . '}'
        : ';');
  }

  /**
   * Overrides closure printing.
   *
   * Overrides:
   * - Space between the 'use' keyword and the use statements.
   * - Add HTML spans.
   */
  protected function pExpr_Closure(Expr\Closure $node) {
    if ($this->options['html']) {
      $span = '<span class="php-keyword">';
      $endspan = '</span>';
      $amp = htmlentities('&');
    }
    else {
      $span = '';
      $endspan = '';
      $amp = '&';
    }

    return ($node->static ? $span . 'static' . $endspan . ' ' : '')
      . $span . 'function' . $endspan . ' ' . ($node->byRef ? $amp : '')
      . '(' . $this->pCommaSeparated($node->params) . ')'
      . (!empty($node->uses) ? ' ' . $span . 'use' . $endspan . ' (' . $this->pCommaSeparated($node->uses) . ')' : '')
      . (NULL !== $node->returnType ? ' : ' . $this->pType($node->returnType) : '')
      . ' {' . $this->pStmts($node->stmts) . "\n" . '}';
  }

  /**
   * Overrides class constant printing to include HTML.
   */
  protected function pStmt_ClassConst(Stmt\ClassConst $node) {
    if (!$this->options['html']) {
      return parent::pStmt_ClassConst($node);
    }

    return $this->pModifiers($node->flags) .
      '<span class="php-keyword">const</span> ' .
      $this->pCommaSeparated($node->consts) . ';';
  }

  /**
   * Overrides constant printing to include HTML.
   */
  protected function pStmt_Const(Stmt\Const_ $node) {
    if (!$this->options['html']) {
      return parent::pStmt_Const($node);
    }

    return '<span class="php-keyword">const</span> ' .
      $this->pCommaSeparated($node->consts) . ';';
  }

  /**
   * Overrides printing of start/end ?php tags to include HTML.
   */
  protected function pStmt_InlineHTML(Stmt\InlineHTML $node) {
    if (!$this->options['html']) {
      return parent::pStmt_InlineHTML($node);
    }

    $newline = $node->getAttribute('hasLeadingNewline', TRUE) ? "\n" : '';

    return '<span class="php-boundary">' . htmlentities('?>') . '</span>' .
      htmlentities($this->pNoIndent($newline . $node->value)) .
      '<span class="php-boundary">' . htmlentities('<?php') . '</span>' . "\n";
  }

  /**
   * Overrides namespace printing to include HTML.
   */
  protected function pStmt_Namespace(Stmt\Namespace_ $node) {
    if (!$this->options['html']) {
      return parent::pStmt_Namespace($node);
    }

    if ($this->canUseSemicolonNamespaces) {
      return '<span class="php-keyword">namespace</span> ' .
        '<span class="php-function-or-constant">' . $this->p($node->name) . '</span>;' .
        "\n" . $this->pStmts($node->stmts, FALSE);
    }
    else {
      return '<span class="php-keyword">namespace</span>' .
        (NULL !== $node->name ?
          ' <span class="php-function-or-constant">' . $this->p($node->name) . '</span>' : '') .
        ' {' . $this->pStmts($node->stmts) . "\n" . '}';
    }
  }

  /**
   * Overrides printing of use statement to include HTML.
   */
  protected function pStmt_Use(Stmt\Use_ $node) {
    if (!$this->options['html']) {
      return parent::pStmt_Use($node);
    }

    return '<span class="php-keyword">use</span> ' .
      $this->pOurUsetype($node->type) . $this->pCommaSeparated($node->uses) . ';';
  }

  /**
   * Overrides printing of use statement to include HTML.
   */
  protected function pStmt_GroupUse(Stmt\GroupUse $node) {
    if (!$this->options['html']) {
      return parent::pStmt_GroupUse($node);
    }

    return '<span class="php-keyword">use</span> ' .
      $this->pOurUsetype($node->type) .
      '<span class="php-function-or-constant">' . $this->pName($node->prefix) . '</span>' .
      '\{' . $this->pCommaSeparated($node->uses) . '};';
  }

  /**
   * Overrides printing of use statement to include HTML.
   */
  protected function pStmt_TraitUse(Stmt\TraitUse $node) {
    if (!$this->options['html']) {
      return parent::pStmt_TraitUse($node);
    }

    return '<span class="php-keyword">use</span> ' .
      $this->pCommaSeparated($node->traits) .
      (empty($node->adaptations) ? ';'
        : ' {' . $this->pStmts($node->adaptations) . "\n" . '}');
  }

  /**
   * Overrides printing of use statement to include HTML.
   */
  protected function pStmt_TraitUseAdaptation_Precedence(Stmt\TraitUseAdaptation\Precedence $node) {
    if (!$this->options['html']) {
      return parent::pStmt_TraitUseAdaptation_Precedence($node);
    }

    $trait = $this->p($node->trait);
    $span_class = 'php-function-or-constant function member-of-class-' . $trait;

    return '<span class="php-function-or-constant">' . $trait . '</span>::' .
      '<span class="' . $span_class . '">' . $node->method . '</span>' .
      ' <span class="php-keyword">insteadof</span> ' . $this->pCommaSeparated($node->insteadof) . ';';
  }

  /**
   * Overrides printing of use statement to include HTML.
   */
  protected function pStmt_TraitUseAdaptation_Alias(Stmt\TraitUseAdaptation\Alias $node) {
    if (!$this->options['html']) {
      return parent::pStmt_TraitUseAdaptation_Alias($node);
    }

    $trait = (NULL != $node->trait ? $this->p($node->trait) : '');
    if ($trait) {
      $span_class = 'php-function-or-constant function member-of-class-' . $trait;
    }
    else {
      $span_class = 'php-function-or-constant function';
    }

    return ($trait ? '<span class="php-function-or-constant">' . $trait . '</span>::' : '') .
      '<span class="' . $span_class . '">' . $node->method . '</span> ' .
      '<span class="php-keyword">as</span>' .
      (NULL !== $node->newModifier ? ' ' . rtrim($this->pModifiers($node->newModifier), ' ') : '') .
      (NULL !== $node->newName ? ' <span class="php-function-or-constant">' . $node->newName . '</span>' : '') . ';';
  }

  /**
   * Overrides printing of use statement to include HTML.
   */
  protected function pStmt_UseUse(Stmt\UseUse $node) {
    if (!$this->options['html']) {
      return parent::pStmt_UseUse($node);
    }

    return $this->pOurUsetype($node->type) .
       '<span class="php-function-or-constant">' . $this->p($node->name) . '</span>' .
      ($node->name->getLast() !== $node->alias ? ' <span class="php-keyword">as</span> <span class="php-function-or-constant">' . $node->alias . '</span>' : '');
  }

  /**
   * The original pUsetype function was private, so we had to make our own.
   */
  protected function pOurUseType($type) {
    $keyword = $type === Stmt\Use_::TYPE_FUNCTION ? 'function'
      : ($type === Stmt\Use_::TYPE_CONSTANT ? 'const' : '');
    if (!$keyword || !$this->options['html']) {
      return $keyword;
    }
    return '<span class="php-keyword">' . $keyword . '</span> ';
  }

  /**
   * The original pMaybeMultiline function was private; make our own.
   */
  private function pOurMaybeMultiline(array $nodes, $trailingComma = FALSE) {
    if (!$this->ourHasNodeWithComments($nodes)) {
      return $this->pCommaSeparated($nodes);
    }
    else {
      return $this->pCommaSeparatedMultiline($nodes, $trailingComma) . "\n";
    }
  }

  /**
   * The original hasNodeWithComments function was private; make our own.
   */
  protected function ourHasNodeWithComments(array $nodes) {
    foreach ($nodes as $node) {
      if ($node && $node->getAttribute('comments')) {
        return TRUE;
      }
    }
    return FALSE;
  }

}
