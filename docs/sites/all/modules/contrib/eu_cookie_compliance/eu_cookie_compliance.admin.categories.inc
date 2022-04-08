<?php

/**
 * @file
 * This file provides administration forms for the cookie categories.
 */

/**
 * Form to order cookie consent categories & an overview at the same time.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 *
 * @return array
 *   Form render array.
 */
function eu_cookie_compliance_admin_categories_form(array $form, array $form_state) {
  $form = array(
    '#tree' => TRUE,
    'categories' => array(),
  );
  $categories = variable_get('eu_cookie_compliance_categories', array());
  $form['#submit'][] = 'eu_cookie_compliance_admin_categories_form_submit';
  $form['eu_cookie_compliance_categories'] = array('#type' => 'hidden');

  $options_list = _eu_cookie_compliance_get_category_checkbox_default_state_options_list();

  foreach ($categories as $machine_name => $category) {
    $form['categories'][$machine_name]['label'] = array(
      '#markup' => filter_xss($category['label']),
    );
    $form['categories'][$machine_name]['machine_name'] = array(
      '#markup' => check_plain($machine_name),
    );
    $form['categories'][$machine_name]['description'] = array(
      '#markup' => filter_xss($category['description']),
    );
    $form['categories'][$machine_name]['checkbox_default_state'] = array(
      '#markup' => isset($options_list[$category['checkbox_default_state']]) ? $options_list[$category['checkbox_default_state']] : $options_list['unchecked'],
    );
    // This field is invisible, but contains sort info (weights).
    $form['categories'][$machine_name]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#title_display' => 'invisible',
      '#default_value' => $category['weight'],
    );
    $options = array();
    // Adapt links to follow realm language if present.
    $query_parameters = drupal_get_query_parameters();
    if (isset($query_parameters['variable_realm_key_language'])) {
      $language_list = language_list();
      $language = $language_list[$query_parameters['variable_realm_key_language']] ?: FALSE;
      if ($language) {
        $options['language'] = $language;
      }
    }
    $form['categories'][$machine_name]['op'] = array(
      '#markup' =>
      l(t('Edit'), '/admin/config/system/eu-cookie-compliance/categories/' . check_plain($machine_name) . '/edit', $options) .
      ' | ' .
      l(t('Delete'), '/admin/config/system/eu-cookie-compliance/categories/' . check_plain($machine_name) . '/delete', $options),
    );
  }

  $options = array();
  // Adapt links to follow realm language if present.
  $query_parameters = drupal_get_query_parameters();
  if (isset($query_parameters['variable_realm_key_language'])) {
    $language_list = language_list();
    $language = $language_list[$query_parameters['variable_realm_key_language']] ?: FALSE;
    if ($language) {
      $options['language'] = $language;
    }
  }

  $form['add_new'] = array(
    '#type' => 'markup',
    '#markup' => t('<ul class="action-links"><li><a href="@path">Add new category</a></li></ul>', array(
      '@path' => url('admin/config/system/eu-cookie-compliance/categories/add', $options),
    )),
  );

  $form = system_settings_form($form);
  unset($form['#theme']);

  return $form;
}

/**
 * Implements hook_theme().
 */
function theme_eu_cookie_compliance_admin_categories_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form['categories']) as $machine_name) {
    $form['categories'][$machine_name]['weight']['#attributes']['class'] = array('categories-order-weight');
    $rows[] = array(
      'data' => array(
        array('class' => array('slide-cross')),
        drupal_render($form['categories'][$machine_name]['label']),
        drupal_render($form['categories'][$machine_name]['machine_name']),
        drupal_render($form['categories'][$machine_name]['description']),
        drupal_render($form['categories'][$machine_name]['checkbox_default_state']),
        drupal_render($form['categories'][$machine_name]['weight']),
        drupal_render($form['categories'][$machine_name]['op']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(
    '',
    t('Label'),
    t('Machine name'),
    t('Description'),
    t('Checkbox default state'),
    t('Weight'),
    t('Operations'),
  );
  $options = array();
  // Adapt links to follow realm language if present.
  $query_parameters = drupal_get_query_parameters();
  if (isset($query_parameters['variable_realm_key_language'])) {
    $language_list = language_list();
    $language = $language_list[$query_parameters['variable_realm_key_language']] ?: FALSE;
    if ($language) {
      $options['language'] = $language;
    }
  }
  $output = drupal_render_children($form, array('add_new'));
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'empty' => t('No categories exist yet. <a href="@path">Add new category.</a>', array(
      '@path' => url('admin/config/system/eu-cookie-compliance/categories/add', $options),
    )),
    'attributes' => array(
      'id' => 'categories-order',
    ),
  )
  );

  $output .= drupal_render_children($form);

  drupal_add_tabledrag('categories-order', 'order', 'sibling', 'categories-order-weight');

  return $output;
}

/**
 * Submit handler for ordering cookie consent categories.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function eu_cookie_compliance_admin_categories_form_submit(array $form, array &$form_state) {
  $categories = variable_get('eu_cookie_compliance_categories', array());

  // Update categories with new weight values.
  $weights = $form_state['values']['categories'];
  foreach ($weights as $machine_name => $data) {
    $categories[$machine_name]['weight'] = $data['weight'];
  }

  // Order the categories by their weight.
  uasort($categories, function ($a, $b) {
    return $a['weight'] - $b['weight'];
  });

  if (!empty($form_state['values']['domain_settings']['domain_id'])) {
    $domain_id = $form_state['values']['domain_settings']['domain_id'];
  }
  else {
    $domain_id = NULL;
  }

  $form_state['values'] = array();
  $form_state['values']['eu_cookie_compliance_categories'] = $categories;

  if ($domain_id) {
    $form_state['values']['domain_id'] = $domain_id;
  }
}

/**
 * Provides form for adding / editing a cookie consent category.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   For state array.
 * @param string $machine_name
 *   The machine name of the category.
 *
 * @return array
 *   Form render array.
 */
function eu_cookie_compliance_admin_category_form(array $form, array $form_state, $machine_name = NULL) {
  $form = array();
  $categories = variable_get('eu_cookie_compliance_categories', array());
  // Add this so i18n can add what it needs.
  $form['eu_cookie_compliance_categories'] = array('#type' => 'hidden');
  $form['#submit'][] = 'eu_cookie_compliance_admin_category_form_submit';

  // We are in edit mode.
  if ($machine_name !== NULL) {
    $category = isset($categories[$machine_name]) ? $categories[$machine_name] : NULL;
    if (!$category) {
      drupal_set_message(t('You are tying to edit an invalid cookie category: %name.', array('%name' => $machine_name)), 'error');
      drupal_goto('admin/config/system/eu-cookie-compliance/categories');
    }
  }
  // Or we are in add mode.
  else {
    $category = array(
      'machine_name' => '',
      'label' => '',
      'description' => '',
      'checkbox_default_state' => 'unchecked',
      'weight' => 0,
    );
  }

  $form['category'] = array(
    '#type' => 'value',
    '#value' => $category,
  );

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#description' => t('The name that will be shown to the visitor.'),
    '#required' => TRUE,
    '#default_value' => filter_xss($category['label']) ?: '',
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#default_value' => filter_xss($category['machine_name']),
    '#machine_name' => array(
      'exists' => 'eu_cookie_compliance_admin_category_exists',
      'source' => array('label'),
    ),
    '#disabled' => !empty($category['machine_name']),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('The description that will be shown to the visitor.'),
    '#required' => FALSE,
    '#default_value' => filter_xss($category['description']) ?: '',
  );

  $form['checkbox_default_state'] = array(
    '#type' => 'radios',
    '#title' => t('Checkbox default state'),
    '#description' => t("Determines the default state of this category's selection checkbox on the cookie consent popup."),
    '#options' => _eu_cookie_compliance_get_category_checkbox_default_state_options_list(),
    '#required' => TRUE,
    '#default_value' => isset($category['checkbox_default_state']) ? $category['checkbox_default_state'] : 'unchecked',
  );

  $form = system_settings_form($form);

  $options = array();
  // Adapt links to follow realm language if present.
  $query_parameters = drupal_get_query_parameters();
  if (isset($query_parameters['variable_realm_key_language'])) {
    $language_list = language_list();
    $language = $language_list[$query_parameters['variable_realm_key_language']] ?: FALSE;
    if ($language) {
      $options['language'] = $language;
    }
  }
  if ($category['machine_name']) {
    $form['actions']['delete'] = array(
      '#type' => 'markup',
      '#markup' => l(t('Delete'), 'admin/config/system/eu-cookie-compliance/categories/' . check_plain($category['machine_name']) . '/delete', $options),
    );
  }

  return $form;
}

/**
 * Submit handler for adding / editing a cookie consent category.
 *
 * All that happens here is massaging form values to match what
 * system_settings_form expects. Bonus is that i18n variables can then also work
 * its magic, with a little help from
 * eu_cookie_compliance_variable_settings_form_alter.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function eu_cookie_compliance_admin_category_form_submit(array $form, array &$form_state) {
  $categories = variable_get('eu_cookie_compliance_categories', array());

  if (!isset($categories[$form_state['values']['machine_name']])) {
    $categories[$form_state['values']['machine_name']] = array(
      'weight' => 0,
    );
  }

  $categories[$form_state['values']['machine_name']] = array_merge(
    $categories[$form_state['values']['machine_name']],
    array(
      'machine_name' => $form_state['values']['machine_name'],
      'label' => $form_state['values']['label'],
      'description' => $form_state['values']['description'],
      'checkbox_default_state' => $form_state['values']['checkbox_default_state'],
    )
  );

  $label = $form_state['values']['label'];
  if (!empty($form_state['values']['domain_id'])) {
    $domain_id = $form_state['values']['domain_id'];
  }
  else {
    $domain_id = NULL;
  }

  $form_state['values'] = array();
  $form_state['values']['eu_cookie_compliance_categories'] = $categories;

  if ($domain_id) {
    $form_state['values']['domain_id'] = $domain_id;
  }

  eu_cookie_compliance_clear_caches();
  drupal_set_message(t('The category %label has been saved.', array(
    '%label' => $label,
  )));
  $form_state['redirect'] = 'admin/config/system/eu-cookie-compliance/categories';
}

/**
 * Confirm form for deleting an existing cookie category.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 * @param string $machine_name
 *   The machine name of the category.
 *
 * @return array
 *   Confirm form render array.
 */
function eu_cookie_compliance_admin_category_delete_form(array $form, array &$form_state, $machine_name) {
  $categories = variable_get('eu_cookie_compliance_categories', array());
  $form['eu_cookie_compliance_categories'] = array('#type' => 'hidden');
  $category = isset($categories[$machine_name]) ? $categories[$machine_name] : NULL;
  if (!$category) {
    drupal_set_message(t('You are tying to delete an invalid cookie category: %name.', array('%name' => $machine_name)), 'error');
    drupal_goto('admin/config/system/eu-cookie-compliance/categories');
  }

  $form['machine_name'] = array(
    '#type' => 'value',
    '#value' => $machine_name,
  );
  $question = t('Are you sure you want to delete the %category category?', array(
    '%category' => $category['label'],
  ));

  $form['#submit'][] = 'eu_cookie_compliance_admin_category_delete_form_submit';
  $form['#submit'][] = 'system_settings_form_submit';

  return confirm_form($form, $question, 'admin/config/system/eu-cookie-compliance/categories', t('This action cannot be undone.'));
}

/**
 * Submit handler for deleting an existing cookie category.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Form state array.
 */
function eu_cookie_compliance_admin_category_delete_form_submit(array $form, array &$form_state) {
  $categories = variable_get('eu_cookie_compliance_categories', array());

  $label = $categories[$form_state['values']['machine_name']]['label'];
  unset($categories[$form_state['values']['machine_name']]);

  $form_state['values'] = array();
  $form_state['values']['eu_cookie_compliance_categories'] = $categories;

  eu_cookie_compliance_clear_caches();
  drupal_set_message(t('The category %label has been deleted.', array(
    '%label' => $label,
  )));
  $form_state['redirect'] = 'admin/config/system/eu-cookie-compliance/categories';
}

/**
 * Callback for checking whether a submitted machine name value already exists.
 *
 * @param string $machine_name
 *   The machine name to search for.
 *
 * @return bool
 *   Whether the machine name already exists or not.
 */
function eu_cookie_compliance_admin_category_exists($machine_name) {
  $categories = variable_get('eu_cookie_compliance_categories', array());

  if (isset($categories[$machine_name])) {
    return TRUE;
  }

  return FALSE;
}
