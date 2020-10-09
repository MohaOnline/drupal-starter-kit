<?php

/**
 * @file
 * Definition of synonyms_views_handler_filter_term_tid class.
 */

/**
 * Synonyms friendly taxonomy filter handler.
 */
class synonyms_views_handler_filter_term_tid extends views_handler_filter_term_node_tid {
  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $form['type']['#options']['synonyms_autocomplete'] = t('Synonyms friendly autocomplete');
    $form['type']['#options']['synonyms_select'] = t('Synonyms friendly select');
  }

  function value_form(&$form, &$form_state) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($this->options['vocabulary']);
    if (empty($vocabulary) && $this->options['limit']) {
      $form['markup'] = array(
        '#markup' => '<div class="form-item">' . t('An invalid vocabulary is selected. Please change it in the options.') . '</div>',
      );
      return;
    }

    switch ($this->options['type']) {
      case 'synonyms_autocomplete':
        $tags = array();
        if ($this->value) {
          $result = taxonomy_term_load_multiple($this->value);
          foreach ($result as $entity_term) {
            $tags[] = entity_label('taxonomy_term', $entity_term);
          }
        }
        $tags = drupal_implode_tags($tags);

        $info = $this->synonyms_field_instance();

        if ($info['instance']['widget']['type'] == 'synonyms_autocomplete_taxonomy_term') {
          $widget = $info['instance']['widget']['settings'];
        }
        else {
          $widget = field_info_widget_settings('synonyms_autocomplete_taxonomy_term');
        }
        $autocomplete_path = $widget['synonyms_autocomplete_path'];
        $size = $widget['size'];

        $form['value'] = array(
          '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)) : t('Select terms'),
          '#type' => 'textfield',
          '#default_value' => $tags,
          '#autocomplete_path' => $autocomplete_path . '/' . $this->definition['field_name'] . '/' . $info['instance']['entity_type'] . '/' . $info['instance']['bundle'],
          '#size' => $size,
          '#auto_creation' => FALSE,
          '#attributes' => array('class' => array('synonyms-autocomplete')),
          '#attached' => array('js' => array(
            drupal_get_path('module', 'synonyms') . '/js/synonyms-autocomplete.js' => array(),
          )),
        );
        break;

      case 'synonyms_select':
        $info = $this->synonyms_field_instance();

        $options = array();
        $widget = $info['instance']['widget']['type'] == 'synonyms_select_taxonomy_term' ? $info['instance']['widget']['settings'] : field_info_widget_settings('synonyms_select_taxonomy_term');
        foreach ($info['field']['settings']['allowed_values'] as $tree) {
          if ($vocabulary = taxonomy_vocabulary_machine_name_load($tree['vocabulary'])) {
            switch ($widget['sort']) {
              case 'weight':
                if ($terms = taxonomy_get_tree($vocabulary->vid, $tree['parent'], NULL, TRUE)) {
                  $behavior_implementations = synonyms_behavior_get('select', 'taxonomy_term', field_extract_bundle('taxonomy_term', $vocabulary), TRUE);
                  foreach ($terms as $term) {
                    $options[] = synonyms_select_option_entity($term, 'taxonomy_term', NULL, NULL, array('depth'));
                    foreach ($behavior_implementations as $implementation) {
                      foreach ($implementation['object']->extractSynonyms($term) as $synonym) {
                        $options[] = synonyms_select_option_entity($term, 'taxonomy_term', $synonym, $implementation, array('depth'));
                      }
                    }
                  }
                }
                break;

              case 'name':
                // TODO: is there any way to leverage DB for the sorting routine?
                $options = synonyms_select_taxonomy_term_sort_name_options_recursive($vocabulary, $tree['parent']);
                break;
            }
          }
        }

        $form['value'] = array(
          '#type' => 'select',
          '#title' => $this->options['limit'] ? t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)) : t('Select terms'),
          '#multiple' => TRUE,
          '#options' => $options,
          '#size' => min(9, count($options)),
          '#default_value' => (array) $this->value,
          '#element_validate' => array('synonyms_select_validate'),
        );
        break;

      default:
        parent::value_form($form, $form_state);
        break;
    }
  }

  function value_validate($form, &$form_state) {
    switch ($this->options['type']) {
      case 'synonyms_autocomplete':
        $values = drupal_explode_tags($form_state['values']['options']['value']);
        $tids = $this->synonyms_validate_term_strings($form['value'], $values);

        if ($tids) {
          $form_state['values']['options']['value'] = $tids;
        }
        break;

      case 'synonyms_select':
        break;

      default:
        parent::value_validate($form, $form_state);
        break;
    }
  }

  function exposed_validate(&$form, &$form_state) {
    switch ($this->options['type']) {
      case 'synonyms_autocomplete':
        if (empty($this->options['exposed'])) {
          return;
        }
        if (empty($this->options['expose']['identifier'])) {
          return;
        }

        $identifier = $this->options['expose']['identifier'];
        $values = drupal_explode_tags($form_state['values'][$identifier]);
        $tids = $this->synonyms_validate_term_strings($form[$identifier], $values);

        if ($tids) {
          $this->validated_exposed_input = $tids;
        }
        break;

      case 'synonyms_select':
        if (empty($this->options['exposed'])) {
          return;
        }
        if (empty($this->options['expose']['identifier'])) {
          return;
        }

        $all_key = array_search('All', $form_state['values'][$this->options['expose']['identifier']]);
        if ($all_key !== FALSE) {
          unset($form_state['values'][$this->options['expose']['identifier']][$all_key]);
        }

        if (!empty($form_state['values'][$this->options['expose']['identifier']])) {
          return;
        }
        $this->validated_exposed_input = $form_state['values'][$this->options['expose']['identifier']];
        break;

      default:
        parent::exposed_validate($form, $form_state);
        break;
    }
  }

  /**
   * Validate the user string.
   *
   * In a great extend it does the same job as parent::validate_term_strings(),
   * just that this implementation is synonyms-aware.
   *
   * @param $element
   *   The form element which is used, either the views ui or the exposed
   *   filters.
   * @param $values
   *   The taxonomy names/synonyms which will be converted to tids.
   *
   * @return array
   *   The taxonomy ids for all validated terms.
   */
  protected function synonyms_validate_term_strings($element, $values) {
    if (empty($values)) {
      return array();
    }

    $values = array_map('drupal_strtolower', $values);
    $missing = array_flip($values);

    $tids = array();

    $vocabulary = taxonomy_vocabulary_machine_name_load($this->options['vocabulary']);

    // Firstly looking up the entered tags as if they were term names. Then,
    // the remaining tags are looked up as if they were synonyms of terms.
    // Lastly, if any tags are left at this point, we mark form validation
    // error.
    $query = db_select('taxonomy_term_data', 'td');
    $query->fields('td', array('tid', 'name'));
    $query->condition('td.vid', $vocabulary->vid);
    $query->condition('td.name', $values);
    $query->addTag('term_access');
    $result = $query->execute();
    foreach ($result as $term) {
      unset($missing[drupal_strtolower($term->name)]);
      $tids[] = $term->tid;
    }

    $behavior_implementations = synonyms_behavior_get('autocomplete', 'taxonomy_term', $vocabulary->machine_name, TRUE);
    foreach ($behavior_implementations as $behavior_implementation) {
      if (!empty($missing)) {
        $condition = db_or();
        foreach ($missing as $tag => $v) {
          $condition->condition(AbstractSynonymsBehavior::COLUMN_SYNONYM_PLACEHOLDER, $tag);
        }
        $synonyms = $behavior_implementation['object']->synonymsFind($condition);
        foreach ($synonyms as $synonym) {
          $synonym->synonym = drupal_strtolower($synonym->synonym);
          unset($missing[$synonym->synonym]);
          $tids[] = $synonym->entity_id;
        }
      }
    }

    if (!empty($missing) && !empty($this->options['error_message'])) {
      form_error($element, format_plural(count($missing), 'Unable to find term: @terms', 'Unable to find terms: @terms', array('@terms' => implode(', ', array_keys($missing)))));
    }

    return $tids;
  }

  /**
   * Collect info about field and instance that correspond to this filter.
   *
   * @return array
   *   Array with the following structure:
   *   - field: (array) Field definition array that corresponds to this filter
   *   - instance: (array) Field instance definition array that corresponds to
   *     this filter
   */
  protected function synonyms_field_instance() {
    $entity_type_base_table = $this->view->base_table;
    // TODO: it would be nice to consider the existence of relationships, but
    // I just couldn't figure it out at that time.

    $entity_info = entity_get_info();
    $field_entity_type = FALSE;
    $field = field_info_field($this->definition['field_name']);

    foreach ($field['bundles'] as $entity_type => $bundles) {
      if ($entity_info[$entity_type]['base table'] == $entity_type_base_table) {
        $field_entity_type = $entity_type;
        break;
      }
    }

    if (!$field_entity_type) {
      // Seems like we failed to determine the entity type which is used for
      // this field in the view. Well, it's not a fatal fail, we'll just use
      // whatever then.
      $field_entity_type = array_keys($field['bundles']);
      $field_entity_type = $field_entity_type[0];
    }

    // We just grab the first instance of this field within the determined
    // entity type.
    $bundle = $field['bundles'][$field_entity_type][0];

    $instance = field_info_instance($field_entity_type, $field['field_name'], $bundle);

    return array(
      'field' => $field,
      'instance' => $instance,
    );
  }
}
