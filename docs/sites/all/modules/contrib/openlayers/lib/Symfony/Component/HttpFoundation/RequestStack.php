<?php

namespace OpenlayersSymfony\Component\HttpFoundation;

/**
 * Request stack that controls the lifecycle of requests.
 *
 * @author Benjamin Eberlei <kontakt@beberlei.de>
 */
class RequestStack {

  /**
   * FIX - insert comment here.
   *
   * @var Request[]
   */
  private $requests = array();

  /**
   * Pushes a Request on the stack.
   *
   * This method should generally not be called directly as the stack
   * management should be taken care of by the application itself.
   */
  public function push(Request $request) {
    $this->requests[] = $request;
  }

  /**
   * Pops the current request from the stack.
   *
   * This operation lets the current request go out of scope.
   *
   * This method should generally not be called directly as the stack
   * management should be taken care of by the application itself.
   *
   * @return Request|null
   *   FIX - insert comment here.
   */
  public function pop() {
    if (!$this->requests) {
      return;
    }

    return array_pop($this->requests);
  }

  /**
   * FIX - insert comment here.
   *
   * @return Request|null
   *   FIX - insert comment here.
   */
  public function getCurrentRequest() {
    return end($this->requests) ?: NULL;
  }

  /**
   * Gets the master Request.
   *
   * Be warned that making your code aware of the master request
   * might make it un-compatible with other features of your framework
   * like ESI support.
   *
   * @return Request|null
   *   FIX - insert comment here.
   */
  public function getMasterRequest() {
    if (!$this->requests) {
      return;
    }

    return $this->requests[0];
  }

  /**
   * Returns the parent request of the current.
   *
   * Be warned that making your code aware of the parent request
   * might make it un-compatible with other features of your framework
   * like ESI support.
   *
   * If current Request is the master request, it returns null.
   *
   * @return Request|null
   *   FIX - insert comment here.
   */
  public function getParentRequest() {
    $pos = count($this->requests) - 2;

    if (!isset($this->requests[$pos])) {
      return;
    }

    return $this->requests[$pos];
  }

}
