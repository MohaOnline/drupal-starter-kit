<?php

namespace OpenlayersSymfony\Component\DependencyInjection\Exception;

/**
 * This exception is thrown when a non-existent service is requested.
 *
 * @author Johannes M. Schmitt <schmittjoh@gmail.com>
 */
class ServiceNotFoundException extends InvalidArgumentException {

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $id;

  /**
   * FIX - insert comment here.
   *
   * @var string
   */
  private $sourceId;

  /**
   * FIX - insert comment here.
   */
  public function __construct($id, $sourceId = NULL, \Exception $previous = NULL, array $alternatives = array()) {
    if (NULL === $sourceId) {
      $msg = sprintf('You have requested a non-existent service "%s".', $id);
    }
    else {
      $msg = sprintf('The service "%s" has a dependency on a non-existent service "%s".', $sourceId, $id);
    }

    if ($alternatives) {
      if (1 == count($alternatives)) {
        $msg .= ' Did you mean this: "';
      }
      else {
        $msg .= ' Did you mean one of these: "';
      }
      $msg .= implode('", "', $alternatives) . '"?';
    }

    parent::__construct($msg, 0, $previous);

    $this->id = $id;
    $this->sourceId = $sourceId;
  }

  /**
   * FIX - insert comment here.
   */
  public function getId() {
    return $this->id;
  }

  /**
   * FIX - insert comment here.
   */
  public function getSourceId() {
    return $this->sourceId;
  }

}
