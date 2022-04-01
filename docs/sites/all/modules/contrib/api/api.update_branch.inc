<?php

/**
 * @file
 * Functions for updating branches.
 *
 * Updating a branch means:
 * - Files branch: Checking to see if any files have changed, and queueing them
 *   for parsing, reparsing, or deleting.
 * - PHP functions branch: Getting data from php.net (or similar source) that
 *   lists the built-in PHP functions, and updating the database.
 * - API branch: Getting the data from api.drupal.org (or similar source) for
 *   Core functions that are linked to but not displayed on this API site, and
 *   updating the database.
 *
 * Functions whose names start with _ in this file are only intended to be
 * called by other functions in this file.
 */

module_load_include('inc', 'api', 'api.utilities');

/**
 * Checks all branches to see if it's time to update, and queues if so.
 *
 * @see api_update_branch_php()
 * @see api_update_branch_api()
 * @see api_update_branch_files()
 */
function api_update_all_branches() {
  $queue = DrupalQueue::get('api_branch_update');

  // Check the files branches to see what needs updates.
  foreach (api_get_branches(TRUE) as $branch) {
    if (_api_should_queue($branch)) {
      // Add to queue and mark as queued.
      $queue->createItem(array('type' => 'files', 'branch_id' => $branch->branch_id));
      $branch->queued = time();
      drupal_write_record('api_branch', $branch, 'branch_id');
    }
    else {
      watchdog('api', 'Skipped update of %project %branch',
        array(
          '%branch' => $branch->branch_name,
          '%project' => $branch->project,
        ));
    }
  }

  // Check the reference branches to see what needs updates.
  foreach (api_get_php_branches(TRUE) as $branch) {
    if (_api_should_queue($branch)) {
      // Add to queue and mark as queued.
      $queue->createItem(array('type' => $branch->reference_type, 'branch_id' => $branch->branch_id));
      $branch->queued = time();
      drupal_write_record('api_php_branch', $branch, 'branch_id');
    }
    else {
      watchdog('api', 'Skipped update of reference branch %branch',
        array('%branch' => $branch->title));
    }
  }
}

/**
 * Updates a PHP branch.
 *
 * Queries the branch URL to get an updated list of functions, and saves each
 * function in the database.
 *
 * @param object $branch
 *   Object representing a PHP branch to update.
 *
 * @return bool
 *   TRUE if the branch was updated, and FALSE if there was an error.
 *
 * @see api_update_all_branches()
 */
function api_update_branch_php($branch) {
  $response = drupal_http_request($branch->summary, array('timeout' => 60));
  if ($response->code === '200') {
    $docblocks = array();
    // Note that drupal_json_decode() passes TRUE to json_decode(), so that all
    // returned data is associative arrays and not objects.
    $data = drupal_json_decode($response->data);
    foreach ($data as $name => $item) {
      $docblocks[] = array(
        'branch_id' => $branch->branch_id,
        'object_type' => 'function',
        'object_name' => $name,
        'documentation' => $item['prototype'] . "\n  " . $item['purpose'],
        'member_name' => '',
      );
    }

    if (count($docblocks)) {
      _api_save_php_documentation($docblocks);
    }

    // Mark the branch as having been updated.
    $branch->last_updated = time();
    $branch->queued = 0;
    drupal_write_record('api_php_branch', $branch, array('branch_id'));
    watchdog('api', 'Updated PHP branch %branch_name.',
      array('%branch_name' => $branch->title));
    return TRUE;
  }

  // We didn't get a good response.
  $branch->queued = 0;
  drupal_write_record('api_php_branch', $branch, array('branch_id'));
  watchdog('api', 'Bad HTTP response from PHP branch %branch_name.',
    array('%branch_name' => $branch->title), WATCHDOG_WARNING);
  return FALSE;
}

/**
 * Updates an API branch.
 *
 * Queries the branch URL to get an updated item dump, and saves each item in
 * the database.
 *
 * @param object $branch
 *   Object representing an API branch to update.
 *
 * @return bool
 *   TRUE if the branch was updated, and FALSE if there was an error.
 *
 * @see api_update_all_branches()
 */
function api_update_branch_api($branch) {
  $limit = (int) (isset($branch->page_limit) ? $branch->page_limit : 0);
  $timeout = (int) (isset($branch->timeout) ? $branch->timeout : 0);
  if ($limit <= 0) {
    $limit = 2000;
  }
  if ($timeout <= 0) {
    $timeout = 30;
  }

  // Download JSON by page.
  $base_url = $branch->url . ((strpos($branch->url, '?') > 0) ? '&' : '?') . 'limit=' . $limit . '&page=';

  $docblocks = array();
  $done = array();
  $page = 0;
  $found = TRUE;

  // Continue in this loop as long as the last response was successful and
  // contained new data, but as a failsafe stop after 1000 pages.
  while ($found && $page < 1000) {
    $found = FALSE;
    $response = @drupal_http_request($base_url . $page, array('timeout' => $timeout));
    $page++;
    if (isset($response) && ($response->code === '200')) {
      $data = drupal_json_decode($response->data);
      foreach ($data as $item) {
        $item = (array) $item;
        // Only save new items.
        $key = $item['url'] . '..' . $item['object_type'] . '..' . $item['namespaced_name'];
        if (!isset($done[$key])) {
          $found = TRUE;
          $done[$key] = TRUE;
          $item['branch_id'] = $branch->branch_id;
          $docblocks[] = $item;
        }
      }
    }
  }

  if (count($docblocks)) {
    // We found at least one doc block, so save the data.
    _api_save_api_documentation($docblocks);

    // Mark the branch as having been updated.
    $branch->last_updated = time();
    $branch->queued = 0;
    drupal_write_record('api_php_branch', $branch, array('branch_id'));
    watchdog('api', 'Updated API branch %branch_name in %count page requests.',
      array(
        '%branch_name' => $branch->title,
        '%count' => $page,
      ));
    return TRUE;
  }

  // We didn't get a good response.
  $branch->queued = 0;
  drupal_write_record('api_php_branch', $branch, array('branch_id'));
  watchdog('api',
    'No documentation was downloaded from API branch %branch_name.',
    array('%branch_name' => $branch->title),
    WATCHDOG_WARNING);
  return FALSE;
}

/**
 * Checks to see if we need to queue a branch for updates or not.
 *
 * @param object $branch
 *   Object representing the branch.
 *
 * @return bool
 *   TRUE if we should queue this branch, and FALSE if not.
 */
function _api_should_queue($branch) {
  if (isset($branch->queued) && $branch->queued) {
    // It is already queued up.
    return FALSE;
  }

  $last_run = isset($branch->last_updated) ? $branch->last_updated : 0;
  $now = time();
  $add = 0;
  if (isset($branch->update_frequency)) {
    $add = $branch->update_frequency;
  }
  if ($last_run + $add > $now) {
    // It hasn't been long enough yet.
    return FALSE;
  }

  return TRUE;
}

/**
 * Returns the list of parsing functions for file extensions.
 *
 * @return array
 *   An associative array whose keys are file extensions, and whose values are
 *   the functions used to parse files with that extension. The function should
 *   parse the contents of the file, and save one or more documentation objects
 *   in the database. The function argument is a $docblock array, containing a
 *   sample documentation listing for the file as a whole. In this array, the
 *   following keys are filled in:
 *   - object_name, file_name: Both set to the file name.
 *   - title, basename: Both set to the file basename.
 *   - modified: Set to the time the file was last modified.
 *   - branch: The branch object.
 *   - object_type: Set to 'file'.
 *   - source: Set to the file contents.
 *   - is_drupal: TRUE if this is a Drupal file, as defined by the regular
 *     expression for deciding this that is part of the branch setup.
 */
function api_parse_functions() {
  $functions = array(
    'php' => 'api_parse_php_file',
    'module' => 'api_parse_php_file',
    'inc' => 'api_parse_php_file',
    'install' => 'api_parse_php_file',
    'engine' => 'api_parse_php_file',
    'theme' => 'api_parse_php_file',
    'profile' => 'api_parse_php_file',
    'test' => 'api_parse_php_file',

    'yml' => 'api_parse_yaml_file',

    'txt' => 'api_parse_text_file',
    'info' => 'api_parse_text_file',
    'css' => 'api_parse_text_file',
    'sh' => 'api_parse_text_file',
    'twig' => 'api_parse_twig_file',

    'htm' => 'api_parse_html_file',
    'html' => 'api_parse_html_file',
  );

  drupal_alter('api_parse_functions', $functions);
  return $functions;
}

/**
 * Updates a files branch.
 *
 * Checks the current directories included in the branch to make an updated
 * list of files. Removes documentation from files that no longer exist, adds
 * documentation from new files, and updates documentation for any files that
 * have changed.
 *
 * @param object $branch
 *   Object representing a files branch to update.
 *
 * @return bool
 *   TRUE to indicate that the branch was updated, FALSE if there was a
 *   problem.
 *
 * @see api_update_all_branches()
 */
function api_update_branch_files($branch) {
  $files_to_scan = _api_scan_directories($branch->directories, $branch->excluded_directories, $branch->exclude_files_regexp);
  if (!count($files_to_scan)) {
    watchdog('api', 'No files were found in %project %branch',
      array(
        '%branch' => $branch->branch_name,
        '%project' => $branch->project,
      ), WATCHDOG_ERROR);
    $branch->queued = 0;
    drupal_write_record('api_branch', $branch, array('branch_id'));
    return FALSE;
  }

  $parse_functions = api_parse_functions();

  // List all documented files for the branch.
  $files = array();
  $query = db_select('api_documentation', 'd')
    ->fields('d', array('object_name', 'did'));
  $f = $query->leftJoin('api_file', 'f', 'd.did = f.did');
  $query->fields($f, array('modified', 'queued'))
    ->condition('d.branch_id', $branch->branch_id)
    ->condition('d.object_type', 'file');
  $result = $query->execute();
  foreach ($result as $file) {
    $files[$file->object_name] = $file;
  }

  // Check all the files. If they need an update, add them to the queue.
  foreach ($files_to_scan as $path => $file_name) {
    preg_match('!\.([a-z]*)$!', $file_name, $matches);
    if (isset($matches[1]) && isset($parse_functions[$matches[1]])) {
      $new = FALSE;
      if (isset($files[$file_name])) {
        $parse = (filemtime($path) > $files[$file_name]->modified) && !$files[$file_name]->queued;
        $file = $files[$file_name];
        // All remaining files will be removed.
        unset($files[$file_name]);
      }
      else {
        // New file.
        $parse = TRUE;
        $new = TRUE;
      }
      if ($parse) {
        $queue = DrupalQueue::get('api_parse');
        $queue->createItem(array(
          'parser' => $parse_functions[$matches[1]],
          'path' => $path,
          'branch' => $branch,
          'file' => $file_name,
          'branch_name' => $branch->branch_name,
          // Set the timestamp to the "valid time" stamp of the branch, so
          // that in api_queue_parse_file(), we can make sure the branch has
          // not been updated since this file was queued.
          'timestamp' => $branch->queue_valid,
        ));

        // Make sure this doesn't get added to the parse queue again.
        if ($new) {
          // Add stub to the {api_documentation} and {api_file} table for this
          // file so it doesn't get added to the parse queue again.
          $nid = api_new_documentation_node();
          $doc = array(
            'object_name' => $file_name,
            'file_name' => $file_name,
            'title' => drupal_basename($file_name),
            'object_type' => 'file',
            'branch_id' => $branch->branch_id,
            'queued' => time(),
            'did' => $nid,
          );
          api_add_text_defaults($doc, 'api_documentation');
          drupal_write_record('api_documentation', $doc);
          drupal_write_record('api_file', $doc);
        }
        else {
          // Update the 'queued' field in the {api_file} table so this does not
          // get queued again.
          $file->queued = time();
          drupal_write_record('api_file', $file, 'did');
        }
      }
    }
  }

  // Remove outdated files.
  foreach (array_keys($files) as $file_name) {
    watchdog('api', 'Removing %file from %project %branch.',
      array(
        '%file' => $file_name,
        '%project' => $branch->project,
        '%branch' => $branch->branch_name,
      ));
    $dids = db_select('api_documentation', 'ad')
      ->fields('ad', array('did'))
      ->condition('file_name', $file_name)
      ->condition('branch_id', $branch->branch_id)
      ->execute()
      ->fetchCol();

    module_load_include('inc', 'api', 'api.db');
    api_delete_items($dids, FALSE);
  }

  // Mark the branch as having been updated.
  $branch->last_updated = time();
  $branch->queued = 0;
  drupal_write_record('api_branch', $branch, array('branch_id'));
  watchdog('api',
    'Updated %project %branch_name.',
    array(
      '%branch_name' => $branch->branch_name,
      '%project' => $branch->project,
    ));

  return TRUE;
}

/**
 * Finds all the files in the directories specified for a branch.
 *
 * @param string $directories
 *   List of directories to scan, as text (separated by newlines).
 * @param string $excluded_directories
 *   List of directories to exclude, as text (separated by newlines).
 * @param string $regexps
 *   List of regular expressions to exclude (files or directories), separated
 *   by newlines.
 *
 * @return array
 *   Associative array of files, where the keys are the full paths to the
 *   files and the values are the file names. File names exclude the part of
 *   the full path that is shared by all directories in the scan list. Only
 *   files that can be parsed are included (see api_parse_functions() for a
 *   list). Hidden directories (names starting with .) below top-level entries
 *   in $directories are excluded, as well as directories called CVS.
 */
function _api_scan_directories($directories, $excluded_directories, $regexps) {
  $directory_array = array_filter(explode("\n", $directories));
  $excluded_array = array_filter(explode("\n", $excluded_directories));
  $extensions = array_keys(api_parse_functions());
  $regexps = explode("\n", $regexps);
  $regexps = array_filter(array_map('trim', $regexps));

  // Figure out the shared part of the file name that is in all the directories.
  if (count($directory_array) > 1) {
    $directories_components = array();
    foreach ($directory_array as $directory) {
      $directory_components = array();
      $parts = explode(DIRECTORY_SEPARATOR, $directory);
      $starts_with_sep = (strpos($directory, DIRECTORY_SEPARATOR) === 0);
      foreach ($parts as $part) {
        if (strlen($part)) {
          $previous = reset($directory_components);
          if (strlen($previous) > 0 || $starts_with_sep) {
            $previous .= DIRECTORY_SEPARATOR;
          }
          array_unshift($directory_components, $previous . $part);
        }
      }
      $directories_components[] = $directory_components;
    }

    $common_ancestor_components = call_user_func_array('array_intersect', $directories_components);
    $common_ancestor = reset($common_ancestor_components);
  }
  else {
    $common_ancestor = $directories;
  }

  // Scan the directories. Instead of letting file_scan_directory() do its own
  // recursion, we recurse ourselves. The reason is that we can exclude things
  // as we go, saving time and memory scanning .git, CVS, and the excluded
  // directories.
  $source_files = array();
  $todo = $directory_array;
  while (count($todo) > 0) {
    $directory = array_shift($todo);
    $files = file_scan_directory($directory, '/.*/', array('recurse' => FALSE));
    foreach ($files as $path => $file) {
      // Exclude anything that matches one of our regular expressions.
      $ok = TRUE;
      foreach ($regexps as $regexp) {
        if (preg_match($regexp, $path)) {
          $ok = FALSE;
          break;
        }
      }
      if (!$ok) {
        continue;
      }

      if (is_dir($path)) {
        // See if we want to scan this path, and if so, add to To Do list.
        // Don't keep any directory that starts with ".", or anything in the
        // excluded list. Note that $file->filename includes $directory, so
        // strip that off before checking the name for ".".
        $dir_name = substr($file->filename, strlen($directory));
        if ((strpos($dir_name, '.') !== 0) &&
          !in_array($path, $excluded_array)) {
          $todo[] = $path;
        }
      }
      else {
        // This is a regular file. See if it has an extension we recognize,
        // and save it in our return list if so.
        $ext = pathinfo($path, PATHINFO_EXTENSION);
        if (in_array($ext, $extensions)) {
          $file_name = substr($path, strlen($common_ancestor) + 1);
          $source_files[$path] = $file_name;
        }
      }
    }
  }

  return $source_files;
}

/**
 * Saves PHP branch documentation information to the database.
 *
 * @param array $docblocks
 *   An array containing information about functions found in the branch.
 */
function _api_save_php_documentation(array $docblocks) {
  // Remove all the old items from this branch. We don't care about preserving
  // IDs, so there is no reason to use complicated logic and update existing
  // records.
  db_delete('api_php_documentation')
    ->condition('branch_id', $docblocks[0]['branch_id'])
    ->execute();

  $did = _api_new_php_doc_id();
  $count = 0;
  foreach ($docblocks as $docblock) {
    if (!$count) {
      // Build up queries and save the documentation in chunks, for speed.
      $insertQuery = db_insert('api_php_documentation')
        ->fields(array(
          'did',
          'branch_id',
          'object_name',
          'member_name',
          'object_type',
          'documentation',
        ));
    }
    $docblock['did'] = $did;
    $insertQuery->values($docblock);
    $did++;
    $count++;
    if ($count >= 500) {
      // Using a larger chunk size could risk running into database placeholder
      // limits.
      $insertQuery->execute();
      $count = 0;
    }
  }

  if ($count > 0) {
    $insertQuery->execute();
  }
}

/**
 * Saves API branch documentation information to the database.
 *
 * @param array $docblocks
 *   An array containing information about functions found in the branch.
 */
function _api_save_api_documentation(array $docblocks) {
  // Remove all the old items from this branch. We don't care about preserving
  // IDs, so there is no reason to use complicated logic and update existing
  // records.
  db_delete('api_external_documentation')
    ->condition('branch_id', $docblocks[0]['branch_id'])
    ->execute();

  $did = _api_new_external_doc_id();
  $count = 0;
  foreach ($docblocks as $docblock) {
    if (!$count) {
      // Build up queries and save the documentation in chunks, for speed.
      $insertQuery = db_insert('api_external_documentation')
        ->fields(array(
          'did',
          'branch_id',
          'object_name',
          'namespaced_name',
          'title',
          'member_name',
          'summary',
          'object_type',
          'url',
        ));
    }
    $docblock['did'] = $did;
    $insertQuery->values($docblock);
    $did++;
    $count++;
    if ($count >= 500) {
      // Using a larger chunk size could risk running into database placeholder
      // limits.
      $insertQuery->execute();
      $count = 0;
    }
  }

  if ($count > 0) {
    $insertQuery->execute();
  }
}

/**
 * Returns a new ID for a PHP doc item.
 */
function _api_new_php_doc_id() {
  $select = db_select('api_php_documentation', 'p');
  $select->addExpression('MAX(did)', 'a');
  $ids = $select->groupBy('branch_id')
    ->execute()
    ->fetchCol();

  if (count($ids)) {
    return max($ids) + 1;
  }

  return 1;
}

/**
 * Returns a new ID for an API reference branch doc item.
 */
function _api_new_external_doc_id() {
  $select = db_select('api_external_documentation', 'p');
  $select->addExpression('MAX(did)', 'a');
  $ids = $select->groupBy('branch_id')
    ->execute()
    ->fetchCol();

  if (count($ids)) {
    return max($ids) + 1;
  }

  return 1;
}
