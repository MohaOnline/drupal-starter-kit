<?php

namespace Drupal\dvg_authentication;

/**
 * Decorator class for the Drupal User account class (\stdClass)
 */
abstract class ExternalUserBase {

  /**
   * Drupal user account.
   *
   * @var \stdClass
   */
  protected $account;

  /**
   * The AuthenticationManager.
   *
   * @var \Drupal\dvg_authentication\AuthenticationManager
   */
  protected $authenticationManager;

  /**
   * The AuthenticationProvider that created this object.
   *
   * @var \Drupal\dvg_authentication\AuthenticationProviderBase
   */
  protected $authenticationProvider;

  /**
   * ExternalUser constructor.
   *
   * @param \stdClass $account
   *   Drupal user account object.
   * @param \Drupal\dvg_authentication\AuthenticationProviderBase $authentication_provider
   *   The AuthenticationProvider that provides this user.
   *
   * @throws \Drupal\dvg_authentication\DvgAuthenticationException
   */
  public function __construct(\stdClass $account, AuthenticationProviderBase $authentication_provider) {
    $this->account = $account;
    $this->authenticationManager = AuthenticationManager::getInstance();
    if (!isset($account->data['authentication_provider']) ||
      !$this->authenticationManager->isExternalUser($account)) {
      throw new DvgAuthenticationException('Error constructing ExternalUser: invalid account');
    }
    $this->authenticationProvider = $authentication_provider;
  }

  /**
   * Get the user's authenticationprovider.
   *
   * @return \Drupal\dvg_authentication\AuthenticationProviderBase
   *   The authentication provider that manages this user's authentication.
   */
  public function getProvider() {
    return $this->authenticationProvider;
  }

  /**
   * Get the id of the user's provider.
   *
   * @return string
   *   The id of the AuthenticationProvider.
   */
  public function getProviderId() {
    return $this->authenticationProvider->getId();
  }

  /**
   * Check if the user is logged in by the specified provider.
   *
   * @param string $provider_id
   *   Id of the provider.
   *
   * @return bool
   *   True if the id matches the user's provider.
   */
  public function isProvidedBy($provider_id) {
    return $this->authenticationProvider->getId() === $provider_id;
  }

  /**
   * Check if this is a fake/debug user.
   *
   * @return bool
   *   True if the user is a debug user, default FALSE.
   */
  public function isDebugUser() {
    return FALSE;
  }

  /**
   * Get the authentication level of this user.
   *
   * @return mixed
   *   Identifier of the authentication level.
   */
  public function getLevel() {
    return $this->account->data['authentication_level'];
  }

  /**
   * Check if this user has a certain level.
   *
   * @param string $required_level
   *   The required level.
   * @param bool $strict
   *   If TRUE, the user's level needs to match the requested level, if FALSE
   *   the user's level needs to be equal or greater than the required level.
   *
   * @return bool
   *   TRUE when all conditions are met.
   */
  public function hasLevel($required_level, $strict = FALSE) {
    // Levels are ordered from lowest to highest. Check if the user's level is
    // the requested level or higher.
    $levels = array_keys($this->getProvider()->getLevels());
    $user_level = $this->getLevel();

    // Check if the required level or the user's level is enabled.
    if (!in_array($required_level, $levels, TRUE) || !in_array($user_level, $levels, TRUE)) {
      return FALSE;
    }

    // Check if the user's level matches the required level.
    if ($strict) {
      return $user_level === $required_level;
    }

    // Use the index of the array as weight for the levels.
    $level_weights = array_flip($levels);
    // Check if the user has at least the required level.
    return ($level_weights[$user_level] >= $level_weights[$required_level]);
  }

  /**
   * Get a value of the user.
   *
   * @param string $field_name
   *   Field to fetch. This can be a normal Drupal account field,
   *   or a AuthenticationProvider specific field.
   *
   * @return mixed
   *   The value for the given field, or NULL if not found.
   */
  public function getValue($field_name) {
    $value = NULL;
    switch ($field_name) {
      case 'provider':
        $value = $this->authenticationProvider->getLabel();
        break;

      case 'provider_level':
        $value = $this->authenticationProvider->getLevelLabel($this->getLevel());
        break;

      case 'full_name':
        $name_parts = [
          $this->getValue('first_name'),
          $this->getValue('name_infix'),
          $this->getValue('last_name'),
        ];
        // Concatenate all parts of the name that have a value.
        $value = implode(' ', array_filter($name_parts));
        break;

      default:
        if (isset($this->account->$field_name)) {
          $value = $this->account->$field_name;
        }
    }

    return $value;
  }

}
