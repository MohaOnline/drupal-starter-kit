<?php

/**
 * @file
 * All hooks required for the powertagging field.
 */

/**
 * Implements hook_field_info().
 */
function powertagging_field_info() {
  return array(
    'powertagging' => array(
      'label' => t('PowerTagging Tags'),
      'settings' => array(
        'powertagging_id' => '',
      ),
      'instance_settings' => array(
        'fields' => array(),
      ),
      'description' => t('This field stores concepts in the database.'),
      'default_widget' => 'powertagging_ui',
      'default_formatter' => 'powertagging_list',
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function powertagging_field_settings_form($field, $instance, $has_data) {
  $settings = $field['settings'];

  $powertagging_configs = powertagging_config_load();
  $powertagging_config_options = array();
  foreach ($powertagging_configs as $powertagging_config) {
    $powertagging_config_options[$powertagging_config->powertagging_id] = $powertagging_config->title;
  }

  $form = array();
  $form['powertagging_id'] = array(
    '#type' => 'select',
    '#title' => t('Select the PowerTagging Configuration'),
    '#options' => $powertagging_config_options,
    '#default_value' => $settings['powertagging_id'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Implements hook_field_instance_settings_form().
 */
function powertagging_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];
  $form = array();

  if (!empty($field['settings']['powertagging_id'])) {
    $powertagging_config = powertagging_config_load($field['settings']['powertagging_id']);
    $entity_fields = field_info_instances($instance['entity_type'], $instance['bundle']);

    $form['powertagging_id'] = array(
      '#type' => 'hidden',
      '#value' => $powertagging_config->powertagging_id,
    );

    // Check if this content type already has fields with tags.
    $existing_tag_fields = array();
    foreach (field_info_fields() as $field_id => $field) {
      if (!isset($entity_fields[$field_id])) {
        continue;
      }
      switch ($field['module']) {
        // Check the Taxonomy Term Reference.
        case 'taxonomy':
          if ($field['type'] == 'taxonomy_term_reference') {
            $existing_tag_fields[$field_id] = $entity_fields[$field_id]['label'];
          }
          break;

        // Check the Entity Reference.
        case 'entityreference':
          if ($field['type'] == 'entityreference' && $field['settings']['target_type'] == 'taxonomy_term' && count($field['settings']['handler_settings']['target_bundles']) == 1) {
            $existing_tag_fields[$field_id] = $entity_fields[$field_id]['label'];
          }
      }
    }

    if (!empty($existing_tag_fields)) {
      asort($existing_tag_fields);
      $form['default_tags_field'] = array(
        '#type' => 'select',
        '#options' => $existing_tag_fields,
        '#title' => t('Use tags of an existing tags-field as default values'),
        '#default_value' => isset($settings['default_tags_field']) ? $settings['default_tags_field'] : '',
        '#empty_value' => '',
      );
    }

    $form['special'] = array(
      '#type' => 'fieldset',
      '#title' => t('Special settings'),
      '#collapsible' => FALSE,
      '#tree' => FALSE,
    );

    $form['special']['include_in_tag_glossary'] = array(
      '#type' => 'checkbox',
      '#title' => t('Include in PowerTagging Tag Glossary'),
      '#description' => t('Show tags of this field in the "PowerTagging Tag Glossary" block (if it is enabled).'),
      '#default_value' => isset($settings['include_in_tag_glossary']) ? $settings['include_in_tag_glossary'] : TRUE,
      '#empty_value' => '',
      '#parents' => array('instance', 'settings', 'include_in_tag_glossary'),
    );

    $form['special']['automatically_tag_new_entities'] = array(
      '#type' => 'checkbox',
      '#title' => t('Automatically tag new entities'),
      '#description' => t('When entities get created and don\'t have values for this field yet, they will be tagged automatically.'),
      '#default_value' => isset($settings['automatically_tag_new_entities']) ? $settings['automatically_tag_new_entities'] : FALSE,
      '#empty_value' => '',
      '#parents' => array('instance', 'settings', 'automatically_tag_new_entities'),
    );

    $field_options = powertagging_field_list($instance, 'instance_settings_form');
    $form['fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields to extract tags from'),
      '#options' => $field_options,
      '#default_value' => $settings['fields'],
      '#required' => TRUE,
      '#description' => t('Select the fields from which the concepts and free terms are extracted.'),
    );

    // Add file upload settings if the content type has the appropriate fields.
    $allowed_modules = array('file', 'media');
    $state_fields_list = array();
    foreach ($field_options as $field_name => $title) {
      $field = field_info_field($field_name);
      if (in_array($field['module'], $allowed_modules)) {
        $state_fields_list[] = ':input[name="instance[settings][fields][' . $field_name . ']"]';
      }
    }

    if (!empty($state_fields_list)) {
      $state_fields = implode(', ', $state_fields_list);
      $form['file_upload'] = array(
        '#type' => 'fieldset',
        '#title' => t('File extraction settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#states' => array(
          'visible' => array($state_fields => array('checked' => TRUE)),
        ),
      );

      // Add max file size to the form.
      $max_file_size = floor(file_upload_max_size() / 1048576);
      $max_file_size = ($max_file_size > 10) ? 10 : $max_file_size;
      $file_size_options = array();
      for ($i = 1; $i <= $max_file_size; $i++) {
        $file_size_options[$i * 1048576] = $i . ' MB';
      }
      $default_max_file_size = ($max_file_size > UPLOAD_MAX_FILE_SIZE) ? UPLOAD_MAX_FILE_SIZE : $max_file_size;
      $form['file_upload']['max_file_size'] = array(
        '#type' => 'select',
        '#title' => t('Maximum file size for each attached file'),
        '#description' => t('Only files below the specified value are used for the extraction.'),
        '#options' => $file_size_options,
        '#default_value' => (isset($settings['file_upload']['max_file_size']) ? $settings['file_upload']['max_file_size'] : ($default_max_file_size * 1048576)),
      );

      // Add max file count to the form.
      $file_count_options = array();
      for ($i = 1; $i <= 10; $i++) {
        $file_count_options[$i] = $i;
      }
      $form['file_upload']['max_file_count'] = array(
        '#type' => 'select',
        '#title' => t('Maximum number of files per node'),
        '#description' => t('Only the specified number of files are used for the extraction per node.'),
        '#options' => $file_count_options,
        '#default_value' => (isset($settings['file_upload']['max_file_count']) ? $settings['file_upload']['max_file_count']: UPLOAD_MAX_FILE_COUNT),
      );
    }

    $powertagging_mode = $powertagging_config->config['projects'][$powertagging_config->project_id]['mode'];
    $powertagging_corpus = $powertagging_config->config['projects'][$powertagging_config->project_id]['corpus_id'];
    $concepts_per_extraction = isset($settings['concepts_per_extraction']) ? $settings['concepts_per_extraction'] : $powertagging_config->config['concepts_per_extraction'];
    $concepts_threshold = isset($settings['concepts_threshold']) ? $settings['concepts_threshold'] : $powertagging_config->config['concepts_threshold'];
    $freeterms_per_extraction = isset($settings['freeterms_per_extraction']) ? $settings['freeterms_per_extraction'] : $powertagging_config->config['freeterms_per_extraction'];
    $freeterms_threshold = isset($settings['freeterms_threshold']) ? $settings['freeterms_threshold'] : $powertagging_config->config['freeterms_threshold'];
    $form['treshold'] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings for concepts / categories and free terms'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    powertagging_global_limits_form($form['treshold'], $concepts_per_extraction, $concepts_threshold, $freeterms_per_extraction, $freeterms_threshold, array('instance', 'settings'));
    foreach (array('concepts', 'freeterms') as $concept_type) {
      $form['treshold'][$concept_type]['#description'] .= '<br />' . t('Note: These settings override the global settings defined in the connected PowerTagging configuration.');
    }

    // The most part of the global limits are only visible when PowerTagging is
    // used for annotation.
    if ($powertagging_mode == 'classification') {
      $form['treshold']['concepts']['concepts_threshold']['#access'] = FALSE;
      $form['treshold']['freeterms']['#access'] = FALSE;
    }

    if ($powertagging_mode == 'annotation') {
      if (!empty($powertagging_corpus)) {
        $form['treshold']['concepts']['use_shadow_concepts'] = array(
          '#type' => 'checkbox',
          '#title' => t('Also find concepts that are not directly contained within the content'),
          '#description' => t('It searches for concepts that do not appear in the content but have something to do with it.'),
          '#default_value' => isset($settings['use_shadow_concepts']) ? $settings['use_shadow_concepts'] : FALSE,
          '#parents' => array('instance', 'settings', 'use_shadow_concepts'),
        );
      }

      $form['treshold']['concepts']['browse_concepts_charttypes'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Visually browse concepts'),
        '#options' => array(
          'spider' => 'Visual Mapper (circle visualisation)',
          'tree' => 'Tree View',
        ),
        '#default_value' => (isset($settings['browse_concepts_charttypes']) ? $settings['browse_concepts_charttypes'] : []),
        '#description' => t('If at least one of the visualisation types is selected, users can click on a button to use a visualisation to select additional concepts to use in the thesaurus.') . '<br />' . t('Selecting multiple chart types will allow the user to switch between the chart types.'),
        '#parents' => array('instance', 'settings', 'browse_concepts_charttypes'),
      );

      if (!_semantic_connector_visual_mapper_usable()) {
        $form['treshold']['concepts']['browse_concepts_charttypes']['#disabled'] = TRUE;
        $form['treshold']['concepts']['browse_concepts_charttypes']['#prefix'] = '<div class="messages warning">' . t('To enable the "Visually browse concepts" all requirements of the VisualMapper library need to be met.') . '</div>';
      }
    }

    $form['treshold']['freeterms']['custom_freeterms'] = array(
      '#type' => 'checkbox',
      '#title' => 'Allow users to add custom free terms',
      '#description' => 'If this options is enabled users can add custom free terms by writing text in the search-box of the PowerTagging widget and clicking the enter key.',
      '#default_value' => isset($settings['custom_freeterms']) ? $settings['custom_freeterms'] : TRUE,
      '#parents' => array('instance', 'settings', 'custom_freeterms'),
    );

    // Search settings.
    $form['search'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $form['search']['ac_add_matching_label'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add the matching label to every suggestion in the drop down menu.'),
      '#default_value' => isset($settings['ac_add_matching_label']) ? $settings['ac_add_matching_label'] : FALSE,
      '#parents' => array('instance', 'settings', 'ac_add_matching_label'),
    );

    $form['search']['ac_add_context'] = array(
      '#type' => 'checkbox',
      '#title' => t('Add the context (title of the concept scheme) to every suggestion in the drop down menu.'),
      '#default_value' => isset($settings['ac_add_context']) ? $settings['ac_add_context'] : FALSE,
      '#parents' => array('instance', 'settings', 'ac_add_context'),
    );
  }
  // Global field settings need to be saved first.
  else {
    $form['powertagging_error'] = array(
      '#type' => 'item',
      '#markup' => '<div class="messages error">The global field settings have to be saved before editing the field instance is possible.</div>',
    );
  }

  return $form;
}

/**
 * Implements hook_field_is_empty().
 */
function powertagging_field_is_empty($item, $field) {
  if (is_string($item) && $item !== '') {
    return FALSE;
  }

  return TRUE;
}


/**
 * Implements hook_field_widget_info().
 */
function powertagging_field_widget_info() {
  return array(
    'powertagging_ui' => array(
      'label' => t('User Interface'),
      'field types' => array('powertagging'),
      'settings' => array('size' => 60),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
      'weight' => 0,
    ),
    'powertagging_invisible' => array(
      'label' => t('Invisible Footprint'),
      'field types' => array('powertagging'),
      'settings' => array('size' => 60),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
      'weight' => 1,
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function powertagging_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'powertagging_ui':
      // Show the legend
      $legend_markup = '<div class="powertagging-legend">';
      $legend_types = array(
        'concept' => t('Concepts from the thesaurus'),
        'freeterm' => t('Free terms'),
        'disabled' => t('Already selected tags'),
      );
      foreach ($legend_types as $type => $label) {
        $legend_markup .= '<div class="powertagging-legend-item"><span id="powertagging-legend-item-colorbox-' . $type . '" class="powertagging-legend-item-colorbox">&nbsp;</span>' . $label . '</div>';
      }
      $legend_markup .= '</div>';
      $element['legend'] = array(
        '#type' => 'item',
        '#markup' => $legend_markup,
      );

      // Show the PowerTagging form field
      $element['powertagging'] = array(
        '#title' => $instance['label'],
        '#type' => 'fieldset',
      );

      $powertagging_config = powertagging_config_load($field['settings']['powertagging_id']);
      $project_settings = $powertagging_config->config['projects'][$powertagging_config->project_id];

      // Check if the Visual Mapper has to be added.
      $add_visual_mapper = FALSE;
      if ($project_settings['mode'] == 'annotation' && isset($instance['settings']['browse_concepts_charttypes'])) {
        $chart_types = array_values(array_filter($instance['settings']['browse_concepts_charttypes']));
        $add_visual_mapper = (!empty($chart_types) && _semantic_connector_visual_mapper_usable());
      }

      $entity = NULL;
      switch ($instance['entity_type']) {
        case 'node':
          if (isset($form_state['node'])) {
            $entity = $form_state['node'];
          }
          break;

        case 'user':
          if (isset($form_state['user'])) {
            $entity = $form_state['user'];
          }
          break;

        case 'taxonomy_term':
          if (isset($form_state['term'])) {
            $entity = $form_state['term'];
          }
          break;
      }

      // Check if the instance was correctly configured.
      if (powertagging_check_field_instance($instance)) {
        $tag_ids = array();
        foreach ($items as $item) {
          if (isset($item['tid'])) {
            $tag_ids[] = $item['tid'];
          }
        }
        $tag_string = implode(',', $tag_ids);

        // Get the default tags if required.
        if (empty($tag_string) && !empty($instance['settings']['default_tags_field']) && !is_null($entity)) {
          $default_tags_field = $instance['settings']['default_tags_field'];
          if (!empty($entity->{$default_tags_field})) {
            $language_keys = array_keys($entity->{$default_tags_field});
            $field_values = $entity->{$default_tags_field}[$language_keys[0]];
            if (!empty($field_values)) {
              $default_tags_info_field = field_info_field($default_tags_field);
              $keys = array_keys($default_tags_info_field['columns']);
              unset($default_tags_info_field);
              $default_terms_ids = array();
              foreach ($field_values as $field_value) {
                if (!empty($field_value[$keys[0]])) {
                  $default_terms_ids[] = $field_value[$keys[0]];
                }
              }

              $terms = taxonomy_term_load_multiple($default_terms_ids);
              $default_terms = array();
              foreach ($terms as $term) {
                if (isset($term->field_uri) && !empty($term->field_uri[LANGUAGE_NONE])) {
                  $default_terms[] = $term->name . '|' . $term->field_uri[LANGUAGE_NONE][0]['value'];
                }
                else {
                  $default_terms[] = $term->name . '|';
                }
              }

              $tag_string = implode(',', $default_terms);
            }
          }
        }

        $element['#after_build'] = array('powertagging_add_field_js');
        $element['tag_string'] = array(
          '#type' => 'hidden',
          '#maxlength' => 32000,
          '#default_value' => !empty($tag_string) ? $tag_string : NULL,
          '#element_validate' => array('powertagging_ui_validate'),
          '#attributes' => array(
            'class' => array('powertagging_tag_string'),
          ),
        );

        // Add a field to display an error if the selected language is not
        // supported.
        $error_markup = t('Tagging is not possible for the currently selected language.');
        if (user_access('administer powertagging')) {
          $error_markup .= '<br />' . t('Select a PoolParty language in your !moduleconfig.', array('!moduleconfig' => l(t('powertagging configuration'), 'admin/config/semantic-drupal/powertagging/' . $field['settings']['powertagging_id'])));
        }
        $element['powertagging']['language_error'] = array(
          '#type' => 'item',
          '#markup' => '<div class="messages warning">' . $error_markup . '</div>',
        );

        $element['powertagging']['manual'] = array(
          '#title' => t('Add tags manually'),
          '#type' => 'textfield',
          '#description' => t('The autocomplete-mechanism will suggest concepts from the thesaurus.'),
          '#attributes' => array(
            'class' => array('powertagging_autocomplete_tags'),
          ),
        );

        if ($add_visual_mapper) {
          $element['powertagging']['browse_tags'] = array(
            '#value' => t('Browse tags'),
            '#type' => 'button',
            '#attributes' => array(
              'class' => array('powertagging-browse-tags'),
            ),
          );

          $element['powertagging']['browse_tags_area'] = array(
            '#type' => 'markup',
            '#markup' => '
<div class="powertagging-browse-tags-area" style="display:none;">
  <div class="powertagging-browse-tags-search">
    <label>'. t('Search concept') . '</label>
    <input type="text" class="powertagging-browse-tags-search-ac form-text" />
  </div>
  <div class="powertagging-browse-tags-selection">
    <label>' . t('Tags to add') . '</label>
    <div class="powertagging-browse-tags-selection-results"></div>
     <button class="powertagging-browse-tags-selection-save" type="button">' . t('Accept') . '</button>
     <button class="powertagging-browse-tags-selection-cancel" type="button">' . t('Cancel') . '</button>
  </div>
  <div class="powertagging-browse-tags-vm"></div>
</div>'
          );
        }

        $element['powertagging']['tags_result'] = array(
          '#type' => 'item',
          '#title' => t('Your selected tags'),
          '#markup' => '<div class="powertagging-tag-result"></div>',
        );

        $element['powertagging']['tags'] = array(
          '#type' => 'item',
          '#title' => t('Tags extracted from'),
          '#markup' => '<div class="ajax-progress" style="display:none;"><div class="throbber">' . t('Loading...') . '</div></div><div class="powertagging-extracted-tags" style="display:none;"></div>',
        );

        $element['powertagging']['get_tags'] = array(
          '#value' => t('Get tags'),
          '#type' => 'button',
          '#attributes' => array(
            'class' => array('powertagging-get-tags'),
          ),
        );

        $element['field_instance'] = array(
          '#value' => $instance,
          '#type' => 'value',
        );

        // Get the language of the current entity.
        $language = LANGUAGE_NONE;
        if (!is_null($entity) && property_exists($entity, 'language')) {
          $language = $entity->language;
        }

        $element['language'] = array(
          '#value' => $language,
          '#type' => 'value',
        );
      }
      else {
        $element['powertagging']['error_info'] = array(
          '#type' => 'item',
          '#markup' => t('The "PowerTagging Tags" field needs to be properly configured before content can be tagged.') . '<br />' . t('Make sure all the required fields have a selection and "Number of values" was changed to "Unlimited".'),
        );
      }

      // Add CSS file
      $element['#attached'] = array(
        'css' => array(
          drupal_get_path('module', 'powertagging') . '/css/powertagging.admin.css',
        ),
      );

      // Add the Visual Mapper if required.
      if ($add_visual_mapper) {
        $element['#attached']['js'] = array(
          libraries_get_path('d3js') . '/d3.min.js',
          drupal_get_path('module', 'semantic_connector') . '/js/visual_mapper/visual_mapper.min.js',
        );
        $element['#attached']['css'][] = drupal_get_path('module', 'semantic_connector') . '/js/visual_mapper/visual_mapper.css';
        $element['#attached']['library'] = array(
          array('system', 'ui.dialog'),
        );
      }
      break;

    case 'powertagging_invisible':
      // No form element required, every save retags the entity, but CSS to make
      // sure it is not displayed in any theme.
      // Add CSS file
      $element['#attached'] = array(
        'css' => array(drupal_get_path('module', 'powertagging') . '/css/powertagging.admin.css'),
      );
      break;
  }

  return $element;
}

/**
 * Add all the required JavaScript to the powertagging field widget form.
 *
 * @param array $element
 *   The form element that includes the field-instance-object.
 * @return array
 *   The form element that includes the field-instance-object.
 */
function powertagging_add_field_js(array $element) {
  // AJAX request do not need to add JavaScripts again.
  if (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
    return $element;
  }

  $instance = $element['field_instance']['#value'];
  $language = !empty($element['language']['#value']) ? $element['language']['#value'] : LANGUAGE_NONE;
  $taxonomy_id = powertagging_get_taxonomy_id($instance['settings']['powertagging_id']);
  $selected_tags = array();

  if (!$taxonomy_id) {
    drupal_set_message(t('The project selected in field "%name" is not connected with a vocabulary. Please select one in the connected <a href="!link">PowerTagging configuration area</a>.', array(
      '!link' => url('admin/config/semantic-drupal/powertagging/' . $instance['settings']['powertagging_id']),
      '%name' => $element['#title']
    )), 'warning');
  }

  // Check if all tids are integer values or new free terms.
  if (!empty($element['tag_string']['#value'])) {
    $new_tags = array();
    $tids = array();

    foreach (explode(',', $element['tag_string']['#value']) as $tag_id) {
      if (is_numeric($tag_id) && (intval($tag_id) == floatval($tag_id))) {
        $tids[] = $tag_id;
      }
      elseif (is_string($tag_id) && strpos($tag_id, '|')) {
        $new_tags[] = $tag_id;
      }
    }

    if (!empty($tids)) {
      // Get URI from each term to check if term is a concept or a free term.
      $query = new EntityFieldQuery();
      $result = $query
        ->entityCondition('entity_type', 'taxonomy_term')
        ->propertyCondition('vid', $taxonomy_id)
        ->propertyCondition('tid', $tids, 'IN')
        ->execute();
      if (!empty($result)) {
        // Check if the term needs to be saved as a translation.
        $is_translation = ($language != LANGUAGE_NONE && $language != language_default('language') && powertagging_translation_possible());

        foreach ($result['taxonomy_term'] as $term) {
          $term = taxonomy_term_load($term->tid);
          $selected_tags[] = array(
            'tid' => $term->tid,
            'uri' => empty($term->field_uri) ? '' : $term->field_uri[LANGUAGE_NONE][0]['value'],
            'label' => (!$is_translation) ? $term->name : i18n_taxonomy_term_name($term, $language),
            'type' => empty($term->field_uri) ? 'freeterm' : 'concept',
          );
        }
      }
    }

    foreach ($new_tags as $new_tag) {
      list($label, $uri) = explode('|', $new_tag);
      $selected_tags[] = array(
        'tid' => 0,
        'uri' => empty($uri) ? '' : $uri,
        'label' => $label,
        'type' => empty($uri) ? 'freeterm' : 'concept',
      );
    }
  }

  // Sort the selected tags: concepts on top and free terms to the bottom.
  usort($selected_tags, 'powertagging_sort_selected_tags');

  // Find out what Drupal languages are available for tagging.
  $powertagging_config = powertagging_config_load($instance['settings']['powertagging_id']);
  $project_settings = $powertagging_config->config['projects'][$powertagging_config->project_id];
  $project_languages = $project_settings['languages'];
  $allowed_drupal_languages = array();
  foreach ($project_languages as $drupal_lang => $pp_lang) {
    if (!empty($pp_lang) && ($drupal_lang == LANGUAGE_NONE || $drupal_lang == language_default('language') || powertagging_translation_possible())) {
      $allowed_drupal_languages[] = $drupal_lang;
    }
  }

  // Check if the Visual Mapper has to be added.
  $visual_mapper_chart_types = [];
  if ($project_settings['mode'] == 'annotation' && isset($instance['settings']['browse_concepts_charttypes'])) {
    $chart_types = array_values(array_filter($instance['settings']['browse_concepts_charttypes']));
    if (!empty($chart_types) && _semantic_connector_visual_mapper_usable()) {
      $visual_mapper_chart_types = $chart_types;
    }
  }

  $settings[$instance['field_name']][time()] = array(
    'fields' => powertagging_field_list($instance, 'widget_form'),
    'settings' => array(
      'field_name' => $instance['field_name'],
      'use_fields' => array_keys(array_filter($instance['settings']['fields'])),
      'powertagging_id' => $instance['settings']['powertagging_id'],
      'taxonomy_id' => $taxonomy_id,
      'concepts_per_extraction' => $instance['settings']['concepts_per_extraction']['value'],
      'concepts_threshold' => $instance['settings']['concepts_threshold']['value'],
      'freeterms_per_extraction' => $instance['settings']['freeterms_per_extraction']['value'],
      'freeterms_threshold' => $instance['settings']['freeterms_threshold']['value'],
      // Free terms can not be added manually in the classification mode.
      'custom_freeterms' => ($project_settings['mode'] == 'annotation' ? (isset($instance['settings']['custom_freeterms']) ? $instance['settings']['custom_freeterms'] : TRUE) : FALSE),
      'use_shadow_concepts' => ($project_settings['mode'] == 'annotation' ? (isset($instance['settings']['use_shadow_concepts']) ? $instance['settings']['use_shadow_concepts'] : FALSE) : FALSE),
      'browse_concepts_charttypes' => (!empty($visual_mapper_chart_types) ? $visual_mapper_chart_types : []),
      'concept_scheme_restriction' => (isset($powertagging_config->config['concept_scheme_restriction']) ? $powertagging_config->config['concept_scheme_restriction'] : []),
      'data_properties' => $powertagging_config->config['data_properties'],
      // The currently used Drupal language of the entity.
      'entity_language' => $language,
      // An array of allowed Drupal languages.
      'allowed_languages' => $allowed_drupal_languages,
      'corpus_id' => $project_settings['corpus_id'],
      'max_file_size' => (isset($instance['settings']['file_upload']['max_file_size']) ? $instance['settings']['file_upload']['max_file_size'] : (UPLOAD_MAX_FILE_SIZE * 1048576)),
      'max_file_count' => (isset($instance['settings']['file_upload']['max_file_count']) ? $instance['settings']['file_upload']['max_file_count'] : UPLOAD_MAX_FILE_COUNT),
      'ac_add_matching_label' => (isset($instance['settings']['ac_add_matching_label']) ? $instance['settings']['ac_add_matching_label'] : FALSE),
      'ac_add_context' => (isset($instance['settings']['ac_add_context']) ? $instance['settings']['ac_add_context'] : FALSE),
    ),
    'selected_tags' => $selected_tags,
  );

  drupal_add_js(array('powertagging' => $settings), 'setting');
  drupal_add_library('system', 'ui.autocomplete');
  drupal_add_library('system', 'ui.slide');
  drupal_add_js(drupal_get_path('module', 'powertagging') . '/js/powertagging.field.js');

  return ($element);
}

/**
 * Get the taggable fields for a field instance.
 *
 * Searches for possible fields for the term extraction and return it as a list
 * of select options.
 *
 * @param array $instance
 *   Instance of the PowerTagging field.
 * @param string $type
 *   Defines the return value ('instance_settings_form' | 'widget_form').
 *
 * @return array
 *   List of supported fields depending on $type.
 */
function powertagging_field_list(array $instance, $type) {
  $entity_info = field_info_instances($instance['entity_type'], $instance['bundle']);
  uasort($entity_info, 'powertagging_sort_by_weight');

  return _powertagging_build_field_list($type, $entity_info,
    array(
      'entity_type' => $instance['entity_type'],
      'bundle' => $instance['bundle'],
      'selected_fields' => (isset($instance['settings']) && isset($instance['settings']['fields'])) ? array_keys(array_filter($instance['settings']['fields'])) : [],
    )
  );
}

/**
 * Build the field list recursively.
 *
 * @param string $type
 *   Defines the return value ('instance_settings_form' | 'widget_form').
 * @param array $entity_fields
 *   Assoiative array of field information of a content type keyed by field name.
 * @param array $entity_info
 *   Associative array of additional information of the entity containing
 *   following keys:
 *   - entity_type: The ID of the entity type
 *   - bundle: The bundle of the entity
 *   - selected_fields: Array of fields selected to use for tagging.
 * @param bool $is_sub_entity
 *   TRUE if the entity already is a referenced entity, FALSE if not.
 *
 * @return array
 *
 */
function _powertagging_build_field_list($type, array $entity_fields, $entity_info, $is_sub_entity = FALSE) {
  $allowed_widgets = array(
    'text' => array(
      'text_textfield', // Drupal core
      'text_textarea', // Drupal core
      'text_textarea_with_summary', // Drupal core
    ),
  );
  if (!$is_sub_entity) {
    $allowed_widgets = array_merge($allowed_widgets, array(
      'file' => array(
        'file_generic', // Drupal core
      ),
      'media' => array(
        'media_generic', // Media module: https://www.drupal.org/project/media
      ),
      'entityreference' => array(
        'entityreference_autocomplete', // Entity reference module: https://www.drupal.org/project/entityreference
        'entityreference_autocomplete_tags',
      ),
    ));
  }
  $allowed_modules = array_keys($allowed_widgets);

  $results = array();
  switch ($type) {
    case 'instance_settings_form':
      // Add fields specific to entity types.
      switch ($entity_info['entity_type']) {
        case 'node':
          $node_type = node_type_load($entity_info['bundle']);
          $results['title'] = $node_type->title_label . ' <span class="description">[Text field]</span>';
          break;

        case 'taxonomy_term':
          $results['name'] = 'Name of the term <span class="description">[Text field]</span>';
          $results['description'] = 'Description <span class="description">[Text area]</span>';
          break;

        case 'user':
          $results['name'] = 'Name of the user <span class="description">[Text field]</span>';
          break;
      }

      foreach ($entity_fields as $field_name => $field_data) {
        if (in_array($field_data['widget']['module'], $allowed_modules) &&
          in_array($field_data['widget']['type'], $allowed_widgets[$field_data['widget']['module']])
        ) {
          $widget_info = field_info_widget_types($field_data['widget']['type']);
          // A normal field.
          if ($field_data['widget']['module'] !== 'entityreference') {
            $results[$field_name] = $field_data['label'] . ' <span class="description">[' . $widget_info['label'] . ']</span>';
          }
          // A referenced entity.
          else {
            $field_info = field_info_field($field_name);
            $entity_type = $field_info['settings']['target_type'];
            $bundles = ($entity_type !== 'user') ? $field_info['settings']['handler_settings']['target_bundles'] : ['user'];
            $allowed_entity_types = ['node', 'taxonomy_term', 'user'];
            if (in_array($entity_type, $allowed_entity_types)) {
              foreach ($bundles as $bundle) {
                $sub_entity_fields = field_info_instances($entity_type, $bundle);
                $sub_results = _powertagging_build_field_list($type, $sub_entity_fields,
                  array(
                    'entity_type' => $entity_type,
                    'bundle' => $bundle,
                    'selected_fields' => [],
                  ), TRUE
                );

                foreach ($sub_results as $field_id => $label) {
                  $results[$field_name . '|' . $bundle . '|' . $field_id] = $field_data['label'] . ' <span class="description">[' . $widget_info['label'] . ']</span> --> ' . $bundle . ' --> ' . $label;
                }
              }
            }
          }
        }
      }
      break;

    // JavaScript does not maintain the order during the conversion from
    // associative array to object, so we have to make a normal array out of the
    // field list.
    case 'widget_form':
      $selected_fields = $entity_info['selected_fields'];

      // Add fields specific to entity types.
      switch ($entity_info['entity_type']) {
        case 'node':
          if (in_array('title', $selected_fields)) {
            $results[] = array(
              'field_name' => 'title',
              'module' => 'text',
              'widget' => 'text_textfield_title',
            );
          }
          break;

        case 'taxonomy_term':
          if (in_array('name', $selected_fields)) {
            $results[] = array(
              'field_name' => 'name',
              'module' => 'text',
              'widget' => 'text_textfield_title',
            );
          }
          if (in_array('description', $selected_fields)) {
            $results[] = array(
              'field_name' => 'description',
              'module' => 'text',
              'widget' => 'text_textarea_with_summary',
            );
          }
          break;
      }

      foreach ($entity_fields as $field_name => $field_data) {
        if (in_array($field_data['widget']['module'], $allowed_modules) &&
          in_array($field_data['widget']['type'], $allowed_widgets[$field_data['widget']['module']])
        ) {
          $add_field = FALSE;
          // A normal field.
          if ($field_data['widget']['module'] !== 'entityreference') {
            if (in_array($field_name, $selected_fields)) {
              $add_field = TRUE;
            }
          }
          // A referenced entity.
          else {
            foreach ($selected_fields as $selected_field) {
              if (strpos($selected_field, $field_name . '|') === 0) {
                $add_field = TRUE;
                break;
              }
            }
          }

          if ($add_field) {
            $results[] = array(
              'field_name' => $field_name,
              'module' => $field_data['widget']['module'],
              'widget' => $field_data['widget']['type'],
            );
          }
        }
      }
      break;
  }

  return $results;
}

/**
 * Validation handler for the PowerTagging Tags field.
 */
function powertagging_ui_validate($element, &$form_state) {
  $error = FALSE;

  // Only check if value is not empty.
  if (!empty($element['#value'])) {
    $tag_ids = explode(',', $element['#value']);
    $tids = array();
    $new_tags = array();

    // Check if all tids are integer-values or new freeterms.
    foreach ($tag_ids as $tag_id) {
      if (is_numeric($tag_id) && (intval($tag_id) == floatval($tag_id))) {
        $tids[] = $tag_id;
      }
      elseif (is_string($tag_id)) {
        if (strpos($tag_id, '|')) {
          $new_tags[] = $tag_id;
        }
      }
      else {
        $error = TRUE;
      }
    }

    // Check if all the terms are still existent if there was no error till now.
    if (!$error) {
      $powertagging_id = $form_state['values'][$element['#parents'][0]][$element['#parents'][1]]['field_instance']['settings']['powertagging_id'];
      $powertagging_config = powertagging_config_load($powertagging_id);

      $terms = taxonomy_term_load_multiple($tids);
      // All of the terms are existent.
      if (count($terms) != count($tids)) {
        $error = TRUE;
      }
      // Update data of existing terms if required.
      else {
        $existing_terms_by_uri = array();
        foreach ($terms as $existing_term) {
          if (!empty($existing_term->field_uri) && isset($existing_term->field_uri[LANGUAGE_NONE])) {
            $existing_terms_by_uri[$existing_term->field_uri[LANGUAGE_NONE][0]['value']] = $existing_term;
          }
        }

        if (!empty($existing_terms_by_uri)) {
          $concepts_detail_data = powertagging_get_concept_details($powertagging_config, array_keys($existing_terms_by_uri));
          if (!is_null($concepts_detail_data)) {
            foreach ($concepts_detail_data as $concept_detail_data) {
              if (isset($existing_terms_by_uri[$concept_detail_data->uri])) {
                $existing_term = $existing_terms_by_uri[$concept_detail_data->uri];
                $term_data_changed = powertagging_update_term_concept_details($existing_term, $concept_detail_data);
                // Only save the taxonomy term if any information has changed.
                if ($term_data_changed) {
                  taxonomy_term_save($existing_term);
                }
              }
            }
          }
        }
      }
    }

    // If there is no error at all, add taxonomy terms for the new freeterms.
    if (!$error) {
      $taxonomy_id = powertagging_get_taxonomy_id($powertagging_id);
      if ($taxonomy_id !== FALSE) {
        $new_term_ids = powertagging_add_new_tag($powertagging_config, $new_tags, $taxonomy_id, (isset($form_state['values']['language']) ? $form_state['values']['language'] : LANGUAGE_NONE));
      }

      form_set_value($element, implode(',', array_merge($tids, $new_term_ids)), $form_state);
    }
  }

  if ($error) {
    form_set_error($element['#name'], t('Invalid tag selection.'));
  }
}

/**
 * Add new concepts or free terms to the taxonomy of a PoolParty project.
 *
 * @param object $powertagging_config
 *   The current PowerTagging configuration.
 * @param array $new_tags
 *   Array of strings of new tag to add.
 * @param string $taxonomy_id
 *   The taxonomy id in which the new tag must be stored.
 * @param string $language
 *   The Drupal language of the terms to add.
 *
 * @return array
 *   Array of created term-ids.
 */
function powertagging_add_new_tag($powertagging_config, array $new_tags, $taxonomy_id, $language) {
  $tag_ids = array();
  $parent = powertagging_get_term_list_id($taxonomy_id);

  // Check if the term needs to be saved as a translation.
  $is_translation = ($language != LANGUAGE_NONE && $language != language_default('language') && powertagging_translation_possible());

  // Get detail information of all concepts.
  $new_concept_uris = array();
  foreach ($new_tags as $new_tag) {
    list($label, $uri) = explode('|', $new_tag);
    if (!empty($uri)) {
      $new_concept_uris[] = $uri;
    }
  }
  $concept_details = powertagging_get_concept_details($powertagging_config, $new_concept_uris);
  $concept_details_by_uri = array();
  foreach ($concept_details as $concept_details_concept) {
    $concept_details_by_uri[$concept_details_concept->uri] = $concept_details_concept;
  }
  $concept_details = $concept_details_by_uri;

  foreach ($new_tags as $new_tag) {
    list($label, $uri) = explode('|', $new_tag);

    // Check if the term already exists.
    $query = new EntityFieldQuery();
    $old_term = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('name', $label)
      ->propertyCondition('vid', $taxonomy_id)
      ->execute();

    // If the term already exists and the entered term has no URI then do
    // nothing --> This case should never appear.
    if (!empty($old_term) && empty($uri)) {
      $tid = array_shift($old_term['taxonomy_term']);
      $tag_ids[] = $tid->tid;
      continue;
    }

    // Instantiate a new term.
    $term = new stdClass();
    $term->name = $label;
    $term->vid = $taxonomy_id;

    // If the term exists, set the tid so that the term will be updated.
    if (!empty($old_term)) {
      $tid = array_shift($old_term['taxonomy_term']);
      $term->tid = $tid->tid;
    }

    // If no URI is given, save the term as a free term, otherwise as a concept.
    if (empty($uri)) {
      $term->parent = $parent['freeterms'];
    }
    else {
      $term->field_uri[LANGUAGE_NONE][0]['value'] = $uri;
      $term->parent = $parent['concepts'];

      // Add the concept details to the term.
      if (isset($concept_details[$uri])) {
        powertagging_update_term_concept_details($term, $concept_details[$uri]);
      }
    }

    // Save free terms and concepts in the default language normally.
    if (empty($uri) || !$is_translation) {
      taxonomy_term_save($term);
    }
    // Translated concepts need to be saved as a translated term.
    else {
      $term->name = '[[undefined]]';
      taxonomy_term_save($term);
      i18n_string_translation_update(array(
        'taxonomy',
        'term',
        $term->tid,
        'name',
      ), $label, $language, $term->name);
    }

    $tag_ids[] = $term->tid;
  }

  return $tag_ids;
}

/**
 * Implements hook_field_presave().
 */
function powertagging_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($instance['widget']['type'] == 'powertagging_ui' || $instance['widget']['type'] == 'powertagging_invisible') {
    $values_array = array();
    if (isset($items[0]) && is_string($items[0]) && !empty($items[0])) {
      $items_string = $items[0];
      // Remove the language if one gets added automatically.
      if (!empty($items_string) && count(explode(',', $items_string)) == 1 && !is_numeric($items_string)) {
        if (isset($items[1]) && is_string($items[1]) && !empty($items[1])) {
          $items_string = $items[1];
        }
        else {
          $items_string = '';
        }
      }

      // Make savable values out of the value-string.
      if (!empty($items_string)) {
        foreach (explode(',', $items_string) as $value) {
          if (is_numeric($value)) {
            $values_array[] = array('tid' => $value);
          }
        }
      }
      $items = $values_array;
    }
  }
}

/**
 * Implements hook_field_insert().
 */
function powertagging_field_insert($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($instance['widget']['type'] == 'powertagging_ui' || $instance['widget']['type'] == 'powertagging_invisible') {
    // Empty values sometimes still create NULL values in the field table, this
    // part of the code makes sure it doesn't happen.
    if ($items == array(LANGUAGE_NONE)) {
      $items = array();
    }
  }
}

/**
 * Callback function for extracting concepts and free terms from the content.
 *
 * @return object
 *   A list of suggestions in json format.
 */
function powertagging_extraction () {
  $content = !empty($_POST['content']) ? $_POST['content'] : '';
  $files = !empty($_POST['files']) ? $_POST['files'] : array();
  $entities = !empty($_POST['entities']) ? $_POST['entities'] : array();
  $settings = !empty($_POST['settings']) ? $_POST['settings'] : array();
  $tags = array();

  try {
    $tags = powertagging_extract($content, $files, $entities, $settings);

    if (empty($tags['messages']) && empty($tags['suggestion']['concepts']) && empty($tags['suggestion']['freeterms'])) {
      $tags['messages'][] = array(
        'type' => 'warning',
        'message' => t('No concepts or free terms could be extracted from the entity\'s content.'),
      );
    }
  }
  catch (Exception $e) {
    $tags['suggestion'] = array();
    $tags['messages'][] = array(
      'type' => 'error',
      'message' => t('Error while extracting tags.') . ' ' . $e->getMessage(),
    );
  }

  echo drupal_json_encode($tags);
  exit();
}

/**
 * Extracts concepts and free terms from the content and files.
 *
 * @param string $content
 *   The text content to extract tags from.
 * @param array $files
 *   Array of Drupal file-ids of files to extract tags from.
 * @param array $entities
 *   Associative array of referenced entity IDs keyed by field name.
 * @param array $settings
 *   Array of settings to use for the extraction process.
 *
 * @return array
 *
 * @throws \Exception
 */
function powertagging_extract($content = "", array $files = [], array $entities = [], array $settings = []) {
  $powertagging_config = (isset($settings['powertagging_config']) ? $settings['powertagging_config'] : powertagging_config_load($settings['powertagging_id']));
  $project_settings = $powertagging_config->config['projects'][$powertagging_config->project_id];

  // Check if the content of referenced entities needs to be added first.
  if (!empty($entities)) {
    foreach ($entities as $field_name => $entity_ids) {
      if (!empty($entity_ids)) {
        $field_info = field_info_field($field_name);
        $allowed_entity_types = ['node', 'taxonomy_term', 'user'];
        $entity_type = $field_info['settings']['target_type'];
        if (in_array($entity_type, $allowed_entity_types)) {
          $allowed_bundles = ($entity_type !== 'user') ? $field_info['settings']['handler_settings']['target_bundles'] : ['user'];
          $tag_fields = [];
          foreach ($allowed_bundles as $allowed_bundle) {
            $fields_to_use = [];
            $prefix = $field_name . '|' . $allowed_bundle . '|';
            foreach ($settings['use_fields'] as $field_id) {
              if (strpos($field_id, $prefix) === 0) {
                $fields_to_use[] = substr($field_id, strlen($prefix));
              }
            }
            if (!empty($fields_to_use)) {
              $entity_info = field_info_instances($entity_type, $allowed_bundle);
              foreach ($fields_to_use as $tag_field_name) {
                $field_instance = isset($entity_info[$tag_field_name]) ? $entity_info[$tag_field_name] : FALSE;
                $tag_fields[$allowed_bundle][$tag_field_name] = array(
                  'module' => $field_instance ? $field_instance['widget']['module'] : 'standard field',
                  'type' => $field_instance ? $field_instance['widget']['type'] : '',
                );
              }
            }
          }

          $loaded_entities = entity_load($entity_type, $entity_ids);
          foreach ($loaded_entities as $loaded_entity) {
            if ($entity_type != 'node' || node_access('view', $loaded_entity)) {
              $bundle = '';
              switch ($entity_type) {
                case 'node':
                  $bundle = $loaded_entity->type;
                  break;
                case 'taxonomy_term':
                  $bundle = $loaded_entity->vocabulary_machine_name;
                  break;
                case 'user':
                  $bundle = 'user';
                  break;
              }

              if (isset($tag_fields[$bundle]) && !empty($tag_fields[$bundle])) {
                $tag_contents = powertagging_extract_entity_content($loaded_entity, $tag_fields[$bundle]);
                if (isset($tag_contents['text']) && !empty($tag_contents['text'])) {
                  $content .= ' ' . $tag_contents['text'];
                }
              }
            }
          }
        }
      }
    }
  }

  // Remove line breaks and HTML tags from the content and convert HTML
  // characters to normal ones.
  $content = html_entity_decode(str_replace(array("\r", "\n", "\t"), " ", strip_tags($content)), ENT_COMPAT, 'UTF-8');
  $corpus_id = isset($project_settings['corpus_id']) ? $project_settings['corpus_id'] : '';

  $param = array(
    'projectId' => $powertagging_config->project_id,
    'numberOfConcepts' => (int) $settings['concepts_per_extraction'],
    'numberOfTerms' => (int) $settings['freeterms_per_extraction'],
  );

  if (!empty($corpus_id)) {
    $param['corpusScoring'] = $corpus_id;
    if (isset($settings['use_shadow_concepts']) && $settings['use_shadow_concepts'] && $project_settings['mode'] == 'annotation') {
      $param['shadowConceptCorpusId'] = $corpus_id;
    }
  }
  if (isset($settings['concept_scheme_restriction']) && !empty($settings['concept_scheme_restriction'])) {
    $i = 0;
    foreach ($settings['concept_scheme_restriction'] as $restriction) {
      $param['conceptSchemeFilters[' . $i++ . ']'] = $restriction;
    }
  }
  $tags = array(
    'content' => array(
      'concepts' => array(),
      'freeterms' => array(),
    ),
    'suggestion' => array(
      'concepts' => array(),
      'freeterms' => array(),
    ),
    'messages' => array(),
  );
  $suggestion = array(
    'concepts' => array(),
    'freeterms' => array(),
  );

  // Find out what language to extract.
  $project_languages = $project_settings['languages'];

  // Language mapping for the content languages exists.
  if (isset($project_languages[$settings['entity_language']]) && !empty($project_languages[$settings['entity_language']])) {
    /** @var \SemanticConnectorPPXApi $pp_api */
    $pp_api = powertagging_getPoolPartyApi((int) $settings['powertagging_id']);

    // Extract the concepts and free terms.
    if (isset($settings['taxonomy_id']) && $settings['taxonomy_id'] > 0) {
      if (!empty($content)) {
        // Annotation.
        if ($project_settings['mode'] == 'annotation') {
          $extraction = $pp_api->extractConcepts($content, $project_languages[$settings['entity_language']], $param, 'text');
        }
        // Classification.
        else {
          $extraction = $pp_api->extractCategories($content, $project_languages[$settings['entity_language']], $param, 'text');
        }
        if (is_null($extraction)) {
          throw new Exception(t('Unable to extract from content.'));
        }
        else {
          $extracted_tags = powertagging_extract_tags($extraction, $settings);
          $tags['content'] = $extracted_tags;
          $suggestion['concepts'] = array_merge($suggestion['concepts'], $extracted_tags['concepts']);
          $suggestion['freeterms'] = array_merge($suggestion['freeterms'], $extracted_tags['freeterms']);
        }
      }

      if (!empty($files)) {
        $tags['files'] = array();
        $extracted_files_count = 0;
        foreach ($files as $file_id) {
          $file = file_load($file_id);
          // Use only existing files for tagging.
          if (!file_exists($file->uri)) {
            throw new Exception(t('File %file does not exist.', array('%file' => $file->filename)));
          }
          elseif ($file->filesize <= $settings['max_file_size']) {
            // Annotation.
            if ($project_settings['mode'] == 'annotation') {
              $extraction = $pp_api->extractConcepts($file, $project_languages[$settings['entity_language']], $param, 'file');
            }
            // Classification.
            else {
              $extraction = $pp_api->extractCategories($file, $project_languages[$settings['entity_language']], $param, 'file');
            }
            if (is_null($extraction)) {
              throw new Exception(t('Unable to extract from file %file.', array('%file' => $file->filename)));
            }
            else {
              $extracted_tags = powertagging_extract_tags($extraction, $settings);
              if (!(empty($extracted_tags['concepts']) && empty($extracted_tags['freeterms']))) {
                $tags['files'][$file->filename] = $extracted_tags;
                $suggestion['concepts'] = array_merge($suggestion['concepts'], $extracted_tags['concepts']);
                $suggestion['freeterms'] = array_merge($suggestion['freeterms'], $extracted_tags['freeterms']);
                $extracted_files_count++;
              }
            }
            if ($extracted_files_count >= $settings['max_file_count']) {
              break;
            }
          }
        }
      }
    }

    // Merge all extracted concepts and free terms for the suggestion.
    if (!empty($suggestion['concepts'])) {
      usort($suggestion['concepts'], 'powertagging_sort_by_score');
      $uris = array();
      $count = 1;
      foreach ($suggestion['concepts'] as $tag) {
        if (in_array($tag['uri'], $uris)) {
          continue;
        }
        $tags['suggestion']['concepts'][] = $tag;
        $uris[] = $tag['uri'];
        if ($settings['concepts_per_extraction'] <= $count++) {
          break;
        }
      }
    }
    if (!empty($suggestion['freeterms'])) {
      usort($suggestion['freeterms'], 'powertagging_sort_by_score');
      $labels = array();
      $count = 1;
      foreach ($suggestion['freeterms'] as $tag) {
        if (in_array($tag['label'], $labels)) {
          continue;
        }
        $tags['suggestion']['freeterms'][] = $tag;
        $labels[] = $tag['label'];
        if ($settings['freeterms_per_extraction'] <= $count++) {
          break;
        }
      }
    }
  }

  return $tags;
}

/**
 * Extracts the tags from the extraction result set.
 *
 * @param object $extraction
 *   The extraction result set.
 * @param array $settings
 *   The settings for threshold, concepts_per_extraction, ...
 *
 * @return array
 *   A list of found concepts and free terms with the corresponding taxonomy ids
 *   if available.
 */
function powertagging_extract_tags($extraction, array $settings) {
  $concepts = array();
  $free_terms = array();
  $tags = array(
    'concepts' => array(),
    'freeterms' => array(),
  );

  // Go through the concepts.
  if (isset($extraction->concepts) && !empty($extraction->concepts)) {
    // Ignore all concepts with the score less than the threshold.
    $treshold = (int) $settings['concepts_threshold'];

    foreach ($extraction->concepts as $concept) {
      if ($concept->score >= $treshold) {
        $concepts[] = $concept;
      }
    }

    // Get the corresponding taxonomy term id.
    powertagging_add_tid_to_concept($concepts, $settings['taxonomy_id'], 'concepts', $settings['entity_language']);

    // Ignore all not found taxonomy terms.
    if (!empty($concepts)) {
      foreach ($concepts as $concept) {
        $tags['concepts'][] = array(
          'tid' => $concept->tid,
          'uri' => $concept->uri,
          'label' => $concept->prefLabel,
          'score' => $concept->score,
          'type' => 'concept',
        );
      }
    }
  }

  // Go through the categories if available (handle them as normal concepts).
  if (empty($concepts) && isset($extraction->categories) && !empty($extraction->categories)) {
    $max_categories = (int) $settings['concepts_per_extraction'];
    foreach ($extraction->categories as $category) {
      $concepts[] = $category;
      if (count($concepts) >= $max_categories) {
        break;
      }
    }

    // Get the corresponding taxonomy term id.
    powertagging_add_tid_to_concept($concepts, $settings['taxonomy_id'], 'concepts', $settings['entity_language']);

    // Ignore all not found taxonomy terms.
    if (!empty($concepts)) {
      foreach ($concepts as $concept) {
        $tags['concepts'][] = array(
          'tid' => $concept->tid,
          'uri' => $concept->uri,
          'label' => $concept->prefLabel,
          'score' => $concept->score,
          'type' => 'concept',
        );
      }
    }
  }

  // Go through the free terms.
  if (isset($extraction->freeTerms) && !empty($extraction->freeTerms)) {
    // Ignore all free terms with the score less than the threshold.
    $treshold = (int) $settings['freeterms_threshold'];
    foreach ($extraction->freeTerms as $free_term) {
      if ($free_term->score >= $treshold) {
        $free_terms[] = $free_term;
      }
    }

    // Get the corresponding taxonomy term id.
    powertagging_add_tid_to_concept($free_terms, $settings['taxonomy_id'], 'free_terms', $settings['entity_language']);

    if (!empty($free_terms)) {
      foreach ($free_terms as $free_term) {
        $tags['freeterms'][] = array(
          'tid' => isset($free_term->tid) ? $free_term->tid : 0,
          'uri' => '',
          'label' => $free_term->textValue,
          'score' => $free_term->score,
          'type' => 'freeterm',
        );
      }
    }
  }

  return $tags;
}

/**
 * Add the corresponding taxonomy term id to the concepts or free terms.
 *
 * @param array $concepts
 *   The concepts or free terms found from PP Extractor.
 * @param int $taxonomy_id
 *   The taxonomy id in which the taxonomy is imported.
 * @param string $type
 *   The type of the concepts (concepts or free terms).
 * @param string $language
 *   The language of the concept label.
 */
function powertagging_add_tid_to_concept(array &$concepts, $taxonomy_id, $type, $language = LANGUAGE_NONE) {
  if (empty($concepts)) {
    return;
  }

  switch ($type) {
    case 'concepts':
      // Get all concept uris.
      $uris = array();
      foreach ($concepts as $concept) {
        $uris[] = $concept->uri;
      }

      // Search for the corresponding tid.
      $query = db_select('taxonomy_term_data', 't')
        ->fields('f', array('entity_id', 'field_uri_value'))
        ->fields('t', array('name'))
        ->condition('t.vid', $taxonomy_id)
        ->condition('f.field_uri_value', $uris, 'IN');
      $query->join('field_data_field_uri', 'f', 'f.entity_id = t.tid AND f.entity_type = \'taxonomy_term\'');
      $result = $query->execute()
        ->fetchAll();

      $terms = array();
      foreach ($result as $term) {
        $terms[$term->field_uri_value] = array(
          'tid' => $term->entity_id,
          'name' => $term->name,
        );
      }

      $default_language = language_default('language');

      // Add the tid to each concept if exists otherwise set tid = 0.
      foreach ($concepts as &$concept) {
        if (isset($terms[$concept->uri])) {
          $concept->tid = $terms[$concept->uri]['tid'];
          // Default language --> Save the term label.
          if ($language == LANGUAGE_NONE || $language == $default_language || !powertagging_translation_possible()) {
            if ($concept->prefLabel != $terms[$concept->uri]['name']) {
              $taxonomy_term = taxonomy_term_load($concept->tid);
              $taxonomy_term->name = $concept->prefLabel;
              taxonomy_term_save($taxonomy_term);
            }
          }
          // Not the default language --> Update the translation.
          else {
            i18n_string_translation_update(array(
              'taxonomy',
              'term',
              $concept->tid,
              'name'
            ), $concept->prefLabel, $language, $terms[$concept->uri]['name']);
          }
        }
        else {
          $concept->tid = 0;
        }
      }
      break;

    case 'free_terms':
      $taxonomy = taxonomy_vocabulary_load($taxonomy_id);
      // Add the tid to each free term if exists otherwise set tid to 0.
      foreach ($concepts as &$concept) {
        $terms = taxonomy_get_term_by_name($concept->textValue, $taxonomy->machine_name);
        if (!empty($terms)) {
          $term = array_shift($terms);
          $concept->tid = $term->tid;
        }
        else {
          $concept->tid = 0;
        }
      }
      break;
  }
}

/**
 * Get the taxonomy id (vid) from the project of a PowerTagging configuration.
 *
 * @param string $powertagging_id
 *   The ID of the powertagging configuration.
 *
 * @return int
 *   The vid of the taxonomy.
 */
function powertagging_get_taxonomy_id($powertagging_id) {
  $powertagging_config = powertagging_config_load($powertagging_id);
  if (isset($powertagging_config->config['projects'][$powertagging_config->project_id]) && isset($powertagging_config->config['projects'][$powertagging_config->project_id]['taxonomy_id'])) {
    $vid = $powertagging_config->config['projects'][$powertagging_config->project_id]['taxonomy_id'];
  }
  else {
    $vid = 0;
  }

  return $vid;
}

/**
 * Get the term IDs of the fixed values of the powertagging taxonomy.
 *
 * Get the taxonomy term id from the "Free Terms" and "Concepts" list term;
 * If it does not exists, it will be created.
 *
 * @param int $taxonomy_id
 *   The taxonomy id in which the list is.
 *
 * @return array
 *   The taxonomy term ids.
 */
function powertagging_get_term_list_id($taxonomy_id) {
  $list_ids = array(
    'concepts' => NULL,
    'freeterms' => NULL,
  );
  $list_names = array(
    'concepts' => 'Concepts',
    'freeterms' => 'Free Terms',
  );

  foreach ($list_names as $key => $list_name) {
    // Check if free term list exists.
    $query = new EntityFieldQuery();
    $result = $query
      ->entityCondition('entity_type', 'taxonomy_term')
      ->propertyCondition('vid', (int) $taxonomy_id)
      ->propertyCondition('name', $list_name)
      ->execute();

    if (!empty($result)) {
      foreach ($result['taxonomy_term'] as $term) {
        $parents = taxonomy_get_parents($term->tid);
        $term = taxonomy_term_load($term->tid);
        if (empty($parents) && empty($term->field_uri)) {
          $list_ids[$key] = $term->tid;
          break;
        }
      }
    }

    // If free term list not exists then create it.
    if (is_null($list_ids[$key])) {
      $term = new stdClass();
      $term->name = $list_name;
      $term->vid = $taxonomy_id;
      taxonomy_term_save($term);
      $list_ids[$key] = $term->tid;
    }
  }

  return $list_ids;
}

/**
 * Implements hook_field_formatter_info().
 */
function powertagging_field_formatter_info() {
  return array(
    'powertagging_list' => array(
      'label' => t('Tag list'),
      'field types' => array('powertagging'),
      'settings' => array('add_alt_labels' => FALSE, 'add_hidden_labels' => FALSE),
    ),
  );
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function powertagging_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'powertagging_list') {
    $element['info'] = array(
      '#markup' => '<p>' . t('Select the labels that will be added additionally in a hidden box to each PowerTagging Tag:') . '</p>',
    );
    $element['add_alt_labels'] = array(
      '#title' => t('Alternative labels'),
      '#type' => 'checkbox',
      '#default_value' => $settings['add_alt_labels'],
    );
    $element['add_hidden_labels'] = array(
      '#title' => t('Hidden labels'),
      '#type' => 'checkbox',
      '#default_value' => $settings['add_hidden_labels'],
    );
    $element['help'] = array(
      '#markup' => '<p>' . t('The Drupal default search is improved by indexing the corresponding node with those labels.') . '</p>',
    );
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function powertagging_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $summary = '';

  if ($display['type'] == 'powertagging_list') {
    $labels = array();
    if ($settings['add_alt_labels']) {
      $labels[] = t('Alternative labels');
    }
    if ($settings['add_hidden_labels']) {
      $labels[] = t('Hidden labels');
    }

    $summary = t('Hidden data@labels', array('@labels' => (empty($labels) ? '' : ': ' . implode(', ', $labels))));
  }

  return $summary;
}

/**
 * Implements hook_field_formatter_view().
 */
function powertagging_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $markup = NULL;

  switch ($display['type']) {
    case 'powertagging_list':
      if (!empty($items)) {
        $context = array(
          'items' => $items,
          'entity' => $entity,
          'langcode' => $langcode,
          'instance' => $instance,
        );
        drupal_alter('powertagging_tag_list', $markup, $context);

        if (is_null($markup)) {
          $tids = array();
          foreach ($items as $tid) {
            // Live display.
            if (is_array($tid)) {
              $tids[] = $tid['tid'];
            }
            // Preview mode.
            elseif (is_numeric($tid)) {
              $tids[] = $tid;
            }
          }

          $terms = taxonomy_term_load_multiple($tids);
          $settings = $display['settings'];
          $tags_to_theme = array();
          foreach ($terms as $term) {
            reset($term->field_uri);
            reset($term->field_alt_labels);
            reset($term->field_hidden_labels);
            $alt_labels = [];
            if (!empty($term->field_alt_labels)) {
              foreach ($term->field_alt_labels[key($term->field_alt_labels)] as $alt_label) {
                $alt_labels[] = $alt_label['value'];
              }
            }
            $hidden_labels = [];
            if (!empty($term->field_hidden_labels)) {
              foreach ($term->field_hidden_labels[key($term->field_hidden_labels)] as $hidden_label) {
                $hidden_labels[] = $hidden_label['value'];
              }
            }
            $tags_to_theme[] = array(
              'uri' => (!empty($term->field_uri) ? $term->field_uri[key($term->field_uri)][0]['value'] : ''),
              'html' => field_filter_xss($term->name),
              'alt_labels' => ($settings['add_alt_labels'] ? $alt_labels : []),
              'hidden_labels' => ($settings['add_hidden_labels'] ? $hidden_labels : []),
            );
          }

          $powertagging_config = powertagging_config_load($instance['settings']['powertagging_id']);
          $markup = semantic_connector_theme_concepts($tags_to_theme, $powertagging_config->connection->getId(), $powertagging_config->project_id);
        }
      }
      break;
  }

  if (!is_null($markup)) {
    return array(array('#markup' => $markup));
  }
  else {
    return FALSE;
  }
}

/**
 * Get the matching concepts for GET-parameter "term".
 *
 * @param object $powertagging_config
 *   The PowerTagging configuration.
 * @param string $language
 *   The iso-code of the language you want your suggestions in.
 */
function powertagging_autocomplete_tags($powertagging_config, $language) {
  $string = check_plain($_GET['term']);
  $terms = array();

  if (!empty($string)) {
    $project_languages = $powertagging_config->config['projects'][$powertagging_config->project_id]['languages'];
    if (isset($project_languages[$language]) && !empty($project_languages[$language])) {
      $pp_api = powertagging_getPoolPartyApi($powertagging_config->powertagging_id);
      $params = [];
      if (isset($powertagging_config->config['concept_scheme_restriction']) && !empty($powertagging_config->config['concept_scheme_restriction'])) {
        $params['conceptSchemeFilters'] = $powertagging_config->config['concept_scheme_restriction'];
      }
      $suggested_concepts = $pp_api->suggest($string, $project_languages[$language], $powertagging_config->project_id, $params);
      if (!empty($suggested_concepts)) {
        // Add the Drupal taxonomy ID if it needs to be added.
        if (!property_exists($suggested_concepts[0], 'tid')) {
          powertagging_add_tid_to_concept($suggested_concepts, (int) $powertagging_config->config['projects'][$powertagging_config->project_id]['taxonomy_id'], 'concepts', $project_languages[$language]);
        }

        foreach ($suggested_concepts as $concept) {
          $terms[] = array(
            'tid' => $concept->tid,
            'uri' => $concept->uri,
            'name' => $concept->prefLabel,
            'value' => $concept->prefLabel,
            'type' => 'concept',
            'matching_label' => property_exists($concept, 'matchingLabel') ? $concept->matchingLabel : '',
            'context' => (property_exists($concept, 'conceptSchemes') && !empty($concept->conceptSchemes)) ? $concept->conceptSchemes[0]->title : '',
          );
        }
      }
    }
  }

  // Sort the terms: concepts on top and free terms to the bottom.
  if (!empty($terms)) {
    usort($terms, 'powertagging_autocomplete_tags_sort');
  }

  drupal_json_output($terms);
}

/**
 * Callback function to sort the autocomplete tags.
 */
function powertagging_autocomplete_tags_sort($a, $b) {
  if ((empty($a['uri']) && empty($b['uri'])) || (!empty($a['uri']) && !empty($b['uri']))) {
    return strcasecmp($a['name'], $b['name']);
  }

  return empty($a['uri']) ? 1 : -1;
}

/**
 * Callback function for sorting the fields in the entity info.
 */
function powertagging_sort_by_weight($a, $b) {
  if ($a['widget']['weight'] == $b['widget']['weight']) {
    return 0;
  }
  return ($a['widget']['weight'] < $b['widget']['weight']) ? -1 : 1;
}

/**
 * Callback function for sorting tags by score.
 */
function powertagging_sort_by_score($a, $b) {
  if ($a['score'] == $b['score']) {
    return 0;
  }
  return ($a['score'] < $b['score']) ? 1 : -1;
}

/**
 * Callback function to sort the selected tags.
 */
function powertagging_sort_selected_tags($a, $b) {
  if ((empty($a['uri']) && empty($b['uri'])) || (!empty($a['uri']) && !empty($b['uri']))) {
    return strcasecmp($a['label'], $b['label']);
  }

  return empty($a['uri']) ? 1 : -1;
}

/**
 * Callback function for adding URIs + tid to concepts.
 */
function _powertagging_get_concept_uris() {
  $concepts = !empty($_POST['concepts']) ? $_POST['concepts'] : array();
  $settings = !empty($_POST['settings']) ? $_POST['settings'] : array();

  foreach ($concepts as $concept_index => $concept) {
    $concepts[$concept_index] = (object)$concept;
  }

  // Get the corresponding taxonomy term id.
  powertagging_add_tid_to_concept($concepts, $settings['taxonomy_id'], 'concepts', $settings['entity_language']);

  echo drupal_json_encode($concepts);
  exit();
}
