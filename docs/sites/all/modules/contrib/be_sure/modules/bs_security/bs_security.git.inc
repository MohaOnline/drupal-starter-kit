<?php

/**
 * @file
 * Git part for security.
 */

/**
 * Check if git is used in current project.
 *
 * @return null|string
 */
function bs_security_git_command() {
  $git_command = &drupal_static(__FUNCTION__);

  if (!is_null($git_command)) {
    return $git_command;
  }

  exec('command -v git', $output, $retval);
  if ($retval != 0) {
    return $git_command = FALSE;
  }

  $git_command = $output[0];

  exec("$git_command status", $output, $retval);
  if ($retval != 0) {
    return $git_command = FALSE;
  }

  return $git_command;
}

/**
 * Get status of local git repo.
 */
function _bs_security_git_status() {
  $git_command = bs_security_git_command();

  exec(escapeshellcmd($git_command) . ' status --porcelain', $output, $retval);

  return $output;
}

/**
 * Check status of changed files.
 */
function bs_security_git_status() {
  return !_bs_security_git_status();
}
