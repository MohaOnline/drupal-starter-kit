<?php
/**
 * @file Define a Hosting service class for Apache w/HTTPS.
 */
#module_load_include('service.inc', 'hosting_web_server');
module_load_include('service.inc', 'hosting_https');

class hostingService_http_https_apache extends hostingService_https {
  public $type = 'https_apache';
  public $name = 'Apache HTTPS';

  protected $has_restart_cmd = TRUE;

  /* TODO: Turn this into trait so it can be shared with the base http_apache class. */
  function default_restart_cmd() {
    $command = '/usr/sbin/apachectl'; # a proper default for most of the world
    foreach (explode(':', $_SERVER['PATH']) as $path) {
      $options[] = "$path/apache2ctl";
      $options[] = "$path/apachectl";
    }
    # try to detect the apache restart command
    $options[] = '/usr/local/sbin/apachectl'; # freebsd
    $options[] = '/usr/sbin/apache2ctl'; # debian + apache2
    $options[] = $command;

    foreach ($options as $test) {
      if (is_executable($test)) {
        $command = $test;
        break;
      }
    }

    return "sudo $command graceful";
  }

}


