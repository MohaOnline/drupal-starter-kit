<?php

/**
 * @file
 * Provision/Drush hooks for git commands.
 */

/**
 * Implementation of hook_drush_command().
 */
function provision_git_drush_command() {
  $items['provision-git-fetch'] = array(
    'description' => 'Executes "git fetch".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
    'aliases' => array('fetch'),
  );
  $items['provision-git-push'] = array(
    'description' => 'Executes "git push".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('push'),
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-commit'] = array(
    'description' => 'Commits all added and changed files to the repository.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('commit'),
    'options' => array(
      'message' => 'Optional: The commit message to use. Defaults to a generated message.',
      'push' => 'Push the code after committing. Defaults to FALSE',
      'files' => 'Optional: A list of files to commit.',
      'name' => 'Optional: The name of the user to commit as.',
      'mail' => 'Optional: The email of the user to commit as.',
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-reset'] = array(
    'description' => 'Executes "git reset --hard".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('reset'),
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-add'] = array(
    'description' => 'Adds files and directories to be committed.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('add'),
    'arguments' => array(
      'path' => 'Path to the file or folders you would like to add.',
    ),
    'required-arguments' => TRUE,
    'options' => array(
      'repo_path' => 'Optional: force a repository path. Defaults to the site or platform dir.',
      'force_repo_path_as_toplevel' => 'Stop Git from looking in parent directories. Defaults to FALSE.',
    ),
  );
  $items['provision-git-clone'] = array(
    'description' => 'Git clone a repo in a specified location.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'git_url' => dt('The git repository to clone.'),
      'path' => dt('The location where the git repository will be cloned.'),
      'git_ref' => dt('Parameter for the git checkout command'),
    ),
  );

  // Other commands are defined in sub modules.

  return $items;
}

/**
 * Helper for verifying the chosen path is a git repo.
 *
 * @param bool $set_error
 *   Use drush_set_error when it's not a repo, default to TRUE.
 *   Set to FALSE to silently check if a directory is a Git repo.
 *
 * @return bool
 *   TRUE if it's a functional Git repository, FALSE otherwise.
 */
function provision_git_is_repo($repo_path = NULL, $set_error = TRUE) {
  // If no path specified, use the site or platform path of the alias.
  if (!$repo_path) {
    $repo_path = drush_get_option('repo_path');
  }
  if (!$repo_path) {
    if (d()->type === 'site') {
      $repo_path = d()->site_path ? d()->site_path : d()->root;
    } elseif (d()->type == 'platform') {
      $repo_path = d()->platform->root ? d()->platform->root : d()->root;
    }
  }

  // Do not allow /var/aegir to be repo_path.
  // See https://www.drupal.org/node/2893588
  if ($repo_path == '/var/aegir') {
    $repo_path = NULL;
    drush_set_option('repo_path', $repo_path);
    return drush_set_error('PROVISION_GIT_ERROR', dt('Warning: The "repo_path" for this !type is set to "/var/aegir". This is not allowed. Please manually edit the drush alias file in /var/aegir/.drush/!file.alias.drushrc.php.  Option repo_path has been unset.', array(
      '!file' => ltrim(d()->name, '@'),
      '!type' => d()->type,
    )));
  }

  // If repo_path does not exist, or git status returns False, this is not a git repo
  if (!file_exists($repo_path . '/.git/config') || !drush_shell_cd_and_exec($repo_path, 'git status')) {
    if ($set_error) {
      drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Git repository not found in: !path", array('!path' => $repo_path)));
    }
    return FALSE;
  } else {
    // Find the toplevel directory for this git repository
    if (drush_shell_cd_and_exec($repo_path, 'git rev-parse --show-toplevel')) {
      $top_repo_path = implode("\n", drush_shell_exec_output());
    }
    if (drush_get_option('force_repo_path_as_toplevel', FALSE) && realpath($repo_path) != $top_repo_path) {
      if ($set_error) {
        drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Path !path is not the toplevel dir of this Git repository.", array('!path' => $repo_path)));
      }
      return FALSE;
    }
    drush_log(dt("Git repository found in !path", array('!path' => $top_repo_path)), 'ok');
    drush_set_option('repo_path', $top_repo_path);
    return TRUE;
  }
}

/**
 * Pre provision-git-push hook
 */
function drush_provision_pre_provision_git_push() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-commit hook
 */
function drush_provision_pre_provision_git_commit() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-reset hook
 */
function drush_provision_pre_provision_git_reset() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-add hook
 */
function drush_provision_pre_provision_git_add() {
  provision_git_is_repo();
}

/**
 * Pre provision-git-clone hook
 */
function drush_provision_pre_provision_git_clone() {
  // Nothing to do here.
}

/**
 * Implements the provision-git-fetch command.
 */
function drush_provision_git_fetch() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  if (drush_shell_cd_and_exec($repo_path, 'git fetch')) {
    drush_log(dt('Git repository !path fetched.', array('!path' => $repo_path)), 'ok');
    $out = drush_shell_exec_output();
    if ($out) {
      drush_log(implode("\n", $out), 'ok');
    }
  }
  else {
    return drush_set_error('DRUSH_PROVISION_GIT_FETCH_FAILED', dt("Git fetch failed in !path.\nThe specific errors are below:\n!errors", array('!path' => $repo_path, '!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements the provision-git-push command.
 */
function drush_provision_git_push() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  // Execute git push
  if (drush_shell_cd_and_exec($repo_path, 'git push')) {
    drush_log(dt('Git repository pushed.'), 'ok');
    drush_log(implode("\n", drush_shell_exec_output()), 'ok');
  }
  else {
    return drush_set_error('DRUSH_PROVISION_GIT_PULL_FAILED', dt("Git push failed in !path.\nThe specific errors are below:\n!errors", array('!path' => $repo_path, '!errors' => implode("\n", drush_shell_exec_output()))));
  }
}

/**
 * Implements the provision-git-commit command.
 */
function drush_provision_git_commit() {

  if (d()->type == 'site') {
    $git_url = d()->platform->repo_url;
    $git_path = d()->platform->repo_path;

  } elseif (d()->type == 'platform') {
    $git_url = d()->repo_url;
    $git_path = d()->repo_path;
  }

  $files = json_decode(drush_get_option('files', '[]'));
  $push = drush_get_option('push', FALSE);
  $name = drush_get_option('name', FALSE);
  $mail = drush_get_option('mail', FALSE);
  $hostmaster_uri = d('hostmaster')->uri;

  $repo_path = drush_get_option('repo_path');

  // If files option was detected
  if ($files) {
    $files_list = '';

    foreach ($files as $file) {
      $file = $git_path . '/' . $file;
      if (file_exists($file)) {
        $files_list .= ' ' . $file;
      }
    }

    // If files lists is not empty, add "git add" command to queue.
    if (!empty($files_list)) {
      $cmd = "git add {$files_list}";
    }
  }
  // If no files are specified, add everything,
  else {
    $cmd = 'git add -A';
  }

  if ($cmd) {
    provision_process($cmd, $git_path, dt('Adding files'));
  }

  provision_process('git status', $git_path, dt('Git Status'));

  $target = d()->name;

  //Generate commit message
  $message = array();
  $message[] = "Provision Git Commit";
  $length = strlen(end($message));

  if ($hostmaster_uri) {
    $message[] = "Commited from site $target using $hostmaster_uri";
    $length = strlen(end($message));
  }
  if (drush_get_option('message')) {
    $message[] = str_repeat('=', $length);
    $message[] = drush_get_option('message');
  }

  // Make commit using environment variables for NAME and EMAIL, if specified.
  $env = array();
  if (!empty($name) || !empty($mail)) {
    $message[] = str_repeat('=', $length);
    $message[] = "Committed by user $name <$mail>";
  }
  if (!empty($name)) {
    $env['GIT_AUTHOR_NAME'] = $name;
    $env['GIT_COMMITTER_NAME'] = $name;
  }
  if (!empty($mail)) {
    $env['GIT_AUTHOR_MAIL'] = $mail;
    $env['GIT_COMMITTER_MAIL'] = $mail;
  }

  // Implode message array and prepare for command insertion.
  $message = escapeshellarg(implode("\n", $message));

  // Show git status before commit
  provision_process('git status', $git_path, dt('Git Status'));

  // Unfortunately, git exits with
  // a status of 1 if there are no files to commit. This causes
  // drush_shell_cd_and_exec() to return FALSE and we think the
  // commit failed. So we have to resort to some hackery. We scan
  // the output and count how many files don't starts with '#'.
  // if the count is 1, then we have to assume there was nothing
  // to commit.

  // Run commit process with environment variables.
  $output = provision_process("git commit -m {$message}", $git_path, 'Committing changes', $env, TRUE, NULL, FALSE);

  $process = drush_get_context('provision_process_result');
  if (!$process->isSuccessful()) {
    $comments = 0;
    $lines = 0;
    $lines_array = explode("\n", $output);
    foreach ($lines_array as $line) {
      if (strpos($line, '#') === 0) {
        $comments++;
      }
      else {
        $lines++;
      }
    }
    if ($comments > 0 && $lines == 1) {
      drush_log(dt('Nothing to commit.'), 'warning');
    }
    elseif (strpos($output, 'nothing to commit, working directory clean') === FALSE) {
      return drush_set_error('DRUSH_PROVISION_GIT_COMMIT_FAILED', $output);
    }
  }

  // Push the result, if desired.
  if ($push) {

    // Do a little pre-check to ensure we don't hang at http password.
    $git_url = parse_url($git_url);
    if (($git_url['scheme'] == 'http' || $git_url['scheme'] == 'https') && empty($git_url['pass'])) {
      drush_log('http git url detected, with no password. Unable to push to this repository.', 'warning');
      return;
    }
    else {
      provision_process("git push", $git_path, 'Pushing code to remote repository...');
      provision_process('git status', $git_path, dt('Git Status'));
    }
  }
}

/**
 * Implements the provision-git-reset command.
 */
function drush_provision_git_reset() {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  // Call git reset --hard which resets to the last pulled commit
  if (drush_shell_cd_and_exec($repo_path, 'git reset --hard')) {
    drush_log(dt('Git repository reset.'), 'ok');
    if (file_exists($repo_path . '/.gitmodules')) {
      if (drush_shell_cd_and_exec($repo_path, 'git submodule foreach \'git reset --hard\'')) {
        drush_log(dt('Git submodules reset.'), 'ok');
      }
    }
  }
}

/**
 * Implements the provision-git-clone command.
 */
function drush_provision_git_clone($git_url = '', $path = '', $git_ref = '') {

  // Get URL and path from d()
  // @TODO: Move to a validate hook.
  if (!$git_url) {
    $git_url = d()->repo_url;
  }
  if (!$path) {
    switch (d()->type) {
      case 'site':
        $path = d()->site_path;
        break;
      case 'platform':
      default:
        $path = d()->repo_path;
        break;
    }
  }
  if (!$git_ref) {
    $git_ref = d()->git_ref;
  }

  drush_log(dt('Cloning !url to !path', array('!url' => $git_url, '!path' => $path, 'ok')));

  if (is_dir($path)) {
    return drush_set_error('DRUSH_PROVISION_GIT_CLONE_FAILED', dt('Directory already exists.'));
  }

  $command = 'git clone --recursive ';
  $command .= escapeshellarg(trim($git_url));
  $command .= ' ' . escapeshellarg(trim($path));

  if ($git_ref) {
    $command .= ' --branch ' . escapeshellarg(trim($git_ref));
  }

  // Try to perform the git clone
  provision_process($command);
}

/**
 * Implements the provision-git-add command.
 */
function drush_provision_git_add($path = '') {
  $repo_path = drush_get_option('repo_path');
  $target = d()->name;

  // Call git add.
  // Seems to pass even if the git add failed
  if (drush_shell_cd_and_exec($repo_path, "git add $path")) {
    drush_log(dt('Git add: ' . $path), 'ok');
  }
}


/**
 * Implements drush_hook_provision_verify_validate().
 *
 * This needs to be called in the validate step so that it runs before the
 * Makefile check and the check for a Drupal installation. Those operations,
 * very inconveniently, run in the same function.
 */
function drush_provision_git_provision_verify_validate() {
  if (in_array(d()->type, array('site', 'platform')) && d()->deploy_from_git) {

    switch (d()->type) {
      case 'site':
        $context_type = 'Site';
        $repo_path = d()->site_path;
        $path_exists = provision_file()->exists(d()->site_path)->status();
        // If there's an issue with the site git repo, we end up doing a
        // checkout on the platform, if we aren't careful.
        drush_set_option('force_repo_path_as_toplevel', TRUE);
        break;

      case 'platform':
      default:
        $context_type = 'Platform';
        $repo_path = d()->root;
        $path_exists = provision_file()->exists(d()->root)->status();
        break;
    }
    // If the platform path doesn't exist, we will clone it.
    // Sites aren't verified on install, so we need to clone the repo in
    // drush_provision_git_provision_install_validate() instead.
    if (!$path_exists) {
      drush_log(dt("$context_type path does not exist. Invoking provision-git-clone."), 'ok');
      provision_backend_invoke(d()->name, 'provision-git-clone');
    }
  }
  else {
    // Check to see if we can puick up on existing Git info.
    if (provision_git_is_repo(NULL, FALSE)) {
      _provision_git_update_git_meta_data();
    }
  }
}

/**
 * Implements drush_hook_provision_install_validate().
 *
 * The site directory is created in an implementation of
 * drush_hook_pre_COMMAND(). So to clone the site repo first, we need to do it
 * in an earlier hook.
 */
function drush_provision_git_provision_install_validate() {
  if (d()->type === 'site' && d()->deploy_from_git) {
    drush_set_option('force_repo_path_as_toplevel', TRUE);
    drush_log(dt("Cloning site from: " . d()->repo_url), 'ok');
    provision_backend_invoke(d()->name, 'provision-git-clone');
  }
}

/**
 * Implements drush_hook_post_COMMAND().
 */
function drush_provision_git_post_provision_verify() {
  _provision_git_update_git_meta_data();
}

/**
 * Implements drush_hook_post_COMMAND() for provision-delete command.
 *
 * If repo path is different from root, delete it.
 */
function drush_provision_git_post_provision_delete() {
  // If platform has a repo path that is not the root, it must be deleted as well.
  if (d()->type =='platform' && !empty(d()->repo_path) && d()->repo_path != d()->root) {
    drush_log(dt('Deleting repo path at %path', array(
      '%path' => d()->repo_path
    )), 'ok');
    _provision_recursive_delete(d()->repo_path);
  }
}

/**
 * Update the drush context with the git_url and current git_ref.
 */
function _provision_git_update_git_meta_data() {
  if (in_array(d()->type, array('site', 'platform'))) {

    switch (d()->type) {
      case 'site':
        $repo_path = d()->site_path;
        break;

      case 'platform':
      default:
        $repo_path = d()->repo_path;
        break;
    }

    // If the repo path doesn't even contain a git config, do not update metadata.
    if (!file_exists($repo_path . '/.git/config')) {
      drush_set_option('repo_url', '');
      drush_set_option('git_ref', '');
      return;
    }

    if (drush_shell_cd_and_exec($repo_path, 'git config --local --get remote.origin.url')) {
      $repo_url = implode("\n", drush_shell_exec_output());
      drush_set_option('repo_url', $repo_url);
    }
    if (drush_shell_cd_and_exec($repo_path, 'git describe --tags --exact-match || git symbolic-ref -q HEAD')) {
      $temp_git_ref = str_replace('refs/heads/', '',drush_shell_exec_output());
      $git_ref = (!empty($temp_git_ref) && is_string($temp_git_ref)) ? trim($temp_git_ref) : $temp_git_ref;
      drush_set_option('git_ref', $git_ref);
    }
  }
}


/**
 * Implements hook_provision_drupal_create_directories_alter().
 */
function provision_git_provision_drupal_create_directories_alter(&$mkdir, $url) {
  if (d()->type === 'site' && d()->deploy_from_git) {
    // Since we're cloning these directories, don't chmod/chgrp them.
    // We still need sites/$url as it gets 0555 permission otherwise [#2494113].
    unset($mkdir["sites/$url/modules"]);
    unset($mkdir["sites/$url/themes"]);
    unset($mkdir["sites/$url/libraries"]);
  }
}


/**
 * Register our directory as a place to find provision classes.
 */
function provision_git_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_drush_init().
 */
function provision_git_drush_init() {
  provision_git_register_autoload();
}

/**
 *  Implements hook_provision_services().
 */
function provision_git_provision_services() {
  provision_git_register_autoload();
  return array('git' => NULL);
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * Integrates with Features Update command
 */
function drush_provision_git_post_provision_features_update_all() {

  // provision-git-commit will handle the push.
  if (drush_get_option('commit')) {
    drush_provision_git_commit();
  }

  provision_process('git status', d()->platform->repo_path);
}
