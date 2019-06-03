<?php
function dvg_appointments_admin_form($form, $form_state) {
  $api_clients = array();
  $all_api_clients = module_invoke_all('dvg_appointments_api_client');
  foreach ($all_api_clients as $id => $info) {
    $api_clients[$id] = $info['name'];
  }
  //$current_api_client = (isset($form_state['values']['current_client_api'])) ? $form_state['values']['current_client_api'] : variable_get('dvg_appointments_api_client', FALSE);
  $current_api_client = variable_get('dvg_appointments_api_client', FALSE);

  $form['dvg_appointments_api_client'] = array(
    '#title' => t('Client API'),
    '#type' => 'select',
    '#required' => TRUE,
    '#default_value' => $current_api_client,
    '#options' => $api_clients,
  );
  if ($current_api_client) {
    $class = $all_api_clients[$current_api_client]['class'];

    $form['client_api_options'] = array(
      '#title' => t('@name options', array('@name' => $api_clients[$current_api_client])),
      '#type' => 'fieldset',
    );

    $form['client_api_options'][] = $class::config_form();
  }

  $form['flood_control'] = array(
    '#title' => t('Flood control'),
    '#type' => 'fieldset',
  );
  $form['flood_control']['dvg_appointments_flood_threshold'] = array(
    '#title' => t('Threshold'),
    '#description' => t('Leave empty to disable flood control.'),
    '#type' => 'textfield',
    '#size' => 4,
    '#element_validate' => array('element_validate_integer_positive'),
    '#field_suffix' => t('attempts'),
    '#default_value' => variable_get('dvg_appointments_flood_threshold', ''),
  );
  $form['flood_control']['dvg_appointments_flood_timeout'] = array(
    '#title' => t('Timeout'),
    '#type' => 'textfield',
    '#size' => 4,
    '#required' => TRUE,
    '#element_validate' => array('element_validate_integer_positive'),
    '#field_suffix' => t('minutes'),
    '#default_value' => variable_get('dvg_appointments_flood_timeout', 15),
  );

  return system_settings_form($form);
}
