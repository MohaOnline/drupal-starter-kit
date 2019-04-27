<?php

/**
 * @file
 * Administration functionality for the PoolParty Semantic Connector.
 */

/**
 * Show an overview of all the available semantic Drupal modules.
 *
 * @return array
 *   The renderable array of the module overview.
 */
function semantic_connector_overview() {
  $output = array();

  // Create a list of all "Semantic Drupal" modules.
  $output['modules_title'] = array(
    '#type' => 'markup',
    '#markup' => '<h3 class="semantic-connector-table-title">' . t('Semantic Drupal modules') . '</h3>',
  );

  $installed_modules = module_list();
  $semantic_modules = array(
    'sonr_webmining' => array(
      'title' => 'PoolParty GraphSearch',
      'configuration_path' => 'admin/config/semantic-drupal/sonr-webmining',
    ),
    'pp_taxonomy_manager' => array(
      'title' => 'PoolParty Taxonomy Manager',
      'configuration_path' => 'admin/config/semantic-drupal/pp-taxonomy-manager',
    ),
    'powertagging' => array(
      'title' => 'PowerTagging',
      'configuration_path' => 'admin/config/semantic-drupal/powertagging',
    ),
    'smart_glossary' => array(
      'title' => 'Smart Glossary',
      'configuration_path' => 'admin/config/semantic-drupal/smart-glossary',
    ),
  );

  $module_rows = array();
  $installed_semantic_modules = array();
  foreach ($semantic_modules as $module_key => $module_info) {
    $installed = in_array($module_key, $installed_modules);
    $module_rows[] = array(
      $module_info['title'],
      ($installed ? 'installed' : l(t('Download'), 'http://www.drupal.org/project/' . $module_key)),
      ($installed ? l(t('Configure'), $module_info['configuration_path']) : ''),
    );

    // Create a list of connections used by module and connection-id.
    if ($installed) {
      $installed_semantic_modules[] = $module_key;
    }
  }

  $output['modules'] = array(
    '#theme' => 'table',
    '#header' => array(t('Module'), t('Installed'), t('Configuration')),
    '#rows' => $module_rows,
    '#caption' => NULL,
    '#colgroups' => array(),
    '#sticky' => FALSE,
    '#empty' => '',
  );

  // Find out what connections are used by the installed semantic modules.
  $connections_used = semantic_connector_check_connection_usage($installed_semantic_modules);
  $pp_server_connections = SemanticConnector::getConnectionsByType('pp_server');

  // Build an array of existing connections using a SPARQL endpoint.
  $sparql_endpoint_connections = SemanticConnector::getConnectionsByType('sparql_endpoint');
  $sparql_endpoint_connections_assoc = array();
  /** @var \SemanticConnectorSparqlEndpointConnection $sparql_endpoint_connection */
  foreach ($sparql_endpoint_connections as $sparql_endpoint_connection) {
    $sparql_endpoint_connections_assoc[$sparql_endpoint_connection->getUrl()] = $sparql_endpoint_connection;
  }
  $sparql_endpoint_connections = $sparql_endpoint_connections_assoc;
  unset($sparql_endpoint_connections_assoc);

  // Get all SeeAlso widgets if available.
  $sonr_webmining_similar_configs = array();
  if (module_exists('sonr_webmining_similar')) {
    $similar_configs = sonr_webmining_similar_config_load_multiple();
    /** @var \SonrWebminingSimilarConfig $similar_config */
    foreach ($similar_configs as $similar_config) {
      $key = $similar_config->getConnectionId() . '|' . $similar_config->getSearchSpaceId();
      $sonr_webmining_similar_configs[$key][] = $similar_config;
    }
  }

  // List the PoolParty server connections.
  /** @var \SemanticConnectorPPServerConnection $pp_server_connection */
  foreach ($pp_server_connections as $pp_server_connection) {
    $server_id = $pp_server_connection->getId();
    $server_config = $pp_server_connection->getConfig();
    $server_ppx_projects = $pp_server_connection->getApi('PPX')->getProjects();

    $output['server_anchor_' . $server_id] = array(
      '#markup' => '<a id="pp-server-' . $server_id . '"></a>',
    );
    $server_title = '<h3 class="semantic-connector-table-title">';
    // Check the PoolParty server version if required.
    if (variable_get('semantic_connector_version_checking', TRUE)) {
      $api_version_info = $pp_server_connection->getVersionInfo('PPX');
      if (version_compare($api_version_info['installed_version'], $api_version_info['latest_version'], '<')) {
        $server_title .= '<div class="messages warning">' . t('The connected PoolParty server is not up to date. You are currently running version %installedversion, upgrade to version %latestversion to enjoy the new features.', array('%installedversion' => $api_version_info['installed_version'], '%latestversion' => $api_version_info['latest_version'])) . '</div>';
      }
    }
    $server_title .= '<div class="semantic-connector-led" data-server-id="' . $server_id . '" data-server-type="pp-server" title="' . t('Checking service') . '"></div>';
    $server_title .= t('PoolParty server "%pptitle"', array('%pptitle' => $pp_server_connection->getTitle()));
    $server_title .= '<span class="semantic-connector-url">' . l($pp_server_connection->getUrl(), $pp_server_connection->getUrl() . '/PoolParty', array('attributes' => array('target' => '_blank'))) . '</span></h3>';
    $output['server_title_' . $server_id] = array(
      '#markup' => $server_title,
    );
    $output['server_buttons_' . $server_id] =  array(
      '#markup' => _semantic_connector_theme_connection_buttons($pp_server_connection, !isset($connections_used[$server_id])),
    );

    $project_rows = array();
    if (isset($server_config['projects']) && !empty($server_config['projects'])) {
      foreach ($server_config['projects'] as $project) {
        $project_row = array($project->title);

        foreach ($installed_semantic_modules as $semantic_module_key) {
          switch ($semantic_module_key) {

            // PoolParty Taxonomy Manager cell content.
            case 'pp_taxonomy_manager':
              $project_sync_content = '';
              if (isset($connections_used[$server_id]) && isset($connections_used[$server_id]['pp_taxonomy_manager'])) {
                foreach ($connections_used[$server_id]['pp_taxonomy_manager'] as $pp_taxonomy_manager_use) {
                  if ($pp_taxonomy_manager_use['root_level'] == 'project') {
                    if (in_array($project->id, $pp_taxonomy_manager_use['project_ids'])) {
                      $url = $semantic_modules['pp_taxonomy_manager']['configuration_path'] . '/' . $pp_taxonomy_manager_use['id'];
                      $project_sync_content = l($pp_taxonomy_manager_use['title'], $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</li>';
                      break;
                    }
                  }
                  else {
                    if ($pp_taxonomy_manager_use['project_id'] == $project->id) {
                      $url = $semantic_modules['pp_taxonomy_manager']['configuration_path'] . '/' . $pp_taxonomy_manager_use['id'];
                      $project_sync_content = l($pp_taxonomy_manager_use['title'], $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</li>';
                      break;
                    }
                  }
                }
              }
              if (empty($project_sync_content)) {
                $url = $semantic_modules['pp_taxonomy_manager']['configuration_path'] . '/add/' . $server_id . '/' . $project->id;
                $project_sync_content = l(t('Add new configuration'), $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector')));
              }
              $project_row[] = $project_sync_content;
            break;

            // PowerTagging cell content.
            case 'powertagging':
              // Check if the project is valid for PPX communication (extraction
              // model was already built).
              $project_is_valid = FALSE;
              foreach ($server_ppx_projects as $server_ppx_project) {
                if ($server_ppx_project->uuid == $project->id) {
                  $project_is_valid = TRUE;
                  break;
                }
              }

              // Valid PPX project.
              if ($project_is_valid) {
                $project_powertagging_content = '';
                $powertagging_project_uses = array();
                if (isset($connections_used[$server_id]) && isset($connections_used[$server_id]['powertagging'])) {
                  foreach ($connections_used[$server_id]['powertagging'] as $powertagging_use) {
                    // This PoolParty GraphSearch configuration uses the
                    // PoolParty GraphSearch server using this project on the
                    // current PoolParty server.
                    if ($powertagging_use['project_id'] == $project->id) {
                      $url = $semantic_modules['powertagging']['configuration_path'] . '/' . $powertagging_use['id'];
                      $powertagging_project_uses[] = '<li>' . l($powertagging_use['title'], $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</li>';
                    }
                  }
                }
                if (!empty($powertagging_project_uses)) {
                  $project_powertagging_content .= '<ul>' . implode('', $powertagging_project_uses) . '</ul>';
                }
                $url = $semantic_modules['powertagging']['configuration_path'] . '/add/' . $server_id . '/' . $project->id;
                $project_powertagging_content .= '<div class="add-configuration">' . l(t('Add new configuration'), $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</div>';
              }
              // Project is not available for PowerTagging.
              else {
                $project_powertagging_content = '<div class="semantic-connector-italic">' . t('not supported') . '</div>';
              }
              $project_row[] = $project_powertagging_content;
              break;

            // PoolParty GraphSearch cell content.
            case 'sonr_webmining':
              $project_sonr_content = '';
              // A GraphSearch server is available for this project on the current PP server.
              if (isset($server_config['sonr_configuration']) && !empty($server_config['sonr_configuration']) && isset($server_config['sonr_configuration']['projects'][$project->id])) {
                $sonr_webmining_project_uses = array();
                if (isset($connections_used[$server_id]) && isset($connections_used[$server_id]['sonr_webmining'])) {
                  foreach ($connections_used[$server_id]['sonr_webmining'] as $sonr_webmining_use) {
                    // This PoolParty GraphSearch configuration uses the
                    // PoolParty GraphSearch server using this project on the
                    // current PP server.
                    if (isset($server_config['sonr_configuration']['projects'][$project->id]['search_spaces'][$sonr_webmining_use['project_id']])) {
                      $url = $semantic_modules['sonr_webmining']['configuration_path'] . '/' . $sonr_webmining_use['id'];
                      $sonr_webmining_project_uses[] = '<li>' . l($sonr_webmining_use['title'], $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</li>';
                    }
                  }
                }
                if (!empty($sonr_webmining_project_uses)) {
                  $project_sonr_content .= '<ul>' . implode('', $sonr_webmining_project_uses) . '</ul>';
                }
                $url = $semantic_modules['sonr_webmining']['configuration_path'] . '/add/' . $server_id . '/' . $project->id;
                $project_sonr_content .= '<div class="add-configuration">' . l(t('Add new configuration'), $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</div>';

                if (module_exists('sonr_webmining_similar')) {
                  $project_sonr_content .= '<hr>';
                  $similar_project_uses = array();

                  // Get all GraphSearch SeeAlso Engine configurations for
                  // search spaces available for the current project.
                  $project_conf = $server_config['sonr_configuration']['projects'][$project->id];
                  if (isset($project_conf['search_spaces'])) {
                    foreach ($project_conf['search_spaces'] as $search_space) {
                      $key = $server_id . '|' . $search_space['id'];
                      if (isset($sonr_webmining_similar_configs[$key])) {
                        foreach ($sonr_webmining_similar_configs[$key] as $similar_config) {
                          $url = 'admin/config/semantic-drupal/sonr-webmining/sonr-webmining-similar/' . $similar_config->getId();
                          $similar_project_uses[] = '<li>' . l($similar_config->getTitle(), $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</li>';
                        }
                      }
                    }
                  }

                  if (!empty($similar_project_uses)) {
                    $project_sonr_content .= '<ul>' . implode('', $similar_project_uses) . '</ul>';
                  }
                  $url = 'admin/config/semantic-drupal/sonr-webmining/sonr-webmining-similar/add/' . $server_id . '/' . $project->id;
                  $project_sonr_content .= '<div class="add-configuration">' . l(t('Add new SeeAlso widget'), $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</div>';
                }
              }
              // There is no PoolParty GraphSearch server available for this project on the PP server.
              else {
                $project_sonr_content .= '<div class="semantic-connector-italic">' . t('not supported') . '</div>';
              }
              $project_row[] = $project_sonr_content;
              break;

            // Smart Glossary cell content.
            case 'smart_glossary':
              $project_sparql_content = '';
              if (isset($project->sparql_endpoint_url) && isset($sparql_endpoint_connections[$project->sparql_endpoint_url])) {
                $sparql_endpoint_connection = $sparql_endpoint_connections[$project->sparql_endpoint_url];
                $smart_glossary_project_uses = array();
                if (isset($connections_used[$sparql_endpoint_connection->getId()]) && isset($connections_used[$sparql_endpoint_connection->getId()]['smart_glossary'])) {
                  foreach ($connections_used[$sparql_endpoint_connection->getId()]['smart_glossary'] as $smart_glossary_use) {
                    $url = $semantic_modules['smart_glossary']['configuration_path'] . '/' . $smart_glossary_use['id'];
                    $smart_glossary_project_uses[] = '<li>' . l($smart_glossary_use['title'], $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</li>';
                  }
                }
                if (!empty($smart_glossary_project_uses)) {
                  $project_sparql_content .= '<ul>' . implode('', $smart_glossary_project_uses) . '</ul>';
                }
                $url = $semantic_modules['smart_glossary']['configuration_path'] . '/add/' . $sparql_endpoint_connection->getId();
                $project_sparql_content .= '<div class="add-configuration">' . l(t('Add new configuration'), $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</div>';
                unset($sparql_endpoint_connections[$project->sparql_endpoint_url]);
              }
              else {
                $project_sparql_content .= '<div class="semantic-connector-italic">' . t('not supported') . ' (' . t('refresh PoolParty server details') . ')</div>';
              }
              $project_row[] = $project_sparql_content;
              break;
          }
        }

        // Add the collected data for the project as a row.
        $project_rows[] = $project_row;
      }
    }

    $pp_table_headers = array(t('Projects'));
    foreach ($installed_semantic_modules as $semantic_module_key) {
      $pp_table_headers[] = $semantic_modules[$semantic_module_key]['title'];
    }
    $output['server_projects_' . $server_id] = array(
      '#theme' => 'table',
      '#header' => $pp_table_headers,
      '#rows' => $project_rows,
      '#sticky' => TRUE,
      '#empty' => t('This PoolParty server has no projects available for the configured user.'),
      '#attributes' => array('class' => array('pp-server-projects-table', 'semantic-connector-tablesorter')),
    );
  }

  // Add all the custom SPARQL endpoints.
  if (!empty($sparql_endpoint_connections)) {
    $output['sparql_endpoints_title'] = array(
      '#markup' => '<h3 class="semantic-connector-table-title">' . t('Custom SPARQL endpoints') . '</h3>',
    );
    
    $sparql_endpoint_header = array();
    $sparql_endpoint_rows = array();

    /** @var \SemanticConnectorSparqlEndpointConnection $sparql_endpoint_connection */
    foreach ($sparql_endpoint_connections as $sparql_endpoint_connection) {
      $sparql_connection_use_content = '';
      $sparql_endpoint_config = $sparql_endpoint_connection->getConfig();
      if ($sparql_endpoint_config['pp_server_id'] == 0) {
        $title = '<div class="semantic-connector-led" data-server-id="' . $sparql_endpoint_connection->getId() . '" data-server-type="sparql-endpoint" title="' . t('Checking service') . '"></div>';
        $title .= l($sparql_endpoint_connection->getTitle(), $sparql_endpoint_connection->getUrl(), array('attributes' => array('target' => array('_blank'))));

        if (module_exists('smart_glossary')) {
          $smart_glossary_project_uses = array();
          if (isset($connections_used[$sparql_endpoint_connection->getId()]) && isset($connections_used[$sparql_endpoint_connection->getId()]['smart_glossary'])) {
            foreach ($connections_used[$sparql_endpoint_connection->getId()]['smart_glossary'] as $smart_glossary_use) {
              $smart_glossary_project_uses[] = '<li>' . l($smart_glossary_use['title'], $semantic_modules['smart_glossary']['configuration_path'] . '/' . $smart_glossary_use['id']) . '</li>';
            }
          }
          if (!empty($smart_glossary_project_uses)) {
            $sparql_connection_use_content .= '<ul>' . implode('', $smart_glossary_project_uses) . '</ul>';
          }
          $url = $semantic_modules['smart_glossary']['configuration_path'] . '/add/' . $sparql_endpoint_connection->getId();
          $sparql_connection_use_content .= '<div class="add-configuration">' . l(t('Add new configuration'), $url, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector'))) . '</div>';
          $sparql_endpoint_header = array(t('URL'), t('Smart Glossary'), t('Operations'));
          $sparql_endpoint_rows[] = array(
            $title,
            $sparql_connection_use_content,
            _semantic_connector_theme_connection_buttons($sparql_endpoint_connection, empty($uses)),
          );
        }
        else {
          $sparql_endpoint_header = array(t('URL'), t('Operations'));
          $sparql_endpoint_rows[] = array(
            $title,
            _semantic_connector_theme_connection_buttons($sparql_endpoint_connection, empty($uses)),
          );
        }
      }
    }

    $output['sparql_endpoints'] = array(
      '#theme' => 'table',
      '#header' => $sparql_endpoint_header,
      '#rows' => $sparql_endpoint_rows,
      '#empty' => t('There are no custom SPARQL endpoint connections'),
      '#attributes' => array(
        'id' => 'sparql-endpoints-table',
        'class' => array('semantic-connector-tablesorter'),
      ),
    );
  }

  $output['#attached'] = array(
    'js' => array(drupal_get_path('module', 'semantic_connector') . '/js/semantic_connector.admin.js'),
    'css' => array(drupal_get_path('module', 'semantic_connector') . '/css/semantic_connector.admin.css'),
  );

  // Add the tablesorter library if available.
  $tablesorter_path = 'sites/all/libraries/tablesorter';
  if (module_exists('libraries')) {
    $tablesorter_path = libraries_get_path('tablesorter');
  }
  if (file_exists($tablesorter_path . '/jquery.tablesorter.min.js')) {
    $output['#attached']['js'][] = libraries_get_path('tablesorter') . '/jquery.tablesorter.min.js';
  }

  return $output;
}

/**
 * Theme buttons to edit or delete a Semantic Connector connection.
 *
 * @param object $connection
 *   The Semantic Connector connection to theme the buttons for.
 * @param bool $can_be_deleted
 *   Whether a delete-button should be added or not.
 *
 * @return string
 *   The rendered HTML.
 */
function _semantic_connector_theme_connection_buttons($connection, $can_be_deleted = FALSE) {
  $type = str_replace('_', '-', $connection->getType());
  $output = '<div class="semantic-connector-connection-buttons">';

  // Edit-button.
  $output .= l(t('Edit'), 'admin/config/semantic-drupal/semantic-connector/connections/' . $type . '/' . $connection->getId(), array('attributes' => array('class' => array('semantic-connector-connection-buttons-edit'))));

  // Delete button.
  if ($can_be_deleted) {
    $output .= '|' . l(t('Delete'), 'admin/config/semantic-drupal/semantic-connector/connections/' . $type . '/' . $connection->getId() . '/delete', array('attributes' => array('class' => array('semantic-connector-connection-buttons-delete'))));
  }

  // Refresh projects button.
  if ($connection->getType() == 'pp_server') {
    $output .= '|' . l(t('Refresh server details'), 'admin/config/semantic-drupal/semantic-connector/connections/' . $type . '/' . $connection->getId() . '/refresh', array('attributes' => array('class' => array('semantic-connector-connection-buttons-refresh'))));
  }

  $server_config = $connection->getConfig();
  if (isset($server_config['version'])) {
    $output .= '|<span class="semantic-connector-connection-version">Version: ' . $server_config['version'] . '</span>';
  }

  // Get license information.
  if ($connection->getType() == 'pp_server') {
    $license_information = $connection->getApi('PPT')->getLicense();
    $license_classes = _semantic_connector_check_poolparty_licenses($connection, $license_information, TRUE);
    $output .= '|<span class="semantic-connector-connection-license ' . implode(' ', $license_classes) . '">License valid until: ' . (isset($license_information['expiryDateInMillis']) ? date('j. M Y', $license_information['expiryDateInMillis'] / 1000) : '-') . '</span>';
  }

  $output .= '</div>';
  return $output;
}

/**
 * Ajax callback function for checking if a new PoolParty GraphSearch server is
 * available.
 */
function semantic_connector_new_available($form, $form_state) {
  $available = '<div id="health_info" class="available"><div class="semantic-connector-led led-green" title="Service available"></div>' . t('The server is available.') . '</div>';
  $not_available = '<div id="health_info" class="not-available"><div class="semantic-connector-led led-red" title="Service NOT available"></div>' . t('The server is not available or the credentials are incorrect.') . '</div>';

  $connection_types = array(
    'pp_server',
    'sparql_endpoint',
  );
  $connection_type = isset($form_state['values']['type']) ? $form_state['values']['type'] : 'unknown';
  if (in_array($connection_type, $connection_types) && isset($form_state['values']['url']) && valid_url($form_state['values']['url'], TRUE)) {
    // Create a new connection (without saving) with the current form data.
    $connection = SemanticConnector::getConnection($connection_type);
    $connection->setUrl($form_state['values']['url']);
    $connection->setCredentials(array(
      'username' => $form_state['values']['name'],
      'password' => $form_state['values']['pass'],
    ));

    $availability = $connection->available();
    return $availability ? $available : $not_available;
  }

  return $not_available;
}

/**
 * The form to add a new Semantic Connector connection.
 */
function semantic_connector_connection_add_form($form, &$form_state, $connection_type) {
  $connection_type = str_replace('-', '_', $connection_type);
  drupal_set_title($connection_type == 'pp_server' ? t('Add a PoolParty server connection') : t('Add a SPARQL endpoint connection'));

  $form['server_settings']['type'] = array(
    '#type' => 'hidden',
    '#value' => $connection_type,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Server title'),
    '#description' => t('A short title for the connection.'),
    '#size' => 35,
    '#maxlength' => 60,
    '#required' => TRUE,
  );

  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => $connection_type == 'pp_server' ? t('The URL, where the PoolParty server runs.!brExample: If your PoolParty instance is available at "https://my-poolparty-server.com/PoolParty/", please use "https://my-poolparty-server.com" here.', array('!br' => '<br />')) : t('The URL, where the SPARQL endpoint is available at.'),
    '#size' => 35,
    '#maxlength' => 255,
    '#required' => TRUE,
  );

  $form['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credentials'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['credentials']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Name of a user for the credentials.'),
    '#size' => 35,
    '#maxlength' => 60,
  );
  $form['credentials']['pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('Password of a user for the credentials.'),
    '#size' => 35,
    '#maxlength' => 128,
  );

  $form['health_check'] = array(
    '#type' => 'button',
    '#value' => t('Health check'),
    '#ajax' => array(
      'callback' => 'semantic_connector_new_available',
      'wrapper' => 'health_info',
      'method' => 'replace',
      'effect' => 'slide',
    ),
  );
  $markup = '<div id="health_info">' . t('Click to check if the server is available.') . '</div>';
  $form['health_info'] = array(
    '#markup' => $markup,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/semantic-drupal/semantic-connector',
  );

  $form['#attached'] = array(
    'css' => array(drupal_get_path('module', 'semantic_connector') . '/css/semantic_connector.admin.css'),
  );

  return $form;
}

/**
 * Submit handler for the add-form of a Semantic Connector Connection.
 */
function semantic_connector_connection_add_form_submit($form, &$form_state) {
  SemanticConnector::createConnection($form_state['values']['type'], $form_state['values']['url'], $form_state['values']['title'], array(
    'username' => $form_state['values']['name'],
    'password' => $form_state['values']['pass'],
  ));

  drupal_set_message(t('Successfully added connection "%contitle".', array('%contitle' => $form_state['values']['title'])));
  drupal_goto('admin/config/semantic-drupal/semantic-connector');
}

/**
 * The form to edit an existing Semantic Connector connection.
 */
function semantic_connector_connection_edit_form($form, &$form_state, $connection_type, $connection_id) {
  $connection_type = str_replace('-', '_', $connection_type);
  $connection = SemanticConnector::getConnection($connection_type, $connection_id);

  if (!is_null($connection)) {
    $form['server_settings']['connection_id'] = array(
      '#type' => 'hidden',
      '#value' => $connection_id,
    );

    $form['server_settings']['type'] = array(
      '#type' => 'hidden',
      '#value' => $connection_type,
    );
    
    $connection_overrides = variable_get('semantic_connector_override_connections', array());
    $overridden_values = array();
    if (isset($connection_overrides[$connection_id])) {
      $overridden_values = $connection_overrides[$connection_id];
    }

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Server title'),
      '#description' => t('A short title for the connection.') . (isset($overridden_values['title']) ? ' <span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>' : ''),
      '#size' => 35,
      '#maxlength' => 60,
      '#default_value' => $connection->getTitle(),
      '#required' => TRUE,
    );

    $form['url'] = array(
      '#type' => 'textfield',
      '#title' => t('URL'),
      '#description' => ($connection_type == 'pp_server' ? t('The URL, where the PoolParty server runs.!brExample: If your PoolParty instance is available at "https://my-poolparty-server.com/PoolParty/", please use "https://my-poolparty-server.com" here.', array('!br' => '<br />')) : t('The URL, where the SPARQL endpoint is available at.')) . (isset($overridden_values['url']) ? ' <span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>' : ''),
      '#size' => 35,
      '#maxlength' => 255,
      '#default_value' => $connection->getUrl(),
      '#required' => TRUE,
    );

    $credentials = $connection->getCredentials();
    $form['credentials'] = array(
      '#type' => 'fieldset',
      '#title' => t('Credentials'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['credentials']['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#description' => t('Name of a user for the credentials.') . (isset($overridden_values['username']) ? ' <span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>' : ''),
      '#size' => 35,
      '#maxlength' => 60,
      '#default_value' => $credentials['username'],
    );
    $form['credentials']['pass'] = array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#description' => t('Password of a user for the credentials.') . (isset($overridden_values['password']) ? ' <span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>' : ''),
      '#size' => 35,
      '#maxlength' => 128,
      '#default_value' => $credentials['password'],
    );

    $form['health_check'] = array(
      '#type' => 'button',
      '#value' => t('Health check'),
      '#ajax' => array(
        'callback' => 'semantic_connector_new_available',
        'wrapper' => 'health_info',
        'method' => 'replace',
        'effect' => 'slide',
      ),
    );
    $available = '<div id="health_info" class="available"><div class="semantic-connector-led led-green" title="Service available"></div>' . t('The server is available.') . '</div>';
    $not_available = '<div id="health_info" class="not-available"><div class="semantic-connector-led led-red" title="Service NOT available"></div>' . t('The server is not available or the credentials are incorrect.') . '</div>';
    $markup = $connection->available() ? $available : $not_available;
    $form['health_info'] = array(
      '#markup' => $markup,
    );

    $form['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['cancel'] = array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/semantic-drupal/semantic-connector',
    );

    $form['#attached'] = array(
      'css' => array(drupal_get_path('module', 'semantic_connector') . '/css/semantic_connector.admin.css'),
    );
  }

  return $form;
}

/**
 * Validate-Handler for the edit-form of a Semantic Connector Connection.
 */
function semantic_connector_connection_edit_form_validate($form, &$form_state) {
  // Check if already exists a PoolParty server with the entered URL.
  $connections = SemanticConnector::searchConnections(array('type' => $form_state['values']['type'], 'url' => $form_state['values']['url']));
  if (!empty($connections)) {
    /** @var \SemanticConnectorConnection $connection */
    foreach ($connections as $connection) {
      if ($connection->getId() != $form_state['values']['connection_id']) {
        form_set_error('url', t('There already exists a PoolParty server (%name) with this URL. Please enter a different URL.', array('%name' => $connection->getTitle())));
        break;
      }
    }
  }
}

/**
 * Submit handler for the edit-form of a Semantic Connector Connection.
 */
function semantic_connector_connection_edit_form_submit($form, &$form_state) {
  SemanticConnector::createConnection($form_state['values']['type'], $form_state['values']['url'], $form_state['values']['title'], array(
    'username' => $form_state['values']['name'],
    'password' => $form_state['values']['pass'],
  ), array(), $form_state['values']['connection_id']);

  drupal_set_message(t('Successfully updated connection "%contitle".', array('%contitle' => $form_state['values']['title'])));
  drupal_goto('admin/config/semantic-drupal/semantic-connector');
}

/**
 * List deletion form.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param string $connection_type
 *   The type of the connection.
 * @param int $connection_id
 *   The ID of the Semantic Connector connection to delete.
 *
 * @return array
 *   The Drupal form array of the confirmation form.
 */
function semantic_connector_connection_delete_form($form, &$form_state, $connection_type, $connection_id) {
  $connection_type = str_replace('-', '_', $connection_type);
  $form_state['connection_id'] = $connection_id;
  $form_state['connection_type'] = $connection_type;
  $connection = SemanticConnector::getConnection($connection_type, $connection_id);

  return confirm_form($form,
    t('Are you sure you want to delete the PoolParty server %title?', array('%title' => $connection->getTitle())),
    'admin/config/semantic-drupal/semantic-connector',
    t('This action cannot be undone.'),
    t('Delete configuration'));
}

/**
 * Submit handler for semantic_connector_connection_delete_form().
 */
function semantic_connector_connection_delete_form_submit($form, &$form_state) {
  $connection_id = $form_state['connection_id'];
  $connection_type = $form_state['connection_type'];
  semantic_connector_connection_delete($connection_type, $connection_id, TRUE);
  $form_state['redirect'] = 'admin/config/semantic-drupal/semantic-connector';
}

/**
 * Delete a Semantic Connector connection.
 *
 * @param string $connection_type
 *   The type of the Semantic Connector connection.
 * @param int $connection_id
 *   The ID of the Semantic Connector connection to delete.
 * @param bool $is_user_interaction
 *   Handle this as a user interaction? If TRUE, a Drupal messages regarding the
 *   success of this operation and a goto will be included.
 *
 * @return bool
 *   TRUE if the operation succeeded, FALSE if not.
 */
function semantic_connector_connection_delete($connection_type, $connection_id, $is_user_interaction = FALSE) {
  $success = FALSE;

  // Delete the connection if it is not required by any configuration.
  $connection_usage = semantic_connector_check_connection_usage();
  if (!isset($connection_usage[$connection_id]) || empty($connection_usage[$connection_id])) {
    $connection = SemanticConnector::getConnection($connection_type, $connection_id);
    $connection->delete();
    $success = TRUE;
  }

  if ($is_user_interaction) {
    if ($success) {
      drupal_set_message(t('The connection was deleted successfully.'));
    }
    else {
      drupal_set_message(t('The connection is used by a module and therefore could not be deleted.'), 'error');
    }
    drupal_goto('admin/config/semantic-drupal/semantic-connector');
  }

  return $success;
}

/**
 * Refresh the projects of a Semantic Connector PoolParty server connection.
 *
 * All depending connections (SPARQL-endpoints) get recreated or updated by
 * resaving the connection.
 *
 * @param string $connection_type
 *   The type of connection to refresh ("pp_server" or "sparql_endpoint")
 * @param int $connection_id
 *   The ID of the Semantic Connector connection to refresh
 */
function semantic_connector_refresh_connection($connection_type, $connection_id) {
  $connection_type = str_replace('-', '_', $connection_type);
  $connection = SemanticConnector::getConnection($connection_type, $connection_id);
  if (!is_null($connection)) {
    $connection->save();
    drupal_set_message(t('Successfully refreshed connection "%contitle".', array('%contitle' => $connection->getTitle())));
  }
  else {
    drupal_set_message(t('Refreshing failed due to an invalid connection ID.'));
  }

  drupal_goto('admin/config/semantic-drupal/semantic-connector');
}

/**
 * Check what Semantic Connector connections are used by which module.
 *
 * @param array $modules_to_check
 *   An array of module keys to check for connections.
 *
 * @return array
 *   Associative array of connections usages, categorized by connection_id and
 *   then by module_key.
 */
function semantic_connector_check_connection_usage(array $modules_to_check = array(
  'pp_taxonomy_manager',
  'powertagging',
  'smart_glossary',
  'sonr_webmining',
)) {
  $connections_used = array();

  foreach ($modules_to_check as $module_key) {
    if (module_exists($module_key)) {
      switch ($module_key) {
        case 'powertagging':
          foreach (powertagging_config_load() as $config) {
            if (!isset($connections_used[$config->connection_id])) {
              $connections_used[$config->connection_id] = array();;
            }
            if (!isset($connections_used[$config->connection_id][$module_key])) {
              $connections_used[$config->connection_id][$module_key] = array();
            }
            $connections_used[$config->connection_id][$module_key][] = array(
              'id' => $config->powertagging_id,
              'title' => $config->title,
              'project_id' => $config->project_id,
            );
          }
          break;

        case 'smart_glossary':
          foreach (smart_glossary_config_load() as $config) {
            if (!isset($connections_used[$config->connection_id])) {
              $connections_used[$config->connection_id] = array();;
            }
            if (!isset($connections_used[$config->connection_id][$module_key])) {
              $connections_used[$config->connection_id][$module_key] = array();
            }
            $connections_used[$config->connection_id][$module_key][] = array(
              'id' => $config->sg_id,
              'title' => $config->title,
            );
          }
          break;

        case 'sonr_webmining':
          /** @var \SonrWebminingConfigurationSet $config */
          foreach (sonr_webmining_config_load_multiple() as $config) {
            $connection_id = $config->getConnectionId();
            if (!isset($connections_used[$connection_id])) {
              $connections_used[$connection_id] = array();
            }
            if (!isset($connections_used[$connection_id][$module_key])) {
              $connections_used[$connection_id][$module_key] = array();
            }
            $connections_used[$connection_id][$module_key][] = array(
              'id' => $config->getId(),
              'title' => $config->getTitle(),
              'project_id' => $config->getSearchSpaceId(),
            );
          }
          break;

        case 'pp_taxonomy_manager':
          /** @var \PPTaxonomyManagerConfig $config */
          foreach (pp_taxonomy_manager_config_load_multiple() as $config) {
            $settings = $config->getConfig();
            $connection_id = $config->getConnectionId();
            if (!isset($connections_used[$connection_id])) {
              $connections_used[$connection_id][$module_key] = array();
            }
            $connections_used[$connection_id][$module_key][] = array(
              'id' => $config->getId(),
              'title' => $config->getTitle(),
              'root_level' => $settings['root_level'],
              'project_id' => $config->getProjectId(),
              'project_ids' => $settings['taxonomies'],
            );
          }
          break;
      }
    }
  }

  return $connections_used;
}

/**
 * The form for the global Semantic Connector module configuration.
 */
function semantic_connector_configuration_form($form, &$form_state) {
  // Define the container for the vertical tabs.
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Tab: Notifications.
  $form['notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notifications'),
    '#group' => 'settings',
  );

  $form['notifications']['semantic_connector_version_checking'] = array(
    '#type' => 'checkbox',
    '#title' => t('Service version checking'),
    '#default_value' => variable_get('semantic_connector_version_checking', TRUE),
    '#description' => t("Check for newer versions of PoolParty servers and GraphSearch servers."),
  );

  // Automatic checks.
  $notifications = _semantic_connector_get_global_notification_config();
  $form['notifications']['semantic_connector_notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global notifications'),
    '#tree' => TRUE,
  );

  $form['notifications']['semantic_connector_notifications']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable global notifications'),
    '#default_value' => $notifications['enabled'],
    '#description' => t("If global notifications are enabled, selected checks will be done in a set interval, informing selected users in case any action has to be performed.!brThis information is either provided by adding Drupal warning messages appearing on every Drupal page or by sending mails to the users.", array('!br' => '<br />')),
  );

  $form['notifications']['semantic_connector_notifications']['interval'] = array(
    '#type' => 'radios',
    '#title' => t('Notification interval'),
    '#options' => array(
      '86400' => t('daily'),
      '604800' => t('every 7 days'),
      '2592000' => t('every 30 days'),
    ),
    '#default_value' => $notifications['interval'],
    '#states' => array(
      'visible' => array(
        ':input[name="semantic_connector_notifications[enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $user_roles = user_roles();
  $form['notifications']['semantic_connector_notifications']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles that will receive Drupal warning messages'),
    '#options' => $user_roles,
    '#default_value' => $notifications['roles'],
    '#states' => array(
      'visible' => array(
        ':input[name="semantic_connector_notifications[enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['notifications']['semantic_connector_notifications']['mail_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail addresses to notify via mail'),
    '#description' => t('A comma separated list of mail addresses to send notification mails to.'),
    '#default_value' => $notifications['mail_to'],
    '#size' => 100,
    '#states' => array(
      'visible' => array(
        ':input[name="semantic_connector_notifications[enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $notification_actions = _semantic_connector_get_global_notification_actions();
  $form['notifications']['semantic_connector_notifications']['actions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notify about following required actions:'),
    '#tree' => TRUE,
    '#states' => array(
      'visible' => array(
        ':input[name="semantic_connector_notifications[enabled]"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (empty($notification_actions)) {
    $form['notifications']['semantic_connector_notifications']['actions']['info'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="messages warning">' . t('There are currently no checks for global notifications available.') . '</div>',
    );
  }
  else {
    foreach ($notification_actions as $notification_action) {
      $form['notifications']['semantic_connector_notifications']['actions'][$notification_action['id']] = array(
        '#type' => 'checkbox',
        '#title' => $notification_action['title'],
        '#default_value' => isset($notifications['actions'][$notification_action['id']]) ? $notifications['actions'][$notification_action['id']] : $notification_action['default_value'],
        '#description' => $notification_action['description'],
      );
    }
  }

  $form['notifications']['semantic_connector_notifications']['refresh_notifications'] = array(
    '#type' => 'link',
    '#title' => t('Refresh the global notifications now'),
    '#href' => 'semantic-connector/refresh-notifications',
    '#attributes' => array(
      'class' => array('button'),
    ),
    '#options' => array(
      'query' => array(
        'destination' => current_path(),
      ),
    ),
  );

  // Tab: Module interconnection.
  $form['interconnection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Module interconnection'),
    '#group' => 'settings',
  );

  // Term click destinations.
  $destinations = _semantic_connector_get_destinations();
  foreach ($destinations as $destination_id => $destination) {
    $form['interconnection']['semantic_connector_term_click_destinations']['#tree'] = TRUE;
    // This field is invisible, but contains sort info (weights).
    $form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['weight'] = array(
      '#type' => 'weight',
      // Weights from -255 to +255 are supported because of this delta.
      '#delta' => 255,
      '#title_display' => 'invisible',
      '#default_value' => $destination['weight'],
    );
    $form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['label'] = array(
      '#type' => 'markup',
      '#markup' => $destination['label'],
    );

    // Add a list of sub-destinations if required.
    $connection_list_items = '';
    if ($destination_id == 'smart_glossary_detail_page') {
      $configs = smart_glossary_config_load();
      /** @var \stdClass $config */
      foreach ($configs as $config) {
        $connection_list_items .= '<li>' . l($config->title, 'admin/config/semantic-drupal/smart-glossary/' . $config->sg_id, array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector/config'))) . ' <b>' . ((isset($config->advanced_settings['semantic_connection']) && isset($config->advanced_settings['semantic_connection']['show_in_destinations']) && !$config->advanced_settings['semantic_connection']['show_in_destinations']) ? 'deactivated': 'activated') . '</b></li>';
      }
    }
    elseif ($destination_id == 'sonr_webmining') {
      $config_sets = sonr_webmining_config_load_multiple();
      /** @var \SonrWebminingConfigurationSet $config */
      foreach ($config_sets as $config) {
        $advanced_config = $config->getConfig();
        $connection_list_items .= '<li>' . l($config->getTitle(), 'admin/config/semantic-drupal/sonr-webmining/' . $config->getId(), array('query' => array('destination' => 'admin/config/semantic-drupal/semantic-connector/config'))) . ' <b>' . ((isset($advanced_config['semantic_connection']) && isset($advanced_config['semantic_connection']['show_in_destinations']) && !$advanced_config['semantic_connection']['show_in_destinations']) ? 'deactivated': 'activated') . '</b></li>';
      }
    }
    if (!empty($connection_list_items)) {
      $form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['label']['#markup'] .= '<ul>' . $connection_list_items . '</ul>';
    }

    $form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['list_title'] = array(
      '#type' => 'textfield',
      '#size' => 15,
      '#maxlength' => 255,
      '#default_value' => $destination['list_title'],
    );
    $form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['use'] = array(
      '#type' => 'checkbox',
      '#default_value' => $destination['use'],
    );
  }

  // Save and cancel buttons.
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/semantic-drupal/semantic-connector',
  );

  return $form;
}

/**
 * Submit handler for semantic_connector_configuration_form.
 */
function semantic_connector_configuration_form_submit($form, &$form_state) {
  variable_set('semantic_connector_version_checking', $form_state['values']['semantic_connector_version_checking']);
  variable_set('semantic_connector_term_click_destinations', $form_state['values']['semantic_connector_term_click_destinations']);

  $notifications = $form_state['values']['semantic_connector_notifications'];
  $notifications['roles'] = array_values(array_filter($notifications['roles']));
  variable_set('semantic_connector_notifications', $notifications);

  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Theme function for the configuration set admin form.
 *
 * @param array $variables
 *   An array of variables supported by the theme.
 *
 * @return string
 *   The rendered HTML content of the admin form.
 */
function theme_semantic_connector_configuration_form($variables) {
  $form = $variables['form'];

  if (!isset($form['interconnection']['semantic_connector_term_click_destinations']['#value'])) {
    $rows = array();
    foreach (element_children($form['interconnection']['semantic_connector_term_click_destinations']) as $destination_id) {
      $form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['weight']['#attributes']['class'] = array('term-click-destinations-order-weight');
      $rows[] = array(
        'data' => array(
          drupal_render($form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['label']),
          drupal_render($form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['use']),
          drupal_render($form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['list_title']),
          drupal_render($form['interconnection']['semantic_connector_term_click_destinations'][$destination_id]['weight']),
        ),
        'class' => array('draggable'),
      );
    }

    $header = array(t('Destination name'), t('Show'), t('List title'), t('Weight'));
    try {
      $destination_table = theme(
        'table',
        array(
          'header' => $header,
          'rows' => $rows,
          'attributes' => array('id' => 'term-click-destinations-order'),
        )
      );
    }
    catch (Exception $e) {
      $destination_table = $e->getMessage();
    }
    $form['interconnection']['semantic_connector_term_click_destinations'] = array(
      '#type' => 'item',
      '#title' => t('Term Click Destinations'),
      '#description' => t('Select which items should be displayed when clicking on a term.') . '<br />' . t('A whole destination type can be hidden by deselecting the "Show"-checkbox above, single destinations can be hidden inside their module\'s configuration page.'),
      '#markup' => $destination_table,
    );

    drupal_add_tabledrag('term-click-destinations-order', 'order', 'sibling', 'term-click-destinations-order-weight');
  }

  $output = drupal_render_children($form);
  return $output;
}
