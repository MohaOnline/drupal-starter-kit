<?php

/**
 * @file
 * Functional Content admin forms.
 */

/**
 * Page callback for functional content configuration.
 */
function functional_content_admin_config($form, &$form_state) {
  $form['#attached']['css'][] = drupal_get_path('module', 'functional_content') . '/functional_content.admin.css';

  $form['functional_content_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Functional Content'),
    '#description' => t('Enable Functional Content for the following views'),
    '#weight' => -50,
  );

  $theme = variable_get('theme_default', 'bartik');
  $regions = system_region_list($theme);

  foreach (_functional_content_get_view_displays() as $var_name => $view_info) {
    $is_checked = variable_get($var_name, FALSE);
    $form['functional_content_config'][$var_name] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($view_info['view_label'] . ' - ' . $view_info['display_label']),
      '#default_value' => $is_checked,
    );

    if (!$is_checked) {
      $form['functional_content_config'][$var_name . '__context'] = array(
        '#type' => 'container',
        '#attributes' => array(
          'class' => array('functional_content_config_context'),
        ),
        '#tree' => TRUE,
        '#states' => array(
          'visible' => array(
            ':input[name="' . $var_name . '"]' => array('checked' => TRUE),
          ),
        ),
        'generate_context' => array(
          '#type' => 'checkbox',
          '#title' => t('Generate context'),
          '#default_value' => '',
        ),
        'generate_context_info' => array(
          '#type' => 'container',
          '#tree' => TRUE,
          '#states' => array(
            'visible' => array(
              ':input[name="' . $var_name . '"]' => array('checked' => TRUE),
              ':input[name="' . $var_name . '__context[generate_context]"]' => array('checked' => TRUE),
            ),
          ),
          'name' => array(
            '#type' => 'textfield',
            '#title' => t('Context name'),
            '#default_value' => $view_info['view_name'] . '-' . $view_info['display_name'],
            '#element_validate' => array(
              'functional_content_admin_context_name',
            ),
          ),
          'tag' => array(
            '#type' => 'textfield',
            '#title' => t('Context tag'),
            '#default_value' => t('Functional content'),
          ),
          'region' => array(
            '#type' => 'select',
            '#title' => t('Context region'),
            '#default_value' => 'content',
            '#options' => $regions,
          ),
        ),
      );
    }
  }

  $form['#submit'][] = 'functional_content_admin_config_submit';

  return system_settings_form($form);
}

/**
 * Submit handler for the functional content configuration form.
 */
function functional_content_admin_config_submit($form, &$form_state) {
  // Create the contexts.
  foreach (_functional_content_get_view_displays() as $field_name => $view_info) {
    $enabled = $form_state['values'][$field_name];
    if ($enabled && !empty($form_state['values'][$field_name . '__context']) && !empty($form_state['values'][$field_name . '__context']['generate_context'])) {
      $settings = $form_state['values'][$field_name . '__context'];
      _functional_content_admin_config_create_context(
        $settings['generate_context_info']['name'],
        $settings['generate_context_info']['tag'],
        $settings['generate_context_info']['region'],
        $view_info['view_name'],
        $view_info['display_name']
      );
    }

    // Clean up values because the context-related fields shouldn't be saved in
    // the variable.
    unset($form_state['values'][$field_name . '__context']);
  }
}

/**
 * Helper function that creates a context.
 */
function _functional_content_admin_config_create_context($context_name, $context_tag, $region, $view_name, $display_name) {
  $callback_name = _functional_content_item_name($view_name, $display_name);
  $view_delta = $view_name . '-' . $display_name;

  ctools_include('export');
  $context = ctools_export_new_object('context');
  $context->name = $context_name;
  $context->tag = $context_tag;
  $context->conditions = array(
    'callback' => array(
      'values' => array(
        $callback_name => $callback_name,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-' . $view_delta => array(
          'module' => 'views',
          'delta' => $view_delta,
          'region' => $region,
          'weight' => '-10',
        ),
      ),
    ),
  );
  context_save($context);
}

/**
 * Page callback for functional content.
 */
function functional_content_admin($form, &$form_state) {
  $form['functional_content_nodes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Functional Content'),
    '#weight' => -50,
  );

  // Use a base-field in case entityreference_autocomplete is not available.
  $base_field = array(
    '#type' => 'textfield',
    '#size' => 5,
    '#element_validate' => array(
      'element_validate_integer_positive',
      'functional_content_admin_node_exists',
    ),
  );
  if (module_exists('entityreference_autocomplete')) {
    $base_field = array(
      '#type' => 'entityreference',
      '#era_entity_type' => 'node',
    );

    // We need this submit callback to fix the values, but it is added
    // in an alter which isn't called if domain_settings is enabled.
    if (module_exists('domain_settings')) {
      if (!isset($form['#submit'])) {
        $form['#submit'] = array();
      }
      array_unshift($form['#submit'], '_functional_content_admin_submit_era');
    }
  }

  $hook = 'functional_content';
  foreach (module_implements($hook) as $module) {
    $function = $module . '_' . $hook;
    if (function_exists($function)) {
      $group = 'functional_content_nodes';
      foreach ($function() as $var_name => $var_info) {
        if ($var_name == '#group') {
          $form[$module] = array(
            '#type' => 'fieldset',
            '#title' => check_plain($var_info['label']),
            '#description' => isset($var_info['description']) ? check_plain($var_info['description']) : '',
          );

          $group = $module;

          continue;
        }
        $form[$group][_function_content_variable_name($var_name)] = $base_field + array(
          '#title' => check_plain($var_info['label']),
          '#description' => isset($var_info['description']) ? check_plain($var_info['description']) : '',
          '#default_value' => functional_content_nid($var_name),
        );
      }
    }
  }

  return system_settings_form($form);
}

/**
 * Implements hook_variable_settings_form_alter().
 *
 * This makes sure that we're the first submit in line, to correct the value
 * provided by entityreference_autocomplete. We need a nid in the variable, not
 * an array with entity info.
 */
function functional_content_variable_settings_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id == 'functional_content_admin' && module_exists('entityreference_autocomplete')) {
    array_unshift($form['#submit'], '_functional_content_admin_submit_era');
  }
}

/**
 * Submit handler that makes sure a nid is saved and not 'node title (nid)'.
 */
function _functional_content_admin_submit_era($form, &$form_state) {
  if (module_exists('entityreference_autocomplete')) {
    foreach ($form_state['values'] as $field_name => $field) {
      if (isset($field['entity_id'])) {
        $form_state['values'][$field_name] = $field['entity_id'];
      }
    }
  }
}

/**
 * Validate function that make sure context name is correct.
 */
function functional_content_admin_context_name($element, &$form_state) {
  if (!$form_state['submitted']) {
    return;
  }

  $parents = array_slice($element['#parents'], 0, -2);
  $values = drupal_array_get_nested_value($form_state['values'], $parents);
  if (empty($values['value']) || empty($values['generate_context'])) {
    return;
  }

  if (empty($element['#value'])) {
    form_error($element, t('%field_name may not be empty.', array('%field_name' => $element['#title'])));
  }
  elseif (!preg_match('!^[a-z0-9_-]+$!', $element['#value'])) {
    form_error($element, t('%field_name can only consist of lowercase letters, underscores, dashes, and numbers.', array('%field_name' => $element['#title'])));
  }
}

/**
 * Validate function that make sure the node id exists.
 */
function functional_content_admin_node_exists($element, &$form_state) {
  if (!$form_state['submitted']) {
    return;
  }
  $value = $element['#value'];
  if (!empty($value) && !node_load($value)) {
    form_error($element, t('%name must be a valid node ID.', array('%name' => $element['#title'])));
  }
}
