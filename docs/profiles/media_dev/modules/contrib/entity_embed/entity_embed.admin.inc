<?php

/**
 * @file
 * Admin page callbacks for the entity_embed module.
 */

/**
 * Menu callback; Provide the dialog for embedding entities.
 */
function entity_embed_dialog($filter_format, $embed_button) {
  $input = $_POST;

  $dialog_options = array();

  if (isset($input['dialog'])) {
    $dialog_options = $input['dialog'];
  }

  // Render the content.
  $form = drupal_get_form('entity_embed_dialog_form', $input, $filter_format, $embed_button);

  // Determine the title: use the title provided by the form if any, otherwise
  // get it from the menu item.
  $title = isset($form['#title']) ? $form['#title'] : drupal_get_title();

  $content = drupal_render($form);
  drupal_process_attached($form);

  $commands[] = entity_embed_command_open_modal_dialog($title, $content, $dialog_options);

  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Form constructor for the entity embed dialog form.
 *
 * Allows the user to select and embed an entity.
 *
 * @see entity_embed_dialog_form_validate()
 * @ingroup forms
 */
function entity_embed_dialog_form($form, &$form_state, $input, $filter_format, $embed_button) {
  $values = isset($form_state['values']) ? $form_state['values'] : array();

  // Initialize entity element with form attributes, if present.
  $entity_element = empty($values['attributes']) ? array() : $values['attributes'];
  // The default values are set directly from \Drupal::request()->request,
  // provided by the editor plugin opening the dialog.
  if (empty($form_state['entity_element'])) {
    $form_state['entity_element'] = isset($input['editor_object']) ? $input['editor_object'] : array();
  }

  $entity_element += $form_state['entity_element'];
  $entity_element += array(
    'data-entity-type' => $embed_button->entity_type,
    'data-entity-uuid' => '',
    'data-entity-id' => '',
    'data-entity-embed-display' => 'default',
    'data-entity-embed-settings' => array(),
    'data-align' => '',
    'data-caption' => '',
    'data-view-mode' => '',
  );

  if (empty($form_state['step'])) {
    // If an entity has been selected, then always skip to the embed options.
    if (!empty($entity_element['data-entity-type']) && (!empty($entity_element['data-entity-uuid']) || !empty($entity_element['data-entity-id']))) {
      $form_state['step'] = 'embed';
    }
    else {
      $form_state['step'] = 'select';
    }
  }

  $form['#tree'] = TRUE;
  $form['#attached']['library'][] = array('entity_embed', 'drupal.editor.dialog');
  $form['#prefix'] = '<div id="entity-embed-dialog-form">';
  $form['#suffix'] = '</div>';

  switch ($form_state['step']) {
    case 'select':
      $form['#title'] = t('Embed entity');

      $form['attributes']['data-entity-type'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-type'],
      );

      $label = t('Label');
      // Attempt to display a better label if we can by getting it from
      // the label field definition.
      $entity_info = entity_get_info($entity_element['data-entity-type']);

      if (!empty($entity_info['label'])) {
        $label = check_plain($entity_info['label']);
      }

      $default = '';
      if (!empty($entity_element['data-entity-uuid']) && module_exists('uuid')) {
        $entity = entity_uuid_load($entity_element['data-entity-type'], array($entity_element['data-entity-uuid']));
        $entity = reset($entity);
      }
      else {
        $entity = entity_load_single($entity_element['data-entity-type'], $entity_element['data-entity-id']);
      }

      if ($entity) {
        $default = entity_label($entity_element['data-entity-type'], $entity);
      }

      $form['attributes']['data-entity-id'] = array(
        '#type' => 'textfield',
        '#title' => $label,
        '#default_value' => $default,
        '#autocomplete_path' => 'entity-embed/autocomplete-entity/' . $filter_format->format . '/' . $embed_button->name,
        '#required' => TRUE,
        '#description' => t('Type label and pick the right one from suggestions. Note that the unique ID will be saved.'),
      );

      // Integrate with the Media module to support selecting files to embed.
      if ($entity_element['data-entity-type'] == 'file' && module_exists('media')) {
        $form['attributes']['data-entity-id'] = array(
          '#type' => 'media',
          '#title' => $label,
          '#default_value' => $entity_element['data-entity-id'],
          '#required' => TRUE,
          '#description' => t('Select a file to embed using the media browser.'),
          '#media_options' => array(
            'global' => array(
              'types' => isset($embed_button->entity_type_bundles) ? $embed_button->entity_type_bundles : array(),
            ),
          ),
        );
      }

      $form['attributes']['data-entity-uuid'] = array(
        '#type' => 'value',
        '#title' => $entity_element['data-entity-uuid'],
      );
      $form['actions'] = array(
        '#type' => 'actions',
      );
      $form['actions']['save_modal'] = array(
        '#attributes' => array(
          'class' => array('button--primary'),
        ),
        '#type' => 'submit',
        '#value' => t('Next'),
        // No regular submit-handler. This form only works via JavaScript.
        '#submit' => array(),
        '#ajax' => array(
          'callback' => 'submitSelectForm',
          'event' => 'click',
        ),
      );
      break;

    case 'embed':
      $form['#title'] = t('Edit embedded entity');

      if (!empty($entity_element['data-entity-uuid']) && module_exists('uuid')) {
        $entity = entity_uuid_load($entity_element['data-entity-type'], array($entity_element['data-entity-uuid']));
        $entity = reset($entity);
      }
      else {
        $entity = entity_load_single($entity_element['data-entity-type'], $entity_element['data-entity-id']);
      }

      $text = t('Preview selected entity');
      if ($entity) {
        $text = entity_label($entity_element['data-entity-type'], $entity);
      }

      $uri = entity_uri($entity_element['data-entity-type'], $entity);

      $form['entity'] = array(
        '#type' => 'item',
        '#title' => t('Selected entity'),
        '#markup' => l($text, $uri['path'], $uri['options'] + array('attributes' => array('target' => '_blank'))),
      );
      $form['attributes']['data-entity-type'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-type'],
      );
      $form['attributes']['data-entity-id'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-id'],
      );
      $form['attributes']['data-entity-uuid'] = array(
        '#type' => 'value',
        '#value' => $entity_element['data-entity-uuid'],
      );

      $options = array();

      $entity_info = entity_get_info($entity_element['data-entity-type']);

      foreach ($entity_info['view modes'] as $view_mode => $info) {
        $options[$view_mode] = check_plain($info['label']);
      }

      $form['attributes']['data-view-mode'] = array(
        '#type' => 'select',
        '#title' => t('Display as'),
        '#options' => $options,
        '#default_value' => $entity_element['data-view-mode'],
        '#required' => TRUE,
        '#ajax' => array(
          'callback' => 'updatePluginConfigurationForm',
          'wrapper' => 'data-entity-embed-settings-wrapper',
          'effect' => 'fade',
        ),
      );
      $form['attributes']['data-entity-embed-settings'] = array(
        '#type' => 'container',
        '#prefix' => '<div id="data-entity-embed-settings-wrapper">',
        '#suffix' => '</div>',
      );
      $form['attributes']['data-embed-button'] = array(
        '#type' => 'value',
        '#value' => $embed_button->name,
      );
      $form['attributes']['data-entity-label'] = array(
        '#type' => 'value',
        '#value' => $embed_button->button_label,
      );

      // @todo: Potentially support 'display plugins' in the future.
//      $plugin_id = !empty($values['attributes']['data-entity-embed-display']) ? $values['attributes']['data-entity-embed-display'] : $entity_element['data-entity-embed-display'];
//      if (!empty($plugin_id)) {
//        if (is_string($entity_element['data-entity-embed-settings'])) {
//          $entity_element['data-entity-embed-settings'] = Json::decode($entity_element['data-entity-embed-settings'], TRUE);
//        }
//        $display = $this->displayPluginManager()->createInstance($plugin_id, $entity_element['data-entity-embed-settings']);
//        $display->setContextValue('entity', $entity);
//        $display->setAttributes($entity_element);
//        $form['attributes']['data-entity-embed-settings'] += $display->buildConfigurationForm($form, $form_state);
//      }

      $filters = filter_list_format($filter_format->format);

      // When Drupal core's filter_align is being used, the text editor may
      // offer the ability to change the alignment.
      if (isset($entity_element['data-align']) && isset($filters['filter_align']) && $filters['filter_align']->status) {
        $form['attributes']['data-align'] = array(
          '#title' => t('Align'),
          '#type' => 'radios',
          '#options' => array(
            'none' => t('None'),
            'left' => t('Left'),
            'center' => t('Center'),
            'right' => t('Right'),
          ),
          '#default_value' => $entity_element['data-align'] === '' ? 'none' : $entity_element['data-align'],
          '#wrapper_attributes' => array('class' => array('container-inline')),
          '#attributes' => array('class' => array('container-inline')),
          '#parents' => array('attributes', 'data-align'),
        );
      }

      // When Drupal core's filter_caption is being used, the text editor may
      // offer the ability to add a caption.
      if (isset($entity_element['data-caption']) && isset($filters['filter_caption']) && $filters['filter_caption']->status) {
        $form['attributes']['data-caption'] = array(
          '#title' => t('Caption'),
          '#type' => 'textfield',
          '#default_value' => $entity_element['data-caption'] === '' ? '' : $entity_element['data-caption'],
          '#wrapper_attributes' => array('class' => array('container-inline')),
          '#attributes' => array('class' => array('container-inline')),
          '#parents' => array('attributes', 'data-caption'),
        );
      }

      $form['actions'] = array(
        '#type' => 'actions',
      );
      $form['actions']['back'] = array(
        '#type' => 'submit',
        '#value' => t('Back'),
        // No regular submit-handler. This form only works via JavaScript.
        '#submit' => array(),
        '#ajax' => array(
          'callback' => 'goBack',
          'event' => 'click',
        ),
      );
      $form['actions']['save_modal'] = array(
        '#attributes' => array(
          'class' => array('button--primary'),
        ),
        '#type' => 'submit',
        '#value' => t('Embed'),
        // No regular submit-handler. This form only works via JavaScript.
        '#submit' => array(),
        '#ajax' => array(
          'callback' => 'submitEmbedForm',
          'event' => 'click',
        ),
      );
      break;
  }

  return $form;
}

/**
 * Form validation handler for entity_embed_dialog_form().
 *
 * @see entity_embed_dialog_form()
 */
function entity_embed_dialog_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  switch ($form_state['step']) {
    case 'select':
      if ($entity_type = $values['attributes']['data-entity-type']) {
        $title = trim($values['attributes']['data-entity-id']);

        $entity_info = entity_get_info($entity_type);

        // Prevent errors if the entity type has no label key.
        if (empty($entity_info['entity keys']['label'])) {
          form_set_error('data-entity-id', t('Unable to find label for @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
        }

        // The entity ID may be either the ID (integer) of the entity or the
        // entity's title (string).
        if (is_numeric($title)) {
          $entities = entity_load($entity_type, array($title));
        }
        else {
          $entities = entity_load($entity_type, FALSE, array($entity_info['entity keys']['label'] => array($title)));
        }

        $entity = reset($entities);

        if (!empty($entity)) {
          list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);

          if (!entity_access('view', $entity_type, $entity)) {
            form_set_error('data-entity-id', t('Unable to access @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
          }
          else {
            form_set_value($form['attributes']['data-entity-id'], $id, $form_state);

            if (isset($entity->uuid)) {
              form_set_value($form['attributes']['data-entity-uuid'], $entity->uuid, $form_state);
            }
            else {
              form_set_value($form['attributes']['data-entity-uuid'], '', $form_state);
            }
          }
        }
        else {
          form_set_error('data-entity-id', t('Unable to load @type entity @id.', array('@type' => $entity_type, '@id' => $title)));
        }
      }
      break;
  }
}

/**
 * Form submission handler to update the plugin configuration form.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function updatePluginConfigurationForm(&$form, $form_state) {
  return $form['attributes']['data-entity-embed-settings'];
}

/**
 * Form submission handler to go back to the previous step of the form.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function goBack(&$form, $form_state) {
  $form_state['step'] = 'select';
  $form_state['rebuild'] = TRUE;
  $rebuild_form = drupal_rebuild_form('entity_embed_dialog_form', $form_state, $form);
  unset($rebuild_form['#prefix'], $rebuild_form['#suffix']);
  $status_messages = array('#theme' => 'status_messages');
  $output = drupal_render($rebuild_form);
  drupal_process_attached($rebuild_form);
  $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
  $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Form submission handler that selects an entity and display embed settings.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function submitSelectForm(&$form, $form_state) {
  // Display errors in form, if any.
  if (form_get_errors()) {
    unset($form['#prefix'], $form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);
  }
  else {
    $form_state['step'] = 'embed';
    $form_state['rebuild'] = TRUE;
    $rebuild_form = drupal_rebuild_form('entity_embed_dialog_form', $form_state, $form);
    unset($rebuild_form['#prefix'], $rebuild_form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($rebuild_form);
    drupal_process_attached($rebuild_form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

/**
 * Form submission handler embeds selected entity in WYSIWYG.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @param $form_state
 *   An associative array containing the current state of the form.
 */
function submitEmbedForm(&$form, $form_state) {
  $values = $form_state['values'];

  // Display errors in form, if any.
  if (form_get_errors()) {
    unset($form['#prefix'], $form['#suffix']);
    $status_messages = array('#theme' => 'status_messages');
    $output = drupal_render($form);
    $output = '<div>' . drupal_render($status_messages) . $output . '</div>';
    $commands[] = ajax_command_html('#entity-embed-dialog-form', $output);
  }
  else {
    // Serialize entity embed settings to JSON string.
    if (!empty($values['attributes']['data-entity-embed-settings'])) {
      $values['attributes']['data-entity-embed-settings'] = drupal_json_encode($values['attributes']['data-entity-embed-settings']);
    }

    $commands[] = entity_embed_command_editor_dialog_save($values);
    $commands[] = entity_embed_command_close_modal_dialog();
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}
