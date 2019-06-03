<?php
/**
 * @file
 * The Provision_ComposerGitCreateProject class.
 */

class Provision_ComposerGitCreateProject extends Provision_ShellCommand {

  // The prefix used for properties in Aegir contexts.
  protected $context_prefix = 'composer_git_';

  // List of properties to load from the Aegir context.
  protected $context_properties = ['path', 'project_url', 'version'];

  // The local path in which we'll deploy the project.
  protected $path = FALSE;

  // The Composer package to use to create the project.
  protected $package = FALSE;

  // The version of the Composer package to use.
  protected $version = FALSE;

  public function validateProvisionVerify() {
    if ($this->pathExists($this->path)) {
      return $this->notice(dt('Composer project path already exists. Aborting.'));
    }
    else {
      return $this->deployPlatform();
    }
  }

  protected function deployPlatform() {
    $this->notice(dt('Deploying platform.'));
    return $this->createProject();
  }

  protected function createProject() {
    $this->notice(dt('Creating `:version` version of project from `:project_url` at `:path`', [
      ':version' => $this->version,
      ':project_url' => $this->project_url,
      ':path' => $this->path,
    ]));

    $commands = [
      $this->buildCloneProjectCommand(),
      $this->buildCheckoutVersionCommand(),
      $this->buildCreateProjectCommand(),
    ];

    foreach ($commands as $command) {
      $result = $this->execCommand($command);
    }
    return $result;
  }

  protected function buildCloneProjectCommand() {
    $command = 'git clone ';
    $command .= escapeshellarg(trim($this->project_url));
    $command .= ' ';
    $command .= escapeshellarg(trim($this->path));
    return $command;
  }

  protected function buildCheckoutVersionCommand() {
    $command = 'cd ';
    $command .= escapeshellarg(trim($this->path));
    $command .= ' && git checkout ';
    $command .= escapeshellarg(trim($this->version));
    return $command;
  }

  protected function buildCreateProjectCommand() {
    $command = 'cd ';
    $command .= escapeshellarg(trim($this->path));
    $command .= ' && composer create-project --no-dev --no-interaction --no-progress';
    return $command;
  }

  public function postProvisionDelete() {
    if ($this->path != d()->root) {
      $this->notice(dt('Deleting Composer project path at: ') . d()->composer_git_path);
      _provision_recursive_delete($this->path);
    }
  }

}
