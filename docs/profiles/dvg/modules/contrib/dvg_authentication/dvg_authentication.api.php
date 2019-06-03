<?php

/**
 * @file
 * Hooks provided by DvG Authentication.
 */

/**
 * Expose new authentication providers to the authentication manager.
 *
 * Modules may implement this hook to add custom AuthenticationProviders
 * for external authorised users.
 *
 * @return array
 *   List of AuthenticationProviders, keyed by identifier.
 */
function hook_dvg_authentication_register_providers() {
  return [
    'dummy' => 'AuthenticationProviderDummy',
    'digid' => 'AuthenticationProviderDigiD',
  ];
}

/**
 * Add a dependency of the required providers.
 *
 * Modules may implement this hook to add AuthenticationProviders
 * to their forms.
 *
 * @return array
 *   List of required AuthenticationProviders by identifier.
 */
function hook_dvg_authentication_required_providers() {
  return [
    'dummy',
    'digid',
  ];
}

/**
 * Provide alternative form functionality forms when the user has a permission.
 *
 * If the user has the given permission the callback will be called instead of
 * denying access to the form.
 *
 * When the user has access to multiple alternatives, only the first will be
 * called.
 *
 * @return array
 *   An array with permissions as key and as value a callback handling the
 *   alternative execution path.
 *   The callback gets passed the $build variable from hook_node_view_alter().
 *
 * @see hook_node_view_alter()
 */
function hook_dvg_authentication_selection_alternatives() {
  $alternatives = [
    'authentication servicedesk' => 'dvg_authentication_servicedesk_auth_selection_alternative',
  ];
  return $alternatives;
}

/**
 * Modify the user values returned by dvg_authentication.
 *
 * @param mixed $value
 *   The unaltered value to be returned.
 * @param string $field_name
 *   The field name for which a value is requested.
 */
function hook_dvg_authentication_user_value_alter(&$value, $field_name) {
  if ($field_name === 'bsn' && user_access('authentication servicedesk')) {
    $value = $_SESSION['servicedesk']['bsn'] ?? NULL;
  }
}
