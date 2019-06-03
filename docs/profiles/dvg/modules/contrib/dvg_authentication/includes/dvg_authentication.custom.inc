<?php

/**
 * @file
 * Contains hooks to simplify using external authentication in other modules.
 */

use Drupal\dvg_authentication\AuthenticationManager;

/**
 * Get the login selection screen for the enabled authentication providers.
 *
 * @param string $module_name
 *   The machine name of the module.
 *
 * @return array|bool
 *   Return a render array or FALSE;
 */
function dvg_authentication_authentication_select_block($module_name) {
  $manager = AuthenticationManager::getInstance();

  $providers = $manager->getModuleAuthenticationProviders($module_name);
  if (!empty($providers)) {
    return $manager->buildLoginSelection($providers);
  }

  return [];
}

/**
 * Check if a user is logged in by any provider configured for the provider.
 *
 * @param \stdClass $account
 *   Drupal user object.
 * @param array $allowed_providers
 *   List of provider_id's that are allowed for this call.
 * @param bool $debug_mode
 *   If true, this function also checks if the user is in debug mode.
 *
 * @return bool
 *   TRUE if the current user is logged in and provided by one of the
 *   authentication methods enabled for the supplied module.
 */
function dvg_authentication_external_logged_in(\stdClass $account, array $allowed_providers, $debug_mode = FALSE) {
  $manager = AuthenticationManager::getInstance();
  $externalUser = $manager->getExternalUser($account);
  $providers = $manager->getAuthenticationProviders($allowed_providers);

  if ($externalUser && !empty($providers)) {
    // Loop through the enabled provider to check if the current user is logged
    // in using that authentication method.
    foreach ($providers as $provider) {
      if ($externalUser->isProvidedBy($provider->getId())) {
        if ($debug_mode) {
          return $externalUser->isDebugUser();
        }
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Get a value of the external user object, if available.
 *
 * @param string $field_name
 *   Name of the field, e.g. 'bsn' to get a DigiD user's BSN.
 *
 * @return mixed
 *   The value, if available.
 */
function dvg_authentication_get_external_user_value($field_name) {
  $value = NULL;
  if ($external_user = AuthenticationManager::getInstance()->getExternalUser()) {
    $value = $external_user->getValue($field_name);
  }
  drupal_alter('dvg_authentication_user_value', $value, $field_name);
  return $value;
}

/**
 * Check if the authentication provider has been enabled on the node.
 *
 * @param \stdClass $node
 *   The drupal node.
 * @param string $provider_id
 *   The provider id.
 *
 * @return bool
 *   If the specified $provider_id is enabled for authentication on $node.
 */
function dvg_authentication_node_authentication_provider_enabled(\stdClass $node, $provider_id) {
  $authentication_manager = AuthenticationManager::getInstance();
  if ($authentication_manager->nodeHasAuthentication($node)) {
    $settings = $authentication_manager->getNodeAuthenticationSettings($node);
    return isset($settings['methods'][$provider_id]);
  }

  return FALSE;
}

/**
 * Get the authentication provider logo.
 *
 * @param string $provider_id
 *   The authentication provider.
 *
 * @return mixed|null
 *   The logo path or NULL.
 */
function dvg_authentication_get_provider_logo($provider_id) {
  $provider = AuthenticationManager::getInstance()->getAuthenticationProvider($provider_id);
  if ($provider) {
    return $provider->getConfig('logo');
  }
  return NULL;
}
