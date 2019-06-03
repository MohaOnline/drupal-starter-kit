<?php

/**
 * Base class for HTTPS enabled virtual hosts.
 *
 * This class primarily abstracts the process of making sure the relevant keys
 * are synched to the server when the config files that use them get created.
 */
class Provision_Config_Http_Https_Site extends Provision_Config_Http_Site {
  public $template = 'vhost_https.tpl.php';
  public $disabled_template = 'vhost_https_disabled.tpl.php';
  public $https_cert_ok = TRUE;

  public $description = 'encrypted virtual host configuration';

  function write() {
    if ($this->https_enabled && $this->https_key && !drush_get_error()) {
      $path = dirname($this->data['https_cert']);
      // Make sure the ssl.d directory in the server ssl.d exists. 
      provision_file()->create_dir($path, 
      dt("HTTPS Certificate directory for %key on %server", array(
        '%key' => $this->https_key,
        '%server' => $this->data['server']->remote_host,
      )), 0700);

      // Copy the certificates to the server's ssl.d directory.
      if (!provision_file()->copy($this->data['https_cert_source'], $this->data['https_cert'])->status()) {
        if (drush_get_option('hosting_https_fail_task_if_certificate_fails', FALSE)) {
          drush_set_error('HTTPS_CERT_COPY_FAIL', dt('failed to copy HTTPS certificate in place'));
        }
        else {
          drush_log(dt('failed to copy HTTPS certificate in place'), 'warning');
        }
        $this->https_cert_ok = FALSE;
      }
      if (!provision_file()->copy($this->data['https_cert_key_source'], $this->data['https_cert_key'])->status()) {

        if (drush_get_option('hosting_https_fail_task_if_certificate_fails', FALSE)) {
          drush_set_error('HTTPS_KEY_COPY_FAIL', dt('failed to copy HTTPS key in place'));
        }
        else {
          drush_log(dt('failed to copy HTTPS key in place'), 'warning');
        }
        $this->https_cert_ok = FALSE;
      }
      // Copy the chain certificate, if it is set.
      if (!empty($this->data['https_chain_cert_source'])) {
        if (!provision_file()->copy($this->data['https_chain_cert_source'], $this->data['https_chain_cert'])->status()) {
          if (drush_get_option('hosting_https_fail_task_if_certificate_fails', FALSE)) {
            drush_set_error('HTTPS_CHAIN_COPY_FAIL', dt('failed to copy HTTPS certficate chain in place'));
          }
          else {
            drush_log(dt('failed to copy HTTPS certficate chain in place'), 'warning');
          }
          $this->https_cert_ok = FALSE;
        }
      }

      // If cert is not ok, turn off ssl_redirection.
      if ($this->https_cert_ok == FALSE) {
        $this->data['ssl_redirection'] = FALSE;
      }

      // Sync the key directory to the remote server.
      $this->data['server']->sync($path);
    }

    // Call parent's write AFTER ensuring the certificates are in place to prevent
    // the vhost from referencing missing files.
    parent::write();
  }

  /**
   * Remove a stale certificate file from the server.
   */
  function unlink() {
    parent::unlink();

    if ($this->https_enabled) {
      // TODO: Delete the certificate. Presumably this should look something like:
      // $this->server->service('Certificate')->delete_certificates($this->https_key);
    }
  }
  
}

