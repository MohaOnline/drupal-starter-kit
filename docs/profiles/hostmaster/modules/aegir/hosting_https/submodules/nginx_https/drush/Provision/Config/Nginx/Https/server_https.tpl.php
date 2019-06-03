<?php

/**
 * @file
 * Template file for the Nginx server configuration.
 */

// Inject the HTTP configuration.
include provision_class_directory('Provision_Config_Nginx_Server') . '/server.tpl.php';

const CHALLENGE_PATH_EXTERNAL = '/.well-known/acme-challenge';

$server = d();
if (($server->Certificate_service_type == 'LetsEncrypt') && ($challenge_path_internal = $server->letsencrypt_challenge_path)) {
  drush_log(dt("Injecting Let's Encrypt 'well-known' ACME challenge directory ':path' into Nginx vhost entry.", array(
    ':path' => $challenge_path_internal,
  )));
?>


#######################################################
###  nginx default server overrides for HTTPS set-up
#######################################################

# Allow access to the letsencrypt.org ACME challenges directory.
# See https://github.com/lukas2511/dehydrated/blob/master/docs/wellknown.md.
# This will override the default "nginx default server" stanza for HTTP (port
# 80), which will be ignored because this one is set as the default.
server {
  listen       <?php print '*:' . $http_port; ?> default_server;
  server_name  _;
  location / {
    return 404;
  }
  location ^~ <?php print CHALLENGE_PATH_EXTERNAL ?> {
    alias <?php print $challenge_path_internal ?>;
    try_files $uri 404;
  }
}
<?php
}
?>


#######################################################
###  nginx default HTTPS server
#######################################################
<?php
// TODO: Check/document what "satellite mode" is.
$satellite_mode = drush_get_option('satellite_mode');
if (!$satellite_mode && $server->satellite_mode) {
  $satellite_mode = $server->satellite_mode;
}
?>

server {
  listen       <?php print '*:' . $https_port; ?>;
  server_name  _;
  location / {
<?php if ($satellite_mode == 'boa'): /* TODO: Remove BOA-specific settings. Find ways to re-implement them via hooks, etc.*/?>
    root   /var/www/nginx-default;
    index  index.html index.htm;
<?php else: ?>
    return 404;
<?php endif; ?>
  }
}
