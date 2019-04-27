<?php

/**
 * @file
 * Administration functionality for the PowerTagging Corpus Management module.
 */

/**
 * List all existing corpora of all connected PoolParty connections.
 */
function powertagging_corpus_list_corpora($form, &$form_state) {
  $form = array();

  $form['powertagging_corpus_title'] = array(
    '#type' => 'markup',
    '#markup' => '<h3 class="semantic-connector-table-title">' . t('PowerTagging Corpus Management - Corpora List') . '</h3>',
  );

  $connections = SemanticConnector::getConnectionsByType('pp_server');
  if (!empty($connections)) {
    $connection_options = array();
    /* @var $connection SemanticConnectorConnection */
    foreach ($connections as $connection) {
      $connection_options[$connection->getId()] = $connection->getTitle();
    }
    $form['pp_connections'] = array(
      '#type' => 'select',
      '#title' => t('Select the PoolParty server'),
      '#options' => $connection_options,
      '#default_value' => key($connection_options),
      '#ajax' => array(
        'callback' => 'powertagging_corpus_load_corpora_info',
        'wrapper' => 'powertagging-corpus-corpora-table',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#attributes' => array(
        'autocomplete' => 'off',
      ),
    );

    /* @var $connection SemanticConnectorConnection */
    foreach ($connections as $connection) {
      $projects = $connection->getApi('PPT')->getProjects();

      // Get the project options for the currently configured PoolParty server.
      $project_options = array();
      foreach ($projects as $project) {
        $project_options[$project->id] = $project->title;
      }

      // Configuration set admin page.
      $form['pp_projects_' . $connection->getId()] = array(
        '#type' => 'select',
        '#title' => 'Select a project',
        '#prefix' => '<div id="projects-replace">',
        '#suffix' => '</div>',
        '#options' => $project_options,
        '#default_value' => key($project_options),
        '#ajax' => array(
          'callback' => 'powertagging_corpus_load_corpora_info',
          'wrapper' => 'powertagging-corpus-corpora-table',
          'method' => 'replace',
          'effect' => 'fade',
        ),
        '#states' => array(
          'visible' => array(
            ':input[name="pp_connections"]' => array('value' => $connection->getId()),
          ),
        ),
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
      );

      // Set a default value.
      if (!isset($form_state['values']['pp_projects_' . $connection->getId()])) {
        $form_state['values']['pp_projects_' . $connection->getId()] = key($project_options);
      }

      $credentials = $connection->getCredentials();
      $connection_options[implode('|', array(
        $connection->getTitle(),
        $connection->getUrl(),
        $credentials['username'],
        $credentials['password'],
      ))] = $connection->getTitle();
    }

    // Set a default value.
    if (!isset($form_state['values']['pp_connections'])) {
      $form_state['values']['pp_connections'] = key($connection_options);
    }
  }

  $analysis_running = FALSE;

  // Add a default corpus list table.
  $rows = array();
  if (isset($form_state['values'])) {
    $values = $form_state['values'];
    if (isset($values['pp_connections']) && isset($values['pp_projects_' . $values['pp_connections']]) && !empty($values['pp_projects_' . $values['pp_connections']])) {
      $connection = SemanticConnector::getConnection('pp_server', $values['pp_connections']);
      $project_id = $values['pp_projects_' . $values['pp_connections']];

      /** @var \SemanticConnectorPPTApi $ppt_api */
      $ppt_api = $connection->getApi('PPT');
      $corpora = $ppt_api->getCorpora($project_id);
      if (!empty($corpora)) {
        $pp_languages = $ppt_api->getLanguages();
        $powertagging_configs = powertagging_config_load();
        $analysis_running = $ppt_api->isCorpusAnalysisRunning($project_id);
        foreach ($corpora as $corpus) {
          // List all PowerTagging configurations connected the corpora.
          $powertagging_links = array();
          foreach ($powertagging_configs as $powertagging_config) {
            // Correct connection and project.
            if ($powertagging_config->connection_id == $connection->getId() && $powertagging_config->project_id == $project_id) {
              $project_settings = $powertagging_config->config['projects'][$powertagging_config->project_id];
              // Correct corpus.
              if (isset($project_settings['corpus_id']) && $project_settings['corpus_id'] == $corpus['corpusId']) {
                $powertagging_links[] = l($powertagging_config->title, 'admin/config/semantic-drupal/powertagging/' . $powertagging_config->powertagging_id);
              }
            }
          }

          // Build the metadata.
          $metadata = $ppt_api->getCorpusMetadata($project_id, $corpus['corpusId']);
          $metadata_array = array(
            t('Created') . ': ' . format_date(strtotime($metadata['created']), 'short'),
            t('Last modified') . ': ' . format_date(strtotime($metadata['lastModified']), 'short'),
            t('Language') . ': ' . $pp_languages[$metadata['language']],
            t('Stored documents') . ': ' . $metadata['storedDocuments'],
            t('Extracted terms') . ': ' . $metadata['extractedTerms'],
          );

          // Set the operations.
          $actions = array(
            l(t('Add content'), 'admin/config/semantic-drupal/powertagging/powertagging-corpus/add-content-to-corpus/' . $connection->getId() . '/' . $project_id . '/' . $corpus['corpusId'], array('attributes' => array('class' => $metadata['quality'] == 'good' ? array('semantic-connector-italic') : array()))),
            l(t('Analyze corpus'), 'admin/config/semantic-drupal/powertagging/powertagging-corpus/analyze-corpus/' . $connection->getId() . '/' . $project_id . '/' . $corpus['corpusId'], array('attributes' => array('class' => ($analysis_running || $corpus['upToDate']) ? array('semantic-connector-italic') : array())))
          );

          // Create the row.
          $rows[] = array(
            $corpus['corpusName'],
            '<div class="semantic-connector-led led-' . ($metadata['quality'] == 'good' ? 'green' : ($metadata['quality'] == 'moderate' ? 'yellow' : 'red') . '" title="' . $metadata['quality']) . '"></div>' . (!empty($metadata['quality']) ? $metadata['quality'] : 'no analysis run yet'),
            '<ul><li>' . implode('</li><li>', $metadata_array) . '</li></ul>',
            $corpus['upToDate'] ? t('yes') : '<b>' . t('NO') . '</b>',
            (!empty($powertagging_links) ? '<ul><li>' . implode('</li><li>', $powertagging_links) . '</li></ul>' : '-'),
            implode(' | ', $actions),
          );
        }
      }
    }
  }

  $form['powertagging_corpus_configurations'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Corpus name'),
      t('Quality'),
      t('Additional information'),
      t('Up to date'),
      t('Connected PowerTagging configurations'),
      t('Operations'),
    ),
    '#rows' => $rows,
    '#empty' => t('The selected project does not yet have any corpora.'),
    '#caption' => $analysis_running ? '<div class="messages warning">' . t('A corpus analysis is currently running for one of the corpora of the selected project.') . '</div>' : NULL,
    '#attributes' => array(
      'id' => 'powertagging-corpus-corpora-table',
      //'class' => array('semantic-connector-tablesorter'),
    ),
    /*'#attached' => array(
      'js' => array(drupal_get_path('module', 'powertagging_corpus') . '/js/powertagging_corpus.admin.js'),
    ),*/
  );

  return $form;
}

/**
 * Callback element needs only select the portion of the form to be updated.
 * Since #ajax['callback'] return can be HTML or a renderable array (or an
 * array of commands), we can just return a piece of the form.
 */
function powertagging_corpus_load_corpora_info($form, $form_state) {
  return $form['powertagging_corpus_configurations'];
}

/**
 * Add content to an existing PoolParty corpus.
 *
 * @param int $connection_id
 *   The ID of the PoolParty connection to use.
 * @param string $project_id
 *   The ID of the PoolParty project to use.
 * @param string $corpus_id
 *   The corpus to add the content into.
 *
 * @return array
 *   The form array.
 */
function powertagging_corpus_add_content_form($form, &$form_state, $connection_id, $project_id, $corpus_id) {
  $corpus_id = urldecode($corpus_id);

  // Check if the batch is running in background.
  module_load_include('inc', 'powertagging', 'includes/powertagging.admin');
  $bid = powertagging_batch_running('admin/config/semantic-drupal/powertagging/powertagging-corpus/add-content-to-corpus/' . $connection_id . '/' . $project_id . '/' . $corpus_id);
  if ($bid !== FALSE) {
    drupal_goto('batch', array('query' => array('op' => 'start', 'id' => $bid)));
  }

  // Check if the connection exists.
  $connection = SemanticConnector::getConnection('pp_server', $connection_id);
  if (!is_null($connection)) {
    /** @var \SemanticConnectorPPTApi $ppt_api */
    $ppt_api = $connection->getApi('PPT');

    // Check if the project exists.
    $projects = $ppt_api->getProjects();
    foreach ($projects as $project) {
      if ($project->id == $project_id) {

        // Check if the corpus exists.
        $corpora = $ppt_api->getCorpora($project_id);
        foreach ($corpora as $corpus) {
          if ($corpus_id == $corpus['corpusId']) {
            // The form was not yet submitted.
            if (!isset($form_state['storage']['confirm'])) {
              $form['connection_id'] = array(
                '#type' => 'value',
                '#value' => $connection_id,
              );
              $form['project_id'] = array(
                '#type' => 'value',
                '#value' => $project_id,
              );
              $form['corpus_id'] = array(
                '#type' => 'value',
                '#value' => $corpus_id,
              );

              $corpus_metadata = $ppt_api->getCorpusMetadata($project_id, $corpus_id);
              if (isset($corpus_metadata['quality']) && $corpus_metadata['quality'] == 'good') {
                // Date selection.
                $form['quality_warning'] = array(
                  '#type' => 'markup',
                  '#markup' => '<div class="messages warning">' . t('The quality of the selected corpus is already good, adding more content will not improve its quality any further.') . '</div>',
                );
              }

              // Date selection.
              $form['use_date'] = array(
                '#type' => 'checkbox',
                '#title' => t('Restrict the content on time basis'),
                '#default_value' => FALSE,
              );

              $form['date_select'] = array(
                '#type' => 'fieldset',
                '#title' => t('Date restriction'),
                '#states' => array(
                  'visible' => array(
                    ':input[name="use_date"]' => array('checked' => TRUE),
                  ),
                ),
              );

              $form['date_select']['date_from'] = array(
                '#type' => 'date',
                '#title' => t('From'),
                '#description' => t('Only use content created from this day on.'),
              );

              $form['date_select']['date_to'] = array(
                '#type' => 'date',
                '#title' => t('To'),
                '#description' => t('Only use content created to this day.'),
              );

              // Content selection.
              $form['content'] = array(
                '#type' => 'fieldset',
                '#title' => t('Content to push into the corpus'),
                '#tree' => TRUE,
              );

              $form['content']['entity_types'] = array(
                '#type' => 'checkboxes',
                '#title' => t('Entity types to push into the corpus'),
                '#options' => array(
                  'node' => t('Nodes'),
                  'user' => t('Users'),
                  'taxonomy_term' => t('Taxonomy Terms'),
                ),
                '#default_value' => array(),
                '#required' => TRUE,
              );

              // Nodes.
              $form['content']['node'] = array(
                '#type' => 'fieldset',
                '#title' => t('Nodes'),
                '#states' => array(
                  'visible' => array(
                    ':input[name="content[entity_types][node]"]' => array('checked' => TRUE),
                  ),
                ),
              );

              $node_types = node_type_get_types();
              $node_type_options = array();
              foreach ($node_types as $node_type) {
                $node_type_options[$node_type->type] = $node_type->name;
              }
              $form['content']['node']['node_types'] = array(
                '#type' => 'checkboxes',
                '#title' => t('Node types to push into the corpus'),
                '#options' => $node_type_options,
                '#default_value' => array(),
                '#states' => array(
                  'required' => array(
                    ':input[name="content[entity_types][node]"]' => array('checked' => TRUE),
                  ),
                ),
              );

              foreach ($node_type_options as $bundle => $node_type) {
                $bundle_fields = powertagging_field_list(array(
                  'entity_type' => 'node',
                  'bundle' => $bundle
                ), 'instance_settings_form');
                $form['content']['node'][$bundle] = array(
                  '#type' => 'checkboxes',
                  '#title' => t('Fields of node type "%nodetype" to push into the corpus', array('%nodetype' => $node_type)),
                  '#options' => $bundle_fields,
                  '#default_value' => array(),
                  '#states' => array(
                    'visible' => array(
                      ':input[name="content[node][node_types][' . $bundle . ']"]' => array('checked' => TRUE),
                    ),
                    'required' => array(
                      ':input[name="content[node][node_types][' . $bundle . ']"]' => array('checked' => TRUE),
                    ),
                  ),
                );
              }

              // Users.
              $form['content']['user'] = array(
                '#type' => 'fieldset',
                '#title' => t('Users'),
                '#states' => array(
                  'visible' => array(
                    ':input[name="content[entity_types][user]"]' => array('checked' => TRUE),
                  ),
                ),
              );

              $bundle_fields = powertagging_field_list(array(
                'entity_type' => 'user',
                'bundle' => 'user'
              ), 'instance_settings_form');
              if (!empty($bundle_fields)) {
                $form['content']['user']['user'] = array(
                  '#type' => 'checkboxes',
                  '#title' => t('Fields of users to push into the corpus'),
                  '#options' => $bundle_fields,
                  '#default_value' => array(),
                  '#states' => array(
                    'required' => array(
                      ':input[name="content[entity_types][user]"]' => array('checked' => TRUE),
                    ),
                  ),
                );
              }
              else {
                $form['content']['user']['empty'] = array(
                  '#type' => 'markup',
                  '#markup' => t('<div class="messages warning">' . t('Users currently don\'t have any fields, which could be used as data for the corpus.') . '</div>'),
                );
              }

              // Taxonomy Terms.
              $form['content']['taxonomy_term'] = array(
                '#type' => 'fieldset',
                '#title' => t('Taxonomy Terms'),
                '#states' => array(
                  'visible' => array(
                    ':input[name="content[entity_types][taxonomy_term]"]' => array('checked' => TRUE),
                  ),
                ),
              );

              $vocabularies = taxonomy_get_vocabularies();
              $vocabulary_options = array();
              foreach ($vocabularies as $vocabulary) {
                $vocabulary_options[$vocabulary->vid] = $vocabulary->name;
              }
              $form['content']['taxonomy_term']['vocabularies'] = array(
                '#type' => 'checkboxes',
                '#title' => t('Vocabularies to push into the corpus'),
                '#options' => $vocabulary_options,
                '#default_value' => array(),
                '#states' => array(
                  'required' => array(
                    ':input[name="content[entity_types][taxonomy_term]"]' => array('checked' => TRUE),
                  ),
                ),
              );

              foreach ($vocabularies as $vocabulary) {
                $bundle = $vocabulary->vid;
                $bundle_fields = powertagging_field_list(array(
                  'entity_type' => 'taxonomy_term',
                  'bundle' => $vocabulary->machine_name
                ), 'instance_settings_form');
                $form['content']['taxonomy_term'][$bundle] = array(
                  '#type' => 'checkboxes',
                  '#title' => t('Fields of vocabulary "%vocabularyname" to push into the corpus', array('%vocabularyname' => $vocabulary->name)),
                  '#options' => $bundle_fields,
                  '#default_value' => array(),
                  '#states' => array(
                    'visible' => array(
                      ':input[name="content[taxonomy_term][vocabularies][' . $bundle . ']"]' => array('checked' => TRUE),
                    ),
                    'required' => array(
                      ':input[name="content[taxonomy_term][vocabularies][' . $bundle . ']"]' => array('checked' => TRUE),
                    ),
                  ),
                );
              }

              $form['entities_per_request'] = array(
                '#type' => 'textfield',
                '#title' => t('Entities per request'),
                '#description' => t('The number of entities, that get processed during one HTTP request. (Allowed value range: 1 - 100)') . '<br />' . t('The higher this number is, the less HTTP requests have to be sent to the server until the batch finished pushing ALL your entities into the corpus, what results in a shorter duration of the bulk pushing process.') . '<br />' . t('Numbers too high can result in a timeout, which will break the whole bulk pushing process.') . '<br />' . t('If entities are configured to get pushed with uploaded files, a value of 5 or below is recommended.'),
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

              // Save and cancel buttons.
              $form['submit'] = array(
                '#type' => 'submit',
                '#value' => t('Push content'),
              );
              $form['cancel'] = array(
                '#type' => 'link',
                '#title' => t('Cancel'),
                '#href' => 'admin/config/semantic-drupal/powertagging/powertagging-corpus',
                '#suffix' => '</div>',
              );

              return $form;
            }
            // The form was already submitted --> displayed the confirm form.
            else {
              // Add basic information about the connection parameters.
              $content = '<p>';
              $content .= t('PoolParty connection') .  ': <b>' . $connection->getTitle() . '</b><br />';
              $content .= t('Project') .  ': <b>' . $project->title . '</b><br />';
              $content .= t('Corpus') .  ': <b>' . $corpus['corpusName'] . '</b>';
              $content .= '</p>';

              // Add information about the content to push into the corpus.
              $content .= '<p>Content that gets pushed into the corpus:</p>';
              $start_date = 0;
              $end_date = 0;
              if ($form_state['values']['use_date']) {
                $start_date = strtotime($form_state['values']['date_from']['year'] . '-' . $form_state['values']['date_from']['month'] . '-' . $form_state['values']['date_from']['day']);
                $end_date = strtotime($form_state['values']['date_to']['year'] . '-' . $form_state['values']['date_to']['month'] . '-' . $form_state['values']['date_to']['day']);
              }

              // Calculate the entity counts.
              $content_list = array();
              foreach ($form_state['values']['content_selected'] as $entity_type => $content_types_fields) {
                foreach (array_keys($content_types_fields) as $content_type) {
                  switch ($entity_type){
                    case "node":
                      $query = db_select('node', 'n')
                        ->fields('n', array('nid'))
                        ->condition('type', $content_type);
                      break;

                    case "taxonomy_term":
                      $query = db_select('taxonomy_term_data', 't')
                        ->fields('t', array('tid'))
                        ->condition('vid', $content_type);
                      break;

                    // Users.
                    default:
                      $query = db_select('users', 'u')
                        ->fields('u', array('uid'));
                  }

                  if ($entity_type != 'taxonomy_term' && $form_state['values']['use_date']) {
                    $query->condition('created', $start_date, '>=');
                    $query->condition('created', $end_date, '<=');
                  }

                  $entity_count = $query->execute()->rowCount();
                  $content_list[] = $entity_count . ' ' . str_replace(' ', ' ', $entity_type) . 's' . ($entity_type != 'taxonomy_term' ? ' of type "' . $content_type . '"' : '');
                }
              }

              // Show a list of entity counts.
              $content .= '<ul><li>' . implode('</li><li>', $content_list) . '</li></ul>';

              return confirm_form($form,
                t("Confirm your selection"),
                current_path(),
                $content,
                t("Confirm")
              );
            }
          }
        }
        drupal_set_message(t('The selected corpus could not be found in the PoolParty project.'), 'error');
        break;
      }
    }
    drupal_set_message(t('The selected project could not be found on the PoolParty server.'), 'error');
  }
  else {
    drupal_set_message(t('The provided PoolParty connection does not exist.'), 'error');
  }

  drupal_goto('admin/config/semantic-drupal/powertagging/powertagging-corpus');
  return $form;
}

/**
 * The validation handler for the PowerTagging Corpus Management configuration admin form.
 */
function powertagging_corpus_add_content_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // The form was not yet committed.
  if (!isset($form_state['storage']['confirm'])) {
    $entity_types = array_filter($values['content']['entity_types']);
    if (!empty($entity_types)) {
      $form_state['values']['content_selected'] = array();
      foreach ($entity_types as $entity_type) {
        switch ($entity_type) {
          case 'node':
            $node_types = array_filter($values['content'][$entity_type]['node_types']);
            if (empty($node_types)) {
              form_set_error('content[node][node_types]', t('Please select at least one node type.'));
            }
            else {
              foreach ($node_types as $node_type) {
                $field_values = array_filter($values['content'][$entity_type][$node_type]);
                if (empty($field_values)) {
                  form_set_error('content[node][node_types]', t('Please select at least one field for node type "%nodetype".', array('%nodetype' => $node_type)));
                }
                else {
                  $form_state['values']['content_selected'][$entity_type][$node_type] = $field_values;
                }
              }
            }
            break;

          case 'user':
            // If users don't have any valid fields ignore the entity type
            // selection "users".
            if (isset($values['content'][$entity_type])) {
              $field_values = array_filter($values['content'][$entity_type]['user']);
              if (empty($field_values)) {
                form_set_error('content[user][user]', t('Please select at least one field for users.'));
              }
              else {
                $form_state['values']['content_selected'][$entity_type]['user'] = $field_values;
              }
            }
            break;

          case 'taxonomy_term':
            $vocabularies = array_filter($values['content'][$entity_type]['vocabularies']);
            if (empty($vocabularies)) {
              form_set_error('content[taxonomy_term][vocabularies]', t('Please select at least one vocabulary.'));
            }
            else {
              foreach ($vocabularies as $vocabulary) {
                $field_values = array_filter($values['content'][$entity_type][$vocabulary]);
                if (empty($field_values)) {
                  form_set_error('content[taxonomy_term][vocabularies]', t('Please select at least one field for vocabulary "%vocabulary".', array('%vocabulary' => $vocabulary)));
                }
                else {
                  $form_state['values']['content_selected'][$entity_type][$vocabulary] = $field_values;
                }
              }
            }
            break;
        }
      }
    }

    if (isset($values['inform_user']) && !empty($values['inform_user']) && !empty($values['inform_user_email']) && !valid_email_address($values['inform_user_email'])) {
      form_set_error('inform_user_email', t('Please enter a valid email address.'));
    }
  }
}

/**
 * The submit handler for the PowerTagging Corpus Management configuration admin form.
 */
function powertagging_corpus_add_content_form_submit($form, &$form_state) {
  // The form gets submitted --> move on to a confirmation step.
  if (!isset($form_state['storage']['confirm'])) {
    $form_state['storage']['values'] = $form_state['values'];
    $form_state['storage']['confirm'] = TRUE;
    $form_state['rebuild'] = TRUE;
  }
  // The confirmation form was submitted, start the batch operation.
  else {
    $values = $form_state['storage']['values'];
    $entities_per_request = $values['entities_per_request'];
    $batch = array(
      'title' => t('Push entities into the corpus'),
      'operations' => array(),
      'init_message' => t('Pushing the selected content into the corpus.'),
      'progress_message' => '',
      'finished' => 'powertagging_corpus_push_entity_batch_finished',
    );

    $start_date = 0;
    $end_date = 0;
    if ($values['use_date']) {
      $start_date = strtotime($values['date_from']['year'] . '-' . $values['date_from']['month'] . '-' . $values['date_from']['day']);
      $end_date = strtotime($values['date_to']['year'] . '-' . $values['date_to']['month'] . '-' . $values['date_to']['day']);
    }

    $entities_info = array();
    foreach ($values['content_selected'] as $entity_type => $content_types_fields) {
      foreach (array_keys($content_types_fields) as $content_type) {
        $bundle = $content_type;
        switch ($entity_type){
          case "node":
            $query = db_select('node', 'n')
              ->fields('n', array('nid'))
              ->condition('type', $content_type);
            break;

          case "taxonomy_term":
            $query = db_select('taxonomy_term_data', 't')
              ->fields('t', array('tid'))
              ->condition('vid', $content_type);

            $vocabulary = taxonomy_vocabulary_load($content_type);
            $bundle = $vocabulary->machine_name;
            break;

          // Users.
          default:
            $query = db_select('users', 'u')
              ->fields('u', array('uid'));
        }

        if ($entity_type != 'taxonomy_term' && $values['use_date']) {
          $query->condition('created', $start_date, '>=');
          $query->condition('created', $end_date, '<=');
        }

        $entity_ids = $query->execute()->fetchCol();
        foreach ($entity_ids as $entity_id) {
          $entities_info[] = array(
            'id' => $entity_id,
            'entity_type' => $entity_type,
            'content_type' => $content_type,
            'bundle' => $bundle,
          );
        }
      }
    }

    $corpus_details = array(
      'connection_id' => $values['connection_id'],
      'project_id' => $values['project_id'],
      'corpus_id' => $values['corpus_id'],
    );

    $batch_infos = array(
      'total' => count($entities_info),
      'start_time' => time(),
      'inform_user_email' => (isset($values['inform_user']) && !empty($values['inform_user']) && !empty($values['inform_user_email'])) ? $values['inform_user_email'] : '',
    );

    for ($i = 0; $i < count($entities_info); $i += $entities_per_request) {
      $entities_info_slice = array_slice($entities_info, $i, $entities_per_request);
      $batch['operations'][] = array(
        'powertagging_corpus_push_entity_batch',
        array(
          $entities_info_slice,
          $values['content_selected'],
          $corpus_details,
          $batch_infos,
        ),
      );
    }

    // Add for each operation some info data.
    /*$batch_info = array(
      'inform_user_email' => $settings['inform_user_email'],
    );
    foreach ($batch['operations'] as &$operation) {
      $operation[1][] = $batch_info;
    }*/

    batch_set($batch);
  }
}

/**
 * Start an analysis of an existing PoolParty corpus.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param int $connection_id
 *   The ID of the PoolParty connection to use.
 * @param string $project_id
 *   The ID of the PoolParty project to use.
 * @param string $corpus_id
 *   The corpus to start the analysis for.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_corpus_analyze_corpus_form($form, &$form_state, $connection_id, $project_id, $corpus_id) {
  $corpus_id = urldecode($corpus_id);

  // Check if the connection exists.
  $connection = SemanticConnector::getConnection('pp_server', $connection_id);
  if (!is_null($connection)) {
    /** @var \SemanticConnectorPPTApi $ppt_api */
    $ppt_api = $connection->getApi('PPT');

    // Check if the project exists.
    $projects = $ppt_api->getProjects();
    $project_found = FALSE;
    foreach ($projects as $project) {
      if ($project->id == $project_id) {
        $project_found = TRUE;

        // Check if the corpus exists.
        $corpora = $ppt_api->getCorpora($project_id);
        foreach ($corpora as $corpus) {
          if ($corpus_id == $corpus['corpusId']) {
            $form['connection_id'] = array(
              '#type' => 'value',
              '#value' => $connection_id,
            );
            $form['project_id'] = array(
              '#type' => 'value',
              '#value' => $project_id,
            );
            $form['corpus_id'] = array(
              '#type' => 'value',
              '#value' => $corpus_id,
            );
            $form['corpus_label'] = array(
              '#type' => 'value',
              '#value' => $corpus['corpusName'],
            );

            $content = '<p>';
            $content .= t('PoolParty connection') .  ': <b>' . $connection->getTitle() . '</b><br />';
            $content .= t('Project') .  ': <b>' . $project->title . '</b><br />';
            $content .= t('Corpus') .  ': <b>' . $corpus['corpusName'] . '</b>';
            $content .= '</p>';

            $content .= '<p>' . t('This process gets the corpus up to date, improving the quality of the free term extraction.') . '</p>';

            return confirm_form($form,
              t("Are you sure you want to start the corpus analysis?"),
              'admin/config/semantic-drupal/powertagging/powertagging-corpus',
              $content,
              t("Start the corpus analysis")
            );
          }
        }
        if (!$project_found) {
          drupal_set_message(t('The selected corpus could not be found in the PoolParty project.'), 'error');
        }
        break;
      }
    }
    drupal_set_message(t('The selected project could not be found on the PoolParty server.'), 'error');
  }
  else {
    drupal_set_message(t('The provided PoolParty connection does not exist.'), 'error');
  }

  drupal_goto('admin/config/semantic-drupal/powertagging/powertagging-corpus');
  return $form;
}

/**
 * Submit-Handler of powertagging_corpus_analyze_corpus_form().
 */
function powertagging_corpus_analyze_corpus_form_submit($form, &$form_state) {
  $connection = $connection = SemanticConnector::getConnection('pp_server', $form_state['values']['connection_id']);

  /** @var \SemanticConnectorPPTApi $ppt_api */
  $ppt_api = $connection->getApi('PPT');

  if (!$ppt_api->isCorpusAnalysisRunning($form_state['values']['project_id'])) {
    $result = $ppt_api->analyzeCorpus($form_state['values']['project_id'], $form_state['values']['corpus_id']);
    if ($result['success']) {
      drupal_set_message(t('Successfully started an analysis for corpus "%corpusname".', array('%corpusname' => $form_state['values']['corpus_label'])));
    }
    else {
      drupal_set_message(t('An error occurred while starting the analysis of corpus "%corpusname".', array('@corpusname' => $form_state['values']['corpus_label']) . (isset($result['message']) && !empty($result['message'])) ? ' message: ' . $result['message'] : ''), 'error');
    }
  }
  else {
    drupal_set_message(t('There is already a corpus analysis running for the selected project. Only one corpus analysis for a PoolParty project can run at a time.'), 'error');
  }

  drupal_goto('admin/config/semantic-drupal/powertagging/powertagging-corpus');
}