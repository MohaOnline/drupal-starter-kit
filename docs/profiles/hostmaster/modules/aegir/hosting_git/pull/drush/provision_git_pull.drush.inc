<?php

/**
 * Implementation of hook_drush_command().
 */
function provision_git_pull_drush_command() {
  // @TODO: Move to provision_git_drush_command().
  $items['provision-git-pull'] = array(
    'description' => 'Executes "git pull".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'reset' => 'Resets any working tree changes that would block a rebase.  USE WITH CAUTION',
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
    'aliases' => array('pull'),
  );
  return $items;
}



/**
 * Pre provision-git-pull hook
 */
function drush_provision_pre_provision_git_pull() {
  provision_git_is_repo();
}

/**
 * Implements the provision-git-pull command.
 */
function drush_provision_git_pull() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  // Only git reset --hard if option says so.
  if (drush_get_option('reset') == TRUE) {
    provision_backend_invoke($target, 'provision-git-reset');
  }

  // Execute git pull --rebase
  // @TODO: make rebase optional as well??  Haven't thought this through yet.
  if (drush_shell_cd_and_exec($repo_path, 'git pull --rebase')) {
    drush_log(dt('Git repository !path pulled.', array('!path' => $repo_path)), 'ok');
    drush_log(implode("\n", drush_shell_exec_output()), 'ok');

    // Submodule in git? lets update those.
    if (file_exists($repo_path . '/.gitmodules')) {
      drush_shell_cd_and_exec($repo_path, 'git submodule init');
      drush_log(implode("\n", drush_shell_exec_output()), 'ok');
      drush_shell_cd_and_exec($repo_path, 'git submodule update');
      drush_log(implode("\n", drush_shell_exec_output()), 'ok');
    }
  }
  else {
    return drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Git pull failed in !path.\nThe specific errors are below:\n!errors", array('!path' => $repo_path, '!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements hook_post_hosting_TASK_TYPE_task().
 *
 * After pull, run a full hostmaster verify task to ensure git metadata is saved.
 */
function hosting_git_post_hosting_git_pull_task($task, $data) {
  _hosting_git_verify_site_platform($task);
}
