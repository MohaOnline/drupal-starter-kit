<?php

/**
 * @file
 * Git webhook code
 */

/**
 * URL callback that is invoked by git to create a code pull task.
 */
function hosting_git_pull_callback($name, $hash) {

  // Load the node & list of allowed IPs.
  $node = hosting_context_load($name);
  $name_clean = check_plain($name);
  $allowed_ips = explode("\n", trim(variable_get('hosting_git_pull_webhook_ip_acl', HOSTING_GIT_WEBHOOK_DEFAULT_ALLOWED_IPS)));
  array_filter(array_map('trim', $allowed_ips));

  // Make sure we got the node.
  if (!$node) {
    $message = "No node found with the name '$name_clean'.";
  }
  // Make sure the security code is valid.
  elseif (_hosting_git_pull_webhook_hash_create($node) != $hash) {
    $message = "Security code $hash is not valid!";
    $status = HOSTING_GIT_WEBHOOK_STATUS_INVALID_CODE;
  }
  // Make sure the node has pull callback enabled.
  elseif ($node->git['pull_method'] != HOSTING_GIT_PULL_CALLBACK) {
    $message = "Node $name_clean is NOT configured to use Pull Code URL callback!";
  }
  // Make sure the client's IP address is on the list.
  elseif (!hosting_git_pull_ip_match(ip_address(), $allowed_ips)) {
    $message = ip_address() . " is not authorized to invoke a Pull Code request.";
    $status = HOSTING_GIT_WEBHOOK_STATUS_ACCESS_DENIED;
  }
  // All checks pass! Server is allowed to trigger tasks!
  else {
    // Check JSON payload here... If we see a GitHub payload with branches,
    $data = json_decode(file_get_contents('php://input'), true);

    if (is_array($data)) {

      // GitHub / GitLab.
      if (!empty($data['ref'])) {
        if ($data['ref'] == 'refs/heads/' . $node->git['git_ref']) {
          $status = HOSTING_GIT_WEBHOOK_STATUS_OK;
        } else {
          $status = HOSTING_GIT_WEBHOOK_STATUS_OTHER_REF;
        }
      }

      // BitBucket.
      if (!empty($data['push']['changes'][0]['new']['name'])) {
        if ($data['push']['changes'][0]['new']['name'] == $node->git['git_ref']) {
          $status = HOSTING_GIT_WEBHOOK_STATUS_OK;
        } else {
          $status = HOSTING_GIT_WEBHOOK_STATUS_OTHER_REF;
        }
      }
    }
    else {
      // No payload ... no way to filter on branch.
      $status = HOSTING_GIT_WEBHOOK_STATUS_OK;
    }


    if ($status == HOSTING_GIT_WEBHOOK_STATUS_OK) {
      $message = "Webhook Received! Invoked by " . ip_address();
      $args = array('reset_hard' => TRUE);

      // @TODO: Determine if environment is on a branch. Pull only works when on a branch.
      $task = hosting_add_task($node->nid, 'git-pull', $args);

      if ($task->nid) {
        $message .= " Task started: " . url("node/{$task->nid}", array('absolute' => TRUE));
      }
      else {
        $message .= " ERROR! Unable to save task!";
      }
    }
    elseif ($status == HOSTING_GIT_WEBHOOK_STATUS_OTHER_REF) {
      $message = "Webhook for OTHER branch Received! Ignoring";
    }
  }

  // Log It, only if there is a status.
  if (isset($status)) {
    db_update('hosting_git_pull')
      ->fields(array(
        'last_pull' => REQUEST_TIME,
        'last_pull_status' => $status,
        'last_pull_ip' => ip_address(),
      ))
      ->condition('nid', $node->nid)
      ->execute();

    if ($status != HOSTING_GIT_WEBHOOK_STATUS_OK && $status != HOSTING_GIT_WEBHOOK_STATUS_OTHER_REF) {
      drupal_add_http_header('Status', '403 Forbidden');
    }
  }

  // Output a message, no matter what.
  watchdog('hosting_git_pull', $message, array(), WATCHDOG_INFO);
  print $message;
  print "\n\n";

  // Save a variable to help when using the settings page.
  variable_set('hosting_git_pull_webhook_last_ip', ip_address());
}

/**
 * Check whether a given ip address matches a list of allowed ip addresses.
 *
 * A CIDR is allowed. Returns TRUE if $list is empty.
 *
 * @param string $ip
 *   The ip addy to test.
 * @param array $list
 *   The list to test against.
 */
function hosting_git_pull_ip_match($ip, $list) {
  if (empty(array_filter($list))) {
    return TRUE;
  }
  foreach ($list as $cidr) {
    if (trim($ip) === trim($cidr)) {
      return TRUE;
    }
    @list($net, $mask) = explode('/', trim($cidr));
    if (isset($mask)) {
      if ($mask > 32) {
        // IPv6 is not supported... yet.
        return FALSE;
      }
      $bitmask = ~((1 << (32 - $mask)) - 1);
      if ((ip2long($net) & $bitmask) == (ip2long($ip) & $bitmask)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
