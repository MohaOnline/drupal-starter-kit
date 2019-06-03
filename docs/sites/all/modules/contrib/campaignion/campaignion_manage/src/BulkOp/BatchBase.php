<?php

namespace Drupal\campaignion_manage\BulkOp;

class BatchBase {
  public function __construct(&$data) {}

  /**
   * Prepare for working on a new batch.
   *
   * @param array $context
   *   The batch-API context.
   */
  public function start(&$context) {
  }

  public function apply($contact, &$result) {}
  public function commit() {}
}
