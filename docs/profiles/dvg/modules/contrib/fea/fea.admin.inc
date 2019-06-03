<?php
/**
 * Administration form to configure FEA.
 */
function fea_settings_form() {
  $form = array();
  $permissions = fea_permission();
  $ids = array_map('_fea_strip_safe_form_id', array_keys(_fea_get_form_ids()));

  $form['form_ids'] = array(
    '#title' => t('Form IDs'),
    '#type' => 'fieldset',
  );
  $form['form_ids']['fea__display_form_id'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display all form ids for users with the %perm permission.', array(
      '%perm' => $permissions['administer fea settings']['title'],
    )),
    '#default_value' => variable_get('fea__display_form_id', FALSE),
  );
  $form['form_ids']['fea__index_new_elements'] = array(
    '#type' => 'checkbox',
    '#title' => t('Index new form elements when encountered.', array(
      '%perm' => $permissions['administer fea settings']['title'],
    )),
    '#default_value' => variable_get('fea__index_new_elements', TRUE),
  );

  if (isset($_GET['add'])) {
    drupal_set_message(t('The added form id will not be saved until this form is submitted.'), 'warning');
    $ids[] = $_GET['add'];
  }
  $form['form_ids']['form_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('Enable FEA on these form ids'),
    '#description' => t('Put each form id on a new line. No wildcards allowed.'),
    '#default_value' => implode("\n", $ids),
  );

  return system_settings_form($form);
}

/**
 * Form validate function to explode form ids.
 */
function fea_settings_form_validate(&$form, &$form_state) {
  $ids = array_filter(array_map('trim', explode("\n", $form_state['values']['form_ids'])));
  $form_ids = _fea_get_form_ids();

  if ($blacklisted = array_intersect($ids, fea_get_form_id_blacklist())) {
    form_set_error('form_ids', format_plural(
      count($blacklisted),
     'The form with id %ids is blacklisted and can\'t be added.',
     'The forms with ids %ids are blacklisted and can\'t be added.',
      array('%ids' => implode(', ', $blacklisted))
    ));
    return;
  }

  foreach ($ids as $index => $id) {
    if (!isset($form_ids[_fea_safe_form_id($id)])) {
      variable_set(_fea_safe_form_id($id), array(
        'form_id' => $id,
        'invert' => FALSE,
        'elements' => array(),
      ));
    }
    else {
      unset($form_ids[_fea_safe_form_id($id)]);
    }
  }

  // Delete remaining form_ids.
  foreach (array_keys($form_ids) as $id) {
    variable_del($id);
  }

  // Don't store this value.
  unset($form_state['values']['form_ids']);
}

/**
 * Administration form to configure field access.
 *
 * @todo: auto enable parent fieldsets
 */
function fea_access_form() {
  $form = array();
  $roles = user_roles();
  $form_ids = _fea_get_form_ids(TRUE);

  foreach ($form_ids as $safe_form_id => $config) {
    $config_backup = $config;
    drupal_alter('fea_form_config', $config, $config['form_id']);
    $config_overrides = drupal_array_diff_assoc_recursive($config, $config_backup);

    $form[$safe_form_id] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($config['form_id']),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE,
    );
    $form[$safe_form_id]['form_id'] = array(
      '#type' => 'value',
      '#value' => $config['form_id'],
    );
    $form[$safe_form_id]['invert'] = array(
      '#type' => 'checkbox',
      '#title' => t('Revoke access to selected elements'),
      '#description' => t('Uncheck this to revoke access to all <em>unselected</em> elements'),
      '#default_value' => $config['invert'],
    );
    if (isset($config_overrides['invert'])) {
      $form[$safe_form_id]['invert']['#disabled'] = TRUE;
      $form[$safe_form_id]['invert']['#attributes']['title'] = t('Overridden in code.');
    }

    $form[$safe_form_id]['elements'] = array(
      '#theme' => 'fea_admin_form_table',
    );

    if (!empty($config)) {
      ksort($config['elements']);

      foreach ($config['elements'] as $element_id => $info) {
        $form[$safe_form_id]['elements'][$element_id]['tree'] = array(
          '#type' => 'value',
          '#value' => $info['tree'],
        );
        $form[$safe_form_id]['elements'][$element_id]['type'] = array(
          '#type' => 'value',
          '#value' => $info['type'],
        );

        foreach ($roles as $role) {
          $form[$safe_form_id]['elements'][$element_id]['roles'][$role] = array(
            '#title' => $role,
            '#type' => 'checkbox',
            '#return_value' => $role,
            '#default_value' => isset($info['roles'][$role]) && $info['roles'][$role],
          );

          if (isset($config_overrides['elements'][$element_id])) {
            $form[$safe_form_id]['elements'][$element_id]['roles'][$role]['#disabled'] = TRUE;
            $form[$safe_form_id]['elements'][$element_id]['roles'][$role]['#attributes']['title'] = t('Overridden in code.');
          }
        }
      }
    }
    else {
      $form[$safe_form_id]['#description'] = t('No form elements indexed.');
    }
  }

  // Check if all forms are indexed.
  foreach (array_keys($form_ids) as $safe_form_id) {
    if (empty($form[$safe_form_id]['elements'])) {
      drupal_set_message(t('Please visit the page with form id %form_id on it to be able to configure the elements here.', array(
        '%form_id' => _fea_strip_safe_form_id($safe_form_id),
      )), 'warning', FALSE);
    }
  }

  // No form ids configured?
  if (empty($form_ids)) {
    drupal_set_message(t('No configured form ids. Please add them on the !link page.', array(
      '!link' => l(t('settings'), 'admin/config/user-interface/fea/settings'),
    )), 'error', FALSE);
  }

  return system_settings_form($form);
}

/**
 * Theme function to arrange the FEA access permissions form in tables.
 */
function theme_fea_admin_form_table($vars) {
  $roles = user_roles();

  $header = array(
    t('Form element'),
    t('Element type'),
  );
  foreach ($roles as $rid => $role) {
    $header[] = check_plain($role);
  }

  $rows = array();
  foreach (element_children($vars['form']) as $child) {
    $element = $vars['form'][$child];

    $row = array(
      check_plain(implode($element['tree']['#value'], ' > ')),
      check_plain($element['type']['#value']),
    );
    foreach (element_children($element['roles']) as $roles_child) {
      $element['roles'][$roles_child]['#title_display'] = 'invisible';
      $row[] = render($element['roles'][$roles_child]);
    }

    $rows[] = $row;
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}
