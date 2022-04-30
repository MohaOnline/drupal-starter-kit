<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Exception;

/**
 * This exception is thrown when a non-existent parameter is used.
 *
 * @author Fabien Potencier <fabien@symfony.com>
 */
class ParameterNotFoundException extends InvalidArgumentException {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $key;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $sourceId;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $sourceKey;

  /**
   * FIX - insert comment here.
   *
   * @var array
   */
  private $alternatives;

  /**
   * Constructor.
   *
   * @param string $key
   *   The requested parameter key.
   * @param string $sourceId
   *   The service id that references the non-existent parameter.
   * @param string $sourceKey
   *   The parameter key that references the non-existent parameter.
   * @param \Exception $previous
   *   The previous exception.
   * @param string[] $alternatives
   *   Some parameter name alternatives.
   */
  public function __construct($key, $sourceId = NULL, $sourceKey = NULL, \Exception $previous = NULL, array $alternatives = array()) {
    $this->key = $key;
    $this->sourceId = $sourceId;
    $this->sourceKey = $sourceKey;
    $this->alternatives = $alternatives;

    parent::__construct('', 0, $previous);

    $this->updateRepr();
  }

  /**
   * FIX - insert comment here.
   */
  public function updateRepr() {
    if (NULL !== $this->sourceId) {
      $this->message = sprintf('The service "%s" has a dependency on a non-existent parameter "%s".', $this->sourceId, $this->key);
    }
    elseif (NULL !== $this->sourceKey) {
      $this->message = sprintf('The parameter "%s" has a dependency on a non-existent parameter "%s".', $this->sourceKey, $this->key);
    }
    else {
      $this->message = sprintf('You have requested a non-existent parameter "%s".', $this->key);
    }

    if ($this->alternatives) {
      if (1 == count($this->alternatives)) {
        $this->message .= ' Did you mean this: "';
      }
      else {
        $this->message .= ' Did you mean one of these: "';
      }
      $this->message .= implode('", "', $this->alternatives) . '"?';
    }
  }

  /**
   * FIX - insert comment here.
   */
  public function getKey() {
    return $this->key;
  }

  /**
   * FIX - insert comment here.
   */
  public function getSourceId() {
    return $this->sourceId;
  }

  /**
   * FIX - insert comment here.
   */
  public function getSourceKey() {
    return $this->sourceKey;
  }

  /**
   * FIX - insert comment here.
   */
  public function setSourceId($sourceId) {
    $this->sourceId = $sourceId;

    $this->updateRepr();
  }

  /**
   * FIX - insert comment here.
   */
  public function setSourceKey($sourceKey) {
    $this->sourceKey = $sourceKey;

    $this->updateRepr();
  }

}
