<?php

/**
 * Page callback for testdata configuration.
 */
function dvg_global_testdata($form, &$form_state) {

  $items = dvg_global_testdata_items();

  foreach ($items as $key => $value) {
    $form[$key] = $value;
  }
  // Clear cache after saving this form.
  $form['#submit'][] = '_dvg_global_cache_clear_all';

  return system_settings_form($form);
}

/**
 * Helper for testdata items.
 */
function dvg_global_testdata_items() {
  $items = array();
  $modules = module_implements('dvg_global_testdata');
  foreach ($modules as $implementing_module) {
    $testdata_module = module_invoke($implementing_module, 'dvg_global_testdata');
    foreach ($testdata_module as $testdata_key => $testdata_value) {
      $items[$testdata_key] = $testdata_value;
    }
  }
  return $items;
}

/**
 * Page callback for texts configuration.
 */
function dvg_global_texts($form, &$form_state) {

  // Footer texts.
  $form['dvg_custom__footer_texts'] = array(
    '#type' => 'fieldset',
    '#title' => t('Footer'),
    '#weight' => -40,
    '#tree' => TRUE,
  );

  $num_footer_texts = variable_get('dvg_custom__num_footer_texts', 3);
  $default_values = variable_get('dvg_custom__footer_texts', array());
  for ($i = 1; $i <= $num_footer_texts; $i++) {
    $field_name = 'dvg_custom__footer_text_' . $i;
    $field_value = !empty($default_values[$field_name]['value']) ? $default_values[$field_name] : array('value' => '', 'format' => 'filtered_html');

    $form['dvg_custom__footer_texts'][$field_name] = array(
      '#size' => '80',
      '#weight' => $i,
      '#type' => 'text_format',
      '#format' => $field_value['format'],
      '#title' => t('Footer line @num', array('@num' => $i)),
      '#rows' => 3,
      '#default_value' => $field_value['value'],
    );
  }

  // 503 page texts.
  $form['503'] = array(
    '#type' => 'fieldset',
    '#title' => t('Temporary Unavailable page'),
    '#description' => t('Use this to customize the !link.', array(
      '!link' => l(t('503 Service Temporary Unavailable page'), '503-page'),
    )),
  );
  $form['503']['dvg_global_503_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#default_value' => variable_get('dvg_global_503_page_title', ''),
  );

  $body = variable_get('dvg_global_503_page_body', array(
    'value' => '',
    'format' => 'filtered_html',
  ));
  $form['503']['dvg_global_503_page_body'] = array(
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#required' => TRUE,
    '#default_value' => $body['value'],
    '#format' => $body['format'],
  );

  $themes = array();
  foreach (list_themes() as $name => $info) {
    if ($info->status) {
      $themes[$name] = $info->info['name'];
    }
  }
  $form['503']['dvg_global_plain_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => $themes,
    '#required' => TRUE,
    '#default_value' => variable_get('dvg_global_plain_theme', 'dvg_plain_theme'),
    '#format' => $body['format'],
  );

  // Clear cache after saving this form.
  $form['#submit'][] = '_dvg_global_cache_clear_all';

  return system_settings_form($form);
}

/**
 * Validation helper to validate number is a valid Basic Page node.
 */
function _dvg_global_validate_basicpage_nid($element, &$form_state) {
  $value = $element['#value'];

  if ($node = node_load($value)) {
    if ($node->type != 'page') {
      form_error($element, t('%name must be a valid Basic Page node ID.', array('%name' => $element['#title'])));
    }
  }
}
