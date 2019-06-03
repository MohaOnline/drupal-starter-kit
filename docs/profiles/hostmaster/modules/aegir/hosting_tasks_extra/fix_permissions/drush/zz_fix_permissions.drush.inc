<?php
/**
 * @file Drush hooks for the fix_permissions extension.
 */

/**
 * Implements drush_HOOK_pre_COMMAND().
 */
function drush_zz_fix_permissions_post_provision_verify() {
  if (d()->type === 'platform') {
    $success = drush_shell_exec("sudo /usr/local/bin/fix-drupal-platform-permissions.sh --root=%s", d()->root);
  }
  elseif (d()->type === 'site') {
    $success = drush_shell_exec("sudo /usr/local/bin/fix-drupal-site-permissions.sh --site-path=%s", d()->site_path);
  }
  else {
    return TRUE;
  }

  $result = drush_shell_exec_output();
  foreach ($result as $index => $line) {
    if (!$success) { // Log the entire error as a warning.
      $line_status = 'warning';
    }
    else { // Only log the last line as success.
      $line_status = $index+1 == count($result) ? 'success' : 'notice';
    }
    drush_log($line, $line_status);
  }
}

/**
 * Implements hook_provision_drupal_create_directories_alter().
 */
function zz_fix_permissions_provision_drupal_create_directories_alter(&$chmod, $url) {
  // We only handle chmod's for 'verify' tasks, atm.
  $cmd = drush_get_command();
  if ($cmd['command'] === 'provision-verify') {
    foreach ($chmod as $path => $perm) {
      // Disable Aegir core's chmod, since we handle it now.
      $chmod[$path] = FALSE;
    }
  }
}

