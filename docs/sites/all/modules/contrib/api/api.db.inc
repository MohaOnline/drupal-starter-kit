<?php

/**
 * @file
 * Database CRUD operations for the API module.
 */

/**
 * Saves an API branch.
 *
 * @param object $branch
 *   A branch object.
 */
function api_save_branch($branch) {
  $branch->data = serialize($branch->data);
  // Update the queue valid timestamp for this branch so that old queue jobs
  // are invalid. But make sure it at least increments the old time by a
  // second.
  $min_time = isset($branch->queue_valid) ? $branch->queue_valid + 1 : 0;
  $new_time = time();
  $new_time = ($new_time < $min_time) ? $min_time : $new_time;
  $branch->queue_valid = $new_time;
  // Update the last updated timestamp for this branch so that it will be
  // updated as soon as possible.
  $branch->last_updated = 0;

  if (empty($branch->branch_id)) {
    drupal_write_record('api_branch', $branch);
  }
  else {
    drupal_write_record('api_branch', $branch, 'branch_id');

    // We've just updated a files branch, so we need to mark all of the files
    // that were in the parse queue so that they are re-queued next time
    // we scan this branch for files.
    $query = db_select('api_file', 'f')
      ->fields('f', array('did'))
      ->condition('queued', 0, '<>');
    $query->leftJoin('api_documentation', 'd', 'f.did = d.did');
    $dids = $query->condition('d.branch_id', $branch->branch_id)
      ->execute()
      ->fetchCol();
    if (count($dids)) {
      db_update('api_file')
        ->condition('did', $dids)
        ->fields(array('queued' => 0))
        ->execute();
    }
  }

  if ($branch->preferred) {
    // Set all other branches within this project/core compatibility to not
    // preferred, since this one is preferred.
    db_update('api_branch')
      ->condition('project', $branch->project)
      ->condition('core_compatibility', $branch->core_compatibility)
      ->condition('branch_name', $branch->branch_name, '<>')
      ->fields(array('preferred' => 0))
      ->execute();
  }

  api_reset_branches();
}

/**
 * Deletes a branch and all of its related documentation records.
 *
 * @param int $branch_id
 *   The numeric ID of the branch to delete.
 * @param bool $finalize
 *   TRUE to redo the menus etc., FALSE to postpone (because other deletes
 *   are coming).
 */
function api_delete_branch($branch_id, $finalize = TRUE) {
  // Find out which documentation IDs need to be deleted.
  $dids = db_select('api_documentation', 'ad')
    ->fields('ad', array('did'))
    ->condition('branch_id', $branch_id)
    ->execute()
    ->fetchCol();

  // Fully delete these items, reference counts, and the branch record.
  api_delete_items($dids);
  db_delete('api_reference_counts')
    ->condition('branch_id', $branch_id)
    ->execute();
  db_delete('api_branch')
    ->condition('branch_id', $branch_id)
    ->execute();

  if ($finalize) {
    api_reset_branches();
  }
}

/**
 * Saves an API reference branch.
 *
 * @param object $branch
 *   A reference branch object.
 */
function api_save_php_branch($branch) {
  // If core compatibility has not yet been set and we've just added an
  // API reference branch, set it now.
  $version = variable_get('api_default_core_compatibility', NULL);
  if (!$version && isset($branch->data['core_compatibility'])) {
    variable_set('api_default_core_compatibility', $branch->data['core_compatibility']);
  }

  $branch->data = serialize($branch->data);
  if (empty($branch->branch_id)) {
    drupal_write_record('api_php_branch', $branch);
  }
  else {
    drupal_write_record('api_php_branch', $branch, 'branch_id');
  }

  api_get_php_branches(TRUE);
}

/**
 * Deletes a reference branch and all of its related documentation records.
 *
 * @param int $branch_id
 *   The numeric ID of the branch to delete.
 */
function api_delete_php_branch($branch_id) {
  // This could be a PHP or API reference branch. We could check the type,
  // but it is also fine to just delete from both documentation tables.
  db_delete('api_php_documentation')
    ->condition('branch_id', $branch_id)
    ->execute();
  db_delete('api_external_documentation')
    ->condition('branch_id', $branch_id)
    ->execute();
  db_delete('api_php_branch')
    ->condition('branch_id', $branch_id)
    ->execute();

  api_get_php_branches(TRUE);
}

/**
 * Saves an API project.
 *
 * @param object $project
 *   A project to save.
 * @param string $old_name
 *   If the project_name (primary key) field is changing, provide the previous
 *   name.
 */
function api_save_project($project, $old_name = '') {
  if (!strlen($old_name)) {
    $old_name = $project->project_name;
  }
  db_delete('api_project')
    ->condition('project_name', $old_name)
    ->execute();
  drupal_write_record('api_project', $project);

  if ($old_name != $project->project_name) {
    // Update existing branches to the new name.
    db_update('api_branch')
      ->condition('project', $old_name)
      ->fields(array('project' => $project->project_name))
      ->execute();
  }

  // See if our current default project is valid, and reset if not.
  $default = variable_get('api_default_project', '');
  if (!api_project_exists($default)) {
    variable_set('api_default_project', $project->project_name);
  }

  api_reset_branches();
}

/**
 * Deletes a project and all of its branches and documentation records.
 *
 * @param string $project_name
 *   The name of the project to delete.
 */
function api_delete_project($project_name) {
  // Delete all of the branches.
  $branch_ids = db_select('api_branch', 'b')
    ->condition('project', $project_name)
    ->fields('b', array('branch_id'))
    ->execute()
    ->fetchCol();
  foreach ($branch_ids as $id) {
    api_delete_branch($id, FALSE);
  }

  // Delete the project itself.
  db_delete('api_project')
    ->condition('project_name', $project_name)
    ->execute();

  // Clean up.
  api_reset_branches();
}

/**
 * Checks to see if a project exists.
 *
 * @param string $project_name
 *   Name to check for.
 *
 * @return bool
 *   TRUE if the project exists, and FALSE if not.
 */
function api_project_exists($project_name) {
  $result = db_select('api_project', 'p')
    ->fields('p')
    ->condition('project_name', $project_name)
    ->execute()
    ->fetchObject();
  return isset($result->project_name) && $result->project_name == $project_name;
}

/**
 * Fixes up branches, after one is updated, added, or deleted.
 */
function api_reset_branches() {
  // Recalculate the default branch.
  $project = variable_get('api_default_project', '');
  $version = variable_get('api_default_core_compatibility', '');
  $branches = api_get_branches(TRUE);
  $default_id = variable_get('api_default_branch', -99);
  $values_set = FALSE;
  foreach ($branches as $branch) {
    if (($branch->project == $project) && $branch->preferred && ($branch->core_compatibility == $version) && ($default_id != $branch->branch_id)) {
      $values_set = TRUE;
      variable_set('api_default_branch', $branch->branch_id);
      break;
    }
  }

  $branches = api_get_branches(TRUE);

  // Make sure the default branch variable is set to something.
  $default_id = variable_get('api_default_branch', NULL);
  $default_branch = api_get_branch_by_id($default_id);
  if (!$default_branch && count($branches) > 0) {
    $default_branch = reset($branches);
  }

  if (isset($default_branch) && (!$default_id || ($default_branch->branch_id != $default_id))) {
    $values_set = TRUE;
    variable_set('api_default_branch', $default_branch->branch_id);
  }

  // Make sure the default project and core compatibility are set to agree
  // with the default branch.
  if ($default_branch && (!$project || ($project != $default_branch->project))) {
    $values_set = TRUE;
    variable_set('api_default_project', $default_branch->project);
  }
  if ($default_branch && (!$version || ($version != $default_branch->core_compatibility))) {
    $values_set = TRUE;
    variable_set('api_default_core_compatibility', $default_branch->core_compatibility);
  }

  if ($values_set) {
    drupal_set_message(t('Default branch, project, and/or core compatibility values were reset to stay consistent'));
  }

  // Reset the branch lists.
  drupal_static_reset('api_get_branch_names');
  $branches = api_get_branches(TRUE);

  // Reweight all branches.
  usort($branches, 'api_branch_sort');
  $weight = 0;
  foreach ($branches as $branch) {
    $branch->weight = $weight;
    $weight += 1;
    drupal_write_record('api_branch', $branch, 'branch_id');
  }
  drupal_static_reset('api_get_branches');

  // Tell the menu system it needs rebuilding.
  variable_set('menu_rebuild_needed', TRUE);
}

/**
 * Sorts branches by project, version number, and branch name.
 *
 * Callback for usort() within api_reset_branches().
 */
function api_branch_sort($a, $b) {
  // Sort by project.
  if ($a->project != $b->project) {
    $default_project = variable_get('api_default_project', '');
    // If one of a or b is in the default project and the other isn't, that
    // one is preferred.
    if ($a->project == $default_project) {
      return -1;
    }
    if ($b->project == $default_project) {
      return 1;
    }

    // If one is core and the other isn't, that's preferred.
    if ($a->project_type == 'core' && $b->project_type != 'core') {
      return -1;
    }
    if ($a->project_type != 'core' && $b->project_type == 'core') {
      return 1;
    }

    // Otherwise, sort alphabetically.
    return strcasecmp($a->project, $b->project);
  }

  // Sort by core compatibility.
  if ($a->core_compatibility != $b->core_compatibility) {
    $default_version = variable_get('api_default_core_compatibility', '');

    // If one of a or b is in the default compatibility and the other isn't,
    // that one is preferred.
    if ($a->core_compatibility == $default_version) {
      return -1;
    }
    if ($b->core_compatibility == $default_version) {
      return 1;
    }

    // Otherwise, sort by version.
    return version_compare($a->core_compatibility, $b->core_compatibility);
  }

  // Sort by preferred (preferred over not preferred within one project and
  // core compatibility).
  if ($a->preferred && !$b->preferred) {
    return -1;
  }
  if ($b->preferred && !$a->preferred) {
    return 1;
  }

  // If all else fails, just do alphabetical.
  return strcasecmp($a->branch_name, $b->branch_name);
}

/**
 * Completely deletes an array of documentation items.
 *
 * All records pertaining to the items, including nodes and comments, are
 * removed. If there are a lot, the batch API may be invoked, or a job may
 * be queued.
 *
 * @param int[] $dids
 *   Array of the IDs of the items to delete.
 * @param bool $interactive
 *   TRUE if the batch_set() can be used if there are too many items to delete;
 *   FALSE if queue jobs need to be set up instead.
 *
 * @return bool
 *   TRUE if the items were deleted directly; FALSE if a batch or queue job
 *   was set up to delete some of them. In this case, in interactive mode, the
 *   calling function needs to ensure that the batch is processed. This is
 *   automatic if it's part of a form submission handler, but not otherwise.
 */
function api_delete_items(array $dids, $interactive = TRUE) {
  // Count the cumulative effect of calls in this page request.
  $total_count = &drupal_static(__FUNCTION__, 0);

  // If there's nothing to do, return early.
  $count = count($dids);
  if ($count < 1) {
    return TRUE;
  }

  // Delete the items from api tables in batches of 100, because placeholder
  // queries can be a problem. This is fast though, so do not worry about
  // batching.
  $done = 0;
  $to_reparse = array();
  while ($done < $count) {
    $this_batch = array_slice($dids, $done, 100);
    $done += 100;

    db_delete('api_documentation')
      ->condition(db_or()
        ->condition('did', $this_batch)
        ->condition('class_did', $this_batch)
      )
      ->execute();
    db_delete('api_file')
      ->condition('did', $this_batch)
      ->execute();
    db_delete('api_namespace')
      ->condition('did', $this_batch)
      ->execute();
    db_delete('api_function')
      ->condition('did', $this_batch)
      ->execute();
    db_delete('api_members')
      ->condition(db_or()
        ->condition('did', $this_batch)
        ->condition('class_did', $this_batch)
      )
      ->execute();

    // For overrides, we want to delete any records for these IDs. But there
    // could also be overrides information referencing these. We don't want to
    // lose the records entirely, but we need to mark those items to be
    // reparsed so the overrides can be recalculated.
    db_delete('api_overrides')
      ->condition('did', $this_batch)
      ->execute();

    $bad_overrides = db_select('api_overrides', 'ao')
      ->condition('overrides_did', $this_batch)
      ->fields('ao', array('did'))
      ->execute()
      ->fetchCol();
    if (count($bad_overrides)) {
      db_update('api_overrides')
        ->fields(array('overrides_did' => 0))
        ->condition('did', $bad_overrides)
        ->execute();
      $to_reparse = array_merge($to_reparse, $bad_overrides);
    }

    $bad_overrides = db_select('api_overrides', 'ao')
      ->condition('documented_did', $this_batch)
      ->fields('ao', array('did'))
      ->execute()
      ->fetchCol();
    if (count($bad_overrides)) {
      db_update('api_overrides')
        ->fields(array('documented_did' => 0))
        ->condition('did', $bad_overrides)
        ->execute();
      $to_reparse = array_merge($to_reparse, $bad_overrides);
    }

    // We need to do something similar in api_reference_storage with the
    // extends_did column.
    db_delete('api_reference_storage')
      ->condition('from_did', $this_batch)
      ->execute();

    $bad_overrides = db_select('api_reference_storage', 'ars')
      ->condition('extends_did', $this_batch)
      ->fields('ars', array('from_did'))
      ->execute()
      ->fetchCol();
    if (count($bad_overrides)) {
      db_update('api_reference_storage')
        ->fields(array('extends_did' => 0))
        ->condition('from_did', $bad_overrides)
        ->execute();
      $to_reparse = array_merge($to_reparse, $bad_overrides);
    }
  }

  // We may have also now found some documentation items whose files need to
  // be reparsed. But there is no sense trying to reparse items that we just
  // deleted.
  $to_reparse = array_unique(array_diff($to_reparse, $dids));
  api_mark_items_for_reparse($to_reparse);

  // The slow part is deleting the nodes. If there are more than 100 to delete
  // during this page request, do them using batch/queue jobs.
  // Also, if this particular call has only a few nodes, go ahead and delete,
  // to avoid having a ton of small jobs in the queue from a branch update.
  $total_count += $count;
  if ($total_count < 100 || $count < 5) {
    node_delete_multiple($dids);
    return TRUE;
  }

  // If we get here, we need to use batch/queue to delete the nodes.
  if ($interactive) {
    batch_set(array(
      'operations' => array(
        array('_api_delete_nodes_batch', array($dids)),
      ),
      'file' => drupal_get_path('module', 'api') . '/api.db.inc',
    ));
  }
  else {
    $queue = DrupalQueue::get('api_node_delete');
    $done = 0;
    while ($done < $count) {
      $this_batch = array_slice($dids, $done, 100);
      $done += 100;
      $queue->createItem(array('nids' => $this_batch));
    }
  }

  return FALSE;
}

/**
 * Deletes API nodes using the Batch API.
 *
 * @param int[] $nids
 *   IDs of the nodes to delete.
 * @param array $context
 *   Batch context.
 */
function _api_delete_nodes_batch(array $nids, array &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($nids);
    $context['sandbox']['nids'] = $nids;
  }

  $this_batch = array_slice($context['sandbox']['nids'], $context['sandbox']['progress'], 100);
  $context['sandbox']['progress'] += count($this_batch);
  node_delete_multiple($this_batch);
  $context['finished'] = min(1, $context['sandbox']['progress'] / $context['sandbox']['max']);
  if (!count($this_batch)) {
    // Just make sure we don't run an endless loop here.
    $context['finished'] = 1;
  }
}

/**
 * Flags a file, branch, or all branches, to be reparsed on the next cron run.
 *
 * @param string|id|null $branch_or_file
 *   (optional) Identifier or ID number of the branch to reparse, or name of a
 *   single file to reparse. If omitted all branches will be reparsed. File
 *   names must include the path relative to the common path to the directories
 *   indexed by this branch.
 * @param bool $is_branch_name
 *   If passing in a branch name, set to TRUE so that if it happens to be an
 *   integer, it won't be considered to be a branch ID number by mistake.
 *   Otherwise, just leave this as the default FALSE.
 *
 * @return int
 *   Number of files marked for reparsing.
 */
function api_mark_for_reparse($branch_or_file = NULL, $is_branch_name = FALSE) {
  $time_in_past = 52;

  // See if it's a request to reparse everything.
  if (empty($branch_or_file)) {
    return db_update('api_file')
      ->fields(array(
        'modified' => $time_in_past,
      ))
      ->execute();
  }

  // See if it's a request for a particular branch.
  if (!$is_branch_name && is_numeric($branch_or_file) && ($branch_or_file == intval($branch_or_file))) {
    $branch_id = $branch_or_file;
  }
  else {
    $branch_id = db_select('api_branch', 'b')
      ->fields('b', array('branch_id'))
      ->condition('branch_name', $branch_or_file)
      ->execute()
      ->fetchField();
  }

  if (!empty($branch_id)) {
    // Reparse a branch.
    // This is a query using a WHERE EXISTS, and there is no apparent way
    // to write it using the database API, so use ANSI syntax.
    // @codingStandardsIgnoreLine
    $out = db_query("UPDATE {api_file} SET modified = :mod_time WHERE EXISTS ( SELECT * FROM {api_documentation} d WHERE d.branch_id = :branch_id AND d.object_type = 'file' AND d.did = {api_file}.did )", array(':mod_time' => $time_in_past, ':branch_id' => $branch_id));
    return $out->rowCount();
  }

  // If we get here, it's a request to reparse just one file. So find the file
  // ID, and then request its reparse.
  $did = db_select('api_documentation', 'd')
    ->fields('d', array('did'))
    ->condition('file_name', $branch_or_file)
    ->condition('object_type', 'file')
    ->execute()
    ->fetchField();

  if ($did) {
    $out = db_update('api_file')
      ->fields(array(
        'modified' => $time_in_past,
      ))
      ->condition('did', $did)
      ->execute();

    return $out;
  }

  return 0;
}

/**
 * Marks the files containing a set of documentation IDs for reparse.
 *
 * @param int[] $dids
 *   Documentation IDs whose files need reparsing.
 */
function api_mark_items_for_reparse(array $dids) {
  $count = count($dids);
  if ($count < 1) {
    return;
  }

  // Find all the affected file IDs.
  // Use batches of 200 due to placeholders.
  $done = 0;
  $file_ids = array();
  while ($done < $count) {
    $this_batch = array_slice($dids, $done, 200);
    $done += 200;

    $query = db_select('api_documentation', 'ad');
    $query->innerJoin('api_documentation', 'adf', 'ad.file_name = adf.file_name');
    $files = $query
      ->condition('adf.object_type', 'file')
      ->condition('ad.did', $this_batch)
      ->fields('adf', array('did'))
      ->execute()
      ->fetchCol();
    $file_ids = array_merge($file_ids, $files);
  }
  $file_ids = array_unique($file_ids);

  // Now mark these files for reparse, again in batches.
  $count = count($file_ids);
  if ($count < 1) {
    // We didn't find any files after all.
    return;
  }
  $done = 0;
  while ($done < $count) {
    $this_batch = array_slice($file_ids, $done, 200);
    $done += 200;
    db_update('api_file')
      ->fields(array('modified' => 52))
      ->condition('did', $this_batch)
      ->execute();
  }
}

/**
 * Resets the parse queue.
 *
 * Empties all parse jobs from the queue, and sets all files to "unqueued"
 * status.
 */
function api_reset_parse_queue() {
  $queue = DrupalQueue::get('api_parse');
  $queue->deleteQueue();

  db_update('api_file')
    ->fields(array(
      'queued' => 0,
    ))
    ->execute();
}
