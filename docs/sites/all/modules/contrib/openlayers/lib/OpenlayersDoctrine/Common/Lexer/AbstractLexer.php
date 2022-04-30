<?php

namespace OpenlayersDoctrine\Common\Lexer;

/**
 * Base class for writing simple lexers, i.e. for creating small DSLs.
 *
 * @since 2.0
 *
 * @author Guilherme Blanco <guilhermeblanco@hotmail.com>
 * @author Jonathan Wage <jonwage@gmail.com>
 * @author Roman Borschel <roman@code-factory.org>
 */
abstract class AbstractLexer {
  /**
   * FIX - insert comment here.
   *
   * @var array
   *   Array of scanned tokens
   */
  private $tokens = array();

  /**
   * FIX - insert comment here.
   *
   * @var int
   *   Current lexer position in input string
   */
  private $position = 0;

  /**
   * FIX - insert comment here.
   *
   * @var int
   *   Current peek of current lexer position
   */
  private $peek = 0;

  /**
   * FIX - insert comment here.
   *
   * @var array
   *   The next token in the input.
   */
  public $lookahead;

  /**
   * FIX - insert comment here.
   *
   * @var array
   *   The last matched/seen token.
   */
  public $token;

  /**
   * Sets the input data to be tokenized.
   *
   * The Lexer is immediately reset and the new input tokenized.
   * Any unprocessed tokens from any previous input are lost.
   *
   * @param string $input
   *   The input to be tokenized.
   */
  public function setInput($input) {
    $this->tokens = array();
    $this->reset();
    $this->scan($input);
  }

  /**
   * Resets the lexer.
   */
  public function reset() {
    $this->lookahead = NULL;
    $this->token = NULL;
    $this->peek = 0;
    $this->position = 0;
  }

  /**
   * Resets the peek pointer to 0.
   */
  public function resetPeek() {
    $this->peek = 0;
  }

  /**
   * Resets the lexer position on the input to the given position.
   *
   * @param int $position
   *   Position to place the lexical scanner.
   */
  public function resetPosition($position = 0) {
    $this->position = $position;
  }

  /**
   * Checks whether a given token matches the current lookahead.
   *
   * @param int $token
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isNextToken($token) {
    return NULL !== $this->lookahead && $this->lookahead['type'] === $token;
  }

  /**
   * Checks whether any of the given tokens matches the current lookahead.
   *
   * @param array $tokens
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isNextTokenAny(array $tokens) {
    return NULL !== $this->lookahead && in_array($this->lookahead['type'], $tokens, TRUE);
  }

  /**
   * Moves to the next token in the input string.
   *
   * A token is an associative array containing three items:
   *  - 'value'    : the string value of the token in the input string
   *  - 'type'     : the type of the token (identifier, numeric, string, input
   *                 parameter, none)
   *  - 'position' : the position of the token in the input string.
   *
   * @return array|null
   *   the next token; null if there is no more tokens left
   */
  public function moveNext() {
    $this->peek = 0;
    $this->token = $this->lookahead;
    $this->lookahead = (isset($this->tokens[$this->position]))
            ? $this->tokens[$this->position++] : NULL;

    return $this->lookahead !== NULL;
  }

  /**
   * FIX - insert comment here.
   *
   * Tells the lexer to skip input tokens until it sees a token with the
   * given value.
   *
   * @param string $type
   *   The token type to skip until.
   */
  public function skipUntil($type) {
    while ($this->lookahead !== NULL && $this->lookahead['type'] !== $type) {
      $this->moveNext();
    }
  }

  /**
   * Checks if given value is identical to the given token.
   *
   * @param mixed $value
   *   FIX - insert comment here.
   * @param int $token
   *   FIX - insert comment here.
   *
   * @return bool
   *   FIX - insert comment here.
   */
  public function isA($value, $token) {
    return $this->getType($value) === $token;
  }

  /**
   * Moves the lookahead token forward.
   *
   * @return array|null
   *   The next token or NULL if there are no more tokens ahead.
   */
  public function peek() {
    if (isset($this->tokens[$this->position + $this->peek])) {
      return $this->tokens[$this->position + $this->peek++];
    }
    else {
      return NULL;
    }
  }

  /**
   * Peeks at the next token, returns it and immediately resets the peek.
   *
   * @return array|null
   *   The next token or NULL if there are no more tokens ahead.
   */
  public function glimpse() {
    $peek = $this->peek();
    $this->peek = 0;
    return $peek;
  }

  /**
   * Scans the input string for tokens.
   *
   * @param string $input
   *   A query string.
   */
  protected function scan($input) {
    static $regex;

    if (!isset($regex)) {
      $regex = '/(' . implode(')|(', $this->getCatchablePatterns()) . ')|'
                   . implode('|', $this->getNonCatchablePatterns()) . '/i';
    }

    $flags = PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE | PREG_SPLIT_OFFSET_CAPTURE;
    $matches = preg_split($regex, $input, -1, $flags);

    foreach ($matches as $match) {
      // Must remain before 'value' assignment since it can change content.
      $type = $this->getType($match[0]);

      $this->tokens[] = array(
        'value' => $match[0],
        'type'  => $type,
        'position' => $match[1],
      );
    }
  }

  /**
   * Gets the literal for a given token.
   *
   * @param int $token
   *   FIX - insert comment here.
   *
   * @return string
   *   FIX - insert comment here.
   */
  public function getLiteral($token) {
    $className = get_class($this);
    $reflClass = new \ReflectionClass($className);
    $constants = $reflClass->getConstants();

    foreach ($constants as $name => $value) {
      if ($value === $token) {
        return $className . '::' . $name;
      }
    }

    return $token;
  }

  /**
   * Lexical catchable patterns.
   *
   * @return array
   *   FIX - insert comment here.
   */
  abstract protected function getCatchablePatterns();

  /**
   * Lexical non-catchable patterns.
   *
   * @return array
   *   FIX - insert comment here.
   */
  abstract protected function getNonCatchablePatterns();

  /**
   * Retrieve token type. Also processes the token value if necessary.
   *
   * @param string $value
   *   FIX - insert comment here.
   *
   * @return int
   *   FIX - insert comment here.
   */
  abstract protected function getType(&$value);

}
