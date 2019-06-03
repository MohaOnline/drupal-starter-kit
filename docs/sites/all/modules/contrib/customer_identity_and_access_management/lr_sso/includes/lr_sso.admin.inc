<?php
/**
 * @file
 * Admin page callback file for the advance_social_sharing module.
 */
module_load_include('inc', 'lr_ciam', 'includes/lr_ciam.admin');
/**
 * Function that build an abstract representation of a HTML form for Social Sharing settings.
 *
 * @global string $base_url URL to home page.
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form.
 * @return array A renderable form array for the respective request.
 */
function lr_sso_settings_form($form, &$form_state) {
  $sso_group = 'sso';
  $form[$sso_group] = array(
    '#type' => 'fieldset',
    '#title' => t('Single Sign On Settings'),
    '#group' => 'settings',
  );
  $form[$sso_group]['lr_sso_enable'] = array(
    '#type' => 'radios',
    '#title' => t('Do you want to enable Single sign on (SSO)?'),
    '#default_value' => variable_get('lr_sso_enable', 0),
    '#options' => array(
      1 => t('Yes'),
      0 => t('No'),
    ),
  );
 
  $form['#submit'][] = 'lr_sso_settings_form_submit';
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Submit sso settings.
 *
 * @param array $form Nested array of form elements that comprise the form.
 * @param array $form_state A keyed array containing the current state of the form
 * @return bool
 */
function lr_sso_settings_form_submit($form, &$form_state) {
  if (!lr_ciam_save_settings($form_state)) {
    return FALSE;
  }
}
