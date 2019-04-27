<?php

/**
 * @file
 * Administrative functions used for the PowerTagging SeeAlso Engine module.
 */

/**
 * List all saved PowerTagging SeeAlso widgets.
 *
 * @return string
 *   The rendered HTML of the list of widgets.
 */
function powertagging_similar_admin_settings() {
  $output = array();

  $output['powertagging_similar_title'] = array(
    '#type' => 'markup',
    '#markup' => '<h3 class="semantic-connector-table-title">' . t('PowerTagging SeeAlso widgets') . '</h3>',
  );

  $powertagging_config_details = db_select('powertagging', 'p')
    ->fields('p', array('powertagging_id', 'title', 'connection_id'))
    ->execute()
    ->fetchAllAssoc('powertagging_id');

  $widgets = powertagging_similar_widget_load();
  $rows = array();
  foreach ($widgets as $widget) {
    $actions = array(
      l(t('Edit'), 'admin/config/semantic-drupal/powertagging/powertagging-similar/widgets/' . $widget->id),
      l(t('Go to block'), 'admin/structure/block/manage/powertagging_similar/powertagging_similar_widget_' . $widget->id . '/configure'),
      l(t('Delete'), 'admin/config/semantic-drupal/powertagging/powertagging-similar/widgets/' . $widget->id . '/delete'),
    );

    $rows[] = array(
      $title = '<div class="semantic-connector-led" data-server-id="' . $powertagging_config_details[$widget->powertagging_id]->connection_id . '" data-server-type="pp-server" title="' . t('Checking service') . '"></div>' . $widget->title,
      l($powertagging_config_details[$widget->powertagging_id]->title, 'admin/config/semantic-drupal/powertagging/' . $widget->powertagging_id),
      implode(' | ', $actions),
    );
  }

  $output['powertagging_similar_widgets'] = array(
    '#theme' => 'table',
    '#header' => array(
      t('Name'),
      t('PowerTagging Configuration'),
      t('Operations'),
    ),
    '#rows' => $rows,
    '#attributes' => array(
      'id' => 'powertagging-similar-widgets-table',
      'class' => array('semantic-connector-tablesorter'),
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
 * The form for a PowerTagging SeeAlso widget configuration.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $widget
 *   A PowerTagging Similiar Content widget.
 *
 * @return array
 *   The Drupal form array.
 */
function powertagging_similar_widget_form($form, &$form_state, $widget = NULL) {
  // A new widget gets created.
  if (is_null($widget)) {
    $widget = powertagging_similar_widget_default_config_load();
    $is_new = true;
  }
  else {
    $form['id'] = array(
      '#type' => 'hidden',
      '#value' => $widget->id,
    );
    $is_new = false;
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('Name of the PowerTagging configuration.') . (isset($overridden_values['title']) ? ' <span class="semantic-connector-overridden-value">' . t('Warning: overridden by variable') . '</span>' : ''),
    '#size' => 35,
    '#maxlength' => 255,
    '#default_value' => $widget->title,
    '#required' => TRUE,
  );

  $powertagging_ids = db_select('powertagging', 'p')
    ->fields('p', array('powertagging_id', 'title'))
    ->execute()
    ->fetchAllKeyed();

  $form['powertagging_id'] = array(
    '#type' => 'select',
    '#title' => t('PowerTagging Configuration'),
    '#options' => $powertagging_ids,
    '#required' => TRUE,
    '#default_value' => ($widget->powertagging_id > 0 ? $widget->powertagging_id : key($powertagging_ids)),
  );

  $form['content_types']['#tree'] = TRUE;
  foreach ($powertagging_ids as $powertagging_id => $title) {
    $field_instances = powertagging_get_tagging_field_instances($powertagging_id);
    $fields = powertagging_get_tagging_field_options_list($field_instances);

    $form['content_types'][$powertagging_id] = array(
      '#type' => 'item',
      '#states' => array(
        'visible' => array(
          ':input[name="powertagging_id"]' => array('value' => $powertagging_id),
        ),
      ),
    );

    // Content types available containing PowerTagging fields.
    if (!empty($fields)) {
      $weighted_content_types = array();
      $added_field_keys = array();

      // Add existing configuration first.
      if (!empty($widget->config['content_types']) && isset($widget->config['content_types'][$powertagging_id])) {
        foreach ($widget->config['content_types'][$powertagging_id] as $content_type) {
          // Check if this content type still exists.
          if (isset($fields[$content_type['entity_key']])) {
            $content_type['entity_label'] = $fields[$content_type['entity_key']];
            $weighted_content_types[] = $content_type;
            $added_field_keys[] = $content_type['entity_key'];
          }
        }
      }

      // Add new content configuration at the end of the list.
      foreach ($fields as $field_keys => $field_label) {
        if (!in_array($field_keys, $added_field_keys)) {
          $weighted_content_types[] = array(
            'entity_key' => $field_keys,
            'entity_label' => $field_label,
            'show' => FALSE,
            'title' => '',
            'count' => 5,
          );
        }
      }

      foreach ($weighted_content_types as $weight => $content_type) {
        $key = $content_type['entity_key'];
        $form['content_types'][$powertagging_id]['content'][$key]['node'] = array(
          '#markup' => $content_type['entity_label'],
        );

        // This field is invisible, but contains sort info (weights).
        $form['content_types'][$powertagging_id]['content'][$key]['weight'] = array(
          '#type' => 'weight',
          // Weights from -255 to +255 are supported because of this delta.
          '#delta' => 255,
          '#title_display' => 'invisible',
          '#default_value' => $weight,
        );

        $form['content_types'][$powertagging_id]['content'][$key]['show'] = array(
          '#type' => 'checkbox',
          '#default_value' => $content_type['show'],
        );

        $form['content_types'][$powertagging_id]['content'][$key]['title'] = array(
          '#type' => 'textfield',
          '#default_value' => $content_type['title'],
          '#states' => array(
            'disabled' => array(
              ':input[name="merge_content"]' => array('checked' => TRUE),
            ),
          ),
        );

        $form['content_types'][$powertagging_id]['content'][$key]['count'] = array(
          '#type' => 'select',
          '#options' => array_combine(range(1, 10),range(1, 10)),
          '#default_value' => $content_type['count'],
          '#states' => array(
            'disabled' => array(
              ':input[name="merge_content"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
    }
    // No content type available.
    else {
      $form['content_types'][$powertagging_id]['title'] = array(
        '#type' => 'markup',
        '#markup' => t('No content type is connected to this PowerTagging configuration.'),
      );
    }
  }

  $form['display_type'] = array(
    '#type' => 'select',
    '#title' => t('How to display the content'),
    '#description' => t('How to display the items in the list of similar content.'),
    '#options' => array(
      'default' => 'Title as a link (default)',
      'view_mode' => 'Customized display ("Powertagging SeeAlso content" view mode)'
    ),
    '#default_value' => $widget->config['display_type'],
  );

  $form['merge_content'] = array(
    '#type' => 'checkbox',
    '#title' => t('Merge content'),
    '#description' => t('Display all content types in a single content list.'),
    '#default_value' => $widget->config['merge_content'],
  );

  $form['merge_content_count'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to display'),
    '#description' => t('The maximum number of similar items you want to display.'),
    '#options' => array_combine(range(1, 10),range(1, 10)),
    '#default_value' => $widget->config['merge_content_count'],
    '#states' => array(
      'visible' => array(
        ':input[name="merge_content"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  if (!$is_new) {
    $form['delete'] = array(
      '#type' => 'button',
      '#value' => t('Delete'),
      '#attributes' => array('onclick' => 'window.location.href = "' . url('admin/config/semantic-drupal/powertagging/powertagging-similar/widgets/' . $widget->id . '/delete') . '"; return false;'),
    );
  }
  $form['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => (isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/semantic-drupal/powertagging/powertagging-similar'),
  );

  return $form;
}

/**
 * Theme function for the PowerTagging SeeAlso widget configuration form.
 *
 * @param array $variables
 *   An array of variables supported by the theme.
 *
 * @return string
 *   The rendered HTML content of the form.
 */
function theme_powertagging_similar_widget_form($variables) {
  $form = $variables['form'];

  foreach ($form['content_types'] as $powertagging_id => &$content_type) {
    if (isset($content_type['content'])) {
      $rows = array();
      foreach (element_children($content_type['content']) as $content_id) {
        $content_type['content'][$content_id]['weight']['#attributes']['class'] = array('content-types-' . $powertagging_id . '-order-weight');
        $rows[] = array(
          'data' => array(
            drupal_render($content_type['content'][$content_id]['node']),
            drupal_render($content_type['content'][$content_id]['show']),
            drupal_render($content_type['content'][$content_id]['title']),
            drupal_render($content_type['content'][$content_id]['count']),
            drupal_render($content_type['content'][$content_id]['weight']),
          ),
          'class' => array('draggable'),
        );
      }

      $header = array(t('Content'), t('Show'), t('Title'), t('Number of items to display'), t('Weight'));
      $content_type['content'] = array(
        '#type' => 'item',
        '#title' => t('Content types'),
        '#description' => t('Choose the content you want to display in the widget and in which order.'),
        '#markup' => theme(
          'table',
          array(
            'header' => $header,
            'rows' => $rows,
            'attributes' => array('id' => 'content-types-' . $powertagging_id . '-order'),
          )
        ),
      );

      drupal_add_tabledrag('content-types-' . $powertagging_id . '-order', 'order', 'sibling', 'content-types-' . $powertagging_id . '-order-weight');
    }
    unset($content_type);
  }

  $output = drupal_render_children($form);
  return $output;
}

/**
 * Validate handler for powertagging_similar_widget_form.
 */
function powertagging_similar_widget_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // At least one content type needs to be shown for the widget.
  // Is there even content available?
  if (!isset($values['content_types'][$values['powertagging_id']]['content'])) {
    form_set_error('', t('At least one content type needs to be selected to be displayed in the widget.'));
  }
  else {
    $content_types = $values['content_types'][$values['powertagging_id']]['content'];
    $content_selected = FALSE;
    foreach ($content_types as $entity_key => $content_type) {
      if ($content_type['show']) {
        $content_selected = TRUE;

        // Selected content types need a title.
        if (!$values['merge_content'] && trim($content_type['title']) == '') {
          form_set_error('content_types][' . $values['powertagging_id'] . '][content][' . $entity_key . '][title', t('Selected content types need a title.'));
        }
      }
    }
    // Is any content selected?
    if (!$content_selected) {
      form_set_error('', t('At least one content type needs to be selected to be displayed in the widget.'));
    }
  }
}

/**
 * Submit handler for powertagging_similar_widget_form.
 */
function powertagging_similar_widget_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  foreach ($values['content_types'] as &$content_type) {
    $weighted_content = array();
    foreach ($content_type['content'] as $entity_id => $content) {
      $weight = $content['weight'];
      unset($content['weight']);
      $content['entity_key'] = $entity_id;
      $weighted_content[$weight] = $content;
    }
    if (!empty($weighted_content)) {
      ksort($weighted_content);
      $weighted_content = array_values($weighted_content);
    }
    $content_type = $weighted_content;
    unset($content_type);
  }

  // Serialize the configuration data.
  $serialized_config = serialize(array(
    'content_types' => $values['content_types'],
    'display_type' => $values['display_type'],
    'merge_content' => $values['merge_content'],
    'merge_content_count' => $values['merge_content_count']
  ));

  // A new widget gets created.
  if (!isset($form_state['values']['id'])) {
    db_insert('powertagging_similar_widgets')
      ->fields(array(
        'title' => $form_state['values']['title'],
        'powertagging_id' => $form_state['values']['powertagging_id'],
        'config' => $serialized_config,
      ))
      ->execute();
  }
  // An existing widget gets updated.
  else {
    // Save configuration into database.
    db_update('powertagging_similar_widgets')
      ->fields(array(
        'title' => $form_state['values']['title'],
        'powertagging_id' => $form_state['values']['powertagging_id'],
        'config' => $serialized_config,
      ))
      ->condition('id', $form_state['values']['id'])
      ->execute();
  }

  drupal_set_message(t('PowerTagging SeeAlso widget "%title" has been saved.', array('%title' => $form_state['values']['title'])));
  $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging/powertagging-similar';
}

/**
 * List deletion form.
 *
 * @param array $form
 *   The form array.
 * @param array &$form_state
 *   The form_state array.
 * @param object $config
 *   A PowerTagging SeeAlso widget configuration.
 *
 * @return array
 *   The Drupal form array of the confirmation form.
 */
function powertagging_similar_widget_delete_form($form, &$form_state, $config) {
  $form_state['config'] = $config;
  return confirm_form($form,
    t('Are you sure you want to delete PowerTagging SeeAlso widget "%title"?', array('%title' => $config->title)),
    'admin/config/semantic-drupal/powertagging/powertagging-similar',
    t('This action cannot be undone.'),
    t('Delete configuration'));
}

/**
 * Submit handler for powertagging_delete_form().
 */
function powertagging_similar_widget_delete_form_submit($form, &$form_state) {
  $config = $form_state['config'];
  powertagging_similar_widget_delete($config->id);
  drupal_set_message(t('PowerTagging SeeAlso widget "%title" has been deleted.', array('%title' => $config->title)));
  $form_state['redirect'] = 'admin/config/semantic-drupal/powertagging/powertagging-similar';
}
