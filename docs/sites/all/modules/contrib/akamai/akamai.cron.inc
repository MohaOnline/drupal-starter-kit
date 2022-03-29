<?php

/**
 * @file
 * Contains functions for running cron tasks.
 *
 * @see akamai_cron()
 */

/**
 * Processes queued purge requests and submits them to the CCU API in batches.
 */
function akamai_cron_process_queue() {
  $end = time() + variable_get('akamai_cron_queue_time_limit', AKAMAI_CRON_QUEUE_TIME_LIMIT_DEFAULT);
  $queue = DrupalQueue::get('akamai_ccu');
  $client = akamai_get_client();
  $batch_size = variable_get('akamai_batch_size', NULL);
  $batcher = new \Drupal\akamai\Batcher($client, $batch_size);

  while (time() < $end && ($item = $queue->claimItem())) {
    try {
      $batcher->insertItem($item);
    }
    catch (Exception $e) {
      watchdog_exception('akamai', $e);
    }
  }

  // Submit at least one batch.
  // Continue submitting batches until time limit is reached.
  while (!$batcher->isEmpty()) {
    $batch = $batcher->getBatch();
    $response = akamai_submit_batch($batch);
    if (!empty($response) || !variable_get('akamai_queue_on_failure', TRUE)) {
      foreach ($batch->getItems() as $item) {
        $queue->deleteItem($item);
      }
    }
    else {
      foreach ($batch->getItems() as $item) {
        $queue->releaseItem($item);
      }
    }

    if (time() > $end) {
      break;
    }
  }

  // Release any items that didn't make it into a batch.
  if (!$batcher->isEmpty()) {
    $unsubmitted_items = $batcher->getItems();
    foreach ($unsubmitted_items as $item) {
      $queue->releaseItem($item);
    }
  }
}

/**
 * Submits a batch of queued purge request items to the CCU API.
 */
function akamai_submit_batch(\Drupal\akamai\Batch $batch) {
  try {
    return akamai_submit_purge_request($batch->getHostname(), $batch->getPaths());
  }
  catch (Exception $e) {
    watchdog_exception('akamai', $e);
  }
  return FALSE;
}

/**
 * Checks the status of pending purge requests.
 */
function akamai_cron_check_status() {
  $end = time() + variable_get('akamai_purge_status_time_limit', AKAMAI_PURGE_STATUS_TIME_LIMIT_DEFAULT);

  $query = db_select('akamai_purge_requests', 'pr')
    ->fields('pr', ['purge_id', 'progress_uri'])
    ->condition('check_after', REQUEST_TIME, '<')
    ->isNull('completion_time')
    ->orderBy('submission_time', 'ASC')
    ->range(0, 200);
  $result = $query->execute();

  while (time() < $end && ($item = $result->fetchObject())) {
    _akamai_update_status($item->progress_uri, $item->purge_id);
  }

  _akamai_clear_purge_request_data();
}

/**
 * Checks the status of a purge request and updates its record.
 */
function _akamai_update_status($progress_uri, $purge_id) {
  try {
    $time = time();
    $client = akamai_get_client();
    $response = $client->checkProgress($progress_uri);
    $record = [
      'purge_id' => $purge_id,
      'last_checked' => $time,
      'status' => $response->purgeStatus,
    ];
    if (empty($response->completionTime)) {
      $record['check_after'] = $time + $response->pingAfterSeconds;
    }
    else {
      $record['completion_time'] = strtotime($response->completionTime);
    }
    return drupal_write_record('akamai_purge_requests', $record, 'purge_id');
  }
  catch (Exception $e) {
    watchdog_exception('akamai', $e);
  }
  return FALSE;
}

/**
 * Deletes old purge request items.
 */
function _akamai_clear_purge_request_data() {
  try {
    $hours = variable_get('akamai_purge_log_duration_complete', AKAMAI_PURGE_REQUEST_HOURS_TO_KEEP_DEFAULT);
    if (!empty($hours)) {
      db_delete('akamai_purge_requests')
        ->condition('completion_time', strtotime($hours . ' hours ago'), '<')
        ->execute();
    }

    $hours = variable_get('akamai_purge_log_duration_incomplete', AKAMAI_PURGE_REQUEST_HOURS_TO_KEEP_DEFAULT);
    if (!empty($hours)) {
      db_delete('akamai_purge_requests')
        ->condition('submission_time', strtotime($hours . ' hours ago'), '<')
        ->execute();
    }
  }
  catch (Exception $e) {
    watchdog_exception('akamai', $e);
  }
}
