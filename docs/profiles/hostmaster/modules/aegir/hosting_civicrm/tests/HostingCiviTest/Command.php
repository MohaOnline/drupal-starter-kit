<?php

namespace HostingCiviTest;

use Exception;
use Symfony\Component\Process\Process;
use Symfony\Component\Process\Exception\ProcessFailedException;

class Command {

  /**
   * Wrapper for exec() using Symfony's Process class.
   * Throws an Exception if anything is sent on stderr.
   *
   * @param string $command Base command to execute.
   * @param array $args Command arguments (they get automatically escaped).
   * @param boolean $return_output Return the command output, instead of echoing it.
   *
   * @throws ProcessFailedException, Exception
   *
   * @link http://symfony.com/doc/current/components/process.html
   */
  public static function exec($command, $args = [], $return_output = FALSE) {
    $output = '';

    if (is_array($args)) {
      foreach ($args as $arg) {
        $command .= ' ' . escapeshellarg($arg);
      }
    }
    elseif (!empty($args)) {
      throw new Exception("exec: args must be wrapped in an array.");
    }

    drush_log(dt('┌----------✄------------✄------------✄---------------┐'), 'ok');
    drush_log(dt('| Running: !command', array('!command' => $command)), 'ok');

    $process = new Process($command);
    $process->setTimeout(3600);

    $process->run(function ($type, $data) {
      // $type === $process::ERR || $type === Process::OUT
      // NB: drush outputs all status messages on stderr, so all the same to us.
      // https://github.com/drush-ops/drush/issues/707

      // Filter out DB listings, because they take a lot of space.
      // This is perhaps a bit risky, but they're really annoying...
      // Ex: '  system' (i.e. they are prefixed by two spaces, only 1 word).
      if (preg_match('/^  ([_a-z]+)/', $data)) {
        return;
      }

      // Using plain echo on stdout caused weird buffering issues.
      // Maybe using fprint to stderr would work, but this works too.
      drush_print($data, 0, STDERR);

      if (strpos($data, '[error]') !== FALSE) {
        // Ignore this error, doesn't affect PHP >= 5.6
        // https://docs.acquia.com/articles/php-56-and-mbstringhttpinput-errors
        if (strpos($data, 'Multibyte string input conversion in PHP is active') === FALSE) {
          throw new Exception("Exec stderror: $data");
        }
      }
    });

    $process->wait();

    if (!$process->isSuccessful()) {
      throw new ProcessFailedException($process);
    }

    drush_log(dt('| Finished: !command', array('!command' => $command)), 'ok');
    drush_log(dt('└--------✄------------ done -------------✄-----------┘'), 'ok');

    return TRUE;
  }

  /**
   * Wrapper for exec() using Symfony's Process class, returns the output.
   * Throws an Exception if anything is sent on stderr.
   *
   * @param string $command Base command to execute.
   * @param array $args Command arguments (they get automatically escaped).
   * @param boolean $return_output Return the command output, instead of echoing it.
   *
   * @throws ProcessFailedException, Exception
   *
   * @link http://symfony.com/doc/current/components/process.html
   */
  public static function execReturn($command, $args = []) {
    $output = '';

    if (is_array($args)) {
      foreach ($args as $arg) {
        $command .= ' ' . escapeshellarg($arg);
      }
    }
    elseif (!empty($args)) {
      throw new Exception("exec: args must be wrapped in an array.");
    }

    drush_log(dt('┌----------------------------------------------------┐'), 'ok');
    drush_log(dt('| Running: !command', array('!command' => $command)), 'ok');

    $process = new Process($command);
    $process->setTimeout(3600);

    $process->run();
    $process->wait();

    $output = $process->getOutput();

    if (strpos($output, '[error]') !== FALSE) {
      // Ignore this error, doesn't affect PHP >= 5.6
      // https://docs.acquia.com/articles/php-56-and-mbstringhttpinput-errors
      if (strpos($output, 'Multibyte string input conversion in PHP is active') === FALSE) {
        throw new Exception("Exec stderror: $output");
      }
    }

    if (!$process->isSuccessful()) {
      throw new ProcessFailedException($process);
    }

    drush_log(dt('| Finished: !command', array('!command' => $command)), 'ok');
    drush_log(dt('└--------------------- done -------------------------┘'), 'ok');

    return $output;
  }

}
