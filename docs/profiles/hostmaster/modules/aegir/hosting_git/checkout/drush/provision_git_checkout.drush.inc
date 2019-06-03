<?php

/**
 * Implementation of hook_drush_command().
 */
function provision_git_checkout_drush_command() {
  $items['provision-git-checkout'] = array(
    'description' => 'Git checkout a branch or tag in a specified location.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'arguments' => array(
      'git_ref' => dt('Parameter for the git checkout command'),
    ),
    'options' => array(
      'branch' => "Optional: If the branch doesn't exist, create it. Equivalent to git checkout -b branchname",
      'push' => "Optional: Push the new branch to the remote origin.",
      'reset' => "Optional: Reset changes before checking out. Uncommitted changes will be lost."
    )
  );
  return $items;
}

/**
 * Pre provision-git-checkout hook
 */
function drush_provision_pre_provision_git_checkout() {
  provision_git_is_repo();
}

/**
 * Implements the provision-git_checkout command
 */
function drush_provision_git_checkout($git_ref) {

  // Get URL and path from d()
  // @TODO: Move to a validate hook.
  if (!$git_ref) {
    $git_ref = d()->git_ref;
  }
  if (d()->type == 'site') {
    $path = d()->site_path;
  }
  else {
    $path = d()->root;
  }

  // Make sure there is a git ref
  if (empty($git_ref)) {
    return drush_set_error('DRUSH_PROVISION_GIT_CHECKOUT_FAILED', dt("Git checkout failed: No git_ref specified."));
  }

  $target = d()->name;

  // Only git reset --hard if option says so.
  if (drush_get_option('reset') == TRUE) {
    provision_process("git reset --hard", $path);
  }
  provision_process("git fetch", $path);

  drush_log(dt('Checking out git reference !git_ref', array('!git_ref' => $git_ref), 'ok'));

  $git_ref_arg = trim($git_ref);
  if (drush_get_option('branch')) {
    $command = "git checkout -b $git_ref_arg";
  }
  else {
    $command = "git checkout $git_ref_arg";
  }

  provision_process($command, $path);
  _provision_git_update_git_meta_data();

  if (drush_get_option('push')) {
    provision_process("git push --set-upstream origin $git_ref_arg", $path);
  }
}

/**
 * Implements hook_post_hosting_TASK_TYPE_task().
 *
 * After checkout, run a full hostmaster verify task to ensure git metadata is saved.
 */
function hosting_git_post_hosting_git_checkout_task($task, $data) {
  _hosting_git_verify_site_platform($task);
}
