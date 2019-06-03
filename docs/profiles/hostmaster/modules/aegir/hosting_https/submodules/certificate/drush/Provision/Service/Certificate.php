<?php

/**
 * The service type base class.
 */
class Provision_Service_Certificate extends Provision_Service {
  public $service = 'Certificate';

  // Whether the certificate can generate a default server certificate.
  public $can_generate_default = FALSE;

  /**
   * Called on provision-verify.
   */
  function verify() {
    #$this->create_config(d()->type);
    #$this->parse_configs();
  }

  /**
   * PUBLIC API
   */

  /**
   * Return the path where we'll generate our certificates.
   */
  function get_source_path($https_key) {
    // Default to the ~/config/ssl.d directory.
    return "{$this->server->ssld_path}/{$https_key}";
  }

  /**
   * Retrieve an array containing the actual files for this https_key.
   *
   * Always attempt to generate new certificates.  The upstream script should
   * recognize non-expired ones, and leave them in place.  So it checks for
   * us.  We don't need to check which ones are still valid.
   */
  function get_certificates($https_key) {
    $this->generate_certificates($https_key);
    return $this->get_certificate_paths($https_key);
  }

  /**
   * Retrieve an array containing source and target paths for this https_key.
   */
  function get_certificate_paths($https_key) {
    // This is a dummy implementation. We should probably move this into an
    // interface.
    return TRUE;
  }

  /**
   * Generate a self-signed certificate for the provided key.
   */
  function generate_certificates($https_key) {
    // This is a dummy implementation. We should probably move this into an
    // interface.
    return TRUE;
  }

  /**
   * Commonly something like running the restart_cmd or sending SIGHUP to a process.
   */
  function parse_configs() {
    return TRUE;
  }

  /**
   * Generate a site specific configuration file.
   */
  function create_site_config() {
    return TRUE;
  }

  /**
   * Remove an existing site configuration file.
   */
  function delete_site_config() {
    return TRUE;
  }

  /**
   * Add a new platform specific configuration file.
   */
  function create_platform_config() {
    return TRUE;
  }

  /**
   * Remove an existing platform configuration file.
   */
  function delete_platform_config() {
    return TRUE;
  }

  /**
   * Create a new server specific configuration file.
   */
  function create_server_config() {
    return TRUE;
  }

  /**
   * Remove an existing server specific configuration file.
   */
  function delete_server_config() {
    return TRUE;
  }
}
