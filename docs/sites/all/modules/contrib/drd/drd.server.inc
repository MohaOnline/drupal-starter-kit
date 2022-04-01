<?php

/**
 * Build the server edit page
 *
 * @param $sid
 * @return array|mixed
 */
function drd_server_edit_page($sid) {
  // If this server does not exist return page not found
  if (drd_server_info($sid) === FALSE) {
    drupal_not_found();
    exit;
  }
  return drupal_get_form('drd_server_edit', $sid);
}

/**
 * Build the form to edit Drupal core properties.
 *
 * @param array $form
 * @param array $form_state
 * @param int $sid
 * @return array
 */
function drd_server_edit($form, &$form_state, $sid = 0) {
  $server = _drd_server_defaults($sid);
  $form['#server'] = $server;

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#default_value' => $server['name'],
    '#max_length' => 255,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#default_value' => $server['url'],
    '#max_length' => 255,
    '#disabled' => ($server['sid'] > 0),
    '#description' => t('The domain (and optionally path) to the root of your Drupal core installation, e.g. www.example.com or www.example.com/drupal. If you have a multi-site setup, enter one of the domains.'),
  );

  _drd_tags_build_form('server', $server['sid'], $form);

  $form += _drd_http_header_build_form('server', $server['http_headers']);

  $form['usessl'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use SSL'),
    '#default_value' => $server['usessl'],
  );
  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $server['status'],
  );
  $form['remoteinstall'] = array(
    '#type' => 'checkbox',
    '#title' => t('Support remote installation'),
    '#default_value' => $server['remoteinstall'],
  );
  $form['remoteinstallinfo'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="remoteinstall"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['remoteinstallinfo']['info'] = array(
    '#markup' => t('To support remote installation, go to your remote Drupal core, move the file drd_server.install.php.txt to the Drupal core root directory and rename it to drd_server.install.php.'),
    '#prefix' => '<div class="messages warning">',
    '#suffix' => '</div>',
  );
  $form += _drd_server_aes_config($server['aes_impl'], $server['aes_cipher'], $server['aes_iv']);

  $form['ssh2'] = array(
    '#type' => 'fieldset',
    '#title' => t('SSH2 Settings'),
    '#access' => function_exists('ssh2_connect'),
  );
  $form['ssh2']['ssh2_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable SSH2 support'),
    '#default_value' => $server['settings']['ssh2']['enable'],
  );
  $form['ssh2']['settings'] = array(
    '#type' => 'container',
    '#states' => array(
      'invisible' => array(
        'input[name="ssh2_enable"]' => array('checked' => FALSE),
      ),
    ),
  );
  $form['ssh2']['settings']['ssh2_host'] = array(
    '#type' => 'textfield',
    '#title' => 'Hostname or IP',
    '#default_value' => $server['settings']['ssh2']['host'],
  );
  $ssh_auth_options = array(
    '1' => t('Username and password'),
    '2' => t('Public key'),
  );
  if (function_exists('ssh2_auth_agent')) {
    $ssh_auth_options['3'] = t('Agent');
  }
  $form['ssh2']['settings']['ssh2_auth_mode'] = array(
    '#type' => 'select',
    '#title' => t('Auth mode'),
    '#options' => $ssh_auth_options,
    '#default_value' => $server['settings']['ssh2']['auth']['mode'],
  );
  $form['ssh2']['settings']['ssh2_port'] = array(
    '#type' => 'textfield',
    '#title' => 'Port',
    '#default_value' => $server['settings']['ssh2']['port'],
  );
  $form['ssh2']['settings']['ssh2_auth_username'] = array(
    '#type' => 'textfield',
    '#title' => 'User name',
    '#default_value' => $server['settings']['ssh2']['auth']['username'],
  );
  $form['ssh2']['settings']['ssh2_auth_mode_1'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="ssh2_auth_mode"]' => array('value' => '1'),
      ),
    ),
  );
  $form['ssh2']['settings']['ssh2_auth_mode_1']['ssh2_auth_password'] = array(
    '#type' => 'password',
    '#title' => 'Password',
    '#default_value' => $server['settings']['ssh2']['auth']['password'],
  );
  $form['ssh2']['settings']['ssh2_auth_mode_2'] = array(
    '#type' => 'container',
    '#states' => array(
      'visible' => array(
        ':input[name="ssh2_auth_mode"]' => array('value' => '2'),
      ),
    ),
  );
  $form['ssh2']['settings']['ssh2_auth_mode_2']['ssh2_auth_file_public_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Public key filename',
    '#default_value' => $server['settings']['ssh2']['auth']['file_public_key'],
  );
  $form['ssh2']['settings']['ssh2_auth_mode_2']['ssh2_auth_file_private_key'] = array(
    '#type' => 'textfield',
    '#title' => 'Private key filename',
    '#default_value' => $server['settings']['ssh2']['auth']['file_private_key'],
  );
  $form['ssh2']['settings']['ssh2_auth_mode_2']['ssh2_auth_key_secret'] = array(
    '#type' => 'password',
    '#title' => 'Passphrase for encrypted private key',
    '#default_value' => $server['settings']['ssh2']['auth']['key_secret'],
  );
  $form['ssh2']['settings']['drush_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Drush support'),
    '#default_value' => $server['settings']['ssh2']['drush'],
  );

  if ($server['sid']) {
    $sub_form = drd_action_remote('drd.config.server.read', 'server', $server['sid']);
    if (!empty($sub_form) && is_array($sub_form)) {
      $form = array_merge($form, $sub_form);
    }
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Validates the edited Drupal core properties before submitting them.
 *
 * @param array $form
 * @param array $form_state
 */
function drd_server_edit_validate($form, &$form_state) {
  _drd_server_validate($form_state['values']);
}

/**
 * Submits the edited Drupal core properties and installs the Drupal core if required.
 *
 * @param array $form
 * @param array $form_state
 */
function drd_server_edit_submit($form, &$form_state) {
  $values = $form_state['values'];
  $server = $form['#server'];
  $goto = _drd_server_save($values, $server);

  if (!empty($goto)) {
    drupal_goto($goto);
  }
}

/**
 * @param int $sid
 * @return array
 */
function drd_server_view($sid) {
  $build['info'] = drd_server_info($sid);

  // If this server does not exist return page not found
  if ($build['info'] === FALSE) {
    drupal_not_found();
    exit;
  }

  $url = drd_url_by_id('server', $sid);
  $output = '';
  drd_info('server', $sid, $url, $output);
  $form = drupal_get_form('drd_action_form', 'server', $sid);
  $build['actions_form'] = array(
    '#markup' => drupal_render($form),
  );
  $build['detail'] = array(
    '#markup' => $output,
  );

  return $build;
}

/**
 * @param int $sid
 * @return array
 */
function drd_server_view_heartbeat($sid) {
  $build['info'] = drd_server_info($sid);

  // If this server does not exist return page not found
  if ($build['info'] === FALSE) {
    drupal_not_found();
    exit;
  }

  include_once('drd.monitor.inc');

  $url = drd_url_by_id('server', $sid);
  $output = '';
  drd_charts('server', $sid, $url, $output);
  $build['detail'] = array(
    '#markup' => $output,
  );

  return $build;
}

/** ======================================================================
 *
 * Helper functions
 *
 */

/**
 * Internal callback to determine a fully propagated array with server values.
 * @param int $sid
 * @param string $name
 * @return array
 */
function _drd_server_defaults($sid = 0, $name = '') {
  if ($sid) {
    $server = (array) drd_server_load($sid);
    if (is_string($server['settings'])) {
      $server['settings'] = unserialize($server['settings']);
    }
  }
  else {
    $aes_default = _drd_server_aes_config('mcrypt', '', '');
    $server = array(
      'sid' => 0,
      'name' => $name,
      'url' => '',
      'usessl' => 0,
      'status' => 1,
      'remoteinstall' => 0,
      'weight' => 0,
      'aes_key' => '',
      'aes_cipher' => $aes_default['aes_cipher']['#default_value'],
      'aes_iv' => '',
      'aes_impl' => $aes_default['aes_implementation']['#default_value'],
      'drupalversion' => '',
      'settings' => array(),
      'http_headers' => array(),
    );
  }
  if (empty($server['aes_iv'])) {
    $server['aes_iv'] = base64_encode(drd_aes_get_iv());
  }
  $server['settings'] = drupal_array_merge_deep(array(
    'ssh2' => array(
      'enable' => FALSE,
      'host' => '',
      'port' => 22,
      'drush' => FALSE,
      'auth' => array(
        'mode' => 1,
        'username' => '',
        'password' => '',
        'file_public_key' => '',
        'file_private_key' => '',
        'key_secret' => '',
      ),
    ),
  ), $server['settings']);

  return $server;
}

/**
 * Internal callback to validate all server values and to either output possible
 * error as form errors if the validation was called from the form validation
 * context or as drush messages if called from drush.
 *
 * @param array $values
 * @param string $output
 * @return bool
 */
function _drd_server_validate($values, $output = 'form') {
  // Try to validate the URL
  $url = $values['url'];
  $usessl = $values['usessl'];
  $valid = TRUE;
  if (strpos($url, '://') !== FALSE) {
    _drd_server_validate_output($output, 'url', t('The URL can not contain a the protocol, i.e. do not input http:// at the beginning of the URL.'));
    $valid = FALSE;
  }
  if (strpos($url, ' ') !== FALSE) {
    _drd_server_validate_output($output, 'url', t('The URL can not contain space characters.'));
    $valid = FALSE;
  }
  if (strpos($url, '/') === 0) {
    _drd_server_validate_output($output, 'url', t('The URL should not contain leading slashes.'));
    $valid = FALSE;
  }
  if (!empty($url) && drupal_substr($url, -1) == '/') {
    _drd_server_validate_output($output, 'url', t('The URL should not contain trailing slashes.'));
    $valid = FALSE;
  }
  if (empty($url)) {
    _drd_server_validate_output($output, 'url', t('The URL can not be empty.'));
    $valid = FALSE;
  }
  if ($valid) {
    $url = ($usessl ? 'https://' : 'http://') . $url;
    try {
      $dummy = new stdClass();
      $dummy->http_headers = _drd_http_header_extract($values);
      $options = _drd_http_default_options($dummy);
      $output = _drd_xmlrpc($url . '/xmlrpc.php', 'system.getCapabilities', array(), $options);
      if (empty($output['xmlrpc'])) {
        $error = xmlrpc_error();
        if ($error->code == 302) {
          $new_url = str_replace('/xmlrpc.php', '', $error->message);
          _drd_server_validate_output($output, 'url', t('Your URL %url redirects to %target, please adjust your settings accordingly.', array('%url' => $url, '%target' => $new_url)));
          $url = $new_url;
        }
        else {
          watchdog('DRD', 'XMLRPC error code @code: @message', array('@code' => $error->code, '@message' => $error->message,), WATCHDOG_ALERT);
          $valid = FALSE;
        }
      }
    }
    catch (Exception $e) {
      $valid = FALSE;
    }
    if (!$valid) {
      _drd_server_validate_output($output, 'url', t('Failed to connect to the URL %url.', array('%url' => $url)));
    }
    else {
      // Check the remote installation support
      if ($values['remoteinstall']) {
        $valid = TRUE;
        $url .= '/drd_server.install.php';
        try {
          $response = drupal_http_request($url, array('max_redirects' => 0));
          if ($response->code != 200) {
            $valid = FALSE;
          }
        }
        catch (Exception $e) {
          $valid = FALSE;
        }
        if (!$valid) {
          _drd_server_validate_output($output, 'remoteinstall', t('Remote installation file drd_server.install.php not available on this domain.'));
        }
      }
    }
  }

  return $valid;
}

/**
 * Internal callback called by _drd_server_validate() to generate messages as
 * form errors or drush messages.
 *
 * @param string $output
 * @param string $field
 * @param string $message
 */
function _drd_server_validate_output($output, $field, $message) {
  if ($output == 'form') {
    form_set_error($field, $message);
  }
  else if ($output == 'drush') {
    drush_set_error($message);
  }
}

/**
 * Internal callback to receive all validated server values to create or update
 * server values and perform additional tasks required to utilize that server
 * in the dashboard.
 *
 * @param array $values
 * @param array $server
 * @param bool $is_drush
 * @return bool|string
 */
function _drd_server_save($values, $server, $is_drush = FALSE) {
  $changed = FALSE;
  $goto = FALSE;
  $remote_update = FALSE;
  $aes_changed = ($server['aes_impl'] != $values['aes_implementation'] || $server['aes_cipher'] != $values['aes_cipher']);

  $server['name'] = $values['name'];
  $server['url'] = $values['url'];
  $server['usessl'] = $values['usessl'];
  $server['status'] = $values['status'];
  $server['remoteinstall'] = $values['remoteinstall'];
  $server['aes_impl'] = $values['aes_implementation'];
  $server['aes_cipher'] = $values['aes_cipher'];
  $server['aes_iv'] = $values['aes_iv'];

  $http_headers_old = isset($server['http_headers']) ? $server['http_headers'] : array();
  unset($server['http_headers']);

  if (empty($values['ssh2_auth_password'])) {
    $values['ssh2_auth_password'] = $server['settings']['ssh2']['auth']['password'];
  }
  if (empty($values['ssh2_auth_key_secret'])) {
    $values['ssh2_auth_key_secret'] = $server['settings']['ssh2']['auth']['key_secret'];
  }
  $server['settings'] = array(
    'ssh2' => array(
      'enable' => $values['ssh2_enable'],
      'host' => $values['ssh2_host'],
      'port' => $values['ssh2_port'],
      'drush' => $values['drush_enable'],
      'auth' => array(
        'mode' => $values['ssh2_auth_mode'],
        'username' => $values['ssh2_auth_username'],
        'password' => $values['ssh2_auth_password'],
        'file_public_key' => $values['ssh2_auth_file_public_key'],
        'file_private_key' => $values['ssh2_auth_file_private_key'],
        'key_secret' => $values['ssh2_auth_key_secret'],
      ),
    ),
  );

  if (empty($server['aes_key']) # There is no password set yet at all
    || $aes_changed # Either implementation or cipher has changed
    ) {
    $changed = TRUE;
    # Let's create a new key
    if ($server['aes_impl'] == 'mcrypt') {
      $key_length = mcrypt_get_key_size($server['aes_cipher'], MCRYPT_MODE_CBC);
    }
    else {
      $key_length = 16;
    }
    $server['aes_key'] = base64_encode(drupal_random_bytes($key_length));
  }

  $new = ($server['sid'] == 0);
  if (!$new) {
    // db_update requires the settings field to be serialized where as
    // drupal_write_record serializes the field itself.
    $server['settings'] = serialize($server['settings']);
    db_update('drd_server')
      ->fields($server)
      ->condition('sid', $server['sid'])
      ->execute();
  }
  else {
    drupal_write_record('drd_server', $server);
    $changed = FALSE;
    if ($server['remoteinstall']) {
      $result = drd_install_drd_server('install server', 'server', $server['sid'], 0, FALSE);
    }
    else {
      $result = $is_drush ? FALSE : t('This Drupal core does not support remote installation of the drd_server module. If you want to support remote installation, go to your remote Drupal core, move the file drd_server.install.php.txt to the Drupal core root directory and rename it to drd_server.install.php! After that, change the setting here for this Drupal core and turn remote installation on.');
    }
    if (!$result) {
      if (!$is_drush) {
        drd_set_message($server, 'Drupal core installation failed, please check manually.', 'error');
      }
    }
    else if ($result == 'Either module DRD_SERVER or AES can not be enabled.') {
      if (!$is_drush) {
        drd_set_message($server, $result, 'error');
      }
    }
    else if ($is_drush) {
      $remote_update = TRUE;
    }
    else {
      drd_set_message((object) $server, $result);
      $query = drupal_get_destination();
      $query['goto'] = '1';
      $query['destination'] = 'admin/drd';
      $query['drd-extras'] = '1';
      $link = l('here', 'admin/drd/server/' . $server['sid'] . '/pushaes', array('query' => $query));
      drd_set_message((object) $server, 'After that, ...');
      drd_set_message((object) $server, '... either click ' . $link . ' to push the AES keys to the Drupal core.');
      drd_set_message((object) $server, '... or edit the remote AES keys manually: Implementation = "' . $server['aes_impl'] . '", Cipher = "' . $server['aes_cipher'] . '", Key = "' . $server['aes_key'] . '", IV = "' . $server['aes_iv'] . '"');
    }
    $goto = 'admin/drd';
  }

  _drd_http_header_save($values, $http_headers_old, $server['sid'], 0);

  if (isset($values['tags'])) {
    _drd_tags_save('server', $server['sid'], $values['tags']);
  }

  if ($is_drush) {
    return $remote_update;
  }
  if ($changed) {
    drd_update_aes($server['sid']);
  }
  if (!$new) {
    drd_action_remote('drd.config.server.save', 'server', $server['sid'], $values);
  }

  return $goto;
}

function _drd_server_aes_config($implementation, $cipher, $iv) {
  $phpsec_load_result = _drd_aes_load_phpsec();
  $phpsec_loaded = ($phpsec_load_result > 0);
  $form = array();
  $encryption_implementations = array();
  if ($phpsec_loaded) {
    $encryption_implementations['phpseclib'] = t('PHP Secure Communications Library (phpseclib)');
  }
  if (extension_loaded('mcrypt')) {
    $encryption_implementations['mcrypt'] = t('Mcrypt extension');
  }
  if (empty($encryption_implementations)) {
    $encryption_implementations = array(t('None!'));
  }
  $form['aes_implementation'] = array(
    '#type' => 'select',
    '#title' => t('AES implementation'),
    '#options' => $encryption_implementations,
    '#default_value' => $implementation,
  );
  $form['aes_cipher'] = array(
    '#type' => 'select',
    '#title' => t('Cipher'),
    '#options' => array(
      'rijndael-128' => 'Rijndael 128',
      'rijndael-192' => 'Rijndael 192',
      'rijndael-256' => 'Rijndael 256'
    ),
    '#default_value' => $cipher,
    '#states' => array(
      'invisible' => array(
        ':input[name="aes_implementation"]' => array('value' => 'phpseclib'),
      ),
    ),
  );
  $form['aes_iv'] = array(
    '#type' => 'textfield',
    '#title' => 'AES IV',
    '#default_value' => $iv,
    '#disabled' => TRUE,
  );
  $form['aes_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#disabled' => TRUE,
    '#description' => t('A random key will be generated automatically depending on other settings regarding implementation and cipher you select.'),
  );
  return $form;
}
