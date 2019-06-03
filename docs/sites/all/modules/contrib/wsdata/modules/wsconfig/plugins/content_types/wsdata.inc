<?php

$plugin = array(
  'title' => t('WSData'),
  'description' => t('A piece of webservice data'),
  'category' => t('WSData'),
  'required context' => new ctools_context_required(t('WSConfig'), 'wsconfig'),
);

function wsconfig_wsdata_content_type_edit_form($form, &$form_state) {
  $conf = &$form_state['conf'];
  $form['selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Data to select'),
    '#description' => t('Path to data'),
    '#default_value' => isset($conf['selector']) ? $conf['selector'] : '',
  );

  $form['classes'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS Classes'),
    '#description' => t('CSS Classes to add to the container'),
    '#default_value' => isset($conf['classes']) ? $conf['classes'] : '',
  );

  return $form;
}

function wsconfig_wsdata_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['selector'] = $form_state['values']['selector'];
  $form_state['conf']['classes'] = $form_state['values']['classes'];
}

function wsconfig_wsdata_content_type_admin_title($subtype, $conf) {
  return t('WSData returned from %selector', array('%selector' => $conf['selector']));
}

//function wsconfig_wsdata_content_type_admin_info($subtype, $conf) {}

function wsconfig_wsdata_content_type_render($subtype, $conf, $panel_args, $context) {
  $data = ctools_context_convert_context($context, $conf['selector']);

  if (!$data) {
    return NULL;
  }
  $page = array();
  if (is_array($data)) {
    foreach ($data as $d) {
      $page[] = array('#markup' => $d);
    }

  } else {
    $page = array('#markup' => $data);
  }

  $block = new stdClass();
 
  $block->content = array(
    '#type' => 'container',
    'items' => $page,
    '#attributes' => array(
      'class' => explode(' ', $conf['classes']),
    ),
  );
  return $block;
}
