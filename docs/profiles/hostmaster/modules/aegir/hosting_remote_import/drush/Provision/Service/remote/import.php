<?php

class Provision_Service_remote_import extends Provision_Service {
  public $service = 'remote_import';

  /**
   * Initialize the service along with the server object.
   */
  function init() {
    // REMEMBER TO CALL THE PARENT!
    parent::init();
  }

  /**
   * Called on provision-verify.
   *
   * We change what we will do based on what the
   * type of object the command is being run against.
   */
  function verify_server_cmd() {
    if ($this->context->type == 'server') {
      // We need to make sure that this is a REMOTE server!
      if (provision_is_local_host($this->server->remote_host)) {
        return drush_set_error('REMOTE_SERVER_IS_LOCAL', dt('The specified server is not a remote server.'));
      }
    }
  }

  function list_sites() {
    return array();
  }

  function fetch_site($site) {
    return FALSE;
  }

  function fetch_settings($old_url) {
    return array();
  }

  function deploy($backup_file, $old_uri, $new_uri, $platform, $db_server) {
    // Need to create a context for this one.
    $options = array();
    $options['context_type'] = 'site';
    $options['uri'] = $new_uri;
    $hash_name = drush_get_option('#name') ? '#name' : 'name';
    $options[$hash_name] = $new_uri;
    $options['platform'] = '@' . $platform;
    $options['root'] = d($options['platform'])->root;
    $options['aliases'] = array();
    $options['redirection'] = 0;
    $options['db_server'] = '@' . $db_server;
    $options += $this->fetch_settings($old_uri);

    drush_invoke_process('@self', 'provision-save', array('@' . $new_uri), $options);

    provision_backend_invoke($new_uri, 'provision-deploy', array($backup_file), array('old_uri' => $old_uri));

    if (!drush_get_error()) {
      provision_backend_invoke($new_uri, 'provision-verify');
      return '@' . $new_uri;
    }
  }
}
