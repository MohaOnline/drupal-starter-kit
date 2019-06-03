<?php

/**
 * @file
 * This file contains the API of the DvG StUF module.
 */

/**
 * Use this hook to alter the SOAP options.
 * For example if you need to add a SSL handshake.
 */
function hook_dvg_stuf_bg_soap_options() {
  $opts = array(
    'ssl' => array(
      'verify_peer' => FALSE,
      'verify_peer_name' => FALSE,
      'local_cert' => variable_get('mymod_soap_local_cert', ''),
      'passphrase' => decrypt(variable_get('mymod_soap_encrypted_passphrase', '')),
    ),
  );
  $options['local_cert'] = variable_get('mymod_soap_local_cert', '');
  $options['passphrase'] = decrypt(variable_get('mymod_soap_encrypted_passphrase', ''));
  $options['stream_context'] = stream_context_create($opts);
}

/**
 * Use this hook to add additional roles that should be allowed to use the prefill.
 */
function hook_dvg_stuf_bg_allowed_prefill_roles_alter(&$roles) {
  // Assign the role used by stuf bg.
  $roles[] = 'user role';
}

/**
 * Use this hook to alter the WSDL and/or method used for StUF calls.
 * The type parameter can be used to identify what type of API call is done.
 *
 * @param string $wsdl
 *   The path to the WSDL that will be used for this call.
 * @param string $method
 *   The method to use for this call.
 * @param string $type
 *   Identifies what StUF BG call is done.
 *   One of:
 *   - natuurlijkpersoon
 *   - gezinssituatieopadresaanvrager
 *   - vestiging
 *   - aoa
 */
function hook_dvg_stuf_bg_api_call_alter(&$wsdl, &$method, $type) {
  if ($type === 'natuurlijkpersoon') {
    // Use a custom wsdl file.
    $wsdl = drupal_get_path('module', 'dvg_stuf_bg_custom_module') . '/wsdl/bg0310/vraagAntwoord/bg0310_beantwoordVraag_vraagAntwoord.wsdl';
    // Call a different method.
    $method = 'npsLv01Alternative';
  }
}
