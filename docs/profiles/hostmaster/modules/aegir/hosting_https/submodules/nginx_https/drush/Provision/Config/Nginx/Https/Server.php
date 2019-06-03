<?php

/**
 * Server config file for Nginx HTTPS.
 *
 * This configuration file replaces the Nginx server configuration file, but
 * inside the template, the original file is once again included.
 *
 * This config is primarily reponsible for enabling the HTTPS relation settings,
 * so that individual sites can just enable them.
 */
class Provision_Config_Nginx_Https_Server extends Provision_Config_Http_Https_Server {
  // We use the same extra_config as the nginx_server config class.
  function process() {
    parent::process();
    $this->data['extra_config'] = "# Extra configuration from modules:\n";
    $this->data['extra_config'] .= join("\n", drush_command_invoke_all('provision_nginx_server_config', $this->data));
  }
}
