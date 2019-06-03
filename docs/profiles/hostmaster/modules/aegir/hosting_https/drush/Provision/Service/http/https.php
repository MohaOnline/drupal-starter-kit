<?php
/**
 * @file
 * The base implementation of the HTTPS capabale web service.
 */

/**
 * The base class for HTTPS supporting servers.
 *
 * In general, these function the same as normal servers, but have an extra
 * port and some extra variables in their templates.
 */
class Provision_Service_http_https extends Provision_Service_http_public {
  protected $https_enabled = TRUE;

  function default_https_port() {
    return 443;
  }

  function init_server() {
    parent::init_server();

    // HTTPS Port.
    $this->server->setProperty('https_port', $this->default_https_port());

    // HTTPS certificate store.
    // The certificates are generated from here, and distributed to the servers,
    // as needed.
    $this->server->ssld_path = $this->server->service('Certificate')->get_source_path('');

    // HTTPS certificate store for this server.
    // This server's certificates will be stored here.
    $this->server->http_ssld_path = "{$this->server->config_path}/ssl.d";
    $this->server->https_enabled = 1;
    $this->server->https_key = 'default';
  }

  function init_site() {
    parent::init_site();

    $this->context->setProperty('https_enabled', 0);
    $this->context->setProperty('https_client_authentication_enabled', 0);
    $this->context->setProperty('https_client_authentication_path', NULL);
    $this->context->setProperty('https_key', NULL);
  }

  function config_data($config = NULL, $class = NULL) {
    $data = parent::config_data($config, $class);
    $data['https_port'] = $this->server->https_port;

    if ($config == 'server') {
      if ($this->server->service('Certificate')->can_generate_default) {
        // Generate a certificate for the default HTTPS vhost, and retrieve the
        // path to the cert and key files. It will be generated if not found.
        $certs = $this->server->service('Certificate')->get_certificates('default');
        $data = array_merge($data, $certs);
      }
    }

    if ($config == 'site' && $this->context->https_enabled) {
      if ($this->context->https_enabled == 2) {
        $data['ssl_redirection'] = TRUE;
        $data['redirect_url'] = "https://{$this->context->uri}";
      }

      if ($this->context->https_key) {
        // Retrieve the paths to the cert and key files.
        // They are generated if not found.
        $certs = $this->server->service('Certificate')->get_certificates($this->context->https_key);
        $data = array_merge($data, $certs);
      }
    }

    return $data;
  }

  /**
   * Verify server.
   */
  function verify_server_cmd() {
    if ($this->context->type === 'server') {
      provision_file()->create_dir($this->server->http_ssld_path,
        dt("HTTPS certificate repository for %server",
        array('%server' => $this->server->remote_host)), 0700);

      $this->sync($this->server->http_ssld_path, array(
        'exclude' => $this->server->http_ssld_path . '/*',  // Make sure remote directory is created
      ));
      $this->sync($this->server->http_ssld_path . '/default');
    }

    // Call the parent at the end. it will restart the server when it finishes.
    parent::verify_server_cmd();
  }

}
