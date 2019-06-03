<?php

/**
 * Page callback for popup admin config.
 */
function dvg_popup_admin_form($form, $form_state) {
  $form['dvg_popup_config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Popup'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );

  $popup_config = _dvg_popup_config();

  $form['dvg_popup_config']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => $popup_config['enabled'],
    '#id' => 'dvg-popup-enabled',
  );

  $states = array(
    'visible' => array(
      '#dvg-popup-enabled' => array(
        'checked' => TRUE,
      ),
    ),
  );

  $form['dvg_popup_config']['cookie_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cookie name'),
    '#default_value' => $popup_config['cookie_name'],
    '#required' => $popup_config['enabled'],
    '#element_validate' => array('dvg_popup_admin_form_cookie_name_validate'),
    '#states' => $states,
  );

  $form['dvg_popup_config']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $popup_config['title'],
    '#required' => $popup_config['enabled'],
    '#states' => $states,
  );

  $form['dvg_popup_config']['body'] = array(
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#format' => $popup_config['body']['format'],
    '#rows' => 3,
    '#default_value' => $popup_config['body']['value'],
    '#required' => $popup_config['enabled'],
    '#states' => $states,
  );

  $form['dvg_popup_config']['button'] = array(
    '#type' => 'textfield',
    '#title' => t('Button text'),
    '#default_value' => $popup_config['button'],
    '#required' => $popup_config['enabled'],
    '#states' => $states,
  );

  $form['#process'][] = 'dvg_popup_admin_form_process';

  return system_settings_form($form);
}

/**
 * Process callback for popup admin config.
 */
function dvg_popup_admin_form_process($form, $form_state) {
  $is_enabled = (!empty($form_state['input']['dvg_popup_config']['enabled']));

  $form['dvg_popup_config']['cookie_name']['#required'] =
    $form['dvg_popup_config']['title']['#required'] =
    $form['dvg_popup_config']['body']['#required'] =
    $form['dvg_popup_config']['button']['#required'] = $is_enabled;

  return $form;
}

/**
 * Validation callback for popup admin config cookie name field.
 */
function dvg_popup_admin_form_cookie_name_validate($element, &$form_state, $form) {
  if (!empty($element['#value']) && preg_match('/[^a-z0-9_-]+/i', $element['#value'])) {
    form_error($element, t('%field contains invalid characters. Use only letters, numbers, hyphens and underscores', array('%field' => t('Cookie name'))));
  }
}
