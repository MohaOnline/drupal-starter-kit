<?php

/**
 * DigiD config form.
 */
function dvg_digid_admin_form() {
  $select_auth_sources = array('' => t('Select an authentication source'));
  if (($library = libraries_load('simplesamlphp')) && !empty($library['loaded'])) {
    $auth_sources_saml = SimpleSAML_Auth_Source::getSources();
    if (count($auth_sources_saml) > 0) {
      $auth_sources = drupal_map_assoc($auth_sources_saml);
    }
    else {
      drupal_set_message(t('SAML configuration not correctly configured. Please setup DigiD service in SAML configuration.'), 'error');
      $auth_sources = array(DVG_DIGID_DUMMY_SERVICE => t('Dummy'));
    }
  }
  else {
    drupal_set_message(t('SAML configuration not correctly configured. Please setup DigiD service in SAML configuration.'), 'error');
    $auth_sources = array(DVG_DIGID_DUMMY_SERVICE => t('Dummy'));
  }
  $select_auth_sources += $auth_sources;
  $auth_source_required = (count($auth_sources) > 0);
  $form = array();

  $form['dvg_digid_auth_source'] = array(
    '#title' => t('Authentication source'),
    '#type' => 'select',
    '#options' => $select_auth_sources,
    '#default_value' => variable_get('dvg_digid_auth_source'),
    '#required' => $auth_source_required,
  );
  $roles = array('' => t('None')) + drupal_map_assoc(user_roles(TRUE));
  $form['dvg_digid_role'] = array(
    '#title' => t('Assign this role to DigiD authenticated users'),
    '#type' => 'select',
    '#options' => $roles,
    '#default_value' => variable_get('dvg_digid_role'),
  );
  $form['dvg_digid_confirm'] = array(
    '#title' => t('Use a confirmation page before redirecting the user to the DigiD identity provider'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('dvg_digid_confirm'),
  );
  $form['dvg_digid_logo'] = array(
    '#title' => t('DigiD logo'),
    '#type' => 'managed_file',
    '#default_value' => variable_get('dvg_digid_logo'),
    '#description' => t("Unfortunately, we can't distribute the official DigiD logo with DVG because of licence limitations.") . '<br />' . l(t('More information and downloadable logo.'), 'http://www.logius.nl/producten/toegang/digid/'),
    '#required' => TRUE,
    '#upload_location' => 'public://',
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'dvg_digid_admin_form_submit';
  return $form;
}

/**
 * Custom submit function for the system settings form.
 */
function dvg_digid_admin_form_submit($form, &$form_state) {
  $file = file_load($form_state['values']['dvg_digid_logo']);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  file_usage_add($file, 'dvg_digid', 'dvg_digid', 0);
}
