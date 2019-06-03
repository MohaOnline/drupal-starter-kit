<?php

/**
 * @file
 * A Let's Encrypt implementation of the Certificate service for Provision.
 */

/**
 * Implements hook_drush_init().
 */
function letsencrypt_drush_init() {
  letsencrypt_provision_register_autoload();
}

/**
 * Register our directory as a place to find provision classes.
 */
function letsencrypt_provision_register_autoload() {
  static $loaded = FALSE;
  if (!$loaded) {
    $loaded = TRUE;
    provision_autoload_register_prefix('Provision_', dirname(__FILE__));
  }
}

/**
 * Implements hook_provision_services().
 *
 * Ensure our classes are loaded early enough to instantiate the Provision
 * services.
 */
function letsencrypt_provision_services() {
  letsencrypt_provision_register_autoload();
}


/**
 * Generate Apache config to allow access to /.well-known/acme-challenge
 *
 * @param string $challenge_path The path where verification file will be stored.
 */
function letsencrypt_generate_apache_wellknown_lines($challenge_path) {
    $lines = array();
    $lines[] = "  # Allow access to letsencrypt.org ACME challenges directory.";
    $lines[] = "  Alias /.well-known/acme-challenge {$challenge_path}";
    $lines[] = "";
    $lines[] = "  <Directory {$challenge_path}>";
    $lines[] = "    Options None";
    $lines[] = "    AllowOverride None";
    $lines[] = "";
    $lines[] = "    # Apache 2.x";
    $lines[] = "    <IfModule !mod_authz_core.c>";
    $lines[] = "      Order allow,deny";
    $lines[] = "      Allow from all";
    $lines[] = "    </IfModule>";
    $lines[] = "";
    $lines[] = "    # Apache 2.4";
    $lines[] = "    <IfModule mod_authz_core.c>";
    $lines[] = "      Require all granted";
    $lines[] = "    </IfModule>";
    $lines[] = "  </Directory>";
    $lines[] = "\n";
    return implode("\n", $lines);
}

/**
 * Implements hook_provision_apache_server_config().
 *
 * @see https://github.com/lukas2511/dehydrated/blob/master/docs/wellknown.md
 */
function letsencrypt_provision_apache_server_config($data) {
  if (d()->type != 'server') {
    return '';
  }
  $server = d();
  if (($server->Certificate_service_type == 'LetsEncrypt') && ($challenge_path = $server->letsencrypt_challenge_path)) {
    drush_log(dt("Injecting Let's Encrypt 'well-known' ACME challenge directory ':path' into Apache default vhost entry.", array(
      ':path' => $challenge_path,
    )));
    return letsencrypt_generate_apache_wellknown_lines($challenge_path);
  }
}

/**
 * Implements hook_provision_apache_vhost_config().
 *
 * @see https://github.com/lukas2511/dehydrated/blob/master/docs/wellknown.md
 */
function letsencrypt_provision_apache_vhost_config($uri, $data) {
  if (d()->type != 'site') {
    return '';
  }

  $server = d()->platform->web_server;
  if (($server->Certificate_service_type == 'LetsEncrypt') && ($challenge_path = $server->letsencrypt_challenge_path)) {
    drush_log(dt("Injecting Let's Encrypt 'well-known' ACME challenge directory ':path' into Apache vhost entry.", array(
      ':path' => $challenge_path,
    )));

    return letsencrypt_generate_apache_wellknown_lines($challenge_path);
  }
}

/**
 * Implements hook_drush_command().
 */
function letsencrypt_drush_command() {
  $items['letsencrypt-force-key-regenerate'] = array(
    'description' => 'Force a new key to be generated".',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUSH,
    'aliases' => array('lfkr'),
  );
  return $items;
}

/**
 * Implements the letsencrypt-force-key-regenerate command.
 */
function drush_letsencrypt_force_key_regenerate() {
  $https_key = d()->https_key;
  $certs = d()->platform->web_server->service('Certificate')->get_certificate_paths($https_key);
  drush_log(print_r($certs, 1), 'warning');

  // Remove only the symlink to the certificate.
  unlink($certs['https_cert_source']);

  // Re-verify the site, this re-generates the certificate.
  $options = array();
  $target = d()->uri;
  provision_backend_invoke($target, 'provision-verify', array(), $options);
}

/**
 * Implements hook_provision_nginx_vhost_config().
 *
 * Allow access to letsencrypt.org ACME challenges directory.
 *
 * This is already defined in the server configuration for HTTP, before
 * certificates are generated, but we still need it for the HTTPS vhost to
 * permit renewals.
 *
 * @see https://github.com/lukas2511/dehydrated/blob/master/docs/wellknown.md
 */
function letsencrypt_provision_nginx_vhost_config($uri, $data) {
  if (d()->type != 'site') {
    return '';
  }

  $server = d()->platform->web_server;
  if (($server->Certificate_service_type == 'LetsEncrypt') && ($challenge_path = $server->letsencrypt_challenge_path)) {
    drush_log(dt("Injecting Let's Encrypt 'well-known' ACME challenge directory ':path' into Nginx vhost entry.", array(
      ':path' => $challenge_path,
    )));

    $lines = array();
    $lines[] = "  # Allow access to letsencrypt.org ACME challenges directory.";
    $lines[] = "  location ^~ /.well-known/acme-challenge {";
    $lines[] = "    alias {$challenge_path};";
    $lines[] = "    try_files \$uri 404;";
    $lines[] = "  }";
    $lines[] = "\n";
    return implode("\n", $lines);
  }
}
