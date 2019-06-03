
<?php if ($this->https_enabled && $this->https_key && $this->https_cert_ok) : ?>

<?php
$satellite_mode = drush_get_option('satellite_mode');
if (!$satellite_mode && $server->satellite_mode) {
  $satellite_mode = $server->satellite_mode;
}

$nginx_has_http2 = drush_get_option('nginx_has_http2');
if (!$nginx_has_http2 && $server->nginx_has_http2) {
  $nginx_has_http2 = $server->nginx_has_http2;
}

if ($nginx_has_http2) {
  $ssl_args = "ssl http2";
}
else {
  $ssl_args = "ssl";
}
?>

server {
  listen       <?php print "*:{$https_port} {$ssl_args}"; ?>;
  server_name  <?php print $this->uri . ' ' . implode(' ', str_replace('/', '.', $this->aliases)); ?>;
<?php if ($satellite_mode == 'boa'): /* TODO: Remove BOA-specific config. */?>
  root         /var/www/nginx-default;
  index        index.html index.htm;
  ### Do not reveal Aegir front-end URL here.
<?php else: ?>
  return 302 <?php print $this->platform->server->web_disable_url . '/' . $this->uri ?>;
<?php endif; ?>
  ssl                        on;
  ssl_certificate_key        <?php print $https_cert_key; ?>;
<?php if (!empty($https_chain_cert)) : ?>
  ssl_certificate            <?php print $https_chain_cert; ?>;
<?php else: ?>
  ssl_certificate            <?php print $https_cert; ?>;
<?php endif; ?>
}

<?php endif; ?>

<?php
  // Generate the standard virtual host too.
  include(provision_class_directory('Provision_Config_Nginx_Site') . '/vhost_disabled.tpl.php');
?>
