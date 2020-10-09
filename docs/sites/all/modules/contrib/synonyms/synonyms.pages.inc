<?php

/**
 * @file
 * Menu page callbacks of Synonyms module.
 */

/**
 * Page callback: Outputs JSON for taxonomy autocomplete suggestions.
 *
 * This callback outputs term name suggestions in response to Ajax requests
 * made by the synonyms autocomplete widget for taxonomy term reference
 * fields. The output is a JSON object of plain-text term suggestions,
 * keyed by the user-entered value with the completed term name appended.
 * Term names containing commas are wrapped in quotes. The search is made
 * with consideration of synonyms.
 *
 * @param string $field_name
 *   The name of the term reference field.
 * @param string $entity_type
 *   Entity type to which the supplied $field_name is attached to
 * @param string $bundle
 *   Bundle name to which the supplied $field_name is attached to
 * @param string $tags_typed
 *   (optional) A comma-separated list of term names entered in the
 *   autocomplete form element. Only the last term is used for autocompletion.
 *   Defaults to '' (an empty string).
 */
function synonyms_autocomplete_taxonomy_term($field_name, $entity_type, $bundle, $tags_typed = '') {
  // If the request has a '/' in the search text, then the menu system will have
  // split it into multiple arguments, recover the intended $tags_typed.
  $args = func_get_args();
  // Shift off the $field_name argument.
  array_shift($args);
  // Shift off the $entity_type argument.
  array_shift($args);
  // Shift off the $bundle argument.
  array_shift($args);
  $tags_typed = implode('/', $args);

  // Make sure the field exists and is a taxonomy field.
  if (!($field = field_info_field($field_name)) || $field['type'] != 'taxonomy_term_reference') {
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('Taxonomy field @field_name not found.', array('@field_name' => $field_name));
    exit;
  }

  if (!($instance = field_info_instance($entity_type, $field['field_name'], $bundle))) {
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('There was not found an instance of @field_name in @entity_type.', array(
      '@field_name' => $field_name,
      '@entity_type' => $entity_type,
    ));
    exit;
  }

  $widget = $instance['widget']['type'] == 'synonyms_autocomplete_taxonomy_term' ? $instance['widget']['settings'] : field_info_widget_settings('synonyms_autocomplete_taxonomy_term');

  // How many suggestions maximum we are able to output.
  $max_suggestions = $widget['suggestion_size'];

  // Whether we are allowed to suggest more than one entry per term, shall that
  // entry be either term name itself or one of its synonyms.
  $suggest_only_unique = $widget['suggest_only_unique'];

  // The user enters a comma-separated list of tags. We only autocomplete the
  // last tag.
  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));

  $tags_typed_tids = array();
  if (!empty($tags_typed)) {
    $efq = new EntityFieldQuery();
    $efq->entityCondition('entity_type', 'taxonomy_term');
    $efq->propertyCondition('name', $tags_typed);
    $tags_typed_tids = $efq->execute();
    if (isset($tags_typed_tids['taxonomy_term'])) {
      $tags_typed_tids = array_keys($tags_typed_tids['taxonomy_term']);
    }
  }

  // Array of found suggestions. Each subarray of this array will represent a
  // single suggestion entry.
  // - tid: (int) tid of the suggested term
  // - name: (string) name of the suggested term
  // - synonym: (string) optional synonym string that matched this entry
  // - behavior_implementation: (array) optional behavior implementation that
  //   provided the synonym
  $tags_return = array();
  if ($tag_last != '') {
    // Part of the criteria for the query come from the field's own settings.
    $vocabularies = array();
    $tmp = taxonomy_vocabulary_get_names();
    foreach ($field['settings']['allowed_values'] as $tree) {
      $vocabularies[$tmp[$tree['vocabulary']]->vid] = $tree['vocabulary'];
    }
    $vocabularies = taxonomy_vocabulary_load_multiple(array_keys($vocabularies));

    // Firstly getting a list of tids that match by $term->name.
    $query = db_select('taxonomy_term_data', 't');
    $query->addTag('translatable');
    $query->addTag('term_access');

    // Do not select already entered terms.
    if (!empty($tags_typed_tids)) {
      $query->condition('t.tid', $tags_typed_tids, 'NOT IN');
    }
    // Select rows that match by term name.
    $result = $query
      ->fields('t', array('tid', 'name'))
      ->condition('t.vid', array_keys($vocabularies))
      ->condition('t.name', '%' . db_like($tag_last) . '%', 'LIKE')
      ->range(0, $max_suggestions)
      ->execute();
    foreach ($result as $v) {
      $tags_return[] = (array) $v;
    }

    // Now we go vocabulary by vocabulary looking through synonym fields.
    foreach ($vocabularies as $vocabulary) {
      // Now we go a synonym field by synonym field gathering suggestions.
      $bundle = field_extract_bundle('taxonomy_term', $vocabulary);

      $behavior_implementations = synonyms_behavior_get('autocomplete', 'taxonomy_term', $bundle, TRUE);
      foreach ($behavior_implementations as $implementation) {
        $condition = db_and();
        $condition->condition(AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER, '%' . db_like($tag_last) . '%', 'LIKE');

        if (!empty($tags_typed_tids)) {
          $condition->condition(AbstractSynonymsBehavior::COLUMN_ENTITY_ID_PLACEHOLDER, $tags_typed_tids, 'NOT IN');
        }
        if ($suggest_only_unique && !empty($tags_return)) {
          $tmp = array();
          foreach ($tags_return as $tag_return) {
            $tmp[] = $tag_return['tid'];
          }
          $condition->condition(AbstractSynonymsBehavior::COLUMN_ENTITY_ID_PLACEHOLDER, $tmp, 'NOT IN');
        }

        $new_tids = array();
        foreach ($implementation['object']->synonymsFind($condition) as $synonym) {
          if (!$suggest_only_unique || !in_array($synonym->entity_id, $new_tids)) {
            $tags_return[] = array(
              'tid' => $synonym->entity_id,
              'name' => '',
              'synonym' => $synonym->synonym,
              'behavior_implementation' => $implementation,
            );
            $new_tids[] = $synonym->entity_id;
          }
        }
      }
    }

    $synonym_terms = array();
    foreach ($tags_return as $v) {
      if (isset($v['synonym'])) {
        $synonym_terms[] = $v['tid'];
      }
    }

    if (!empty($synonym_terms)) {
      $synonym_terms = taxonomy_term_load_multiple($synonym_terms);
      foreach ($tags_return as &$v) {
        if (isset($v['synonym'])) {
          $entity_ids = entity_extract_ids('taxonomy_term', $synonym_terms[$v['tid']]);
          $v['name'] = $synonym_terms[$v['tid']]->name;
          $v['bundle'] = $entity_ids[2];
        }
      }
    }
    if (count($tags_return) > $max_suggestions) {
      $tags_return = array_slice($tags_return, 0, $max_suggestions);
    }
  }
  $prefix = empty($tags_typed) ? '' : drupal_implode_tags($tags_typed) . ', ';
  drupal_json_output(synonyms_autocomplete_format($tags_return, $prefix));
}

/**
 * Page callback: Outputs JSON for entity autocomplete suggestions.
 *
 * This callback outputs entity name suggestions in response to Ajax requests
 * made by the synonyms autocomplete widget for entity reference fields. The
 * output is a JSON object of plain-text entity suggestions, keyed by the
 * user-entered value with the completed entity name appended. Entity names
 * containing commas are wrapped in quotes. The search is made with
 * consideration of synonyms.
 *
 * @param string $field_name
 *   The name of the entity reference field.
 * @param string $entity_type
 *   Entity type to which the supplied $field_name is attached to
 * @param string $bundle
 *   Bundle name to which the supplied $field_name is attached to
 * @param string $tags_typed
 *   (optional) A comma-separated list of entity names entered in the
 *   autocomplete form element. Only the last term is used for autocompletion.
 *   Defaults to '' (an empty string).
 */
function synonyms_autocomplete_entity($field_name, $entity_type, $bundle, $tags_typed = '') {
  // If the request has a '/' in the search text, then the menu system will have
  // split it into multiple arguments, recover the intended $tags_typed.
  $args = func_get_args();
  // Shift off the $field_name argument.
  array_shift($args);
  // Shift off the $entity_type argument.
  array_shift($args);
  // Shift off the $bundle argument.
  array_shift($args);
  $tags_typed = implode('/', $args);

  if (!($field = field_info_field($field_name)) || $field['type'] != 'entityreference') {
    print t('Entity reference field @field_name not found.', array('@field_name' => $field_name));
    exit;
  }

  if (!($instance = field_info_instance($entity_type, $field['field_name'], $bundle))) {
    // Error string. The JavaScript handler will realize this is not JSON and
    // will display it as debugging information.
    print t('There was not found an instance of @field_name in @entity_type.', array(
      '@field_name' => $field_name,
      '@entity_type' => $entity_type,
    ));
    exit;
  }

  $widget = $instance['widget']['type'] == 'synonyms_autocomplete_entity' ? $instance['widget']['settings'] : field_info_widget_settings('synonyms_autocomplete_entity');

  // How many suggestions maximum we are able to output.
  $max_suggestions = $widget['suggestion_size'];

  // Whether we are allowed to suggest more than one entry per term, shall that
  // entry be either term name itself or one of its synonyms.
  $suggest_only_unique = $widget['suggest_only_unique'];

  $tags_typed = drupal_explode_tags($tags_typed);
  $tag_last = drupal_strtolower(array_pop($tags_typed));
  $prefix = count($tags_typed) ? drupal_implode_tags($tags_typed) . ', ' : '';

  $handler = entityreference_get_selection_handler($field, $instance, $entity_type, NULL);

  $tags_typed_entity_ids = array();
  if (!empty($tags_typed)) {
    foreach ($tags_typed as $v) {
      foreach ($handler->getReferencableEntities($v, '=') as $target_entity_ids) {
        $tags_typed_entity_ids = array_merge($tags_typed_entity_ids, array_keys($target_entity_ids));
      }
    }
  }

  $matches = array();
  if ($tag_last) {
    foreach ($handler->getReferencableEntities($tag_last) as $target_entity_ids) {
      foreach (array_diff_key($target_entity_ids, drupal_map_assoc($tags_typed_entity_ids)) as $target_id => $label) {
        // We do not use the label such as given us by
        // $handler->getReferencableEntities() because some handlers may include
        // more than just plain entity label. However, our validate handler
        // expects the exact labels in the text field. So we assure we put a
        // label there.
        // These entities have already been loaded by $handler, so we shouldn't
        // care that much performance-wise about loading them in batch.
        $entity = entity_load($field['settings']['target_type'], array($target_id));
        $entity = reset($entity);
        $matches[] = array(
          'target_id' => $target_id,
          'name' => entity_label($field['settings']['target_type'], $entity),
        );
        if (count($matches) == $max_suggestions) {
          break (2);
        }
      }
    }

    if (count($matches) < $max_suggestions) {
      $behavior_implementations = synonyms_behavior_get('autocomplete', $field['settings']['target_type'], synonyms_field_target_bundles($field), TRUE);
      foreach ($behavior_implementations as $implementation) {
        $condition = db_and();
        $condition->condition(AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER, '%' . db_like($tag_last) . '%', 'LIKE');

        if (!empty($tags_typed_entity_ids)) {
          $condition->condition(AbstractSynonymsBehavior::COLUMN_ENTITY_ID_PLACEHOLDER, $tags_typed_entity_ids, 'NOT IN');
        }
        if ($suggest_only_unique && !empty($matches)) {
          $tmp = array();
          foreach ($matches as $match) {
            $tmp[] = $match['target_id'];
          }
          $condition->condition(AbstractSynonymsBehavior::COLUMN_ENTITY_ID_PLACEHOLDER, $tmp, 'NOT IN');
        }

        $new_target_ids = array();
        foreach ($implementation['object']->synonymsFind($condition) as $synonym) {
          if (!$suggest_only_unique || !in_array($synonym->entity_id, $new_target_ids)) {
            $matches[] = array(
              'target_id' => $synonym->entity_id,
              'synonym' => $synonym->synonym,
              'behavior_implementation' => $implementation,
            );
            $new_target_ids[] = $synonym->entity_id;
            if (count($matches) == $max_suggestions) {
              break (2);
            }
          }
        }
      }
    }

    $synonym_entities = array();
    foreach ($matches as $match) {
      if (!isset($match['wording']) && isset($match['synonym'])) {
        $synonym_entities[] = $match['target_id'];
      }
    }
    if (!empty($synonym_entities)) {
      $synonym_entities = entity_load($field['settings']['target_type'], $synonym_entities);
      foreach ($matches as $k => $match) {
        if (!isset($match['name']) && isset($match['synonym'])) {
          if (entity_access('view', $field['settings']['target_type'], $synonym_entities[$match['target_id']])) {
            $entity_ids = entity_extract_ids($field['settings']['target_type'], $synonym_entities[$match['target_id']]);
            $matches[$k]['name'] = entity_label($field['settings']['target_type'], $synonym_entities[$match['target_id']]);
            $matches[$k]['bundle'] = $entity_ids[2];
          }
          else {
            unset($matches[$k]);
          }
        }
      }
      $matches = array_values($matches);
    }
  }

  drupal_json_output(synonyms_autocomplete_format($matches, $prefix));
}

/**
 * Supportive function to format autocomplete suggestions.
 *
 * @param array $matches
 *   Array of matched entries. It should follow this structure:
 *   - name: (string) String to be inserted into autocomplete textfield if user
 *     chooses this autocomplete entry
 *   - synonym: (string) If this entry is matched through a synonym, put that
 *     synonym here
 *   - behavior_implementation: (array) If this entry is matched through a
 *     synonym, put here the behavior implementation array that provided this
 *     match
 *   - bundle: (string) Bundle of the entity that is suggested in this entry
 * @param string $prefix
 *   Any prefix to be appended to 'name' property of $matches array when
 *   inserting into the autocomplete textfield. Normally it is the already
 *   entered entries in the textfield
 *
 * @return array
 *   Array of formatted autocomplete response entries ready to be returned to
 *   the autocomplete JavaScript
 */
function synonyms_autocomplete_format($matches, $prefix) {
  $output = array();

  $entity_info = array();

  foreach ($matches as $match) {
    $n = synonyms_autocomplete_escape($match['name']);
    while (isset($output[$prefix . $n])) {
      $n .= ' ';
    }
    $wording = check_plain($match['name']);
    if (isset($match['synonym'])) {
      if (!isset($entity_info[$match['behavior_implementation']['entity_type']])) {
        $entity_info[$match['behavior_implementation']['entity_type']] = entity_get_info($match['behavior_implementation']['entity_type']);
      }

      $wording = format_string(filter_xss_admin($match['behavior_implementation']['settings']['wording']), array(
        '@entity' => $match['name'],
        '@synonym' => $match['synonym'],
        '@field_name' => drupal_strtolower($match['behavior_implementation']['label']),
        '@bundle' => $entity_info[$match['behavior_implementation']['entity_type']]['bundles'][$match['bundle']]['label'],
      ));
    }
    $output[$prefix . $n] = $wording;
  }
  return $output;
}

/**
 * Default theme implementation for behavior settings form element.
 */
function theme_synonyms_behaviors_settings($variables) {
  drupal_add_css(drupal_get_path('module', 'synonyms') . '/synonyms.css');

  $element = &$variables['element'];

  $table = array(
    'header' => array(t('Field')),
    'rows' => array(),
    'empty' => t('Seems like there are no fields for which synonyms functionality available. Try adding a text field to get started.'),
  );

  $instance_ids = array();
  foreach (element_children($element) as $behavior) {
    $table['header'][] = check_plain($element[$behavior]['#title']);
    $instance_ids = array_unique(array_merge($instance_ids, element_children($element[$behavior])));
  }

  foreach ($instance_ids as $instance_id) {
    $row = array();
    $row_title = '';
    foreach (element_children($element) as $behavior) {
      if (isset($element[$behavior][$instance_id]['#title']) && !$row_title) {
        $row_title = check_plain($element[$behavior][$instance_id]['#title']);
      }
      $row[] = array(
        'data' => isset($element[$behavior][$instance_id]) ? drupal_render($element[$behavior][$instance_id]) : t('Not implemented'),
        'class' => array('synonyms-behavior-settings', 'synonyms-behavior-settings-' . $behavior),
      );
    }
    array_unshift($row, $row_title);
    $table['rows'][] = $row;
  }

  return '<div id="' . $element['#id'] . '">' . theme('table', $table) . drupal_render_children($element) . '</div>';
}

/**
 * Page menu callback for managing Synonyms settings of entity types.
 */
function synonyms_settings_overview() {
  $output = array();

  $output['table'] = array(
    '#theme' => 'table',
    '#header' => array(t('Entity type'), t('Bundle'), t('Manage')),
    '#rows' => array(),
  );

  foreach (entity_get_info() as $entity_type => $entity_info) {
    if (synonyms_entity_type_load($entity_type)) {
      foreach ($entity_info['bundles'] as $bundle => $bundle_info) {
        $output['table']['#rows'][] = array(
          $entity_info['label'],
          $bundle == $entity_type ? '' : $bundle_info['label'],
          l(t('Edit'), 'admin/structure/synonyms/' . $entity_type . '/' . $bundle),
        );
      }
    }
  }

  return $output;
}

/**
 * Synonyms settings form for a specific entity type and bundle name.
 *
 * @param string $entity_type
 *   Entity type for which to generate synonyms settings form
 * @param string $bundle
 *   Bundle name for which to generate synonyms settings form
 */
function synonyms_settings_form($form, &$form_state, $entity_type, $bundle) {
  $form['settings'] = array(
    '#tree' => TRUE,
    '#theme' => 'synonyms_behaviors_settings',
    '#id' => 'synonyms-behaviors-settings-wrapper',
    '#entity_type' => $entity_type,
    '#bundle' => $bundle,
  );

  $behaviors = synonyms_behaviors();

  foreach ($behaviors as $behavior => $behavior_info) {
    $form['settings'][$behavior] = array(
      '#title' => $behavior_info['title'],
    );

    $behavior_implementations = synonyms_behavior_get($behavior, $entity_type, $bundle);

    foreach ($behavior_implementations as $implementation) {
      $form['settings'][$behavior][$implementation['provider']]['#title'] = $implementation['label'];

      if (isset($form_state['values']['settings'][$behavior][$implementation['provider']])) {
        $behavior_settings = (bool) $form_state['values']['settings'][$behavior][$implementation['provider']]['enabled'];
      }
      else {
        $behavior_settings = isset($implementation['settings']);
      }

      if ($behavior_settings) {
        if (isset($form_state['values']['settings'][$behavior][$implementation['provider']]['settings'])) {
          $behavior_settings = $form_state['values']['settings'][$behavior][$implementation['provider']]['settings'];
        }
        elseif (isset($implementation['settings'])) {
          $behavior_settings = $implementation['settings'];
        }
        else {
          $behavior_settings = array();
        }
      }

      $form['settings'][$behavior][$implementation['provider']]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable'),
        '#default_value' => $behavior_settings !== FALSE,
      );

      $settings_form = ctools_plugin_get_function($behavior_info, 'settings form callback');
      if ($settings_form) {
        $form['settings'][$behavior][$implementation['provider']]['enabled']['#ajax'] = array(
          'callback' => 'synonyms_settings_form_ajax',
          'wrapper' => $form['settings']['#id'],
        );

        if ($behavior_settings !== FALSE) {
          $form['settings'][$behavior][$implementation['provider']]['settings'] = $settings_form($form, $form_state, $behavior_settings);
        }
      }
    }
  }

  $form['actions'] = array(
    '#type' => '#actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Submit handler for 'synonyms_settings_form' form.
 *
 * Store synonyms behavior settings.
 */
function synonyms_settings_form_submit($form, &$form_state) {
  foreach ($form_state['values']['settings'] as $behavior => $settings) {
    foreach ($settings as $provider => $behavior_settings) {
      $behavior_implementation = array(
        'entity_type' => $form['settings']['#entity_type'],
        'bundle' => $form['settings']['#bundle'],
        'provider' => $provider,
        'behavior' => $behavior,
        'settings' => isset($behavior_settings['settings']) ? $behavior_settings['settings'] : NULL,
      );
      if ($behavior_settings['enabled']) {
        synonyms_behavior_implementation_save($behavior_implementation);
      }
      else {
        synonyms_behavior_implementation_delete($behavior_implementation);
      }
    }
  }
  drupal_set_message(t('Synonyms settings have been successfully saved.'));
  $form_state['redirect'] = array('admin/structure/synonyms');
}

/**
 * Ajax callback function for synonyms settings form.
 */
function synonyms_settings_form_ajax($form, &$form_state) {
  return $form['settings'];
}
