<?php

namespace Drupal\campaignion_manage\BulkOp;

interface BatchInterface {
  /**
   * Process the current .
   *
   * @param array $data
   *   Parameters for the bulk-operation.
   */
  public function getBatch(&$data);

  /**
   * @param array $data
   *   Parameters for the bulk-operation.
   * @param array $results
   *   Extracted form $context['results'].
   */
  public function batchFinish(&$data, &$results);
}
