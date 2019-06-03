<?php
/**
 * @file
 * hosting_dns hosting_dns.form.inc
 */

/**
 * Configuration form for the global DNS settings.
 */
function hosting_dns_settings($form, &$form_state) {
  $form['hosting_dns_default_ttl'] = array(
    '#type' => 'select',
    '#title' => t('Default TTL'),
    '#description' => t('The default time to live for newly created DNS records.'),
    '#options' => _hosting_dns_ttl_options(),
    '#default_value' => variable_get('hosting_dns_default_ttl', 86400),
    '#required' => TRUE,
  );

  $form['hosting_dns_default_dns_records'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default DNS records for new domains'),
    '#id' => 'dns-records-wrapper',
  );

  $default_values = isset($form_state['input']['hosting_dns_default_dns_records'])
    ? $form_state['input']['hosting_dns_default_dns_records']
    : variable_get('hosting_dns_default_dns_records', array());

  _hosting_dns_dns_records_form($form, $form_state, $default_values);

  return system_settings_form($form);
}

/**
 * Per site settings form for the DNS settings.
 *
 * @see hosting_dns_manage_records_submit()
 */
function hosting_dns_manage_records($form, &$form_state, $site, $domain = NULL) {
  $form_state['#site'] = $site;
  $form_state['#domain'] = !empty($domain) ? $domain : $site->title;

  $form['hosting_dns_default_dns_records'] = array(
    '#type' => 'fieldset',
    '#title' => t('DNS records'),
    '#id' => 'dns-records-wrapper',
  );

  $existing_dns_records = !empty($site->dns_records[$form_state['#domain']]) ? $site->dns_records[$form_state['#domain']] : array();

  $default_values = isset($form_state['input']['hosting_dns_default_dns_records'])
    ? $form_state['input']['hosting_dns_default_dns_records']
    : !empty($site->dns_records[$form_state['#domain']])
      ? hosting_dns_expand_records($existing_dns_records)
      : _hosting_dns_obtain_default_records($site, $form_state['#domain']);

  _hosting_dns_dns_records_form($form, $form_state, $default_values);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['deploy'] = array(
    '#type' => 'submit',
    '#value' => t('Save and deploy DNS records'),
    // Append to below submit function.
    '#submit' => array('hosting_dns_manage_records_submit', 'hosting_dns_manage_records_submit_deploy'),
  );

  return $form;
}

/**
 * Submit handler for the per site settings form.
 *
 * @see hosting_dns_manage_records()
 */
function hosting_dns_manage_records_submit($form, &$form_state) {
  $site = $form_state['#site'];

  $records = hosting_dns_collapse_records($form_state['values']['hosting_dns_default_dns_records']);

  $site->dns_records[$form_state['#domain']] = $records;
  hosting_dns_save($site);
}

/**
 * Helper function for collpasing DNS records.
 *
 * @param array $input_records
 *
 * @return array
 */
function hosting_dns_collapse_records(array $input_records) {
  $records = array();

  foreach ($input_records as $record) {
    // Skip the 'Add record' link.
    if (!is_array($record)) {
      continue;
    }

    // Prepare collection array.
    if (!array_key_exists($record['title'], $records)) {
      $records[$record['title']] = array();
    }

    // Restructure the array, omitting the 'Remove' buttons.
    $records[$record['title']][] = array(
      'dns_type' => $record['dns_type'],
      'dns_ttl' => $record['dns_ttl'],
      'dns_value' => $record['dns_value'],
    );
  }

  // Sort entries by title.
  ksort($records);
  return $records;
}

/**
 * Helper function for collpasing DNS records.
 *
 * @param array $records
 *
 * @return array
 */
function hosting_dns_expand_records(array $input_records) {
  $return = array();

  foreach ($input_records as $title => $records) {
    foreach ($records as $record) {
      $return[] = array(
        'title' => $title,
        'dns_type' => $record['dns_type'],
        'dns_ttl' => $record['dns_ttl'],
        'dns_value' => $record['dns_value'],
      );
    }
  }

  return $return;
}

/**
 * Submit and deploy handler for the per site settings form.
 *
 * @see hosting_dns_manage_records()
 * @see hosting_dns_manage_records_submit()
 */
function hosting_dns_manage_records_submit_deploy($form, &$form_state) {
  $site = $form_state['#site'];
  hosting_add_task($site->nid, 'dns-deploy');
}

/**
 * DNS records form constructor.
 *
 * This function generates the DNS records fieldset on forms to prevent code
 * duplication.
 *
 * @param $form
 * @param $form_state
 * @param array $default_values
 *   An array of default values to populate the form with.
 */
function _hosting_dns_dns_records_form(&$form, &$form_state, $default_values = array()) {
  $form['#tree'] = TRUE;
  $form['#validate'][] = 'hosting_dns_form_validate';
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'hosting_dns') . '/hosting_dns.css',
  );

  // Build the fieldset with the proper number of names. We'll use
  // $form_state['num_names'] to determine the number of textfields to build.
  if (empty($form_state['num_names'])) {
    $form_state['num_names'] = sizeof($default_values) > 0 ? sizeof($default_values) : 0;
  }

  for ($i = 0; $i < $form_state['num_names']; $i++) {
    $form['hosting_dns_default_dns_records'][$i] = array(
      '#type' => 'container',
      '#attributes' => array('class' => array('container-inline')),
    );

    $form['hosting_dns_default_dns_records'][$i]['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#title_display' => 'invisible',
      '#default_value' => isset($default_values[$i]['title']) ? $default_values[$i]['title'] : '',
      '#required' => TRUE,
      '#attributes' => array(
        'class' => array('dns-form-element-big'),
        'placeholder' => t('Name'),
      ),
    );

    $form['hosting_dns_default_dns_records'][$i]['dns_ttl'] = array(
      '#type' => 'select',
      '#title' => t('TTL'),
      '#title_display' => 'invisible',
      '#options' => _hosting_dns_ttl_options(),
      '#default_value' => isset($default_values[$i]['dns_ttl']) ? $default_values[$i]['dns_ttl'] : variable_get('hosting_dns_default_ttl', 86400),
      '#required' => TRUE,
      '#attributes' => array(
        'class' => array('dns-form-element-small'),
      ),
    );

    $form['hosting_dns_default_dns_records'][$i]['dns_type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#title_display' => 'invisible',
      '#options' => _hosting_dns_type_options(),
      '#default_value' => isset($default_values[$i]['dns_type']) ? $default_values[$i]['dns_type'] : '',
      '#required' => TRUE,
      '#attributes' => array(
        'class' => array('dns-form-element-small'),
      ),
    );

    $form['hosting_dns_default_dns_records'][$i]['dns_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value'),
      '#title_display' => 'invisible',
      '#default_value' => isset($default_values[$i]['dns_value']) ? $default_values[$i]['dns_value'] : '',
      '#required' => TRUE,
      '#attributes' => array(
        'class' => array('dns-form-element-big'),
        'placeholder' => t('Value'),
      ),
    );

    $form['hosting_dns_default_dns_records'][$i]['remove_record'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('hosting_dns_form_remove_record'),
      '#limit_validation_errors' => array(),
      '#ajax' => array(
        'callback' => 'hosting_dns_form_ajax_callback',
        'wrapper' => 'dns-records-wrapper',
      ),
      '#button_index' => $i,
      '#name' => 'remove-button-'.$i,
    );
  }

  $form['hosting_dns_default_dns_records']['add_record'] = array(
    '#type' => 'submit',
    '#value' => t('Add record'),
    '#submit' => array('hosting_dns_form_add_record'),
    '#limit_validation_errors' => array(),
    '#ajax' => array(
      'callback' => 'hosting_dns_form_ajax_callback',
      'wrapper' => 'dns-records-wrapper',
    ),
  );
}

/**
 * Callback for both ajax-enabled buttons.
 *
 * Selects and returns the fieldset with the names in it.
 */
function hosting_dns_form_ajax_callback($form, $form_state) {
  return $form['hosting_dns_default_dns_records'];
}

/**
 * Submit handler for the "add-one-more" button.
 *
 * Increments the max counter and causes a rebuild.
 */
function hosting_dns_form_add_record($form, &$form_state) {
  $form_state['num_names'] ++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove one" button.
 *
 * Decrements the max counter and causes a form rebuild.
 */
function hosting_dns_form_remove_record($form, &$form_state) {
  if ($form_state['num_names'] > 0) {
    $form_state['num_names']--;
  }

  // Get the delta of the value to delete (this was stored on the button).
  $index = $form_state['clicked_button']['#button_index'];

  // Drop the row from the input values and reorder the array.
  unset($form_state['input']['hosting_dns_default_dns_records'][$index]);
  $form_state['input']['hosting_dns_default_dns_records'] = array_values($form_state['input']['hosting_dns_default_dns_records']);

  $form_state['rebuild'] = TRUE;
}

/**
 * Form validation for the hosting_dns_settings form.
 *
 * @see hosting_dns_settings()
 */
function hosting_dns_form_validate($form, $form_state) {

  // Throw an error if the domain is not known to the site.
  if (!empty($form_state['#domain']) && $form_state['#domain'] != $form_state['#site']->title && !in_array($form_state['#domain'], $form_state['#site']->aliases)) {
    form_set_error('', t('The entered domain appears not to be associated with this site.'));
  }

  // Loop over all individual records.
  foreach($form_state['values']['hosting_dns_default_dns_records'] as $index => &$row) {

    // Remove the non-numeric items (buttons) in the array.
    if (!is_numeric($index)) {
      unset($form_state['values']['hosting_dns_default_dns_records'][$index]);
      continue;
    }

    // Force the title to be lowercase.
    $row['title'] = strtolower(trim($row['title']));

    // TODO: Find out the exact do's and don'ts.
    // Check for non-allowed characters.
    if (preg_match("/^[a-z0-9-._*@]*$/", $row['title']) === 0) {
      form_set_error('hosting_dns_default_dns_records]['.$index.'][title', t('This is not a valid DNS name, it can only contain letters, numbers, at signs (@) asterisks (*), dashes (-), dots (.) and underscores (_).'));
    }

    // Check if a valid IPv4 address is specified for the A record.
    if ($row['dns_type'] == 'A' && filter_var($row['dns_value'], FILTER_VALIDATE_IP, FILTER_FLAG_IPV4) === false) {
      form_set_error('hosting_dns_default_dns_records]['.$index.'][dns_value', t('This is not a valid IPv4 address, and cannot be used as an A record.'));
    }

    // Check if a valid IPv6 address is specified for the AAAA record.
    if ($row['dns_type'] == 'AAAA' && filter_var($row['dns_value'], FILTER_VALIDATE_IP, FILTER_FLAG_IPV6) === false) {
      form_set_error('hosting_dns_default_dns_records]['.$index.'][dns_value', t('This is not a valid IPv6 address, and cannot be used as an A record.'));
    }
  }
}
