<?php

class Provision_ShellCommand {

  // The prefix used for properties in Aegir contexts.
  protected $context_prefix = '';

  // List of properties to load from the Aegir context.
  protected $context_properties = [];

  /**
   * Initialize properties from the current Aegir context.
   */
  public function __construct() {
    foreach ($this->context_properties as $property) {
      $this->setProperty($property);
    }
  }

  protected function setProperty($property) {
    $context_property_name = $this->context_prefix . $property;
    $context_property = d()->$context_property_name;
    if (is_null($context_property)) {
      return $this->notice(dt('Skipping unset context property: ') . $context_property_name);
    }
    $this->$property = $context_property;
  }

  protected function error($message) {
    return drush_set_error('PLATFORM_GIT_CLONE_FAILED', $message);
  }

  protected function abort($message) {
    return drush_user_abort($message);
  }

  protected function log($message, $type) {
    drush_log($message, $type);
    return true;
  }

  protected function notice($message) {
    return $this->log($message, 'notice');
  }

  protected function warning($message) {
    return $this->log($message, 'warning');
  }

  protected function success($message) {
    return $this->log($message, 'success');
  }

  protected function pathExists($path) {
    if (!empty($path)) {
      return provision_file()->exists($path)->status();
    }
    $this->warning(dt('No path provided.'));
    return FALSE;
  }

  /**
   * Run a command in a subshell, and post the output once complete.
   */
  protected function runCommand($command) {
    $this->notice("Running `$command`");
    if (drush_shell_exec($command)) {
      $this->success(dt('Command succeeded.'));
      if ($output = drush_shell_exec_output()) {
        $this->success(implode("\n", $output));
      }
      return TRUE;
    }
    else {
      $this->error(dt('Command failed. The specific errors follow:'));
      if ($output = drush_shell_exec_output()) {
        $this->error(implode("\n", $output));
      }
      return FALSE;
    }
  }

  /**
   * Run a command in a subprocess, and stream the output.
   */
  protected function execCommand($command) {
    $this->notice("Executing: `$command`");
    $descriptorspec = [
      0 => ['pipe', 'r'], // stdin
      1 => ['pipe', 'w'], // stdout
      2 => ['pipe', 'w'], // stderr
    ];
    $process = proc_open($command, $descriptorspec, $pipes, realpath('./'));
    if (is_resource($process)) {
      $sockets = array($pipes[1], $pipes[2]);
      $write = $except = NULL;
      while (stream_select($sockets, $write, $except, $timeout = 300)) {
        foreach ($sockets as $socket) {
          if (!feof($socket) && $output = fgets($socket)) {
            $this->notice($output);
          }
          else {
            foreach ($sockets as $pipe) {
              fclose($pipe);
            }
            // Workaround for proc_close()'s propensity to return an error status.
            $status = proc_get_status($process);
            $exit = proc_close($process);
            $return = $status['running'] ? $exit : $status['exitcode'];
            if ($return === 0) {
              return $this->success("Finished running: `$command`");
            }
            else {
              return $this->error(dt('An error occured when running command! (returned :exit)', [':exit' => $return]));
            }
          }
        }
      }
    }
  }

}
