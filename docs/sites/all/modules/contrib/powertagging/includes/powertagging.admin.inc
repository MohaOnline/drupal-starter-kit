<?php

/**
 * @file
 * Administration functionality for the PowerTagging.
 */

/**
 * List all saved PowerTagging configurations.
 *
 * @return string
 *   The rendered HTML of the list of configurations.
 */
function powertagging_list() {
  $output = array();

  $output['powertagging_title'] = array(
    '#type' => 'markup',
    '#markup' => '<h3 class="semantic-connector-table-title">' . t('PowerTagging configurations') . '</h3>',
  );

  $config_sets = powertagging_config_load();
  $rows = array();
  foreach ($config_sets as $config) {
    $field_instances = powertagging_get_tagging_field_instances($config->powertagging_id);
    $fields = powertagging_get_tagging_field_options_list($field_instances);
    $fields = empty($fields) ? '<div class="semantic-connector-italic">' . t('not yet set') . '</div>' : theme('item_list', array('items' => $fields));
    $actions = array(
      l(t('Edit'), 'admin/config/semantic-drupal/powertagging/' . $config->powertagging_id),
      l(t('Clone'), 'admin/config/semantic-drupal/powertagging/' . $config->powertagging_id . '/clone'),
      l(t('Delete'), 'admin/config/semantic-drupal/powertagging/' . $config->powertagging_id . '/delete'),
    );
    $title = '<div class="semantic-connector-led" data-server-id="' . $config->connection->getId() . '" data-server-type="pp-server" title="' . t('Checking service') . '"></div>';
    $title .= $config->title;
    $rows[] = array(
      $title,
      l($config->connection->getTitle(), $config->connection->getUrl() . '/PoolParty', array('attributes' => array('target' => '_blank'))),
      (isset($config->config['projects'][$config->project_id]) ? $config->config['projects'][$config->project_id]['label'] : '<i>' . t('project label not found') . '</i>'),
      $fields,
      implode(' | ', $actions),
    );
  }

  $output['powertagging'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Name'),
      t('PoolParty server'),
      t('Selected project'),
      t('Available in entity type'),
      t('Operations'),
    ),
    '#rows' => $rows,
    '#attributes' => array(
      'id' => 'powertagging-configurations-table',
      'class' => array('semantic-connector-tablesorter'),
    ),
    '#attached' => array(
      'js' => array(drupal_get_path('module', 'powertagging') . '/js/powertagging.admin.js'),
      'css' => array(drupal_get_path('module', 'powertagging') . '/css/powertagging.admin.css'),
    ),
  );

  // Add the tablesorter library if available.
  $tablesorter_path = 'sites/all/libraries/tablesorter';
  if (module_exists('libraries')) {
    $tablesorter_path = libraries_get_path('tablesorter');
  }
  if (file_exists($tablesorter_path . '/jquery.tablesorter.min.js')) {
    $output['#attached']['js'] = array(libraries_get_path('tablesorter') . '/jquery.tablesorter.min.js');
  }

  return $output;
}

/**
 * Ajax callback function for checking if a new PoolParty server is available.
 */
function powertagging_new_available($form, $form_state) {
  $available = '<div id="health_info" class="available"><div class="semantic-connector-led led-green" title="Service available"></div>' . t('The server is available.') . '</div>';
  $not_available = '<div id="health_info" class="not-available"><div class="semantic-connector-led led-red" title="Service NOT available"></div>' . t('The server is not available or the credentials are incorrect.') . '</div>';

  if (isset($form_state['values']['url']) && valid_url($form_state['values']['url'], TRUE)) {
    // Create a new connection (without saving) with the current form data.
    $connection = SemanticConnector::getConnection('pp_server');
    $connection->setUrl($form_state['values']['url']);
    $connection->setCredentials(array(
      'username' => $form_state['values']['name'],
      'password' => $form_state['values']['pass'],
    ));

    $availability = $connection->getApi('PPX')->available();
    if (isset($availability['message']) && !empty($availability['message'])) {
      return '<div id="health_info" class="not-available"><div class="semantic-connector-led led-red" title="Service NOT available"></div>' . $availability['message'] . '</div>';
    }
    else {
      return $availability['success'] ? $available : $not_available;
    }
  }

  return $not_available;
}

/**
 * The form for the global PowerTagging module configuration.
 */
function powertagging_configuration_form($form, &$form_state) {
  // PowerTagging Glossary block configuration.
  $glossary_items_max_options = range(0, 10);
  $glossary_items_max_options[0] = 'Unlimited';
  $glossary_conf = _powertagging_get_tag_glossary_settings();
  $form['powertagging_tag_glossary'] = array(
    '#type' => 'fieldset',
    '#title' => t('"PowerTagging Tag Glossary" block'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['powertagging_tag_glossary']['glossary_items_max'] = array(
    '#type' => 'select',
    '#options' => $glossary_items_max_options,
    '#title' => t('Number of tag glossary items'),
    '#description' => t('The maximum number of items to show in the "PowerTagging Tag Glossary" block'),
    '#default_value' => $glossary_conf['items_max'],
  );

  $form['powertagging_tag_glossary']['glossary_use_dbpedia_definition'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fetch definitions from DBpedia'),
    '#description' => t('If a concept has no definition but an exactMatch-entry to DBpedia, the abstract of the DBpedia entry will be used instead'),
    '#default_value' => $glossary_conf['use_dbpedia_definition'],
  );

  $form['powertagging_tag_glossary']['glossary_definition_max_characters'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum characters of definitions'),
    '#description' => t('The maximum number of characters of a definition that will be displayed initially for each concept. If a definition is longer it will be cut on word-basis and ellipsis will be added.!linebreakLeave this field empty if you don\'t want the number of characters to be limited.', array('!linebreak' => '<br />')),
    '#default_value' => $glossary_conf['definition_max_characters'],
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => (isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/semantic-drupal/powertagging'),
  );

  return $form;
}

/**
 * Validation handler for the global PowerTagging configuration form.
 */
function powertagging_configuration_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  $glossary_config = [
    'items_max' => $values['glossary_items_max'],
    'use_dbpedia_definition' => $values['glossary_use_dbpedia_definition'],
    'definition_max_characters' => $values['glossary_definition_max_characters'],
  ];
  variable_set('powertagging_tag_glossary_settings', $glossary_config);
}

/**
 * A form to update the PP server connection of a PowerTagging configuration.
 *
 * This form is used when creating a completely new PowerTagging configuration
 * or when the PoolParty server connection needs to be changed or a different
 * project shell be used for an existing PowerTagging configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $config
 *   A PowerTagging configuration.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_connection_form($form, &$form_state, $config = NULL) {
  $settings = [];
  if (!is_null($config) && !empty($config)) {
    $is_configuration_new = FALSE;
    $form['powertagging_id'] = array(
      '#type' => 'hidden',
      '#value' => $config->powertagging_id,
    );

    $form['title'] = array(
      '#type' => 'hidden',
      '#value' => $config->title,
    );

    $settings = $config->config;
  }
  else {
    $is_configuration_new = TRUE;
    $config = powertagging_default_config_load();

    $form['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('Name of the PowerTagging configuration.'),
      '#size' => 35,
      '#maxlength' => 255,
      '#default_value' => $config->title,
      '#required' => TRUE,
      '#validated' => TRUE,
    );
  }

  $connection_overrides = variable_get('powertagging_override_connections', array());
  $overridden_values = array();
  if (!$is_configuration_new && isset($connection_overrides[$config->powertagging_id])) {
    $overridden_values = $connection_overrides[$config->powertagging_id];
  }

  // Container: Server settings.
  $form['server_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('1. Select the PoolParty server to use'),
  );

  if (isset($overridden_values['connection_id'])) {
    $form['server_settings']['overridden_connection'] = array(
      '#type' => 'markup',
      '#markup' => '<span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>',
    );
  }

  // Display any unfulfilled module requirement.
  $requirement_errors = powertagging_check_requirements();
  if (!empty($requirement_errors)) {
    $requirement_errors_markup = '';
    foreach ($requirement_errors as $requirement_error) {
      $requirement_errors_markup .= '<div class="messages error">' . $requirement_error . '</div>';
    }
    $form['server_settings']['requirements'] = array(
      '#type' => 'item',
      '#markup' => $requirement_errors_markup,
    );
  }

  $connections = SemanticConnector::getConnectionsByType('pp_server');
  if (!empty($connections)) {
    $connection_options = array();
    foreach ($connections as $connection) {
      $credentials = $connection->getCredentials();
      $connection_options[implode('|', array($connection->getTitle(), $connection->getUrl(), $credentials['username'], $credentials['password']))] = $connection->getTitle();
    }
    $form['server_settings']['load_connection'] = array(
      '#type' => 'select',
      '#title' => t('Load an existing PoolParty server connection'),
      '#options' => $connection_options,
      '#empty_option' => '',
      '#default_value' => '',
    );
  }

  // Container: Connection details.
  $form['server_settings']['connection_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Connection details'),
  );

  $form['server_settings']['connection_details']['connection_id'] = array(
    '#type' => 'hidden',
    '#value' => $config->connection->getId(),
  );

  $form['server_settings']['connection_details']['server_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Server title'),
    '#description' => t('A short title for the server below.'),
    '#size' => 35,
    '#maxlength' => 60,
    '#default_value' => $config->connection->getTitle(),
    '#required' => TRUE,
  );

  $form['server_settings']['connection_details']['url'] = array(
    '#type' => 'textfield',
    '#title' => t('URL'),
    '#description' => t('URL, where the PoolParty server runs.'),
    '#size' => 35,
    '#maxlength' => 255,
    '#default_value' => $config->connection->getUrl(),
    '#required' => TRUE,
  );

  $credentials = $config->connection->getCredentials();
  $form['server_settings']['connection_details']['credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('Credentials'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['server_settings']['connection_details']['credentials']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#description' => t('Name of a user for the credentials.'),
    '#size' => 35,
    '#maxlength' => 60,
    '#default_value' => $credentials['username'],
  );
  $form['server_settings']['connection_details']['credentials']['pass'] = array(
    '#type' => 'textfield',
    '#title' => t('Password'),
    '#description' => t('Password of a user for the credentials.'),
    '#size' => 35,
    '#maxlength' => 128,
    '#default_value' => $credentials['password'],
  );

  $form['server_settings']['health_check'] = array(
    '#type' => 'button',
    '#value' => t('Health check'),
    '#ajax' => array(
      'callback' => 'powertagging_new_available',
      'wrapper' => 'health_info',
      'method' => 'replace',
      'effect' => 'slide',
    ),
  );

  if ($is_configuration_new) {
    $markup = '<div id="health_info">' . t('Click to check if the server is available.') . '</div>';
  }
  else {
    $available = '<div id="health_info" class="available"><div class="semantic-connector-led led-green" title="Service available"></div>' . t('The server is available.') . '</div>';
    $not_available = '<div id="health_info" class="not-available"><div class="semantic-connector-led led-red" title="Service NOT available"></div>' . t('The server is not available or the credentials are incorrect.') . '</div>';
    $markup = $config->connection->available() ? $available : $not_available;
  }
  $form['server_settings']['health_info'] = array(
    '#markup' => $markup,
  );

  // Container: Project loading.
  $form['project_load'] = array(
    '#type' => 'fieldset',
    '#title' => t('2. Load the projects'),
  );

  $form['project_load']['load_projects'] = array(
    '#type' => 'button',
    '#value' => t('Load projects'),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'powertagging_connection_form_get_projects',
      'wrapper' => 'replace-selection-area',
    ),
  );

  // Container: Project selection.
  $form['project_select'] = array(
    '#type' => 'fieldset',
    '#title' => t('3. Select the project to use'),
    '#description' => t('Note: In case this list is still empty after clicking the "Load projects" button make sure that a connection to the PoolParty server can be established and check the rights of your selected user inside PoolParty.'),
    '#prefix' => '<div id="replace-selection-area">',
  );

  // Get the project options for the currently configured PoolParty server.
  $connection = NULL;
  if(isset($form_state['values']) && isset($form_state['values']['url']) && valid_url($form_state['values']['url'], TRUE)) {
    // Create a new connection (without saving) with the current form data.
    $connection = SemanticConnector::getConnection('pp_server');
    $connection->setUrl($form_state['values']['url']);
    $connection->setCredentials(array(
      'username' => $form_state['values']['name'],
      'password' => $form_state['values']['pass'],
    ));
  }
  elseif (!$is_configuration_new) {
    $connection = $config->connection;
  }

  // Get the project options for the currently configured PoolParty server.
  $projects = [];
  if (!is_null($connection)) {
    $projects = $connection->getApi('PPX')->getProjects();
  }

  $project_options = array();
  foreach ($projects as $project) {
    $project_options[$project->uuid] = $project->label;
  }
  if (!empty($project_options) && isset($form_state['values']['project']) && !isset($project_options[$form_state['values']['project']])) {
    $form_state['values']['project'] = key($project_options);
  }

  // configuration set admin page.
  $form['project_select']['project'] = array(
    '#type' => 'select',
    '#title' => t('Select a project'),
    '#prefix' => '<div id="projects-replace">',
    '#suffix' => '</div>',
    '#options' => $project_options,
    '#default_value' => (!$is_configuration_new ? $config->project_id : NULL),
    '#required' => TRUE,
    '#validated' => TRUE,
    '#ajax' => array(
      'callback' => 'powertagging_connection_form_get_concept_schemes',
      'wrapper' => 'concept-schemes-replace',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  if (isset($overridden_values['project_id'])) {
    $form['project_select']['project']['#description'] = '<span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>';
  }

  // Container: Project selection.
  $form['further_restrictions'] = array(
    '#type' => 'fieldset',
    '#title' => t('4. Further restrictions'),
    '#description' => t('Note: A project has to be selected before any further restriction can be added.') . '<br />' . t('The restriction on the concept scheme level requires at least PoolParty version 6.2 to work properly.'),
    '#collapsible' => TRUE,
    '#collapsed' => ($is_configuration_new || !isset($settings['concept_scheme_restriction']) || empty($settings['concept_scheme_restriction'])),
    '#suffix' => '</div>',
  );

  // Get the concept scheme options for the currently configured PoolParty server.
  $concept_schemes = [];
  if (!is_null($connection)) {
    if (isset($form_state['values'])) {
      $project_id = isset($form_state['values']['project']) && !empty($project_options) && isset($project_options[$form_state['values']['project']]) ? $form_state['values']['project'] : '';
    }
    else {
      $project_id = $config->project_id;
    }

    if (!empty($project_id)) {
      $concept_schemes = $connection->getApi('PPT')
        ->getConceptSchemes($project_id);
    }
  }

  $concept_scheme_options = array();
  foreach ($concept_schemes as $concept_scheme) {
    $concept_scheme_options[$concept_scheme->uri] = $concept_scheme->title;
  }

  // configuration set admin page.
  $form['further_restrictions']['concept_scheme_restriction'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Filter by concept scheme'),
    '#description' => t('All concept schemes will be used if no checkbox is selected'),
    '#prefix' => '<div id="concept-schemes-replace">',
    '#suffix' => '</div>',
    '#options' => $concept_scheme_options,
    '#default_value' => (!$is_configuration_new && isset($settings['concept_scheme_restriction']) ? $settings['concept_scheme_restriction'] : []),
    '#validated' => TRUE,
  );

  // Save and cancel buttons.
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => ($is_configuration_new ? t('Save and edit details') : t('Save')),
    '#prefix' => '<div class="admin-form-submit-buttons" style="margin-top: 40px;">',
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => 'admin/config/semantic-drupal/powertagging',
    '#suffix' => '</div>',
  );

  // Add CSS and JS files
  $form['#attached'] = array(
    'js' => array(drupal_get_path('module', 'powertagging') . '/js/powertagging.admin.js'),
    'css' => array(drupal_get_path('module', 'powertagging') . '/css/powertagging.admin.css'),
  );

  return $form;
}

/**
 * Ajax callback function to get a project select list for a given PoolParty
 * server connection configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 *
 * @return array
 *   The select form element containing the project options for the current
 *   PoolParty server connection.
 */
function powertagging_connection_form_get_projects($form, $form_state) {
  $replaced_form = [];
  $replaced_form['project_select'] = $form['project_select'];
  $replaced_form['further_restrictions'] = $form['further_restrictions'];
  return $replaced_form;
}

/**
 * Ajax callback function to get a concept schemes select list for a given
 * PoolParty server connection + project.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 *
 * @return array
 *   The select form element containing the concept scheme options for the
 *   currently selected project.
 */
function powertagging_connection_form_get_concept_schemes($form, $form_state) {
  return $form['further_restrictions']['concept_scheme_restriction'];
}

/**
 * The validation handler for the PowerTagging connection form.
 */
function powertagging_connection_form_validate($form, &$form_state) {
  // Only do project validation during the save-operation, not during
  // AJAX-requests like the health check of the server.
  if ($form_state['triggering_element']['#parents'][0] == 'save') {
    // A title is required.
    if (!isset($form_state['values']['powertagging_id']) && empty($form_state['values']['title'])) {
      form_set_error('title', t('Name field is required.'));
    }

    // A project needs to be selected.
    if (empty($form_state['values']['project'])) {
      form_set_error('project', t('Please select a project.'));
    }
    // And it has to be a valid project (one that is available on the connected
    // PoolParty server).
    else {
      if (isset($form_state['values']['url']) && valid_url($form_state['values']['url'], TRUE)) {
        // Create a new connection (without saving) with the current form data.
        $connection = SemanticConnector::getConnection('pp_server');
        $connection->setUrl($form_state['values']['url']);
        $connection->setCredentials(array(
          'username' => $form_state['values']['name'],
          'password' => $form_state['values']['pass'],
        ));
        $projects = $connection->getApi('PPX')->getProjects();
        $project_is_valid = FALSE;
        foreach ($projects as $project) {
          if ($project->uuid == $form_state['values']['project']) {
            $project_is_valid = TRUE;
            break;
          }
        }
        if ($project_is_valid) {
          // Check if the selected concept schemes are available for this project.
          $concept_scheme_values = array_filter($form_state['values']['concept_scheme_restriction']);
          if (!empty($concept_scheme_values)) {
            $concept_schemes = $connection->getApi('PPT')
              ->getConceptSchemes($form_state['values']['project']);
            foreach ($concept_scheme_values as $concept_scheme_value) {
              $concept_scheme_exists = FALSE;
              foreach ($concept_schemes as $concept_scheme) {
                if ($concept_scheme->uri == $concept_scheme_value) {
                  $concept_scheme_exists = TRUE;
                  break;
                }
              }

              if (!$concept_scheme_exists) {
                form_set_error('concept_scheme_restriction', t('At least one invalid concept scheme has been selected.'));
                break;
              }
            }
          }
        }
        else {
          form_set_error('project', t('The selected project is not available on the given PoolParty server configuration.'));
        }
      }
      else {
        form_set_error('url', t('The field URL must be a valid URL.'));
      }
    }
  }
}

/**
 * The submit handler for the PowerTagging connection form.
 */
function powertagging_connection_form_submit($form, &$form_state) {
  // Always create a new connection, if URL and type are the same the old one
  // will be used anyway.
  $connection = SemanticConnector::createConnection('pp_server', $form_state['values']['url'], $form_state['values']['server_title'], array(
    'username' => $form_state['values']['name'],
    'password' => $form_state['values']['pass'],
  ));
  $concept_scheme_values = array_values(array_filter($form_state['values']['concept_scheme_restriction']));

  // Update an existing configuration set.
  if (isset($form_state['values']['powertagging_id'])) {
    $ptid = $form_state['values']['powertagging_id'];
    $existing_config = powertagging_config_load($ptid);

    // Add config changes.
    $existing_config->config['concept_scheme_restriction'] = $concept_scheme_values;

    db_update('powertagging')
      ->fields(array(
        'connection_id' => $connection->getId(),
        'project_id' => $form_state['values']['project'],
        'config' => serialize($existing_config->config),
      ))
      ->condition('powertagging_id', $ptid)
      ->execute();

    drupal_set_message(t('The connection for PowerTagging configuration %title has been updated.', array('%title' => $form_state['values']['title'])));
  }
  // Create a new configuration set.
  else {
    $default_config = powertagging_default_config_load();
    $default_config->config['concept_scheme_restriction'] = $concept_scheme_values;

    $ptid = db_insert('powertagging')
      ->fields(array(
        'title' => $form_state['values']['title'],
        'connection_id' => $connection->getId(),
        'project_id' => $form_state['values']['project'],
        'config' => serialize($default_config->config),
      ))
      ->execute();

    // Set the last batch tagging variable.
    variable_set('powertagging_last_batch_tagging_' . $ptid, time());

    drupal_set_message(t('PowerTagging configuration %title has been created.', array('%title' => $form_state['values']['title'])));
  }

  $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging/' . $ptid;
}

/**
 * The form for a PowerTagging configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $config
 *   A PowerTagging configuration.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_form($form, &$form_state, $config = NULL) {
  $connection_overrides = variable_get('powertagging_override_connections', array());
  $overridden_values = array();
  if (isset($connection_overrides[$config->powertagging_id])) {
    $overridden_values = $connection_overrides[$config->powertagging_id];
  }

  /** @var \SemanticConnectorPPXApi $ppx_api */
  $ppx_api = $config->connection->getApi('PPX');
  /** @var \SemanticConnectorPPTApi $ppt_api */
  $ppt_api = $config->connection->getApi('PPT');
  $projects = $ppx_api->getProjects();

  // Set action links for bulk operations.
  if (!empty($config->config['projects'])) {
    $project_languages = array();
    foreach ($config->config['projects'][$config->project_id]['languages'] as $pp_language) {
      if (!empty($pp_language)) {
        $project_languages[] = $pp_language;
      }
    }
    if (!empty($project_languages)) {
      $path = 'admin/config/semantic-drupal';
      $action_index = $path . '/powertagging/' . $config->powertagging_id . '/index';
      $action_update_taxonomy = $path . '/powertagging/' . $config->powertagging_id . '/update-taxonomy';

      $form['action_links'] = [
        '#markup' => '
        <ul class="action-links">
            <li>' . l(t('Tag content'), $action_index) . '</li>
            <li>' . l(t('Update vocabulary'), $action_update_taxonomy) . '</li>
        </ul>',
      ];
    }
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name of the PowerTagging configuration.') . (isset($overridden_values['title']) ? ' <span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>' : ''),
    '#size' => 35,
    '#maxlength' => 255,
    '#default_value' => $config->title,
    '#required' => TRUE,
  );

  // Get the project title of the currently configured project.
  $project_title = '<invalid project selected>';
  $pp_server_projects = $config->connection->getApi('PPX')->getProjects();
  foreach ($pp_server_projects as $pp_server_project) {
    if ($pp_server_project->uuid == $config->project_id) {
      $project_title = $pp_server_project->label;
    }
  }

  // Add information about the connection.
  $connection_markup = '';
  // Check the PoolParty server version if required.
  if (variable_get('semantic_connector_version_checking', TRUE)) {
    $api_version_info = $config->connection->getVersionInfo('PPX');
    if (version_compare($api_version_info['installed_version'], $api_version_info['latest_version'], '<')) {
      $connection_markup .= '<div class="messages warning">' . t('The connected PoolParty server is not up to date. You are currently running version %installedversion, upgrade to version %latestversion to enjoy the new features.', array('%installedversion' => $api_version_info['installed_version'], '%latestversion' => $api_version_info['latest_version'])) . '</div>';
    }
  }
  $concept_scheme_labels = [];
  if (isset($config->config['concept_scheme_restriction']) && !empty($config->config['concept_scheme_restriction'])) {
    $concept_schemes = $config->connection->getApi('PPT')
      ->getConceptSchemes($config->project_id);

    foreach ($concept_schemes as $concept_scheme) {
      if (in_array($concept_scheme->uri, $config->config['concept_scheme_restriction'])) {
        $concept_scheme_labels[] = $concept_scheme->title;
      }
    }
  }

  $connection_markup .= '<p id="powertagging-connection-info">' . t('Connected PoolParty server') . ': <b>' . $config->connection->getTitle() . ' (' . $config->connection->getUrl() . ')</b><br />'
    . t('Selected project') . ': <b>' . $project_title . '</b><br />';
  if (!empty($concept_scheme_labels)) {
    $connection_markup .= t('Selected concept schemes restrictions') . ': <b>' . implode('</b>, <b>', $concept_scheme_labels) . '</b><br />';
  }
  $connection_markup .= l(t('Change the connected PoolParty server or project'), 'admin/config/semantic-drupal/powertagging/' . $config->powertagging_id . '/edit_connection') . '</p>';
  $form['pp_connection_markup'] = array(
    '#type' => 'markup',
    '#markup' => $connection_markup,
  );

  // Define the container for the vertical tabs.
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  // Tab: Available project settings.
  $form['project_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project settings'),
    '#group' => 'settings',
    '#tree' => TRUE,
  );

  $available_languages = array(
    LANGUAGE_NONE => (object) array(
      'language' => LANGUAGE_NONE,
      'name' => 'Language neutral',
      'enabled' => TRUE,
    ),
  );
  $available_languages = array_merge($available_languages, language_list());

  if (!empty($projects)) {
    foreach ($projects as $project) {
      if ($project->uuid != $config->project_id) {
        continue;
      }

      $form['project_settings']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project->uuid,
      );
      $form['project_settings']['label'] = array(
        '#type' => 'hidden',
        '#value' => $project->label,
      );
      $form['project_settings']['languages'] = array(
        '#type' => 'item',
        '#title' => t('Map the Drupal languages with the PoolParty project languages'),
      );

      // Language mapping.
      $project_language_options = array();
      foreach ($project->languages as $project_language) {
        $project_language_options[$project_language] = $project_language;
      }

      $states = array();
      foreach ($available_languages as $lang) {
        if ($lang->enabled) {
          $form['project_settings']['languages'][$lang->language] = array(
            '#type' => 'select',
            '#title' => $lang->name,
            '#options' => $project_language_options,
            '#empty_option' => '',
            '#default_value' => ((isset($config->config['projects']) && isset($config->config['projects'][$project->uuid]) && isset($config->config['projects'][$project->uuid]['languages'][$lang->language])) ? $config->config['projects'][$project->uuid]['languages'][$lang->language] : ''),
          );
          $states['#edit-project-settings-languages-' . $lang->language] = array('value' => '');
        }
      }

      // Vocabulary selection.
      // Hidden field for the selecting the vocabulary.
      // It checks the availability of a language.
      $form['project_settings']['no_language_selected'] = array(
        '#type' => 'checkbox',
        '#default_value' => TRUE,
        '#attributes' => array('class' => array('element-hidden')),
        '#states' => array(
          'checked' => $states,
        ),
      );
      $form['project_settings']['taxonomy_name'] = array(
        '#title' => t('Select or enter a new vocabulary'),
        '#type' => 'textfield',
        '#default_value' => (isset($config->config['projects'][$project->uuid]['taxonomy_name']) ? $config->config['projects'][$project->uuid]['taxonomy_name'] : ''),
        '#autocomplete_path' => 'powertagging/autocomplete-taxonomies',
        '#states' => array(
          'required' => array(
            '#edit-project-settings-no-language-selected' => array('checked' => FALSE),
          ),
          'disabled' => $states,
        ),
      );

      if (isset($overridden_values['taxonomy_id'])) {
        $form['project_settings']['taxonomy_name']['#description'] = ' <span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>';
      }

      // Ask if the vocabulary should be removed also if no language is
      // selected.
      if (isset($config->config['projects'][$project->uuid]['taxonomy_name']) && !empty($config->config['projects'][$project->uuid]['taxonomy_name'])) {
        $form['project_settings']['remove_taxonomy'] = array(
          '#type' => 'checkbox',
          '#title' => t('Remove the appropriate vocabulary. All terms and relations of this vocabulary will be removed also.'),
          '#states' => array(
            'visible' => $states,
          ),
        );
      }

      $form['project_settings']['mode'] = array(
        '#type' => 'radios',
        '#title' => t('PowerTagging mode'),
        '#options' => array(
          'annotation' => '<b>' . t('Annotation') . '</b> ' .  t('(use the whole thesaurus to tag the content)'),
          'classification' => '<b>' . t('Classification') . '</b> ' .  t('(categorize the content on the top concept level)'),
        ),
        '#default_value' => (isset($config->config['projects'][$project->uuid]['mode']) ? $config->config['projects'][$project->uuid]['mode'] : 'annotation'),
        '#required' => TRUE,
      );

      // Get the corpus options for the currently configured PoolParty server.
      $corpus_options = array();
      $corpora = $ppt_api->getCorpora($project->uuid);
      foreach ($corpora as $corpus) {
        $corpus_options[$corpus['corpusId']] = $corpus['corpusName'];
      }
      // Get the default value for the corpus selection.
      $corpus_id = '';
      if (isset($config->config['projects'][$project->uuid]['corpus_id']) && !empty($config->config['projects'][$project->uuid]['corpus_id'])) {
        $corpus_id = $config->config['projects'][$project->uuid]['corpus_id'];
      }
      $form['project_settings']['corpus_id'] = array(
        '#type' => 'select',
        '#title' => t('Select the corpus to use'),
        '#description' => t('Usage of a good corpus can improve your free terms considerably.'),
        '#options' => $corpus_options,
        '#default_value' => $corpus_id,
        "#empty_option" => !empty($corpus_options) ? '' : t('- No corpus available -'),
        '#states' => array(
          'visible' => array(':input[name="project_settings[mode]"]' => array('value' => 'annotation')),
        ),
      );

      if (isset($overridden_values['corpus_id'])) {
        $form['project_settings']['corpus_id']['#description'] = '<span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>';
      }
      break;
    }
  }
  else {
    $form['project_settings']['errors'] = array(
      '#type' => 'item',
      '#markup' => '<div class="messages warning">' . t('Either no connection can be established or there are no projects available for the given credentials.') . '</div>',
    );
  }

  // Tab: Global limits.
  $form['global_limits_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global limits'),
    '#group' => 'settings',
  );

  powertagging_global_limits_form($form['global_limits_settings'], $config->config['concepts_per_extraction'], $config->config['concepts_threshold'], $config->config['freeterms_per_extraction'], $config->config['freeterms_threshold']);

  // The most part of the global limits are only visible when PowerTagging is
  // used for annotation.
  $form['global_limits_settings']['concepts']['concepts_threshold']['#states'] = array(
    'visible' => array(':input[name="project_settings[mode]"]' => array('value' => 'annotation')),
  );
  $form['global_limits_settings']['freeterms']['#states'] = array(
    'visible' => array(':input[name="project_settings[mode]"]' => array('value' => 'annotation')),
  );

  // Search for all content types with the PowerTagging field.
  $field_instances = powertagging_get_tagging_field_instances($config->powertagging_id);
  if (!empty($field_instances)) {
    $form['global_limits_settings']['overwriting'] = array(
      '#type' => 'fieldset',
      '#title' => t('List of all content types with "PowerTagging Tags" fields'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Select those content types which ones you want to overwrite the limits with the global limits defined above.'),
      '#tree' => FALSE,
    );
    if (count($field_instances) > 1) {
      $form['global_limits_settings']['overwriting']['select_all_content_types'] = array(
        '#type' => 'checkbox',
        '#title' => t('Select all'),
        '#attributes' => array(
          'onclick' => 'jQuery("#edit-overwrite-content-types").find("input").attr("checked", jQuery(this).attr("checked"));',
        ),
      );
    }
    $form['global_limits_settings']['overwriting']['overwrite_content_types'] = array(
      '#type' => 'checkboxes',
      '#options' => powertagging_get_tagging_field_options_list($field_instances, TRUE),
      '#validated' => TRUE,
    );
  }

  // Tab: Data fetching settings.
  $properties = array(
    'skos:altLabel' => t('Alternative labels'),
    'skos:hiddenLabel' => t('Hidden labels'),
    'skos:scopeNote' => t('Scope notes'),
    'skos:related' => t('Related concepts'),
    'skos:exactMatch' => t('Exact matches'),
  );
  $form['data_properties_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional data'),
    '#group' => 'settings',
  );

  $form['data_properties_settings']['data_properties'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select the properties that will be saved in addition to the taxonomy terms during the PowerTagging process'),
    '#description' => t('If you clear a checkbox, all data of that property will be deleted from the associated vocabulary.'),
    '#options' => $properties,
    '#default_value' => $config->config['data_properties'],
  );

  $form['powertagging_id'] = array(
    '#type' => 'value',
    '#value' => $config->powertagging_id,
  );

  $form['concept_scheme_restriction'] = array(
    '#type' => 'value',
    '#value' => $config->config['concept_scheme_restriction'],
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['delete'] = array(
    '#type' => 'button',
    '#value' => t('Delete'),
    '#attributes' => array('onclick' => 'window.location.href = "' . url('admin/config/semantic-drupal/powertagging/' . $config->powertagging_id . '/delete') . '"; return false;'),
  );
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => (isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/semantic-drupal/powertagging'),
  );

  // Add CSS and JS files
  $form['#attached'] = array(
    'js' => array(drupal_get_path('module', 'powertagging') . '/js/powertagging.admin.js'),
    'css' => array(drupal_get_path('module', 'powertagging') . '/css/powertagging.admin.css'),
  );

  return $form;
}

/**
 * Validate handler for powertagging_form.
 */
function powertagging_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $config = powertagging_config_load($values['powertagging_id']);

  // If there are any requirement errors, do not save the configuration.
  $requirement_errors = powertagging_check_requirements();
  if (!empty($requirement_errors)) {
    $requirement_errors_markup = t('The configuration could not be saved due to unfulfilled module requirements:');
    foreach ($requirement_errors as $requirement_error) {
      $requirement_errors_markup .= '<br />&nbsp;- ' . $requirement_error;
    }

    form_set_error('', $requirement_errors_markup);
    return FALSE;
  }

  $default_language = language_default('language');

  if (isset($values['project_settings']) && !empty($values['project_settings']) && $values['project_settings']['project_id'] == $config->project_id) {
    if (!$values['project_settings']['no_language_selected'] && empty($values['project_settings']['taxonomy_name'])) {
      form_set_error('project_settings][taxonomy_name', t('"Select or enter a new vocabulary" field is required.'));
    }

    if (!powertagging_translation_possible()) {
      foreach ($values['project_settings']['languages'] as $drupal_lang => $pp_lang) {
        if (!empty($pp_lang) && $drupal_lang != LANGUAGE_NONE && $drupal_lang != $default_language) {
          $drupal_languages = language_list();
          form_set_error('project_settings][languages][' . $drupal_lang, t('Language "%language" requires localization of taxonomies as it is not your default site language.<br /> Install and enable module "!internationalizationlink" and its sub-module "Taxonomy translation" to make multilingual tagging possible.', array(
            '%language' => $drupal_languages[$drupal_lang]->name,
            '!internationalizationlink' => l(t('Internationalization'), 'https://www.drupal.org/project/i18n', array('attributes' => array('target' => '_blank'))),
          )));
        }
      }
    }
  }
}

/**
 * Submit handler for powertagging_form.
 */
function powertagging_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Get taxonomy id if a taxonomy name is given for the project.
  if (isset($values['project_settings'])) {
    // Get all existing taxonomies.
    $taxonomy_names = taxonomy_vocabulary_get_names();
    $vids = array();
    foreach ($taxonomy_names as $taxonomy_data) {
      $vids[$taxonomy_data->name] = $taxonomy_data->vid;
    }
    if (!empty($values['project_settings']['taxonomy_name'])) {
      $taxonomy_name = trim(check_plain($values['project_settings']['taxonomy_name']));
      if (isset($vids[$taxonomy_name])) {
        if (isset($values['project_settings']['remove_taxonomy']) && $values['project_settings']['remove_taxonomy']) {
          taxonomy_vocabulary_delete($vids[$taxonomy_name]);
          drupal_set_message(t('Vocabulary %name has been removed.', array('%name' => $values['project_settings']['taxonomy_name'])));
          unset($values['project_settings']['taxonomy_id']);
          unset($values['project_settings']['taxonomy_name']);
        }
        else {
          // Make sure the already existing vocabulary is able to store
          // PowerTagging tags.
          powertagging_modify_vocabulary($vids[$taxonomy_name]);
          $values['project_settings']['taxonomy_id'] = $vids[$taxonomy_name];
        }
      }
      else {
        // If taxonomy does not exists, it will be created.
        $taxonomy = powertagging_create_vocabulary($taxonomy_name);
        $values['project_settings']['taxonomy_id'] = $taxonomy->vid;
      }
    }
  }

  // Get the data properties for data fetching process.
  $data_properties = [];
  if (isset($values['data_properties'])) {
    foreach ($values['data_properties'] as $property) {
      if ($property) {
        $data_properties[] = $property;
      }
    }
  }

  // Delete the data for deselected properties
  $clear_fields = array_diff($form['data_properties_settings']['data_properties']['#default_value'], $data_properties);
  if (!empty($clear_fields)) {
    $properties = array(
      'skos:altLabel' => 'field_alt_labels',
      'skos:hiddenLabel' => 'field_hidden_labels',
      'skos:scopeNote' => 'field_scope_notes',
      'skos:related' => 'field_related_concepts',
      'skos:exactMatch' => 'field_exact_match',
    );
    $tables = [];
    foreach ($clear_fields as $field) {
      $tables[] = 'field_data_' . $properties[$field];
      $tables[] = 'field_revision_' . $properties[$field];
      foreach ($tables as $table) {
        db_query('DELETE f
        FROM {' . $table . '} f
        LEFT JOIN {taxonomy_term_data} t ON f.entity_id = t.tid
        WHERE (
          t.vid = :vid
        )', [':vid' => $values['project_settings']['taxonomy_id']]);
      }
    }
    cache_clear_all('field:taxonomy_term:', 'cache_field', TRUE);
  }

  // Serialize the configuration data.
  $serialized_config = serialize(array(
    'concepts_per_extraction' => $values['concepts_per_extraction']['value'],
    'concepts_threshold' => $values['concepts_threshold']['value'],
    'freeterms_per_extraction' => $values['freeterms_per_extraction']['value'],
    'freeterms_threshold' => $values['freeterms_threshold']['value'],
    'projects' => (isset($values['project_settings']) ? array($values['project_settings']['project_id'] => $values['project_settings']) : array()),
    'concept_scheme_restriction' => $values['concept_scheme_restriction'],
    'data_properties' => $data_properties,
  ));

  // Save configuration into database.
  db_update('powertagging')
    ->fields(array(
        'title' => $form_state['values']['title'],
        'config' => $serialized_config,
      ))
    ->condition('powertagging_id', $form_state['values']['powertagging_id'])
    ->execute();

  // Overwrite content type limits with the global limits for
  // each selected content type.
  // TODO: Remove the same code from the climate_tagger module,
  //       when this module goes productive.
  if (!module_exists('climate_tagger') && isset($values['overwrite_content_types'])) {
    $overwrite_content_types = array_filter($values['overwrite_content_types']);
    if (!empty($overwrite_content_types)) {
      foreach ($overwrite_content_types as $content_type) {
        list($entity_type, $bundle_name, $field_name) = explode(' ', $content_type);
        $instance = field_info_instance($entity_type, $field_name, $bundle_name);
        $limits = array(
          'concepts_per_extraction' => array('value' => $values['concepts_per_extraction']['value']),
          'concepts_threshold' => array('value' => $values['concepts_threshold']['value']),
          'freeterms_per_extraction' => array('value' => $values['freeterms_per_extraction']['value']),
          'freeterms_threshold' => array('value' => $values['freeterms_threshold']['value']),
        );
        $instance['settings'] = array_merge($instance['settings'], $limits);
        field_update_instance($instance);
      }
    }
  }

  drupal_set_message(t('PowerTagging configuration %title has been saved.', array('%title' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging';
}

/**
 * Add a PowerTagging configuration for a predefined PP connection + project.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param int $connection_id
 *   The ID of the PoolParty server connection
 * @param string $project_id
 *   The ID of the project on the PoolParty server
 *
 * @return array
 *   The Drupal form array of the confirmation form.
 */
function powertagging_add_config_for_fixed_connection_form($form, &$form_state, $connection_id, $project_id) {
  $form_state['connection_id'] = $connection_id;
  $form_state['project_id'] = $project_id;

  $connection = SemanticConnector::getConnection('pp_server', $connection_id);
  $pp_config = $connection->getConfig();
  foreach ($pp_config['projects'] as $project) {
    if ($project->id == $project_id) {
      return confirm_form($form,
        t('Are you sure you want to create the PowerTagging configuration?'),
        (isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/semantic-drupal/powertagging'),
        'Selected PoolParty server: <b>' . $connection->getTitle() . '</b><br />Selected project: <b>' .$project->title . '</b>',
        t('Create configuration'));
      break;
    }
  }
}

/**
 * Submit handler for powertagging_add_config_for_fixed_connection_form().
 */
function powertagging_add_config_for_fixed_connection_form_submit($form, &$form_state) {
  $connection = SemanticConnector::getConnection('pp_server', $form_state['connection_id']);
  $project_id = $form_state['project_id'];

  // Get the current list of projects from the server to make sure the project
  // still exists.
  $project_is_valid = FALSE;
  $projects = $connection->getApi('PPX')->getProjects();
  foreach ($projects as $project) {
    if ($project->uuid == $project_id) {
      $project_is_valid = TRUE;

      // Create a new configuration set.
      $default_config = powertagging_default_config_load();
      $configuration_set_title = 'PowerTagging configuration for ' . $connection->getTitle() . ' (' . $project->label . ')';
      $ptid = db_insert('powertagging')
        ->fields(array(
          'title' => $configuration_set_title,
          'connection_id' => $connection->getId(),
          'project_id' => $project_id,
          'config' => serialize($default_config->config),
        ))
        ->execute();

      // Set the last batch tagging variable.
      variable_set('powertagging_last_batch_tagging_' . $ptid, time());

      drupal_set_message(t('PowerTagging configuration "%title" has been created.', array('%title' => $configuration_set_title)));
      // Drupal Goto to forward a destination if one is available.
      if (isset($_GET['destination'])) {
        $destination = $_GET['destination'];
        unset($_GET['destination']);
        drupal_goto('admin/config/semantic-drupal/powertagging/' . $ptid, array('query' => array('destination' => $destination)));
        exit();
      }
      $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging/' . $ptid;
      break;
    }
  }

  if (!$project_is_valid) {
    drupal_set_message(t('Project-ID "%projectid" was not found on PoolParty server "%ppservertitle"', array('%projectid' => $project_id,'%ppservertitle' => $connection->getTitle())));
    $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging';
  }
}

/**
 * The confirmation-form for cloning a PowerTagging config.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $config
 *   A PowerTagging configuration.
 *
 * @return array
 *   The Drupal form array of the confirmation form.
 */
function powertagging_clone_form($form, &$form_state, $config) {
  $form_state['config'] = $config;
  return confirm_form($form,
    t('Are you sure you want to clone the PowerTagging configuration "%title"?', array('%title' => $config->title)),
    'admin/config/semantic-drupal/powertagging',
    '',
    t('Clone configuration'));
}

/**
 * The submit handler of the confirmation-form to clone a PowerTagging config.
 */
function powertagging_clone_form_submit($form, &$form_state) {
  $config = $form_state['config'];

  $ptid = db_insert('powertagging')
    ->fields(array(
      'title' => $config->title . ' (CLONE)',
      'connection_id' => $config->connection_id,
      'project_id' => $config->project_id,
      'config' => serialize($config->config),
    ))
    ->execute();

  // Set the last batch tagging variable.
  variable_set('powertagging_last_batch_tagging_' . $ptid, time());

  drupal_set_message(t('%title has been cloned.', array('%title' => $config->title)));
  $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging/' . $ptid;
}

/**
 * List deletion form.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $config
 *   A PowerTagging configuration.
 *
 * @return array
 *   The Drupal form array of the confirmation form.
 */
function powertagging_delete_form($form, &$form_state, $config) {
  $form_state['config'] = $config;

  // Check if this PowerTagging configuration is still available in a field.
  $field_instances = powertagging_get_tagging_field_instances($config->powertagging_id);
  $fields = powertagging_get_tagging_field_options_list($field_instances);
  if (!empty($fields)) {
    $content_by_field = array();
    foreach ($fields as $entity_key => $content) {
      $exploded_entity_key = explode(' ', $entity_key);
      $content_by_field[$exploded_entity_key[2]][] = $content;
    }
    $content_markup = '';
    foreach ($content_by_field as $field_id => $contents) {
      $content_markup .= '<li>' . t('Field "%fieldid" used in:', array('%fieldid' => $field_id)) . ' ' . implode(', ', $contents) . '</li>';
    }
    $form['powertagging_available_fields'] = array(
      '#type' => 'markup',
      '#markup' => '
<div>
  <p><b>WARNING</b>: ' . t('This PowerTagging configuration is still used in following fields:') . '</p>
  <ul>' . $content_markup . '</ul>
  <p><b>By deleting this PowerTagging configuration all these fields will automatically be deleted too.</b></p>
</div><br />',
    );
  }

  return confirm_form($form,
    t('Are you sure you want to delete %title?', array('%title' => $config->title)),
    'admin/config/semantic-drupal/powertagging',
    t('This action cannot be undone.'),
    t('Delete configuration'));
}

/**
 * Submit handler for powertagging_delete_form().
 */
function powertagging_delete_form_submit($form, &$form_state) {
  $config = $form_state['config'];
  powertagging_config_delete($config->powertagging_id);
  drupal_set_message(t('PowerTagging configuration "%title" has been deleted.', array('%title' => $config->title)));
  $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging';
}

/**
 * Create a new taxonomy for PowerTagging tags.
 *
 * @param string $name
 *   The human readable name of the new taxonomy.
 *
 * @return object
 *   The created vocabulary object.
 */
function powertagging_create_vocabulary($name) {
  // Create the new Drupal taxonomy.
  $machine_name = powertagging_machine_name($name);
  $vocabulary = (object) array(
    'name' => $name,
    'machine_name' => $machine_name,
    'description' => t('Automatically created by PowerTagging Module'),
    'hierarchy' => 2,
  );
  taxonomy_vocabulary_save($vocabulary);

  // Add required fields for PowerTagging.
  powertagging_modify_vocabulary($vocabulary->vid);

  drupal_set_message(t('Vocabulary %name has been created.', array('%name' => $vocabulary->name)));

  return $vocabulary;
}

/**
 * Modify a taxonomy in a way that it can be used to store PowerTagging tags.
 *
 * @param int $vid
 *   The vocabulary id of the taxonomy to modify.
 */
function powertagging_modify_vocabulary($vid) {
  $taxonomy = taxonomy_vocabulary_load($vid);
  $machine_name = $taxonomy->machine_name;

  // Create an URI field if it does not exist yet.
  $field_name = 'field_uri';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text',
      'label'       => t('URI'),
    );
    field_create_field($field);
  }

  // Attach the URI field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('URI'),
      'description' => t('The URI of the concept'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textfield',
        'weight'    => 3,
      ),
    );
    field_create_instance($instance);
  }

  // Create an "alt label" field if it does not exist yet.
  $field_name = 'field_alt_labels';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text',
      'label'       => t('Alternative labels'),
      'description' => t('A list of synonyms'),
      'cardinality' => -1,
      'settings'    => array(
        'max_length' => 1024,
      ),
    );
    field_create_field($field);
  }

  // Attach the "alt label" field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('Alternative labels'),
      'description' => t('A list of synonyms'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textfield',
        'weight'    => 4,
      ),
    );
    field_create_instance($instance);
  }

  // Create an "hidden label" field if it does not exist yet.
  $field_name = 'field_hidden_labels';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text',
      'label'       => t('Hidden labels'),
      'description' => t('A list of secondary variants of this concept'),
      'cardinality' => -1,
      'settings'    => array(
        'max_length' => 1024,
      ),
    );
    field_create_field($field);
  }

  // Attach the "hidden label" field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('Hidden labels'),
      'description' => t('A list of secondary variants of this concept'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textfield',
        'weight'    => 5,
      ),
    );
    field_create_instance($instance);
  }

  // Create an "scope notes" field if it does not exist yet.
  $field_name = 'field_scope_notes';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text_long',
      'label'       => t('Scope notes'),
      'description' => t('An information about the scope of a concept'),
      'cardinality' => -1,
    );
    field_create_field($field);
  }

  // Attach the "scope notes" field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('Scope notes'),
      'description' => t('An information about the scope of a concept'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textarea',
        'weight'    => 6,
      ),
    );
    field_create_instance($instance);
  }

  // Create an "related concepts" field if it does not exist yet.
  $field_name = 'field_related_concepts';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text',
      'label'       => t('Related concepts'),
      'description' => t('URIs to related concepts'),
      'cardinality' => -1,
      'settings'    => array(
        'max_length' => 1024,
      ),
    );
    field_create_field($field);
  }

  // Attach the "related concepts" field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('Related concepts'),
      'description' => t('URIs to related concepts'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textfield',
        'weight'    => 7,
      ),
    );
    field_create_instance($instance);
  }

  // Create an "exact match" field if it does not exist yet.
  $field_name = 'field_exact_match';
  $field      = field_info_field($field_name);
  if (is_null($field)) {
    $field = array(
      'field_name'  => $field_name,
      'type'        => 'text',
      'label'       => t('Exact matches'),
      'description' => t('URIs which show to the same concept at a different data source'),
      'cardinality' => -1,
      'settings'    => array(
        'max_length' => 1024,
      ),
    );
    field_create_field($field);
  }

  // Attach the "exact match" field to the taxonomy if not yet attached.
  $instance = field_info_instance('taxonomy_term', $field_name, $machine_name);
  if (is_null($instance)) {
    $instance = array(
      'field_name'  => $field_name,
      'entity_type' => 'taxonomy_term',
      'bundle'      => $machine_name,
      'label'       => t('Exact matches'),
      'description' => t('URIs which show to the same concept at a different data source'),
      'required'    => FALSE,
      'widget'      => array(
        'type'      => 'text_textfield',
        'weight'    => 8,
      ),
    );
    field_create_instance($instance);
  }

  // Make the vocabulary localized for concept translations.
  if (powertagging_translation_possible() && (!isset($taxonomy->i18n_mode) || $taxonomy->i18n_mode != I18N_MODE_LOCALIZE)) {
    db_update('taxonomy_vocabulary')
      ->fields(array('i18n_mode' => I18N_MODE_LOCALIZE))
      ->condition('vid', $taxonomy->vid)
      ->execute();
  }
}

/**
 * Start the indexing-process for a PowerTagging configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $powertagging_config
 *   A PowerTagging configuration.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_index_form($form, &$form_state, $powertagging_config) {
  // Check if the batch is running in background.
  $bid = powertagging_batch_running('admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id . '/index');
  if ($bid !== FALSE) {
    drupal_goto('batch', array('query' => array('op' => 'start', 'id' => $bid)));
    exit();
  }

  $field_instances = powertagging_get_tagging_field_instances($powertagging_config->powertagging_id);

  if (!empty($field_instances)) {
    // Check if the extraction model is up to date.
    $extraction_model_notifications = _powertagging_check_extraction_models($powertagging_config, FALSE);
    if (!empty($extraction_model_notifications)) {
      // Fixed values for the formatter.
      $form['extraction_model_refresh_required'] = array(
        '#type' => 'markup',
        '#markup' => '<div class="messages warning">' . $extraction_model_notifications[0] . '</div>',
      );
    }

    // Fixed values for the formatter.
    $form['powertagging_config'] = array(
      '#type' => 'value',
      '#value' => $powertagging_config,
    );

    $form['content_types'] = array(
      '#title' => t('Entity types to be included in the batch process'),
      '#type' => 'checkboxes',
      '#options' => powertagging_get_tagging_field_options_list($field_instances, TRUE, TRUE),
      '#required' => TRUE,
    );

    $form['skip_tagged_content'] = array(
      '#title' => t('Skip already tagged content'),
      '#type' => 'radios',
      '#options' => array(
        '1' => t('Yes'),
        '0' => t('No'),
      ),
      '#default_value' => TRUE,
    );

    // Date selection.
    $form['use_date'] = array(
      '#type' => 'checkbox',
      '#title' => t('Restrict the content on time basis'),
      '#default_value' => FALSE,
    );

    $form['created_date_select'] = array(
      '#type' => 'fieldset',
      '#title' => t('Date restriction'),
      '#states' => array(
        'visible' => array(
          ':input[name="use_date"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['created_date_select']['date_from'] = array(
      '#type' => 'date',
      '#title' => t('Created - From'),
      '#description' => t('Only tag content created from this day on. (start of day)'),
    );

    $form['created_date_select']['date_to'] = array(
      '#type' => 'date',
      '#title' => t('Created - To'),
      '#description' => t('Only tag content created to this day. (end of day)'),
    );

    $form['entities_per_request'] = array(
      '#type' => 'textfield',
      '#title' => t('Entities per request'),
      '#description' => t('The number of entities, that get processed during one HTTP request. (Allowed value range: 1 - 100)') . '<br />' . t('The higher this number is, the less HTTP requests have to be sent to the server until the batch finished tagging ALL your entities, what results in a shorter duration of the bulk tagging process.') . '<br />' . t('Numbers too high can result in a timeout, which will break the whole bulk tagging process.') . '<br />' . t('If entities are configured to get tagged with uploaded files, a value of 5 or below is recommended.'),
      '#required' => TRUE,
      '#default_value' => '10',
    );

    if (module_exists('background_batch')) {
      global $user;
      $form['inform_user'] = array(
        '#type' => 'checkbox',
        '#title' => 'Send an e-mail out when the process is completed',
      );
      $form['inform_user_email_container'] = array(
        '#type' => 'container',
        '#attributes' => array('style' => array('padding-left:18px;')),
      );
      $form['inform_user_email_container']['inform_user_email'] = array(
        '#type' => 'textfield',
        '#title' => t('E-mail address'),
        '#description' => t('A summary will be sent to this e-mail address when the process is completed.'),
        '#default_value' => $user->mail,
        '#states' => array(
          'visible' => array(':input[name="inform_user"]' => array('checked' => TRUE)),
        ),
      );
    } else {
      $text = t('For more usability with a large number of entities, please install the "<a href="https://www.drupal.org/project/background_process" target="_blank">Background Process</a>" module and activate the "Background Batch" sub-module.') . '<br />';
      $text .= t('This modules takes over the existing Batch API and runs batch jobs in a background process. This means that if you leave the batch page (e.g. you close the browser), the jobs continues, and you can return to the progress indicator later.');
      $form['install_info'] = array(
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#markup' => $text,
      );
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Index',
    );
    $form['cancel'] = array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id,
      '#suffix' => '</div>',
    );
  }
  else {
    drupal_set_message(t('No taggable content types found for PowerTagging configuration "%ptconfname".', array('%ptconfname' => $powertagging_config->title)), 'error');
    drupal_goto('admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id);
    exit();
  }

  // Add JavaScript
  drupal_add_js(drupal_get_path('module', 'powertagging') . '/js/powertagging.admin.js');

  return $form;
}

/**
 * Validate-Handler of the form to index a PowerTagging configuration.
 */
function powertagging_index_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $entities_per_request = $values['entities_per_request'];

  if (empty($entities_per_request) || !ctype_digit($entities_per_request) || (int) $entities_per_request == 0 || (int) $entities_per_request > 100) {
    form_set_error('entities_per_request', t('Only values in the range of 1 - 100 are allowed for field "Entities per request"'));
  }

  if (isset($values['inform_user']) && !empty($values['inform_user']) && !empty($values['inform_user_email']) && !valid_email_address($values['inform_user_email'])) {
    form_set_error('inform_user_email', t('Please enter a valid email address.'));
  }

  if ($values['use_date']) {
    $from = strtotime($values['date_from']['year'] . '-' . $values['date_from']['month'] . '-' . $values['date_from']['day']);
    $to = strtotime($values['date_to']['year'] . '-' . $values['date_to']['month'] . '-' . $values['date_to']['day']);

    if ($from > $to) {
      form_set_error('created-date-select', t('Please use a valid date range.'));
    }
  }
}

/**
 * Submit-Handler of the form to index a PowerTagging configuration.
 */
function powertagging_index_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Set the date filter.
  $date_filter = [
    'from' => NULL,
    'to' => NULL,
  ];
  if ($values['use_date']) {
    $date_filter['from'] = strtotime($values['date_from']['year'] . '-' . $values['date_from']['month'] . '-' . $values['date_from']['day']);
    // To = selected date + 1 day to simulate until the end of the selected day.
    $date_filter['to'] = strtotime($values['date_to']['year'] . '-' . $values['date_to']['month'] . '-' . $values['date_to']['day']) + 86400;
  }

  // Set the content types.
  $content_types = array();
  foreach ($values['content_types'] as $content_type_id => $value) {
    if (!empty($value)) {
      $content_types[] = $content_type_id;
    }
  }

  powertagging_index($values['powertagging_config'], array(
    'content_types' => $content_types,
    'skip_tagged_content' => $values['skip_tagged_content'],
    'date_filter' => $date_filter,
    'entities_per_request' => $values['entities_per_request'],
    'inform_user_email' => (isset($values['inform_user']) && !empty($values['inform_user']) && !empty($values['inform_user_email'])) ? $values['inform_user_email'] : '',
  ));
}

/**
 * Start the taxonomy term updating for a PowerTagging configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $powertagging_config
 *   A PowerTagging configuration.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_update_taxonomy_form($form, &$form_state, $powertagging_config) {
  // Check if the batch is running in background.
  $bid = powertagging_batch_running('admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id . '/update-taxonomy');
  if ($bid !== FALSE) {
    drupal_goto('batch', array('query' => array('op' => 'start', 'id' => $bid)));
    exit();
  }

  // Taxonomy was configured already.
  if (isset($powertagging_config->config['projects'][$powertagging_config->project_id]) && isset($powertagging_config->config['projects'][$powertagging_config->project_id]['taxonomy_id']) && $powertagging_config->config['projects'][$powertagging_config->project_id]['taxonomy_id'] != 0) {
    $vocabulary = taxonomy_vocabulary_load($powertagging_config->config['projects'][$powertagging_config->project_id]['taxonomy_id']);
    // Taxonomy exists.
    if ($vocabulary !== FALSE) {
      $form['info'] = array(
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#markup' => t('This process updates the concept-details of all taxonomy terms of the connected vocabulary used to save the PowerTagging tags (name, altLabels, ...).'),
      );

      // Fixed values for the formatter.
      $form['powertagging_config'] = array(
        '#type' => 'value',
        '#value' => $powertagging_config,
      );

      $form['vocabulary_name'] = array(
        '#type' => 'value',
        '#value' => $vocabulary->name,
      );

      if (module_exists('background_batch')) {
        global $user;
        $form['inform_user'] = array(
          '#type' => 'checkbox',
          '#title' => 'Send an e-mail out when the process is completed',
        );
        $form['inform_user_email_container'] = array(
          '#type' => 'container',
          '#attributes' => array('style' => array('padding-left:18px;')),
        );
        $form['inform_user_email_container']['inform_user_email'] = array(
          '#type' => 'textfield',
          '#title' => t('E-mail address'),
          '#description' => t('A summary will be sent to this e-mail address when the process is completed.'),
          '#default_value' => $user->mail,
          '#states' => array(
            'visible' => array(':input[name="inform_user"]' => array('checked' => TRUE)),
          ),
        );
      } else {
        $text = t('For more usability with a large number of entities, please install the "<a href="https://www.drupal.org/project/background_process" target="_blank">Background Process</a>" module and activate the "Background Batch" sub-module.') . '<br />';
        $text .= t('This modules takes over the existing Batch API and runs batch jobs in a background process. This means that if you leave the batch page (e.g. you close the browser), the jobs continues, and you can return to the progress indicator later.');
        $form['install_info'] = array(
          '#prefix' => '<div class="form-item">',
          '#suffix' => '</div>',
          '#markup' => $text,
        );
      }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Update vocabulary',
      );
      $form['cancel'] = array(
        '#type' => 'link',
        '#title' => t('Cancel'),
        '#href' => 'admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id,
        '#suffix' => '</div>',
      );

    }
    else {
      drupal_set_message(t('The connected vocabulary does not exist anymore for PowerTagging configuration "%ptconfname".', array('%ptconfname' => $powertagging_config->title)), 'error');
      drupal_goto('admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id);
      exit();
    }
  }
  else {
    drupal_set_message(t('There was no vocabulary created for PowerTagging configuration "%ptconfname" yet.', array('%ptconfname' => $powertagging_config->title)), 'error');
    drupal_goto('admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id);
    exit();
  }

  // Add JavaScript
  drupal_add_js(drupal_get_path('module', 'powertagging') . '/js/powertagging.admin.js');

  return $form;
}

/**
 * Validate-Handler of the form to update a taxonomy of PowerTagging configuration.
 */
function powertagging_update_taxonomy_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  if (isset($values['inform_user']) && !empty($values['inform_user']) && !empty($values['inform_user_email']) && !valid_email_address($values['inform_user_email'])) {
    form_set_error('inform_user_email', t('Please enter a valid email address.'));
  }

  return $form;
}

/**
 * Submit-Handler of powertagging_update_taxonomy_form().
 */
function powertagging_update_taxonomy_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $powertagging_config = $values['powertagging_config'];

  // Get the URIs of all concepts in the connected taxonomy.
  $tid_query = db_select('taxonomy_vocabulary', 'v')
    ->fields('u', array('entity_id'))
    ->condition('v.vid', $powertagging_config->config['projects'][$powertagging_config->project_id]['taxonomy_id']);
  $tid_query->join('field_data_field_uri', 'u', 'u.bundle = v.machine_name AND u.entity_type = \'taxonomy_term\'');
  $tids = $tid_query->execute()
    ->fetchCol();

  $batch_infos = array(
    'total' => count($tids),
    'start_time' => time(),
    'inform_user_email' => (isset($values['inform_user']) && !empty($values['inform_user']) && !empty($values['inform_user_email'])) ? $values['inform_user_email'] : '',
  );

  $tid_chunks = array_chunk($tids, 10);
  $batch_operations = array();
  foreach ($tid_chunks as $tid_chunk) {
    $batch_operations[] = array('powertagging_update_taxonomy_batch', array($tid_chunk, $powertagging_config->powertagging_id, $batch_infos));
  }

  $batch = array(
    'title' => t('Update vocabulary'),
    'operations' => $batch_operations,
    'init_message' => t('Preparing the updating of vocabulary "@taxonomy_name".', array('@taxonomy_name' => $values['vocabulary_name'])),
    'progress_message' => '',
    'finished' => 'powertagging_update_taxonomy_batch_finished',
  );

  batch_set($batch);
  return TRUE;
}

/**
 * Checks if the background batch is running.
 *
 * @param string $url
 *   The URL of the source.
 *
 * @return int|boolean
 *   The batch ID or FALSE if the batch is not running.
 */
function powertagging_batch_running($url) {
  // Check if the Background Batch module is enabled.
  if (!module_exists('background_batch')) {
    return FALSE;
  }

  // Check if the appropriate batch is running.
  $bids = db_select('batch', 'b')
    ->fields('b', array('bid', 'batch'))
    ->orderBy('b.bid', 'ASC')
    ->execute()
    ->fetchAllKeyed(0, 1);

  foreach ($bids as $bid => $data) {
    $batch = unserialize($data);
    if ($batch['source_url'] == $url) {
      return $bid;
      break;
    }
  }

  return FALSE;
}

/**
 * Start the taxonomy term updating for a PowerTagging configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $powertagging_config
 *   A PowerTagging configuration.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_refresh_extraction_model_form($form, &$form_state, $powertagging_config) {
  // Fixed values for the formatter.
  $form['powertagging_config'] = array(
    '#type' => 'value',
    '#value' => $powertagging_config,
  );

  // Get the project label.
  $connection_config = $powertagging_config->connection->getConfig();
  $project_label = t('Project label not found');
  if (isset($connection_config['projects'])) {
    foreach ($connection_config['projects'] as $project) {
      if ($project->id == $powertagging_config->project_id) {
        $project_label = $project->title;
        break;
      }
    }
  }
  $form['powertagging_project_label'] = array(
    '#type' => 'value',
    '#value' => $project_label,
  );

  return confirm_form($form,
    t('Are you sure you want to refresh the extraction model of PoolParty project "%projectname"?', array('%projectname' => $project_label)),
    'admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id,
    t('This process makes the PoolParty extraction use the latest version of the thesaurus.'),
    t('Refresh the extraction model'));
}

/**
 * Submit-Handler of powertagging_refresh_extraction_model_form().
 */
function powertagging_refresh_extraction_model_form_submit($form, &$form_state) {
  $powertagging_config = $form_state['values']['powertagging_config'];

  /** @var \SemanticConnectorPPTApi $ppt_api */
  $ppt_api = $powertagging_config->connection
    ->getApi('PPT');

  $result = $ppt_api->refreshExtractionModel($powertagging_config->project_id);
  if ($result['success']) {
    drupal_set_message(t('Successfully refreshed the extraction model for project "%projectname".', array('%projectname' => $form_state['values']['powertagging_project_label'])));

    // If there are any global notifications and they could be caused by a missing
    // sync, refresh the notifications.
    $notifications = variable_get('semantic_connector_global_notifications', array());
    if (!empty($notifications)) {
      $notification_config = _semantic_connector_get_global_notification_config();
      if (isset($notification_config['actions']['powertagging_refresh_extraction_model']) && $notification_config['actions']['powertagging_refresh_extraction_model']) {
        _semantic_connector_global_notification_check(TRUE);
      }
    }
  }
  else {
    drupal_set_message(t('An error occurred while refreshing the extraction model for project "%projectname".', array('@projectname' => $form_state['values']['powertagging_project_label']) . (isset($result['message']) && !empty($result['message'])) ? ' message: ' . $result['message'] : ''), 'error');
  }

  drupal_goto('admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id);
}

/**
 * A form to update the PP server connection of a PowerTagging configuration.
 *
 * This form is used when creating a completely new PowerTagging configuration
 * or when the PoolParty server connection needs to be changed or a different
 * project shell be used for an existing PowerTagging configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param string $entity_type
 *   The type of content to extract entities for.
 * @param string $bundle
 *   The bundle to extract entities for.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_extract_entities_batch_form($form, &$form_state, $entity_type, $bundle) {
  $extraction_config = _powertagging_get_entity_extraction_settings($entity_type, $bundle);
  if ($extraction_config['enabled']) {
    $form['entity_type'] = [
      '#type' => 'value',
      '#value' => $entity_type,
    ];
    $form['bundle'] = [
      '#type' => 'value',
      '#value' => $bundle,
    ];

    /** @var \SemanticConnectorPPServerConnection $pp_server_connection */
    $pp_server_connection = SemanticConnector::getConnection('pp_server', $extraction_config['connection_id']);
    // Add information about the planned extraction.
    $info_markup = '<p id="powertagging-extraction-info">';
    $info_markup .= t('PoolParty server to use') . ': <b>' . $pp_server_connection->getTitle() . ' (' . $pp_server_connection->getUrl() . ')</b><br />';

    $all_entity_type_labels = [
      'location' => t('Locations'),
      'organization' => t('Organizations'),
      'person' => t('People'),
    ];
    $entity_type_labels = [];
    foreach ($extraction_config['types'] as $type) {
      $entity_type_labels[] = $all_entity_type_labels[$type];
    }
    $info_markup .= t('Entity types to extract') . ': <b>' . implode(', ', $entity_type_labels) . '</b><br />';

    $field_instances = field_info_instances($entity_type, $bundle);
    $field_labels = [];
    foreach ($extraction_config['fields'] as $field_id) {
      $field_labels[] = $field_instances[$field_id]['label'];
    }
    $info_markup .= t('Fields to extract entities for') . ': <b>' . implode(', ', $field_labels) . '</b><br />';

    // Build nodes counts.
    $total_nodes_query = db_select('node', 'n');
    $total_nodes_query->fields('n', ['nid'])
      ->condition('n.type', $bundle);
    $total_count = $total_nodes_query->countQuery()->execute()->fetchField();

    $extraction_nodes_query = db_select('node', 'n');
    $extraction_nodes_query->fields('n', ['nid'])
      ->condition('n.type', $bundle);
    $extraction_nodes_query->join('powertagging_entity_extraction_cache', 'c', 'c.entity_id = n.nid AND c.entity_type = \'node\'');
    $extraction_nodes_query->distinct();
    $extracted_count = $extraction_nodes_query->countQuery()->execute()->fetchField();

    $info_markup .= '<br />' . t('Nodes of this type with extracted entities') . ': <b>' . $extracted_count . ' / ' . $total_count . '</b>';

    $info_markup .= '</p>';
    $form['info_markup'] = array(
      '#type' => 'markup',
      '#markup' => $info_markup,
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Extract entities',
    );
    $form['cancel'] = array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => (isset($_GET['destination']) ? $_GET['destination'] : '<front>'),
    );

  }
  else {
    $form['error_markup'] = array(
      '#type' => 'markup',
      '#markup' => '<div class="messages error">' . t('Entity extraction is not enabled for the selected entity type / bundle.') . '</div>',
    );
  }

  return $form;
}

/**
 * Submit handler for the extract entities batch form.
 */
function powertagging_extract_entities_batch_form_submit($form, &$form_state) {
  $entity_type = $form_state['values']['entity_type'];
  $bundle = $form_state['values']['bundle'];

  $nid_query = db_select('node', 'n');
  $nid_query->fields('n', ['nid'])
    ->condition('n.type', $bundle);
  $nids = $nid_query->execute()->fetchCol();
  // Each node is one batch operation.
  $operations = [];
  foreach ($nids as $nid) {
    $operations[] = array(
      'powertagging_extract_entities_batch',
      array($entity_type, $nid),
    );
  }

  if (!empty($operations)) {
    // Delete old entries first.
    db_delete('powertagging_entity_extraction_cache')
      ->condition('entity_type', $entity_type)
      ->condition('bundle', $bundle)
      ->execute();

    $batch = array(
      'title' => t('Extracting entities for node type "@nodetype"', ['@nodetype' => node_type_get_name($bundle)]),
      'operations' => $operations,
      'init_message' => t('Starting the extraction of the entities.'),
      'progress_message' => 'Extracting entities.',
      'finished' => 'powertagging_extract_entities_batch_finished',
    );

    // Start the batch to extract new entities.
    batch_set($batch);
  }
  else {
    drupal_set_message(t('There is no content available to extract entities for.'), 'warning');
    drupal_goto((isset($_GET['destination']) ? $_GET['destination'] : '<front>'));
  }
}

/**
 * Callback function for the autocomplete field to search taxonomies.
 *
 * @param string $string
 *   The search string.
 */
function powertagging_autocomplete_taxonomies($string) {
  $taxonomy_names = array();
  $result = db_select('taxonomy_vocabulary', 'v')
    ->fields('v', array('name'))
    ->condition('name', '%' . db_like($string) . '%', 'LIKE')
    ->range(0, 10)
    ->orderBy('name')
    ->execute();

  foreach ($result as $row) {
    $taxonomy_names[$row->name] = check_plain($row->name);
  }

  drupal_json_output($taxonomy_names);
}

/**
 * Create a machine name from a given name.
 *
 * @param string $name
 *   Name of the skos importer.
 *
 * @return string
 *   The machine name.
 */
function powertagging_machine_name($name) {
  return substr(preg_replace('/[^a-z0-9]+/', '_', strtolower($name)), 0, 255);
}
