<?php

namespace Drupal\dvg_authentication_eherkenning;

use Drupal\dvg_authentication\AuthenticationProviderBase;
use Drupal\dvg_authentication\SamlAuthenticationProviderBase;
use Drupal\dvg_authentication\SamlUser;

/**
 * Class EherkenningAuthenticationProvider.
 */
class EherkenningAuthenticationProvider extends SamlAuthenticationProviderBase {

  /**
   * List of supported eherkenning versions.
   *
   * @var array
   */
  protected static $versions = ['1.7', '1.9', '1.11'];

  /**
   * List of namespaces per version keyed by the eHerkenning version.
   *
   * @var array
   */
  protected static $xmlNamespaces = [
    '1.7' => 'urn:nl:eherkenning:',
    '1.9' => 'urn:etoegang:',
    '1.11' => 'urn:etoegang:',
  ];

  /**
   * {@inheritdoc}
   *
   * @var array
   */
  protected $samlAttributeMapping = [
    'ServiceID' => 'identifier',
    'KvKnr' => 'kvk_number',
    'Vestigingsnr' => 'kvk_department_number',
  ];

  /**
   * {@inheritdoc}
   */
  protected $isLogoutReturnToCallbackSupported = FALSE;

  /**
   * EHerkenning requires an extra logo per authentication level.
   *
   * @var bool
   */
  protected $useLevelLogos = TRUE;

  /**
   * {@inheritdoc}
   */
  public function getId() {
    return 'eherkenning';
  }

  /**
   * {@inheritdoc}
   */
  public function getLabel() {
    return t('eHerkenning');
  }

  /**
   * {@inheritdoc}
   */
  public function getButtonDescription() {
    $args = ['@url' => 'https://www.eherkenning.nl'];
    return t('You are an entrepreneur and registered with the Chamber of Commerce. Login with eHerkenning. More information can be found on <a href="@url">eherkenning.nl</a>.', $args);
  }

  /**
   * {@inheritdoc}
   */
  public function getLevels() {
    return [
      'level_1' => t('level 1'),
      'level_2' => t('level 2'),
      'level_2plus' => t('level 2+'),
      'level_3' => t('level 3'),
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
      '#title' => t('eHerkenning version'),
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
        $_SESSION['dvg_authentication_eherkenning'] = $saml_attributes;
      }
      else {
        watchdog('dvg_authentication_eherkenning', 'Error logging into Drupal. SAML attributes: @attributes', ['@attributes' => var_export($saml_attributes, 1)], WATCHDOG_ERROR);
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
