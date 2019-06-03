<?php
/**
 * @file
 * This file contains the Admin of the DvG StUF module.
 */

/**
 * Admin form for StUF-BG.
 */
function dvg_stuf_bg_admin_form($form, $form_state) {
  $form['stuf_bg'] = array(
    '#type' => 'fieldset',
    '#title' => t('StUF-BG'),
  );
  $form['stuf_bg']['dvg_stuf_bg_sender_organization'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender Organization'),
    '#default_value' => variable_get('dvg_stuf_bg_sender_organization', ''),
    '#required' => TRUE,
  );
  $form['stuf_bg']['dvg_stuf_bg_sender_application'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender Application'),
    '#default_value' => variable_get('dvg_stuf_bg_sender_application', ''),
    '#required' => TRUE,
  );
  $form['stuf_bg']['dvg_stuf_bg_receiver_organization'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Organization'),
    '#default_value' => variable_get('dvg_stuf_bg_receiver_organization', ''),
    '#required' => FALSE,
  );
  $form['stuf_bg']['dvg_stuf_bg_receiver_application'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Application'),
    '#default_value' => variable_get('dvg_stuf_bg_receiver_application', ''),
    '#required' => TRUE,
  );
  $form['stuf_bg']['dvg_stuf_bg_receiver_administration'] = array(
    '#type' => 'textfield',
    '#title' => t('Receiver Administration'),
    '#default_value' => variable_get('dvg_stuf_bg_receiver_administration', ''),
    '#required' => FALSE,
  );
  $form['stuf_bg_natuurlijkpersoon'] = array(
    '#type' => 'fieldset',
    '#title' => t('StUF-BG NatuurlijkPersoon'),
    '#collapsible' => TRUE,
    '#collapsed' => !(dvg_stuf_bg_natuurlijkpersoon_enabled()),
  );
  $form['stuf_bg_natuurlijkpersoon']['dvg_stuf_bg_natuurlijkpersoon_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable StUF-BG NatuurlijkPersoon'),
    '#default_value' => dvg_stuf_bg_natuurlijkpersoon_enabled(),
    '#id' => 'dvg-stuf-bg-natuurlijkpersoon-enabled',
  );
  $form['stuf_bg_natuurlijkpersoon']['dvg_stuf_bg_natuurlijkpersoon_api_method'] = array(
    '#type' => 'radios',
    '#title' => t('API method'),
    '#default_value' => dvg_stuf_bg_natuurlijkpersoon_api_method(),
    '#options' => array('prs' => 'prs', 'vraagAntwoord' => 'vraagAntwoord', 'prsVraagAntwoord' => 'prs & vraagAntwoord'),
    '#description' => t('API method used by the endpoint url.'),
    '#required' => isset($form_state['input']['dvg_stuf_bg_natuurlijkpersoon_enabled']) ? $form_state['input']['dvg_stuf_bg_natuurlijkpersoon_enabled'] : FALSE,
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-natuurlijkpersoon-enabled' => array(
          'checked' => TRUE,
        ),
      ),
    ),
    '#id' => 'dvg-stuf-bg-natuurlijkpersoon-api-method',
  );
  $natuurlijkpersoon_enabled = isset($form_state['input']['dvg_stuf_bg_natuurlijkpersoon_enabled']) ? $form_state['input']['dvg_stuf_bg_natuurlijkpersoon_enabled'] : FALSE;
  $form['stuf_bg_natuurlijkpersoon']['dvg_stuf_bg_natuurlijkpersoon_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API endpoint url'),
    '#default_value' => variable_get('dvg_stuf_bg_natuurlijkpersoon_api_url', ''),
    '#description' => t('Exclude the <code>?WSDL</code> part.'),
    '#required' => $natuurlijkpersoon_enabled,
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-natuurlijkpersoon-enabled' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );
  $vraagantwoord_url_required = ($natuurlijkpersoon_enabled && isset($form_state['input']['dvg_stuf_bg_natuurlijkpersoon_api_method'])) ? $form_state['input']['dvg_stuf_bg_natuurlijkpersoon_api_method'] === 'prsVraagAntwoord' : FALSE;
  $form['stuf_bg_natuurlijkpersoon']['dvg_stuf_bg_natuurlijkpersoon_api_vraagantwoord_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API vraagAntwoord endpoint url'),
    '#default_value' => variable_get('dvg_stuf_bg_natuurlijkpersoon_api_vraagantwoord_url', ''),
    '#description' => t('Exclude the <code>?WSDL</code> part.'),
    '#required' => $vraagantwoord_url_required,
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-natuurlijkpersoon-enabled' => array(
          'checked' => TRUE,
        ),
        ':input[name="dvg_stuf_bg_natuurlijkpersoon_api_method"]' => array(
          'value' => 'prsVraagAntwoord',
        ),
      ),
    ),
  );
  $form['stuf_bg_natuurlijkpersoon']['dvg_stuf_bg_natuurlijkpersoon_api_scope'] = array(
    '#type' => 'radios',
    '#title' => t('API scope'),
    '#default_value' => dvg_stuf_bg_natuurlijkpersoon_api_scope(),
    '#options' => array('all' => 'alles', 'specific' => 'specifiek'),
    '#description' => t('API scope used by the endpoint url.'),
    '#required' => TRUE,
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-natuurlijkpersoon-enabled' => array(
          'checked' => TRUE,
        ),
        ':input[name="dvg_stuf_bg_natuurlijkpersoon_api_method"]' => array(
          array('value' => 'vraagAntwoord'),
          array('value' => 'prsVraagAntwoord'),
        ),
      ),
    ),
  );
  $form['stuf_bg_natuurlijkpersoon']['dvg_stuf_bg_natuurlijkpersoon_gemeentevaninschrijving_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable gemeenteVanInschrijving'),
    '#default_value' => dvg_stuf_bg_natuurlijkpersoon_gemeentevaninschrijving_enabled(),
    '#description' => t('Add gemeenteVanInschrijving to requested properties'),
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-natuurlijkpersoon-enabled' => array(
          'checked' => TRUE,
        ),
        ':input[name="dvg_stuf_bg_natuurlijkpersoon_api_method"]' => array(
          array('value' => 'vraagAntwoord'),
          array('value' => 'prsVraagAntwoord'),
        ),
        ':input[name="dvg_stuf_bg_natuurlijkpersoon_api_scope"]' => array(
          'value' => 'specific',
        ),
      ),
    ),
  );
  $form['stuf_bg_gezinssituatieopadresaanvrager'] = array(
    '#type' => 'fieldset',
    '#title' => t('StUF-BG GezinssituatieOpAdresAanvrager'),
    '#collapsible' => TRUE,
    '#collapsed' => !(dvg_stuf_bg_gezinssituatieopadresaanvrager_enabled()),
  );
  $form['stuf_bg_gezinssituatieopadresaanvrager']['dvg_stuf_bg_gezinssituatieopadresaanvrager_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable StUF-BG GezinssituatieOpAdresAanvrager'),
    '#default_value' => dvg_stuf_bg_gezinssituatieopadresaanvrager_enabled(),
    '#id' => 'dvg-stuf-bg-gezinssituatieopadresaanvrager-enabled',
  );
  $form['stuf_bg_gezinssituatieopadresaanvrager']['dvg_stuf_bg_gezinssituatieopadresaanvrager_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API endpoint url'),
    '#default_value' => variable_get('dvg_stuf_bg_gezinssituatieopadresaanvrager_api_url', ''),
    '#description' => t('Exclude the <code>?WSDL</code> part.'),
    '#required' => isset($form_state['input']['dvg_stuf_bg_gezinssituatieopadresaanvrager_enabled']) ? $form_state['input']['dvg_stuf_bg_gezinssituatieopadresaanvrager_enabled'] : FALSE,
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-gezinssituatieopadresaanvrager-enabled' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );
  $form['stuf_bg_vestiging'] = array(
    '#type' => 'fieldset',
    '#title' => t('StUF-BG Vestiging'),
    '#collapsible' => TRUE,
    '#collapsed' => !(dvg_stuf_bg_vestiging_enabled()),
  );
  $form['stuf_bg_vestiging']['dvg_stuf_bg_vestiging_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable StUF-BG Vestiging'),
    '#default_value' => dvg_stuf_bg_vestiging_enabled(),
    '#id' => 'dvg-stuf-bg-vestiging-enabled',
  );
  $form['stuf_bg_vestiging']['dvg_stuf_bg_vestiging_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API endpoint url'),
    '#default_value' => variable_get('dvg_stuf_bg_vestiging_api_url', ''),
    '#description' => t('Exclude the <code>?WSDL</code> part.'),
    '#required' => isset($form_state['input']['dvg_stuf_bg_vestiging_enabled']) ? $form_state['input']['dvg_stuf_bg_vestiging_enabled'] : FALSE,
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-vestiging-enabled' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );
  $form['stuf_bg_aoa'] = array(
    '#type' => 'fieldset',
    '#title' => t('StUF-BG Adrescheck'),
    '#collapsible' => TRUE,
    '#collapsed' => !(dvg_stuf_bg_aoa_enabled()),
  );
  $form['stuf_bg_aoa']['dvg_stuf_bg_aoa_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable StUF-BG Adrescheck'),
    '#default_value' => dvg_stuf_bg_aoa_enabled(),
    '#id' => 'dvg-stuf-bg-aoa-enabled',
  );
  $form['stuf_bg_aoa']['dvg_stuf_bg_aoa_api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API endpoint url'),
    '#default_value' => variable_get('dvg_stuf_bg_aoa_api_url', ''),
    '#description' => t('Exclude the <code>?WSDL</code> part.'),
    '#required' => isset($form_state['input']['dvg_stuf_bg_aoa_enabled']) ? $form_state['input']['dvg_stuf_bg_aoa_enabled'] : FALSE,
  );

  $form['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug mode'),
    '#collapsible' => TRUE,
    '#collapsed' => !(dvg_stuf_bg_debug_mode()),
  );
  $form['debug']['dvg_stuf_bg_debug_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),
    '#default_value' => dvg_stuf_bg_debug_mode(),
    '#id' => 'dvg-stuf-bg-debug-mode',
  );
  $form['debug']['dvg_stuf_bg_debug_bsn'] = array(
    '#type' => 'textfield',
    '#title' => t('Debug BSN'),
    '#default_value' => variable_get('dvg_stuf_bg_debug_bsn', ''),
    '#description' => t('Debug BSN used when debug mode is on.'),
    '#required' => isset($form_state['input']['dvg_stuf_bg_debug_mode']) ? $form_state['input']['dvg_stuf_bg_debug_mode'] : FALSE,
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-debug-mode' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );
  $form['debug']['dvg_stuf_bg_debug_kvk_vestigings_nummer'] = array(
    '#type' => 'textfield',
    '#title' => t('Debug KvK vestigingsNummer'),
    '#default_value' => variable_get('dvg_stuf_bg_debug_kvk_vestigings_nummer', ''),
    '#description' => t('Debug KvK vestigingsNummer used when debug mode is on.'),
    '#states' => array(
      'visible' => array(
        '#dvg-stuf-bg-vestiging-enabled' => array(
          'checked' => TRUE,
        ),
        '#dvg-stuf-bg-debug-mode' => array(
          'checked' => TRUE,
        ),
      ),
    ),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['settings']['dvg_stuf_bg_cache_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache timout'),
    '#default_value' => variable_get('dvg_stuf_bg_cache_timeout', 3600),
    '#size' => 8,
    '#description' => t('The length the cache, in seconds, before it expires. Must be %min_timeout seconds or greater.', array('%min_timeout' => DVG_STUF_BG_CACHE_MIN_TIMEOUT)),
  );
  $form['settings']['dvg_stuf_api_duration_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Duration threshold'),
    '#default_value' => variable_get('dvg_stuf_api_duration_threshold', 2),
    '#size' => 8,
    '#description' => t('The duration, in seconds, before the remote call gets marked as slow and added to the watchdog.'),
  );
  $form['settings']['dvg_stuf_bg_socket_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Socket timeout'),
    '#default_value' => variable_get('dvg_stuf_bg_socket_timeout', 30),
    '#size' => 8,
    '#description' => t('Timeout, in seconds, for socket based streams.'),
  );
  $form['settings']['dvg_stuf_bg_connection_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Connection timeout'),
    '#default_value' => variable_get('dvg_stuf_bg_connection_timeout', 2),
    '#size' => 8,
    '#description' => t('Timeout, in seconds, for the connection to the SOAP service'),
  );

  return system_settings_form($form);
}

/**
 * Settings validation.
 */
function dvg_stuf_bg_admin_form_validate($form, &$form_state) {
  $timeout = $form_state['values']['dvg_stuf_bg_cache_timeout'];
  // Validate timeout.
  if (!is_numeric($timeout) || ((int) $timeout != $timeout) || $timeout < DVG_STUF_BG_CACHE_MIN_TIMEOUT) {
    form_set_error('dvg_stuf_bg_cache_timeout', t('The timeout must be an integer greater than %min.', array('%min' => DVG_STUF_BG_CACHE_MIN_TIMEOUT)));
  }
  if (!empty($form_state['values']['dvg_stuf_bg_debug_bsn'])) {
    $form_state['values']['dvg_stuf_bg_debug_bsn'] = str_pad($form_state['values']['dvg_stuf_bg_debug_bsn'], 9, '0', STR_PAD_LEFT);
  }
}
