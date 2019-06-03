<?php

namespace Drupal\dvg_authentication_eidas;

use Drupal\dvg_authentication\AuthenticationProviderBase;
use Drupal\dvg_authentication\SamlAuthenticationProviderBase;
use Drupal\dvg_authentication\SamlUser;

/**
 * Class EidasAuthenticationProvider.
 */
class EidasAuthenticationProvider extends SamlAuthenticationProviderBase {

  /**
   * List of supported eIDAS versions.
   *
   * @var array
   */
  protected static $versions = ['1.11'];

  /**
   * List of namespaces per version keyed by the eIDAS version.
   *
   * @var array
   */
  protected static $xmlNamespaces = [
    '1.11' => 'urn:etoegang:',
  ];

  /**
   * {@inheritdoc}
   *
   * @var array
   */
  protected $samlAttributeMapping = [
    'PersonIdentifier' => 'identifier',
    'FirstName' => 'first_name',
    'FamilyNameInfix' => 'infix',
    'FamilyName' => 'last_name',
    'DateOfBirth' => 'date_of_birth',
  ];

  /**
   * {@inheritdoc}
   */
  protected $isLogoutReturnToCallbackSupported = FALSE;

  /**
   * {@inheritdoc}
   */
  public function getId() {
    return 'eidas';
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return t('eIDAS');
  }

  /**
   * {@inheritdoc}
   */
  public function getButtonDescription() {
    return t('Login with your own nationally issued electronic identity credentials.');
  }

  /**
   * {@inheritdoc}
   */
  public function getLevels() {
    return [
      'low' => t('Low'),
      'substantial' => t('Substantial'),
      'high' => t('High'),
    ];
  }

  /**
   * {@inheritdoc}
   */
  protected function getDefaultConfiguration() {
    return [
      'show_confirmation_page' => FALSE,
      'version' => '1.11',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm() {
    $form = parent::buildConfigurationForm();
    $form['version'] = [
      '#type' => 'select',
      '#title' => t('eIDAS version'),
      '#options' => drupal_map_assoc(static::$versions),
      '#default_value' => $this->getConfig('version'),
      '#required' => TRUE,
      '#weight' => -2,
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function parseSamlAttributes(array $saml_attributes) {
    $attributes = parent::parseSamlAttributes($saml_attributes);
    // Format the date_of_birth attribute.
    if (!empty($attributes['date_of_birth'])) {
      $date_of_birth = new \DateObject(strtotime($attributes['date_of_birth']));
      $attributes['date_of_birth'] = date_format_date($date_of_birth, 'custom', 'd-m-Y');
    }
    return $attributes;
  }

  /**
   * {@inheritdoc}
   */
  public function login($level = AuthenticationProviderBase::LEVEL_NONE) {
    // Show the confirmation page, if enabled and the user hasn't confirmed yet.
    if ($this->getConfig('show_confirmation_page') && !isset($_GET['confirm'])) {
      return $this->getLoginConfirmationPage();
    }

    // If the dummy mode is enabled, we can only login with a test account,
    // so redirect to the normal login page and show a message.
    if ($this->isDummyMode($level)) {
      $this->redirectDummyLogin($level);
    }

    $simplesamlphp = $this->getSimpleSaml($level);
    // Is the user logged into SimpleSAMLphp?
    if ($simplesamlphp && $simplesamlphp->isAuthenticated()) {

      // Parse the SAML attributes to an easier to work with array.
      $saml_attributes = $this->parseSamlAttributes($simplesamlphp->getAttributes());
      // Generate an unique identifier based on all saml attributes.
      $nameid = $level . implode('/', $saml_attributes);

      if ($this->authenticationManager->userLogin($this, $nameid, $level)) {
        // Store the SAML values to the session.
        $_SESSION['dvg_authentication_eidas'] = $saml_attributes;
      }
      else {
        watchdog('dvg_authentication_eidas', 'Error logging into Drupal. SAML attributes: @attributes', ['@attributes' => var_export($saml_attributes, 1)], WATCHDOG_ERROR);
        drupal_set_message($this->getErrorMessage(), 'error');
      }
      drupal_goto();
    }
    else {
      $simplesamlphp->requireAuth([
        'ErrorURL' => isset($_GET['destination']) ? url($_GET['destination']) : base_path(),
      ]);
    }

    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function getUser(\stdClass $account) {
    return new SamlUser($account, $this);
  }

}
