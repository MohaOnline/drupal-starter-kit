<?php

/**
 * Implements hook_form().
 *
 * Provides the form for a user to change the option for DLM on email subject
 * lines.
 */
function govcms_dlm_form($form, &$form_state) {
  $form['govcms_dlm_suffix'] = array(
    '#type' => 'select',
    '#title' => t('Select DLM to append to all outgoing e-mails'),
    '#default_value' => variable_get('govcms_dlm_suffix', GOVCMS_DLN_DEFAULT),
    '#options' => array(
      '[SEC=UNCLASSIFIED]' => '[SEC=UNCLASSIFIED]',
      '[DLM:For-Official-Use-Only]' => '[DLM:For-Official-Use-Only]',
      '[DLM:Sensitive:Legal]' => '[DLM:Sensitive:Legal]',
      '[DLM:Sensitive:Personal]' => '[DLM:Sensitive:Personal]',
      '[DLM:Sensitive]' => '[DLM:Sensitive]',
      '[SEC=PROTECTED]' => '[SEC=PROTECTED]',
      '[SEC=PROTECTED,DLM:For-Official-Use-Only]' => '[SEC=PROTECTED,DLM:For-Official-Use-Only]',
      '[SEC=PROTECTED,DLM:Sensitive:Legal]' => '[SEC=PROTECTED,DLM:Sensitive:Legal]',
      '[SEC=PROTECTED,DLM:Sensitive:Personal]' => '[SEC=PROTECTED,DLM:Sensitive:Personal]',
      '[SEC=PROTECTED,DLM:Sensitive]' => '[SEC=PROTECTED,DLM:Sensitive]',
    ),
    '#description' => t("Note: Just because you set this doesn't ensure the email is sent securely, you must also ensure your email gateway is configured appropriately."),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}
