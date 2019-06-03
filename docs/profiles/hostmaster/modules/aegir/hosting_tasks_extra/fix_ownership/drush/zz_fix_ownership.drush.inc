<?php
/**
 * @file Drush hooks for the fix_ownership extension.
 */

/**
 * Implements drush_HOOK_pre_COMMAND().
 */
function drush_zz_fix_ownership_post_provision_verify() {
  if (d()->type === 'platform') {
    $success = drush_shell_exec("sudo /usr/local/bin/fix-drupal-platform-ownership.sh --root=%s --script-user=%s --web-group=%s", d()->root, d()->server->script_user, d()->server->web_group);
  }
  elseif (d()->type === 'site') {
    $success = drush_shell_exec("sudo /usr/local/bin/fix-drupal-site-ownership.sh --site-path=%s --script-user=%s --web-group=%s", d()->site_path, d()->server->script_user, d()->server->web_group);
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
 * Implements hook_provision_drupal_chgrp_directories_alter().
 */
function zz_fix_permissions_provision_drupal_chgrp_directories_alter(&$chgrp, $url) {
  // We only handle chgrp's for 'verify' tasks, atm.
  $cmd = drush_get_command();
  if ($cmd['command'] === 'provision-verify') {
    foreach ($chgrp as $path => $group) {
      // Disable Aegir core's chgrp, since we handle it now.
      $chgrp[$path] = FALSE;
    }
  }
}

