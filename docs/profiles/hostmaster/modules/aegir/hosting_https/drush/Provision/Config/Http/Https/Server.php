<?php

/**
 * Base class for HTTPS enabled server level config.
 */
class Provision_Config_Http_Https_Server extends Provision_Config_Http_Server {
  public $template = 'server_https.tpl.php';
  public $description = 'encryption enabled webserver configuration';

  function can_generate_default() {
    return $this->server->service('Certificate')->can_generate_default;
  }

  function write() {
    parent::write();

    if ($this->https_enabled && $this->https_key && $this->can_generate_default()) {
      $path = dirname($this->data['https_cert']);
      // Make sure the ssl.d directory in the server ssl.d exists.
      provision_file()->create_dir($path,
      dt("Creating HTTPS Certificate directory for %key on %server", array(
        '%key' => $this->https_key,
        '%server' => $this->data['server']->remote_host,
      )), 0700);

      // Copy the certificates to the server's ssl.d directory.
      provision_file()->copy(
        $this->data['https_cert_source'],
        $this->data['https_cert'])
        ->succeed('Copied default HTTPS certificate into place')
        ->fail('Failed to copy default HTTPS certificate into place');
      provision_file()->copy(
        $this->data['https_cert_key_source'],
        $this->data['https_cert_key'])
        ->succeed('Copied default HTTPS key into place')
        ->fail('Failed to copy default HTTPS key into place');
      // Copy the chain certificate, if it is set.
      if (!empty($this->data['https_chain_cert_source'])) {
	      provision_file()->copy(
          $this->data['https_chain_cert_source'],
          $this->data['https_chain_cert'])
          ->succeed('Copied default HTTPS chain certificate key into place')
          ->fail('Failed to copy default HTTPS chain certificate into place');
      }
    }
  }
}
