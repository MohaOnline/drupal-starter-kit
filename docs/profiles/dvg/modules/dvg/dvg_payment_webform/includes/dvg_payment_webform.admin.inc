<?php

function dvg_payment_webform_config($form, &$form_state) {

  $form['dvg_payment_webform'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default payment method'),
  );

  $payment_methods = payment_method_options();
  $form['dvg_payment_webform']['dvg_payment_webform_method'] = array(
    '#type' => 'select',
    '#options' => $payment_methods,
    '#title' => t('Default payment method'),
    '#description' => t(
      'This payment method will be preselected on webforms using payments, ' .
      'unless a different payment method is explicitly chosen for that webform, ' .
      'this method will be used.'
    ),
    '#default_value' => variable_get('dvg_payment_webform_method'),
  );

  return system_settings_form($form);
}
