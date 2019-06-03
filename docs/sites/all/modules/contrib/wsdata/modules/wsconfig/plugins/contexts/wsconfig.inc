<?php

$plugin = array(
  'title' => t('WsConfig'),
  'description' => t('A context from WsConfig.'),
  'context' => 'wsconfig_context_create_wsconfig',
  'edit form' => 'wsconfig_context_wsconfig_settings_form',
  'keyword' => 'wsconfig',
  'no ui' => FALSE,
  'context name' => 'wsconfig',
  'convert list' => array(
    'wsconfig' => t('WSConfig'),
    'wsprocessor' => t('WSProcessor'),
  ),
  'convert' => 'wsconfig_context_wsconfig_convert',
);

function wsconfig_context_create_wsconfig($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('wsconfig');
  $context->plugin = 'wsconfig';
  $context->data['conf'] = $data;
  $context->data['wsdata_run'] = FALSE;
  $context->empty = $empty;
  return $context;
}

function wsconfig_context_wsconfig_settings_form($form, &$form_state) {
  $conf = &$form_state['conf'];

  $form['wsconfig'] = array(
    '#type' => 'select',
    '#title' => t('Web Service Config'),
    '#description' => t('The web service config to use to retrieve the data'),
    '#options' => wsconfig_get_list_by_name(),
    '#default_value' => $conf['wsconfig'],
  );

  foreach (wsconfig_get_list_by_name() as $machinename => $name) {
    $wsconfig = wsconfig_load_by_name($machinename);
    $items = array();
    foreach ($wsconfig->getOperations() as $opname) {
      $items[$opname] = $wsconfig->getMethodName($opname);
    }

    $form['wsconfig_method_' . $machinename] = array(
      '#type' => 'select',
      '#title' => t('Web Service Config Method'),
      '#description' => t('The web service method to use'),
      '#options' => $items,
      '#default_value' => $conf['wsconfig_method_' . $machinename],
      '#states' => array(
        'visible' => array(
          ':input[name="wsconfig"]' => array('value' => $machinename),
        ),
      ),
    );
  }

  $form['replacements'] = array(
      '#type' => 'textfield',
      '#title' => t('Replacements'),
      '#description' => t('Replacements'),
      '#default_value' => $conf['replacements'],
  );
     
  $form['arguments'] = array(
      '#type' => 'textfield',
      '#title' => t('Arguments'),
      '#description' => t('JSON encoded arguments to pass to the wsconfig'),
      '#default_value' => $conf['arguments'],
  );

  $form['options'] = array(
    '#type' => 'textfield',
    '#title' => t('Options'),
    '#description' => t('JSON encoded options'),
    '#default_value' => $conf['options'],
  );

  $form['wsprocessor'] = array(
    '#type' => 'select',
    '#title' => t('Web Service Processor'),
    '#description' => t('The web service processor to parse the data'),
    '#options' => wsconfig_get_data_processors(),
    '#default_value' => $conf['wsprocessor'],
  );

  return $form;
}

function wsconfig_context_wsconfig_settings_form_submit(&$form, &$form_state) {
  $form_state['conf']['wsconfig'] = $form_state['values']['wsconfig'];
  $form_state['conf']['replacements'] = $form_state['values']['replacements'];
  $form_state['conf']['options'] = $form_state['values']['options'];
  $form_state['conf']['arguments'] = $form_state['values']['arguments'];
  $form_state['conf']['wsprocessor'] = $form_state['values']['wsprocessor'];

  foreach ($form_state['values'] as $key => $val) {
    if (strncmp($key, 'wsconfig_method_', strlen('wsconfig_method_')) == 0) {
      $form_state['conf'][$key] = $val;
    }
  }
}

function wsconfig_context_wsconfig_convert($context, $type) {
  if ($context->empty) {
    return NULL;
  }

  if (!$context->data['wsdata_run']) {
    $context->data['wsconfig'] = wsconfig_load_by_name($context->data['conf']['wsconfig']);

    $replacements = json_decode($context->data['conf']['replacements'], TRUE);
    foreach ($replacements as $key => $val) {
      if (substr($key,0,1) != '%') {
        $replacements['%' . $key] = $val;
        unset($replacements[$key]);
      }
    }

    $method = $context->data['conf']['wsconfig_method_' . $context->data['conf']['wsconfig']];
    $arguments = empty($context->data['conf']['arguments']) ? array() : json_decode($context->data['conf']['arguments'], TRUE);
    $options = empty($context->data['conf']['options']) ? array() : json_decode($context->data['conf']['options'], TRUE);

    $result = $context->data['wsconfig']->call(
      $method,
      $replacements,
      $arguments,
      $options
    );

    $context->data['wsprocessor'] = new $context->data['conf']['wsprocessor']($result);
    $context->data['wsdata_run'] = TRUE;
  }

  switch($type) {
    case 'wsprocessor':
      return $context->data['wsprocessor'];
      break;
    case 'wsconfig':
      return $context->data['wsconfig'];
      break;
    default:
      return $context->data['wsprocessor']->getData($type);
  }
  return NULL;
}

