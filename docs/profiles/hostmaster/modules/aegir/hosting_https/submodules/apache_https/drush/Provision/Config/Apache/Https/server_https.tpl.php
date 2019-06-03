<?php if (!empty($https_cert) && !empty($https_cert_key)) : ?>
NameVirtualHost <?php print "*:" . $https_port . "\n"; ?>

<IfModule !ssl_module>
  LoadModule ssl_module modules/mod_ssl.so
</IfModule>

<VirtualHost *:443>
  ServerName default
  Redirect 404 /
  SSLEngine on
  SSLCertificateFile <?php print $https_cert . "\n"; ?>
  SSLCertificateKeyFile <?php print $https_cert_key . "\n"; ?>
<?php if (!empty($https_chain_cert)) : ?>
  SSLCertificateChainFile <?php print $https_chain_cert . "\n"; ?>
<?php endif; ?>
</VirtualHost>
<?php endif; ?>

<?php include(provision_class_directory('Provision_Config_Apache_Server') . '/server.tpl.php'); ?>
