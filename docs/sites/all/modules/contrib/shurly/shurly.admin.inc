<?php

/**
 * @file
 * Administration pages for the Htaccess module.
 */

/**
 * Settings form
 */
function shurly_settings_form($form, &$form_state) {
  global $base_url;

  $form['shurly_url'] = array(
    '#type' => 'fieldset',
    '#title' => t('Base URL'),
    '#description' => t('If you want to use a dedicated url for the short URL\'s, enter below that short base URL to be used.'),
  );

  $form['shurly_url']['shurly_base'] = array(
    '#type' => 'textfield',
    '#description' => t('Default is the base URL of the Drupal installation.'),
    '#default_value' => variable_get('shurly_base', $base_url),
    '#required' => TRUE,
  );

  $form['shurly_redirect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Redirect URL'),
    '#description' => t('Define the redirect page when the short link is deactivated.'),
  );

  $form['shurly_redirect']['shurly_redirect_page'] = array(
    '#type' => 'textfield',
    '#field_prefix' => $base_url . '/',
    '#description' => t('Page displayed when the link is deactivated. If not defined, the default 404 page will be used.'),
    '#default_value' => variable_get('shurly_redirect_page', FALSE),
    '#required' => TRUE,
  );

  $form['shurly_restrictions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Restrictions'),
    '#description' => t('Restrict short URL targets. Be aware of the fact, that the localhost, local network and unresolvable restriction are resolving the given address by staging a DNS request, which can significantly <strong>slow down the short URL creation</strong>!'),
  );

  $form['shurly_restrictions']['shurly_forbid_localhost'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forbid localhost'),
    '#description' => t('Do not allow creation of short URLs targeting localhost addresses.'),
    '#default_value' => variable_get('shurly_forbid_localhost', FALSE),
  );

  $form['shurly_restrictions']['shurly_forbid_private_ips'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forbid private IP ranges'),
    '#description' => t('Do not allow creation of short URLs targeting private IP ranges.'),
    '#default_value' => variable_get('shurly_forbid_private_ips', FALSE),
  );

  $form['shurly_restrictions']['shurly_forbid_unresolvable_hosts'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forbid unresolvable hostnames'),
    '#description' => t('Do not allow creation of short URLs targeting host addresses that cannot be resolved.'),
    '#default_value' => variable_get('shurly_forbid_unresolvable_hosts', FALSE),
  );

  $form['shurly_restrictions']['shurly_forbid_ips'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forbid direct IP redirects'),
    '#description' => t('Do not allow creation of short URLs containing an IP address instead of a human readable hostname.'),
    '#default_value' => variable_get('shurly_forbid_ips', FALSE),
  );

  $form['shurly_restrictions']['shurly_forbid_custom'] = array(
    '#type' => 'checkbox',
    '#title' => t('Forbid URL target by custom pattern'),
    '#description' => t('Define a custom pattern (RegEx) to forbid some kind of target URLs.'),
    '#default_value' => variable_get('shurly_forbid_custom', FALSE),
    '#attributes' => array(
      'onchange' => "jQuery('#shurly_custom_restriction_container').toggle();",
    ),
  );

  $form['shurly_restrictions']['shurly_custom_restriction'] = array(
    '#type' => 'textfield',
    '#title' => t('Custom pattern'),
    '#description' => t('PERL regular expression defining a forbidden URL pattern.'),
    '#default_value' => variable_get('shurly_custom_restriction', FALSE),
    '#prefix' => '<div id="shurly_custom_restriction_container"' . (variable_get('shurly_forbid_custom', FALSE) ? '>' : ' style="display: none;">'),
    '#suffix' => '</div>',
  );

  $form['shurly_restrictions']['shurly_gsb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Google Safe Browsing'),
    '#description' => t('Check if a long URL is blacklisted against Google Safe Browsing. This service
      requires a Google developer account and is limited to 10,000 queries per day.'),
    '#default_value' => variable_get('shurly_gsb', FALSE),
    '#attributes' => array(
      'onchange' => "jQuery('.shurly_gsb_container').toggle();",
    ),
  );

  $form['shurly_restrictions']['shurly_gsb_client'] = array(
    '#type' => 'textfield',
    '#title' => t('Client'),
    '#description' => t('You can choose any name. Google suggests that you choose a name that represents the true identiy
       of the client (ie: name of your company).'),
    '#default_value' => variable_get('shurly_gsb_client', FALSE),
    '#prefix' => '<div class="shurly_gsb_container"' . (variable_get('shurly_gsb_client', FALSE) ? '>' : ' style="display: none;">'),
    '#suffix' => '</div>',
  );

  $form['shurly_restrictions']['shurly_gsb_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#description' => t('Add your API key.'),
    '#default_value' => variable_get('shurly_gsb_apikey', FALSE),
    '#prefix' => '<div class="shurly_gsb_container"' . (variable_get('shurly_gsb_apikey', FALSE) ? '>' : ' style="display: none;">'),
    '#suffix' => '</div>',
  );

  $form['shurly_throttle'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rate limiting'),
    '#tree' => TRUE,
    '#description' => t('Limit requests by IP address. Leave blank for no rate limiting.<br /><strong>Note:</strong> Only roles with the \'Create short URLs\' permission are listed here. Change that permission <a href="!url">here</a>.', array('!url' => url('admin/people/permissions', array('fragment' => 'module-shurly')))),
  );
  $saved = variable_get('shurly_throttle', array());

  foreach (user_roles(FALSE, 'Create short URLs') as $rid => $name) {
    $rate = isset($saved[$rid]['rate']) ? $saved[$rid]['rate'] : NULL;
    $time = isset($saved[$rid]['time']) ? $saved[$rid]['time'] : NULL;

    $form['shurly_throttle'][$rid] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($name),
      '#tree' => TRUE,
    );
    $form['shurly_throttle'][$rid]['rate'] = array(
      '#type' => 'textfield',
      '#size' => '3',
      '#prefix' => '<div class="container-inline">',
      '#field_suffix' => ' ' . t('requests'),
      '#default_value' => $rate,
    );
    $form['shurly_throttle'][$rid]['time'] = array(
      '#type' => 'textfield',
      '#size' => '3',
      '#field_prefix' => t('within'),
      '#field_suffix' => ' ' . t('minutes'),
      '#default_value' => $time,
      '#suffix' => '</div>',
    );
    $form['shurly_throttle'][$rid]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#default_value' => isset($saved[$rid]['weight']) ? $saved[$rid]['weight'] : 0,
      '#description' => t('Order of this role when considering a user with multiple roles. A user\'s lightest role will take precedence.'),
    );

  }

  return system_settings_form($form);
}

function shurly_settings_form_validate($form, $form_state){
  $custom_base_url = $form_state['values']['shurly_base'];
  if(!valid_url($custom_base_url, TRUE)){
    form_set_error('settings_shurly_base', t('The base URL is not valid.'));
    }
}
