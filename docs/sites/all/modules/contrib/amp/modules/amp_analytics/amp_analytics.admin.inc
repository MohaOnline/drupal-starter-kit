<?php

/**
 * @file
 * Administrative page callbacks for the AMP analytics module.
 */

/**
 * Form constructor for the AMP analytics form.
 */
function amp_analytics_admin_form($form, &$form_state) {
  $form['amp_analytics'] = array(
    '#type' => 'fieldset',
    '#title' => t('AMP Analytics'),
    '#group' => 'additional_settings',
    '#weight' => 100,
  );

  $form['amp_analytics']['amp_google_analytics_id'] = array(
    '#title' => t('Google Analytics Web Property ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('amp_google_analytics_id'),
    '#size' => 15,
    '#maxlength' => 20,
    '#description' => t('This ID is unique to each site you want to track separately, and is in the form of UA-xxxxxxx-yy. To get a Web Property ID, <a href="@analytics">register your site with Google Analytics</a>, or if you already have registered your site, go to your Google Analytics Settings page to see the ID next to every site profile. <a href="@webpropertyid">Find more information in the documentation</a>.', array('@analytics' => 'http://www.google.com/analytics/', '@webpropertyid' => url('https://developers.google.com/analytics/resources/concepts/gaConceptsAccounts', array('fragment' => 'webProperty')))),
  );

  return system_settings_form($form);
}

/**
 * Form validation handler for amp_admin_form().
 *
 * @see amp_admin_form_submit()
 */
function amp_analytics_admin_form_validate($form, &$form_state) {
  // Validate the Google Analytics ID.
  if (!empty($form_state['values']['amp_google_analytics_id'])) {
    if (!preg_match('/^UA-\d+-\d+$/', $form_state['values']['amp_google_analytics_id'])) {
      form_set_error('amp_google_analytics_id', t('A valid Google Analytics Web Property ID is case sensitive and formatted like UA-xxxxxxx-yy.'));
    }
  }
}
