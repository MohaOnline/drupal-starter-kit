<?php
/**
 * @file
 * The Provision_PlatformGit class.
 */

class Provision_PlatformGit extends Provision_ShellCommand {

  // The prefix used for properties in Aegir contexts.
  protected $context_prefix = 'git_';

  // List of properties to load from the Aegir context.
  protected $context_properties = ['repository_url', 'repository_path', 'reference'];

  // The Git reporitory URL from whence we'll clone.
  protected $repository_url = FALSE;

  // The local path to which we'll clone the repository.
  protected $repository_path = FALSE;

  // The Git reference that we'll clone or checkout.
  protected $reference = FALSE;

  // A list of references fetched from a Git repository.
  protected $references = [];

  public function validateProvisionVerify() {
    if ($this->pathExists($this->repository_path)) {
      return $this->notice(dt('Platform Git repository path already exists. Aborting.'));
    }
    else {
      return $this->deployPlatform();
    }
  }

  protected function deployPlatform() {
    $this->notice(dt('Deploying platform.'));
    return $this->gitClone() && $this->gitCheckout();
  }

  protected function gitClone() {
    $this->notice(dt('Cloning `:url` to `:path`', [
      ':url' => $this->repository_url,
      ':path' => $this->repository_path,
    ]));

    return $this->runCommand($this->buildGitCloneCommand());
  }

  protected function gitCheckout() {
    if ($this->referenceIsACommit()) {
      $this->notice(dt('Fetching full Git history and checking out commit `:ref`', [
        ':ref' => $this->reference,
      ]));

      return $this->runCommand($this->buildGitCheckoutCommand());
    }
    return TRUE;
  }

  protected function buildGitCloneCommand() {
    $command = 'git clone --recursive --depth 1 --no-progress --quiet';
    if ($this->referenceIsABranch() || $this->referenceIsATag()) {
      $command .= ' --branch ' . escapeshellarg(trim($this->reference));
    }
    $command .= ' ' . escapeshellarg(trim($this->repository_url));
    $command .= ' ' . escapeshellarg(trim($this->repository_path));
    return $command;
  }

  protected function buildGitCheckoutCommand() {
    $command = 'cd ' . escapeshellarg(trim($this->repository_path));
    $command .= ' && git fetch --unshallow && ';
    $command .= 'git checkout ' . escapeshellarg(trim($this->reference));
    return $command;
  }

  protected function referenceIsABranch() {
    return $this->reference && in_array($this->reference, $this->lsRemote('heads'));
  }

  protected function referenceIsATag() {
    return $this->reference && in_array($this->reference, $this->lsRemote('tags'));
  }

  protected function referenceIsACommit() {
    return $this->reference && !$this->referenceIsABranch() && !$this->referenceIsATag();
  }

  protected function lsRemote($type = FALSE) {
    if (empty($this->references)) {
      $this->notice(dt('Scanning remote git repository tags and branches.'));
      $debug = drush_get_context('DRUSH_DEBUG');
      drush_set_context('DRUSH_DEBUG', FALSE);
      drush_shell_exec('git ls-remote ' . escapeshellarg(trim($this->repository_url)));
      drush_set_context('DRUSH_DEBUG', $debug);
      $lines = drush_shell_exec_output();
      foreach ($lines as $line) {
        $ref = explode('/', $line);
        if (isset($ref[1]) && isset($ref[2])) {
          $this->references[$ref[1]][] = $ref[2];
        }
      }
    }
    return $type ? $this->references[$type] : $this->references;
  }

  public function postProvisionDelete() {
    if ($this->repository_path != d()->root) {
      $this->notice(dt('Deleting repo path at: ') . d()->repo_path);
      _provision_recursive_delete($this->repository_path);
    }
  }

}
