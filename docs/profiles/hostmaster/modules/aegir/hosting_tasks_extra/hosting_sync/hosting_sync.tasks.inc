<?php

/**
 * @file
 * Implements a new Aegir task.
 */

/**
 * Implements hook_hosting_tasks().
 */
function hosting_sync_hosting_tasks() {
  $tasks = array();
  $tasks['site']['sync'] = array(
    'title' => t('Sync data'),
    'description' => t('Synchronize database and files from another site and (optionally) run update.php, clear cache, and revert features. <strong>Data on the current site will be DESTROYED!</strong>'),
    'access callback' => 'hosting_sync_hosting_task_menu_access',
    'dialog' => TRUE,
  );
  return $tasks;
}

/**
 * Access Callback for Aegir Tasks.
 *
 * This function defines access to the various aegir tasks.
 *
 * @arg $node object
 *   The node object is running the task. (Site, Platform, Server)
 *
 * @arg $task string
 *   The type of the task that is running.
 *
 * @see hosting_task_menu_access()
 */
function hosting_sync_hosting_task_menu_access($node, $task) {
  // If we are passed the nid by mistake.
  if (!isset($node->nid)) {
    $node = node_load($node);
  }

  if ($task == 'sync' && user_access('create sync task') && $node->type == 'site') {
    // Determine site's profile name.
    $profile = node_load($node->profile);

    $ishostmaster = ($profile && $profile->short_name == 'hostmaster');
    if ($ishostmaster) {
      return FALSE;
    }

    // If we are deleted or in the process of deleting, we can't sync!
    if (hosting_task_outstanding($node->nid, 'delete') || ($node->site_status == HOSTING_SITE_DELETED)) {
      return FALSE;
    }

    // Otherwise, only show if the site is enabled.
    return $node->site_status == HOSTING_SITE_ENABLED;
  }

  return FALSE;
}

/**
 * Helper to get the list of sites.
 *
 * Similar to hosting_get_sites_by_status() but it calls db_rewrite_sql() so that the
 * node access rules are applied (adding support for the 'hosting_client' module).
 */
function _hosting_sync_get_sites($platform = NULL, $profile = NULL, $exclude_nid = NULL, $client = NULL, $status = HOSTING_SITE_ENABLED) {
  $sql = "SELECT n.nid FROM {node} n LEFT JOIN {hosting_site} s ON n.vid = s.vid WHERE n.type = 'site' AND s.status = :status";
  $args = array(':status' => $status);

  if (!is_null($platform)) {
    $sql .= " AND s.platform = :platform";
    $args[':platform'] = $platform;
  }

  if (!is_null($profile)) {
    $sql .= " AND s.profile = :profile";
    $args[':profile'] = $profile;
  }

  if (!is_null($exclude_nid)) {
    $sql .= " AND n.nid != :exclude_nid";
    $args[':exclude_nid'] = $exclude_nid;
  }

  if (!is_null($client)) {
    $sql .= " AND s.client = :client";
    $args[':client'] = $client;
  }

  $result = db_query($sql, $args);

  $nodes = array();
  while ($nid = $result->fetch()) {
    $nodes[$nid->nid] = node_load($nid->nid);
  }
  return $nodes;
}

/**
 * Helper to get a list of platforms that are compatible with the given site.
 *
 *  - $node is the site in question
 *  - $type is whether the $node is the 'source' or 'destination'
 */
function hosting_sync_get_compatible_platforms($node, $type = 'destination') {
  $profile_platform_instances = hosting_package_instances_load(
    array('r.type' => 'platform', 'n.nid' => $node->profile, 'h.status' => HOSTING_PLATFORM_ENABLED));

  $result = array();

  foreach ($profile_platform_instances as $profile_instance) {
    if ($type === 'source') {
      $status = hosting_package_comparison($node->nid, $profile_instance->iid);
    }
    else {
      $status = hosting_package_comparison($profile_instance->iid, $node->nid);
    }

    // If there are no errors, this one is compatible!
    if ($status['error'] == 0) {
      $result[] = array(
        'platform' => $profile_instance->rid,
        'profile' => $profile_instance->nid,
        'profile_instance' => $profile_instance->iid,
        'status' => $status,
      );
    }
  }

  drupal_alter('hosting_sync_compatible_platforms', $result, $node, $type);

  return $result;
}

/**
 * Helper to get the sites for a particular destination.
 */
function hosting_sync_get_compatible_sites($node, $type = 'destination') {
  $result = hosting_sync_get_compatible_platforms($node, $type);

  foreach ($result as $idx => &$info) {
    $sites = _hosting_sync_get_sites($info['platform'], $info['profile'], $node->nid, $node->client);
    if (count($sites) > 0) {
      $info['sites'] = $sites;
    }
    else {
      unset($result[$idx]);
    }
  }

  drupal_alter('hosting_sync_compatible_sites', $result, $node, $type);

  return $result;
}

/**
 * Helper to create a set of radio buttons out of possible sources/destinations.
 */
function _hosting_sync_sites_elements($node, $result, $parents) {
  $elements = array();
  foreach ($result as $info) {
    if ($info['platform'] != $node->platform) {
      $status = $info['status'];
      $description = t("Upgrades: !upgrades, Warnings: !missing, Errors: !errors | <a href='!url' class='hosting-package-comparison-link'>Compare platforms</a>",
        array(
          '!upgrades' => $status['upgrade'],
          '!missing' => $status['missing'] + $status['downgrade'],
          '!errors' => $status['error'],
          '!url' => url('hosting/sync/compare/' . $node->nid . '/' . $info['profile_instance']),
        )
      );
    }
    else {
      $description = t('Current platform');
    }

    $platform_node = node_load($info['platform']);
    $elements[$info['platform']] = array(
      '#type' => 'fieldset',
      '#title' => $platform_node->title,
      '#description' => $description,
      '#attributes' => array('class' => array('hosting-sync-platform-group')),
    );

    foreach ($info['sites'] as $site) {
      $elements[$info['platform']][$site->hosting_name] = array(
        '#type' => 'radio',
        '#title' => $site->title,
        '#parents' => $parents,
        '#return_value' => '@' . $site->hosting_name,
      );
    }
  }

  return $elements;
}

/**
 * Implements hook_hosting_task_TASK_TYPE_form().
 *
 * For "Sync Content" task.
 */
function hosting_task_sync_form($node) {
  $form = array();

  drupal_add_js(drupal_get_path('module', 'hosting_sync') . '/hosting_sync.js');

  // Get a list of compatible sources for this site as the destination.
  $sites = hosting_sync_get_compatible_sites($node, 'destination');

  $form['source'] = array(
    '#type' => 'item',
    '#title' => t('Source'),
    '#required' => TRUE,
    '#description' => t('Choose the site to copy data and/or files from.'),
  );

  $parents = array('parameters', 'source');

  if (!empty($sites)) {
    $form['source'] = array_merge($form['source'], _hosting_sync_sites_elements($node, $sites, $parents));
  }

  // TODO setting or access control?
  if (1) {
    $form['source']['custom_alias'] = array(
      '#type' => 'fieldset',
      '#title' => "A custom Drush alias",
      '#attributes' => array('class' => array('hosting-sync-platform-group')),
    );
    $form['source']['custom_alias']['radio'] = array(
      '#type' => 'radio',
      '#title' => 'Any Drush alias defined on the hostmaster server.',
      '#parents' => $parents,
      '#return_value' => 'CUSTOM',
    );
    $form['source']['custom_alias']['text'] = array(
      '#type' => 'textfield',
      '#title' => "",
      '#parents' => $parents,
      '#states' => array(
        // Only visible this field when backups is enabled.
        'visible' => array(
          ':input[name="parameters[source]"]' => array('value' => 'CUSTOM'),
        ),
        'enabled' => array(
          ':input[name="parameters[source]"]' => array('value' => 'CUSTOM'),
        ),
      ),
    );
  }
  elseif (empty($sites)) {
    $elements['none'] = array(
      '#type' => 'item',
      '#markup' => t('No sources are currently available.'),
    );
  }

  $form['destination'] = array(
    '#type' => 'value',
    '#value' => '@' . $node->hosting_name,
  );

  // What to sync?
  $form['database'] = array(
    '#title' => t('Copy database from source.'),
    '#type' => 'checkbox',
    '#default_value' => 1,
    '#prefix' => '<fieldset><legend>' . t('What to sync?') . '</legend>',
  );
  $form['files'] = array(
    '#title' => t('Copy files from source.'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );
  $form['modules'] = array(
    '#title' => t('Copy modules from source.'),
    '#type' => 'checkbox',
    '#default_value' => 0,
  );
  $form['themes'] = array(
    '#title' => t('Copy themes from source.'),
    '#type' => 'checkbox',
    '#default_value' => 0,
  );
  $form['libraries'] = array(
    '#title' => t('Copy libraries from source.'),
    '#type' => 'checkbox',
    '#default_value' => 0,
    '#suffix' => '</fieldset>',
  );

  // Options.
  $form['registry-rebuild'] = array(
    '#title' => t('Rebuild registry'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );
  $form['updatedb'] = array(
    '#title' => t('Run update.php'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );
  if (_hosting_package_site_has_module($node, 'features')) {
    $form['features-revert-all'] = array(
      '#title' => t('Revert all features'),
      '#type' => 'checkbox',
      '#default_value' => TRUE,
    );
  }
  $form['update-uri'] = array(
    '#title' => t('Update URI in database content'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );
  $form['cache-clear'] = array(
    '#title' => t('Clear cache'),
    '#type' => 'checkbox',
    '#default_value' => 1,
  );
  $form['backup'] = array(
    '#title' => t('Create backup'),
    '#type' => 'checkbox',
    '#description' => t('If this option is enabled, a backup of the destination will be created before any changes are made. <em>This is will allow you to restore later in case this was a mistake!</em>'),
    '#default_value' => 1,
  );

  return $form;
}

/**
 * Validation for hosting_task_hosting_sync_form().
 *
 * @see hosting_task_confirm_form()
 */
function hosting_task_sync_form_validate(&$form, &$form_state) {

  // There has to be a sync source.
  if (empty($form_state['values']['parameters']['source'])) {
    form_set_error('destination', t('No sources are currently available.'));
    return;
  }

  // Can't sync to self.
  if ($form_state['values']['parameters']['source'] == $form_state['values']['parameters']['destination']) {
    form_set_error('destination', t('The source cannot be the same as the destination.'));
  }
}
