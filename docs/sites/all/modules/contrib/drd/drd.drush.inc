<?php

/**
 * @param array $parts
 * @return array
 */
function drd_drush_log_parts($parts = NULL) {
  static $log_parts;
  if (!isset($log_parts)) {
    $log_parts = array('execute');
  }
  if (isset($parts)) {
    $log_parts = $parts;
  }
  return $log_parts;
}

/**
 * Implements hook_drush_command().
 *
 * @return array
 */
function drd_drush_command() {
  $core_options = array(
    'filter-status' => 'Filter the list by the status of the domain: active=1|disabled=0',
    'filter-ssl' => 'Filter the list by the ssl-usage of the domain: yes=1|no=0',
    'filter-drupal-versions' => 'Filter the list by the drupal version of the core',
    'filter-tags' => 'Filter the list by tags of the core',
    'filter-cores' => 'Filter the list by core id(s) they belong to',
  );
  $domain_options = $core_options + array(
    'filter-domains' => 'Filter the list by domain id(s) they belong to',
    'filter-installed' => 'Filter the list by the installation status',
    'filter-warnings' => 'Filter the list by the warnings status',
    'filter-errors' => 'Filter the list by the error status',
  );

  $items = array();

  $items['drd-list-actions'] = array(
    'description' => 'List all the available actions for your DRD network.',
    'options' => array(
      'mode' => array(
        'description' => 'Limit the actions to be listed by their mode: defaults to all but can be either any|server|domain',
        'value' => 'required',
      ),
    ),
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdla'),
  );
  $items['drd-list-cores'] = array(
    'description' => 'List all cores in your DRD network.',
    'options' => $core_options,
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdlc'),
  );
  $items['drd-list-domains'] = array(
    'description' => 'List all domains for all or some DRD cores.',
    'options' => $domain_options,
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdld'),
  );
  $items['drd-list-tags'] = array(
    'description' => 'List all the available tags in your DRD network.',
    'options' => array(
      'mode' => array(
        'description' => 'Limit the tags to be listed by their mode: defaults to all but can be either server or domain',
        'value' => 'required',
      ),
    ),
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdlt'),
  );

  $core_edit_arguments = array(
    'url' => 'Domain (and optionally path) of the remote Drupal core installation.'
  );
  $core_edit_options = array(
    'name' => 'Name of the core, defaults to URL.',
    'tags' => 'Comma separated list of tags for the DRD ui only.',
    'ssl' => 'Whether this core can be accessed via ssl (=1) or not (=0; default).',
    'support-remote-installation' => 'Whether this core supports remote installation (=1) or not (=0; default).',
    'ssh' => 'Whether this core can be accessed via SSH (=1) or not (=0; default).',
    'ssh-host' => 'SSH hostname or ip address.',
    'ssh-port' => 'SSH port defaults to 22.',
    'ssh-auth-mode' => 'How to authenticate a SSH session: 1 = username/password, 2 = public key, 3 = agent.',
    'ssh-auth-username' => 'Username for SSH authentication, required for both authentication methods.',
    'ssh-auth-password' => 'Password for username/password SSH authentication.',
    'ssh-auth-public-key' => 'Filename of the public key for public key SSH authentication.',
    'ssh-auth-private-key' => 'Filename of the private key for public key SSH authentication.',
    'ssh-auth-public-key-passphrase' => 'Passphrase of the private key file for public key SSH authentication.',
    'use-drush' => 'Whether this core has drush installed (=1) or not (=0; default). Requires ssh enabled for this host too.',
    'aes-implementation' => 'Which AES implementation to use, available and default values depend on your system configuration.',
    'aes-cipher' => 'Which AES cipher to use, available and default values depend on AES implementation and your system configuration.',
    'http-header-auth' => 'Whether this core requires http authentication (=1) or not (=0; default).',
    'http-header-auth-username' => 'Username for http authentication.',
    'http-header-auth-password' => 'Password for http authentication.',
    'ip' => 'Outgoing IP address to be added to the remote core as an allowed referer. DRD uses ip.addpspot.com as an alternative if not explicitly defined here.',
    'remote-path' => 'Fully qualified remote path of the drupal installation, used together with SSH if no other option available to remotely install components.'
  );
  $items['drd-core-add'] = array(
    'description' => 'Add a new core to DRD.',
    'arguments' => $core_edit_arguments,
    'options' => $core_edit_options,
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdca'),
    'examples' => array(
      'drush drd-core-add www.example.com' => 'Add that Drupal site as a core to your dashboard.',
      'drush drd-core-add www.example.com/portal' => 'Add that Drupal site which is installed in a subdirectory as a core to your dashboard.',
    ),
  );
  $items['drd-core-update'] = array(
    'description' => 'Update an existing core in DRD.',
    'arguments' => $core_edit_arguments,
    'options' => $core_edit_options,
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdcu'),
  );
  $items['drd-core-remove'] = array(
    'description' => 'Remove an existing core from DRD.',
    'arguments' => array(
      'id' => 'ID of the core to be removed.',
    ),
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdcrm'),
  );

  /*
  $items['drd-tag-add'] = array(
    'description' => 'Add a tag to one or many core(s).',
    'arguments' => array(
      'name' => 'Name of the tag.',
    ),
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdta'),
  );
  $items['drd-tag-remove'] = array(
    'description' => 'Remove a tag from one or many core(s).',
    'arguments' => array(
      'name' => 'Name of the tag.',
    ),
    'drupal dependencies' => array('drd'),
    'aliases' => array('drdtrm'),
  );
  */

  $domain_options += array(
    'ask' => 'Prompt the user to select the core and/or domain to work on'
  );

  $items['drd-action'] = array(
    'description' => 'Trigger a DRD action.',
    'arguments' => array(
      'action' => 'Action name to trigger. If action is not defined, you will get a list to choose from.',
    ),
    'options' => $domain_options,
    'allow-additional-options' => TRUE,
    'examples' => array(
      'drush drd-action drd.list.updates --filter-drupal-version=7.28' => 'Check for project updates on all cores with the given Drupal version.',
      'drush drd-action help:drd.switch.maintenance' => 'Display more detailed help about that specific action.'
    ),
    'drupal dependencies' => array('drd'),
    'aliases' => array('drda'),
  );

  return $items;
}

/**
 * @param string $mode
 * @return array
 */
function _drd_drush_filter($mode) {
  $filter = array(
    'status' => drush_get_option('filter-status', -1),
    'ssl' => drush_get_option('filter-ssl', -1),
    'tags' => drush_get_option_list('filter-tags', array()),
    'drupalversions' => drush_get_option_list('filter-drupal-versions', array()),
    'installed' => drush_get_option('filter-installed', -1),
    'status_w' => drush_get_option('filter-warnings', -1),
    'status_e' => drush_get_option('filter-errors', -1),
    'sids' => drush_get_option_list('filter-cores', array()),
    'dids' => drush_get_option_list('filter-domains', array()),
  );
  if (empty($filter['tags'][0])) {
    $filter['tags'] = array();
  }
  if (empty($filter['drupalversions'][0])) {
    $filter['drupalversions'] = array();
  }
  if (empty($filter['sids'][0])) {
    $filter['sids'] = array();
  }
  if (empty($filter['dids'][0])) {
    $filter['dids'] = array();
  }
  return $filter;
}

/**
 * @param string $mode
 * @param array $ids
 * @param string $name
 * @return bool
 */
function _drd_drush_ask_server_domain($mode, &$ids, $name) {
  if (!drush_get_option('ask', FALSE)) {
    return TRUE;
  }
  $key = ($mode == 'server') ? 'sid' : 'did';
  $prompt = ($mode == 'server') ?
    t('Select the cores to perform the action: !name', array('!name' => $name)) :
    t('Select the domains to perform the action: !name', array('!name' => $name));
  $options = array();

  foreach (db_select('drd_' . $mode, 'o')
    ->fields('o', array($key, 'name'))
    ->condition('o.' . $key, $ids)
    ->execute()
    ->fetchAll() as $option) {
    $options['id-' . $option->{$key}] = $option->name;
  }

  $selected = drush_choice_multiple($options, FALSE, $prompt);
  if (empty($selected)) {
    return FALSE;
  }

  $ids = array();
  foreach ($selected as $id) {
    $ids[] = drupal_substr($id, 3);
  }

  return TRUE;
}

function _drd_drush_ask_action() {
  $options = array();
  $action_list = drush_drd_list_actions(FALSE);
  foreach ($action_list as $mode => $categories) {
    $options['-- ' . $mode . ' --'] = $mode;
    foreach ($categories as $category => $actions) {
      $options['-- ' . $mode . $category . ' --'] = '  ' . $category;
      foreach ($actions as $key => $action) {
        $options[$key] = $action['label'];
      }
    }
  }

  $selected = drush_choice($options, t('Select an action'), '    [!key] !value');
  if (empty($selected)) {
    return FALSE;
  }

  return $selected;
}

function _drd_drush_get_alias($url) {
  static $alias;

  if (!isset($alias)) {
    drush_log('Looking for an alias.');

    $alias = FALSE;
    $alias_name = FALSE;

    drush_sitealias_load_all();
    $all_site_aliases = drush_get_context('site-aliases');
    foreach ($all_site_aliases as $name => $alias_values) {
      if (!empty($alias_values['uri']) && ($url == $alias_values['uri'])) {
        $alias_name = $name;
        break;
      }
    }

    if ($alias_name) {
      drush_log('Found alias !name.', array('!name' => $alias_name));
      $alias = drush_sitealias_get_record($alias_name);
      $result = drush_invoke_process($alias, 'status');
      if (!$result || empty($result['object']['uri']) || strpos($result['object']['uri'], $url) === FALSE) {
        drush_log('Using drush failed.');
        $alias = FALSE;
      }
    }
    else {
      drush_log('No alias found, using local drush not possible');
    }
  }

  return $alias;
}

function _drd_drush_get_remote_alias($server) {
  static $alias;

  if (!isset($alias)) {
    drush_log('Looking for a remote alias.');

    $alias = FALSE;

    include_once 'drd.ssh2.inc';
    $connection = drd_ssh2_connect($server);
    if ($connection) {
      drush_log('Remote SSH connected');
      $remote_path = drush_get_option('remote-path', FALSE);
      if (empty($remote_path)) {
        $all_site_aliases = $connection ? drd_ssh2_command($server, $connection, 'drush site-alias --format=json', TRUE) : array();
        foreach ($all_site_aliases as $name => $alias_values) {
          if (!empty($alias_values['uri']) && ($server->url == $alias_values['uri'])) {
            $alias = '@' . $name;
            break;
          }
        }
      }
      else {
        $alias = '--uri=' . $server->url . ' --root=' . $remote_path;
      }
    }

    if (!empty($alias)) {
      drush_log('Found alias: ' . $alias);
      $result = drd_ssh2_command($server, $connection, 'drush ' . $alias . ' status --format=json', TRUE);
      if (!$result || empty($result['uri']) || strpos($result['uri'], $server->url) === FALSE) {
        drush_log('Using drush failed.');
        $alias = FALSE;
      }
    }
    else {
      drush_log('No remote alias found, using drush over SSH not possible');
    }
  }

  return $alias;
}

/**
 * Get a list of DRD actions.
 *
 * @param bool $formatted
 * @return string|array
 */
function drush_drd_list_actions($formatted = TRUE) {
  $actions = drd_actions();
  $mode = drush_get_option('mode', 'all');
  $rows = array(array(dt('Mode'), dt('Section'), dt('Action'), dt('Description')));
  $action_list = array();

  foreach ($actions as $key => $action) {
    $action_mode = isset($action['mode']) ? $action['mode'] : 'any';
    if ($mode != 'all' && $action_mode != $mode) {
      // This is not for us, skip it
      continue;
    }
    $action_category = empty($action['category']) ? ('-' . t('Main actions') . '-') : $action['category'];
    if (!isset($action_list[$action_mode])) {
      $action_list[$action_mode] = array();
    }
    if (!isset($action_list[$action_mode][$action_category])) {
      $action_list[$action_mode][$action_category] = array();
    }
    $action_list[$action_mode][$action_category][$key] = $action;
  }

  if (!$formatted) {
    return $action_list;
  }

  foreach ($action_list as $mode => $categories) {
    $rows[] = array($mode, '', '', '',);
    foreach ($categories as $category => $actions) {
      $rows[] = array('', $category, '', '',);
      foreach ($actions as $key => $action) {
        $rows[] = array('', '', $key, $action['label'],);
      }
    }
  }
  drush_print_table($rows, TRUE);
}

/**
 * Get a list of DRD cores.
 */
function drush_drd_list_cores() {
  $servers = drd_get_servers(_drd_drush_filter('server'), FALSE);
  $rows = array(array(dt('ID'), dt('Status'), dt('Name'), dt('URL'), dt('SSL'), dt('Drupal')));
  foreach ($servers as $server) {
    $rows[] = array(
      $server->sid,
      $server->status ? t('Active') : t('Disabled'),
      $server->name,
      $server->url,
      $server->usessl ? t('Yes') : t('No'),
      $server->drupalversion,
    );
  }
  drush_print_table($rows, TRUE);
}

/**
 * Get a list of DRD domains.
 */
function drush_drd_list_domains() {
  $domains = drd_get_domains(_drd_drush_filter('domain'), FALSE);
  $rows = array(array(dt('ID'), dt('Status'), dt('Name'), dt('URL'), dt('SSL'), dt('Installed')));
  foreach ($domains as $domain) {
    $rows[] = array(
      $domain->did,
      $domain->status ? t('Active') : t('Disabled'),
      $domain->name,
      $domain->url,
      $domain->usessl ? t('Yes') : t('No'),
      $domain->installed ? t('Yes') : t('No'),
    );
  }
  drush_print_table($rows, TRUE);
}

/**
 * Get a list of DRD tags.
 */
function drush_drd_list_tags() {
  $tids = db_select('drd_term', 't')
    ->fields('t', array('tid'))
    ->distinct()
    ->execute()
    ->fetchAllKeyed(0, 0);
  $tags = taxonomy_term_load_multiple(array_keys($tids));

  $mode = drush_get_option('mode', 'all');
  $rows = array(array(dt('Mode'), dt('ID'), dt('Tag'),));
  $tag_list = array();

  foreach ($tags as $tid => $tag) {
    $tag_modes = db_select('drd_term', 't')
      ->fields('t', array('mode'))
      ->condition('t.tid', $tid)
      ->distinct()
      ->execute()
      ->fetchAllKeyed(0, 0);
    if (in_array('domain', $tag_modes) && in_array('server', $tag_modes)) {
      $tag_mode = 'any';
    }
    else if (in_array('domain', $tag_modes)) {
      $tag_mode = 'domain';
    }
    else if (in_array('server', $tag_modes)) {
      $tag_mode = 'server';
    }
    else {
      $tag_mode = '';
    }
    if ($mode != 'all' && $tag_mode != $mode) {
      // This is not for us, skip it
      continue;
    }
    if (!isset($tag_list[$tag_mode])) {
      $tag_list[$tag_mode] = array();
    }
    $tag_list[$tag_mode][$tid] = $tag->name;
  }
  foreach ($tag_list as $mode => $names) {
    $rows[] = array($mode, '', '',);
    foreach ($names as $tid => $name) {
      $rows[] = array('', $tid, $name,);
    }
  }
  drush_print_table($rows, TRUE);
}

/**
 * Trigger a DRD action.
 */
function drush_drd_action() {
  $args = func_get_args();
  if (count($args) > 0) {
    $action_name = array_shift($args);
    if (strpos($action_name, 'help:') === 0) {
      _drush_drd_action_help(drupal_substr($action_name, 5));
      return;
    }
    $interactive = FALSE;
  }
  else {
    $action_name = _drd_drush_ask_action();
    if (!$action_name) {
      return;
    }
    $interactive = TRUE;
  }
  $action = drd_actions($action_name);
  if (empty($action)) {
    drush_set_error(t('Action !name does not exist. Cancelled.', array('!name' => $action_name)));
  }

  $action_mode = isset($action['mode']) ? $action['mode'] : 'any';

  $sids = in_array($action_mode, array('any', 'server')) ? drd_get_server_ids(_drd_drush_filter('server')) : array();
  $dids = in_array($action_mode, array('any', 'domain')) ? drd_get_domain_ids(_drd_drush_filter('domain')) : array();

  $_POST = array();
  if (!empty($action['fields'])) {
    foreach ($action['fields'] as $key => $def) {
      if (isset($def['#type'])) {
        if ($interactive && ($def['#type'] != 'hidden')) {
          $value = drush_prompt($def['#title'], $def['#default_value'], TRUE, ($def['#type'] == 'password'));
        }
        else {
          $value = drush_get_option($key, $def['#default_value']);
        }
        switch ($def['#type']) {
          case 'hidden':
            continue 2;

          case 'file':
            if (file_exists($value)) {
              $_FILES['files']['tmp_name'][$key] = $value;
              $_FILES['files']['name'][$key] = basename($value);
              $_FILES['files']['type'][$key] = filetype($value);
              $_FILES['files']['size'][$key] = filesize($value);
            }
            break;

          default:
            $_POST[$key] = $value;
            break;
        }
      }
    }
  }

  if (!empty($sids)) {
    if (_drd_drush_ask_server_domain('server', $sids, $action['label'])) {
      drd_action('server', $sids, $action_name, FALSE);
    }
  }
  if (!empty($dids)) {
    if (_drd_drush_ask_server_domain('domain', $dids, $action['label'])) {
      drd_action('domain', $dids, $action_name, FALSE);
    }
  }

  drush_queue_run('drd');
}

/**
 * @param string $action_name
 */
function _drush_drd_action_help($action_name) {
  $action = drd_actions($action_name);
  drush_print($action['label']);
  if (!empty($action['description'])) {
    drush_print($action['description']);
  }
  drush_print(t('Usage: drush drd-action !name [options]', array('!name' => $action_name)));
  drush_print('Options:');
  if (!empty($action['fields'])) {
    foreach ($action['fields'] as $key => $def) {
      if (!isset($def['#type'])) {
        continue;
      }
      switch ($def['#type']) {
        case 'hidden':
          continue 2;

        case 'file':
          $value = 'A fully qualified filename';
          break;

        case 'checkbox':
          $value = '0 or 1';
          break;

        case 'radios':
          $value = implode(', ', array_keys($def['#options']));
          break;

        default:
          $value = 'A text string';
          break;
      }
      $title = (!empty($def['#title']) ? $def['#title'] : '') . ': ';
      drush_print('--' . $key . '  ' . $title . $value, 4);
      if (!empty($def['#description'])) {
        drush_print($def['#description'], 10);
      }
    }
  }
  else {
    drush_print('none', 4);
  }
}

/**
 * @param string $action_name
 * @param string $mode
 * @param int $sid
 */
function drd_drush_action_update_actions($action_name, $mode, $sid) {
  $server = drd_server_load($sid);
  if (!$server->settings['ssh2']['drush']) {
    return;
  }
  include_once 'drd.ssh2.inc';
  $connection = drd_ssh2_connect($server);
  if ($connection) {
    $drush = drd_ssh2_command($server, $connection, 'drush help --format=json', TRUE);
    $drush_fields = array(
      'arguments' => array(
        'prefix' => '',
        'key_value' => FALSE,
      ),
      'options' => array(
        'prefix' => '--',
        'key_value' => TRUE,
      ),
    );
    $drush_actions = array();
    foreach ($drush as $actions) {
      foreach ($actions['commands'] as $action) {
        if ($action['hidden']) {
          continue;
        }
        $key = 'drd-drush-' . str_replace('.', '-', $action['command']);
        $fields = array();
        foreach ($drush_fields as $type => $def) {
          if (!empty($action[$type])) {
            foreach ($action[$type] as $field_name => $field_def) {
              if (strpos($field_name, '=') > 0) {
                continue;
              }
              if (is_array($field_def)) {
                $description = $field_def['description'];
                $default_value = isset($field_def['default']) ? $field_def['default'] : '';
              }
              else {
                $description = $field_def;
                $default_value = '';
              }
              $fields[$key . '-' . $field_name] = array(
                '#type' => 'textfield',
                '#title' => $def['prefix'] . $field_name,
                '#description' => $description,
                '#default_value' => $default_value,
              );
            }
          }
        }
        if ($action['allow-additional-options']) {
          $fields[$key . '-drd-extra-options'] = array(
            '#type' => 'textfield',
            '#title' => t('Extra options'),
          );
        }
        $pos = strpos($action['description'], '. ');
        if (($pos > 0) && strlen($action['description']) > $pos) {
          $label = trim(drupal_substr($action['description'], 0, $pos + 1));
          $fields['info'] = array(
            '#markup' => trim(drupal_substr($action['description'], $pos + 1)),
            '#weight' => -10,
          );
        }
        else {
          $label = $action['description'];
        }
        $drush_actions[$key] = array(
          'category' => '|Drush|' . $actions['title'],
          'label' => $action['command'] . ': ' . $label,
          'mode' => ($action['bootstrap'] < 2) ? 'server' : 'domain',
          'drush' => TRUE,
          'fields' => $fields,
        );
      }
    }
    $actions = drupal_array_merge_deep(variable_get('drd_actions', array()), $drush_actions);
    variable_set('drd_actions', $actions);
  }

}

/**
 * @param string $action_name_long
 * @param string $mode
 * @param int $id
 */
function drd_drush_action_remote($action_name_long, $mode, $id) {
  $args = func_get_args();
  $action_name_long = array_shift($args);
  $mode = array_shift($args);
  $id = array_shift($args);
  $command = drupal_substr($action_name_long, 10);

  $action = drd_actions($action_name_long);

  if ($mode != $action['mode']) {
    return;
  }

  $server = drd_server_load($id, $mode);
  $status = drd_action_remote('drd.status', $mode, $id);
  if (empty($status['drupal_root'])) {
    $status['drupal_root'] = '/var/www';
  }

  if ($mode == 'domain') {
    $domain = drd_domain_load($id);
    $cmd = 'drush -y --uri=' . $domain->url . ' --root=' . $status['drupal_root'] . ' ' . $command;
  }
  else {
    $cmd = 'drush -y --root=' . $status['drupal_root'] . ' ' . $command;
  }
  foreach ($action['fields'] as $field) {
    $arg = array_shift($args);
    if (!empty($arg)) {
      if (strpos($field['#title'], '--') === 0) {
        $cmd .= ' ' . $field['#title'] . '=' . $arg;
      }
      else {
        $cmd .= ' ' . $arg;
      }
    }
  }

  if (!$server->settings['ssh2']['drush']) {
    return;
  }
  include_once 'drd.ssh2.inc';
  $connection = drd_ssh2_connect($server);
  if ($connection) {
    drd_log($mode, $id, $cmd);
    $result = drd_ssh2_command($server, $connection, $cmd);
    if ($result) {
      $message = 'drush ' . $command . ': <pre>' . $result . '</pre>';
      if (isset($domain)) {
        drd_set_message($domain, $message);
      }
      else {
        drd_set_message($server, $message);
      }
      drd_store_message($mode, $id, 'status', $message);
    }
  }
}

function drush_drd_core_add() {
  _drd_drush_core_prepare(FALSE);
}

function drush_drd_core_update() {
  _drd_drush_core_prepare(TRUE);
}

function _drd_drush_core_prepare($update) {
  $args = drush_get_arguments();
  if (count($args) > 1) {
    $url = $args[1];
  }
  else {
    drush_set_error(t('URL missing. Cancelled.'));
    return;
  }

  include_once 'drd.server.inc';
  $sid = 0;
  if ($update) {
    $server = drd_server_load_by_url($url);
    if ($server) {
      $sid = $server->sid;
    }
  }
  $server = _drd_server_defaults($sid, $url);

  list($auth_enabled, $auth_username, $auth_password) = _drd_http_header_prepare($server['http_headers']);
  $values = array(
    'name' => drush_get_option('name', $server['name']),
    'url' => $url,
    'tags' => drush_get_option('tags', _drd_tags_load('server', $server['sid'])),
    'usessl' => drush_get_option('ssl', $server['usessl']),
    'status' => $server['status'],
    'remoteinstall' => drush_get_option('support-remote-installation', $server['remoteinstall']),
    'aes_implementation' => drush_get_option('aes-implementation', $server['aes_impl']),
    'aes_cipher' => drush_get_option('aes-cipher', $server['aes_cipher']),
    'aes_iv' => drush_get_option('aes-cipher', $server['aes_iv']),
    'ssh2_enable' => drush_get_option('ssh', $server['settings']['ssh2']['enable']),
    'ssh2_host' => drush_get_option('ssh-host', $server['settings']['ssh2']['host']),
    'ssh2_port' => drush_get_option('ssh-port', $server['settings']['ssh2']['port']),
    'ssh2_auth_mode' => drush_get_option('ssh-auth-mode', $server['settings']['ssh2']['auth']['mode']),
    'ssh2_auth_username' => drush_get_option('ssh-auth-username', $server['settings']['ssh2']['auth']['username']),
    'ssh2_auth_password' => drush_get_option('ssh-auth-password', $server['settings']['ssh2']['auth']['password']),
    'ssh2_auth_file_public_key' => drush_get_option('ssh-auth-public-key', $server['settings']['ssh2']['auth']['file_public_key']),
    'ssh2_auth_file_private_key' => drush_get_option('ssh-auth-private-key', $server['settings']['ssh2']['auth']['file_private_key']),
    'ssh2_auth_key_secret' => drush_get_option('ssh-auth-public-key-passphrase', $server['settings']['ssh2']['auth']['key_secret']),
    'drush_enable' => drush_get_option('use-drush', $server['settings']['ssh2']['drush']),
    'http_header_auth_enabled' => drush_get_option('http-header-auth', $auth_enabled),
    'http_header_auth_username' => drush_get_option('http-header-auth-username', $auth_username),
    'http_header_auth_password' => drush_get_option('http-header-auth-password', $auth_password),
  );

  drush_log('Validating core values.');
  if (!_drd_server_validate($values, 'drush')) {
    // Parameters are not valid. Output has already been generated.
    drush_log('Core values not valid', 'error');
    return;
  }

  drush_log('Saving core values.');
  $success = _drd_server_save($values, $server, TRUE);
  $ssh_connection = FALSE;
  if (!$success) {
    // Remote installation has failed, let's try through drush.
    drush_log('Remote installation not possible, trying other means.');

    // Let's see if we can use local drush to configure remote core.
    drush_log('Trying option 1: installing drd_server and aes with drush.');

    $alias = _drd_drush_get_alias($url);
    if ($alias) {
      drush_invoke_process($alias, 'pm-enable', array('drd_server'));
      $success = TRUE;
    }

    if (!$success) {
      drush_log('Trying option 2: using SSH');
      $server = drd_server_load_by_url($url);
      $alias = _drd_drush_get_remote_alias($server);
      if ($alias) {
        $ssh_connection = drd_ssh2_connect($server);
        drd_ssh2_command($server, $ssh_connection, 'drush ' . $alias . ' pm-enable drd_server');
        $success = TRUE;
      }
    }

  }

  if (!$success) {
    drupal_set_message('Remote installation and configuration not possible. Please follow instructions in your browser when going to DRD.', 'error');
    return;
  }

  drush_log('Determine outgoing IP address.');
  $ip = drush_get_option('ip', FALSE);
  if (!$ip) {
    $appspot = drupal_http_request('http://ip.appspot.com');
    if ($appspot->code == 200) {
      $ip = trim($appspot->data);
    }
  }
  if (!$ip) {
    drupal_set_message('Can not determine outgoing IP address, giving up!', 'error');
    return;
  }

  $alias = $ssh_connection ? _drd_drush_get_remote_alias($url) : _drd_drush_get_alias($url);
  if ($alias) {
    drush_log('Reading allowed referer.');
    if ($ssh_connection) {
      $result = drd_ssh2_command($server, $ssh_connection, 'drush ' . $alias . ' variable-get drd_allowed_referer');
      $refs = drupal_substr(trim($result), 22, -1);
    }
    else {
      $result = drush_invoke_process($alias, 'variable-get', array('drd_allowed_referer'));
      if (!isset($result['object']['drd_allowed_referer'])) {
        drupal_set_message('Can not push IP address to the remote core using drush.', 'error');
        return;
      }
      $refs = $result['object']['drd_allowed_referer'];
    }
    $refs = str_replace(',', ' ', $refs);
    $refs = str_replace(';', ' ', $refs);
    $referer = explode(' ', $refs);
    if (!in_array($ip, $referer)) {
      drush_log('Adding outgoing IP address to allowed referer.');
      $referer[] = $ip;
      $refs = implode(',', $referer);
      if ($ssh_connection) {
        drd_ssh2_command($server, $ssh_connection, 'drush ' . $alias . ' variable-set drd_allowed_referer ' . $refs);
      }
      else {
        drush_invoke_process($alias, 'variable-set', array('drd_allowed_referer', $refs));
      }
    }
  }
  else {
    $success = FALSE;
  }

  if ($success) {
    drush_log('Push AES keys to remote core.');
    $server = drd_server_load_by_url($url);
    drd_update_aes($server->sid);
    drush_log('Update domain list from core.');
    drd_action('server', array($server->sid), 'drd.server.domains', FALSE);
    drush_queue_run('drd');
    $dids = drd_server_domains($server->sid);
    drush_log('Update info for all domains of core.');
    drd_action('domain', $dids, 'drd.info', FALSE);
    drush_queue_run('drd');
  }
}

function drush_drd_core_remove() {
  $args = drush_get_arguments();
  if (count($args) > 1) {
    array_shift($args);
    $sids = $args;
  }
  else {
    drush_set_option('ask', TRUE);
    $sids = drd_get_server_ids(_drd_drush_filter('server'));
    if (!_drd_drush_ask_server_domain('server', $sids, 'Remove cores')) {
      return;
    }
  }
  $names = array();
  $cores = array();
  foreach ($sids as $sid) {
    $server = drd_server_load($sid);
    if (!empty($server->sid)) {
      $names[] = $server->name;
      $cores[] = $server->sid;
    }
  }
  if (empty($cores)) {
    return;
  }
  $core_names = implode(', ', $names);
  if (!drush_confirm(t('Are you sure you want to remove the following core(s) and all their domains and historical data from DRD: !cores ? This can not be un-done!', array('!cores' => $core_names)))) {
    return;
  }
  foreach ($cores as $sid) {
    drd_action_server_delete('remove', 'server', $sid);
  }
}
