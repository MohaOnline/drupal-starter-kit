<?php

namespace Drupal\dvg_authentication;

/**
 * Class SamlUser.
 *
 * Extends the base class with functionality for SAML based authentication.
 */
class SamlUser extends ExternalUserBase {

  /**
   * The SAML sub class for the authentication provider.
   *
   * Allows this SamlUser to use more functionality of the SAML provider.
   *
   * @var \Drupal\dvg_authentication\SamlAuthenticationProviderBase
   */
  protected $samlAuthenticationProvider;

  /**
   * SamlUser constructor.
   *
   * @param \stdClass $account
   *   Drupal user account object.
   * @param \Drupal\dvg_authentication\SamlAuthenticationProviderBase $authentication_provider
   *   The SamlAuthenticationProviderBase that provides this user.
   *
   * @throws \Drupal\dvg_authentication\DvgAuthenticationException
   */
  public function __construct(\stdClass $account, SamlAuthenticationProviderBase $authentication_provider) {
    parent::__construct($account, $authentication_provider);
    // Our more specific authentication provider reference.
    $this->samlAuthenticationProvider = $authentication_provider;
  }

  /**
   * {@inheritdoc}
   */
  public function getValue($field_name) {
    $fields = $this->samlAuthenticationProvider->getUserAttributes();
    if (in_array($field_name, $fields)) {
      $session_field_id = 'dvg_authentication_' . $this->samlAuthenticationProvider->getId();
      if (isset($_SESSION[$session_field_id][$field_name])) {
        drupal_page_is_cacheable(FALSE);
        return $_SESSION[$session_field_id][$field_name];
      }
    }
    // Return default account fields.
    return parent::getValue($field_name);
  }

  /**
   * {@inheritdoc}
   */
  public function isDebugUser() {
    return ((!isset($_COOKIE['SimpleSAMLSessionID']) && !isset($_COOKIE['SimpleSAMLAuthToken'])) || strpos($this->account->name, 'test.'));
  }

}
