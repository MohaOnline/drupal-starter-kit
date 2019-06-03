<?php

namespace Drupal\dvg_authentication;

use SimpleSAML\Auth\Simple;
use SimpleSAML_Auth_Source;
use SimpleSAML_Auth_State;

/**
 * Common logic for providers using SAML authentication.
 */
abstract class SamlAuthenticationProviderBase extends AuthenticationProviderBase {

  /**
   * Dummy service identifier.
   *
   * Available on test and dev servers where SAML configuration is unavailable.
   */
  public const DUMMY_SERVICE = 'dummy';

  /**
   * Flag to check if the SAML library is loaded.
   *
   * @var bool
   */
  protected $SAMLLibraryLoaded;

  /**
   * SimpleSaml objects.
   *
   * @var array
   */
  protected $simpleSAML = [];

  /**
   * Mapping for the attributes returned by SAML.
   *
   * @var array
   */
  protected $samlAttributeMapping = [];

  /**
   * Check if the returnTo callback is supported by the external SAML provider.
   *
   * @var bool
   */
  protected $isLogoutReturnToCallbackSupported = TRUE;

  /**
   * {@inheritdoc}
   */
  public function getErrorMessage() {
    return t('An error occurred in the communication with @provider. Please try again later. If this error persists, check the @provider website for the latest information.',
      [
        '@provider' => $this->getLabel(),
      ]);
  }

  /**
   * Check if the SAML based Authentication provider is working in Dummy-modus.
   *
   * @param string|bool $level
   *   The level to check or false if this provider doesn't have levels.
   *
   * @return bool
   *   True fi the DUMMY_SERVICE is selected.
   */
  public function isDummyMode($level = AuthenticationProviderBase::LEVEL_NONE) {
    if ($level) {
      return $this->getLevelConfig($level, 'auth_source') === static::DUMMY_SERVICE;
    }
    return $this->getConfig('auth_source') === static::DUMMY_SERVICE;
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm() {
    $form = parent::buildConfigurationForm();

    if (!$this->hasLevels()) {
      $saml_options = $this->getSamlSources();
      $options = $saml_options ?: [self::DUMMY_SERVICE => t('Dummy')];
      $form['auth_source'] = [
        '#title' => t('Authentication source'),
        '#type' => 'select',
        '#options' => $options,
        '#empty_option' => t('Select an authentication source'),
        '#default_value' => $this->getConfig('auth_source'),
        '#required' => TRUE,
        '#weight' => 0,
      ];
      if (!$saml_options) {
        $form['auth_source']['#description'] = t('No SAML authentication available. Setup a service in the SAML configuration.');
      }
    }

    $form['show_confirmation_page'] = [
      '#title' => t('Use a confirmation page before redirecting the user to the @provider identity provider', ['@provider' => $this->getId()]),
      '#type' => 'checkbox',
      '#default_value' => $this->getConfig('show_confirmation_page'),
      '#weight' => 0,
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  protected function buildLevelConfigFields($level) {
    $fields = parent::buildLevelConfigFields($level);

    $saml_options = $this->getSamlSources();
    $options = $saml_options ?: [self::DUMMY_SERVICE => t('Dummy')];
    $fields['auth_source'] = [
      '#title' => t('Authentication source'),
      '#type' => 'select',
      '#options' => $options,
      '#empty_option' => t('Select an authentication source'),
      '#default_value' => $this->getLevelConfig($level, 'auth_source'),
      '#weight' => -1,
      '#id' => $level . '_auth_source',
    ];
    // Require logo if a level is selected.
    if (isset($fields['logo'])) {
      $fields['logo']['#required'] = FALSE;
      $fields['logo']['#states'] = [
        'invisible' => [
          '#' . $level . '_auth_source' => ['value' => ''],
        ],
        'optional' => [
          '#' . $level . '_auth_source' => ['value' => ''],
        ],
      ];
    }

    if (!$saml_options) {
      $fields['auth_source']['#description'] = t('No SAML authentication available. Setup a service in the SAML configuration.');
    }

    return $fields;
  }

  /**
   * {@inheritdoc}
   */
  public function getEnabledLevels() {
    $levels = $this->getLevels();
    $enabled_levels = [];
    foreach (array_keys($levels) as $level) {
      if (!empty($this->getLevelConfig($level, 'auth_source'))) {
        $enabled_levels[$level] = $levels[$level];
      }
    }
    return $enabled_levels;
  }

  /**
   * Build a very crude confirmation page.
   *
   * This contains a link the user has to click to confirm they
   * want to login to eHerkenning, before redirection to the external service.
   *
   * @return string
   *   The content for the confirmation page.
   */
  protected function getLoginConfirmationPage() {

    drupal_set_title(t('Log in with your @provider credentials', ['@provider' => $this->getLabel()]));

    $query = isset($_GET['destination']) ? ['destination' => $_GET['destination']] : [];
    $query['confirm'] = 1;

    // Prevent indexing of this page.
    $element = [
      '#tag' => 'meta',
      '#attributes' => [
        'name' => 'robots',
        'content' => 'noindex, nofollow',
      ],
    ];
    drupal_add_html_head($element, 'dvg_authentication_noindex');

    return l(t('@provider login', ['@provider' => $this->getLabel()]), current_path(), [
      'query' => $query,
      'attributes' => ['class' => [$this->getId() . '-link']],
    ]);
  }

  /**
   * When operating in Dummy mode, redirect to the default user login form.
   *
   * @param string $level
   *   The authentication level.
   */
  protected function redirectDummyLogin($level) {
    if ($this->isDummyMode($level)) {
      drupal_set_message(t('Currently operating in Dummy mode, please login with a !provider test account.', ['!provider' => $this->getLabel()]));
      $options = [];
      // If a destination is set, add it to the options to prevent direct
      // redirection and allow eventually redirecting back to the
      // original target.
      if (isset($_GET['destination'])) {
        $options['query']['destination'] = $_GET['destination'];
        unset($_GET['destination']);
      }
      drupal_goto('user', $options);
    }
  }

  /**
   * Check if the SimpleSAML library is available and loaded.
   *
   * @return bool
   *   TRUE if the library is loaded.
   */
  protected function loadSimpleSamlLibrary() {
    if ($this->SAMLLibraryLoaded === NULL) {
      $this->SAMLLibraryLoaded = FALSE;
      $library = libraries_load('simplesamlphp');
      $this->SAMLLibraryLoaded = $library && !empty($library['loaded']);
    }
    return $this->SAMLLibraryLoaded;
  }

  /**
   * Get the simpleSAML library.
   *
   * @param string $level
   *   The authentication level.
   *
   * @return bool|\SimpleSAML\Auth\Simple
   *   a SimpleSAML configuration object or FALSE when operating in
   *   DUMMY mode or SimpleSAML is not available.
   */
  public function getSimpleSaml($level = AuthenticationProviderBase::LEVEL_NONE) {
    if (!isset($this->simpleSAML[$level])) {
      $this->simpleSAML[$level] = FALSE;

      if ($level) {
        $auth_source = $this->getLevelConfig($level, 'auth_source');
      }
      else {
        $auth_source = $this->getConfig('auth_source');
      }

      if ($auth_source && $auth_source !== static::DUMMY_SERVICE && $this->loadSimpleSamlLibrary()) {
        $this->simpleSAML[$level] = new Simple($auth_source);
      }
    }
    return $this->simpleSAML[$level];
  }

  /**
   * Delete SAML Cookies.
   */
  public function deleteSamlCookies() {
    _drupal_session_delete_cookie('SimpleSAMLSessionID');
    _drupal_session_delete_cookie('SimpleSAMLAuthToken');
  }

  /**
   * Get available SAML sources.
   *
   * @return array
   *   List of available SamlSources if any configured.
   */
  public function getSamlSources() {
    $select_auth_sources = [];

    if ($this->loadSimpleSamlLibrary()) {
      $auth_sources_saml = SimpleSAML_Auth_Source::getSources();
      if ($auth_sources_saml && count($auth_sources_saml) > 0) {
        $select_auth_sources = drupal_map_assoc($auth_sources_saml);
        // Only add auth_sources prefixed with the current provider id.
        $regex = '/' . $this->getId() . '.*/i';
        $select_auth_sources = preg_grep($regex, $select_auth_sources);
      }
    }

    return $select_auth_sources;
  }

  /**
   * Get the SAML attribute mapping for this provider.
   *
   * @return array
   *   List of attributes
   */
  public function getUserAttributes() {
    return array_values($this->samlAttributeMapping);
  }

  /**
   * Parse the SAML attributes using the provider-specific mapping.
   *
   * SAML attributes that are not mapped will be skipped.
   *
   * @param array $saml_attributes
   *   Attributes provided by simpleSAML.
   *
   * @return array
   *   Array containing mapped values.
   */
  protected function parseSamlAttributes(array $saml_attributes) {
    $attributes = [];
    foreach ($saml_attributes as $key => $value) {
      $name_parts = explode(':', $key);
      $attribute_name = array_pop($name_parts);
      if (isset($this->samlAttributeMapping[$attribute_name])) {
        $attributes[$this->samlAttributeMapping[$attribute_name]] = $value[0];
      }
    }
    return $attributes;
  }

  /**
   * Process SamlErrors based on the current URL parameters.
   */
  public function processSamlError() {
    $this->loadSimpleSamlLibrary();

    if ($state = SimpleSAML_Auth_State::loadExceptionState()) {
      $exception = $state[SimpleSAML_Auth_State::EXCEPTION_DATA];

      // Has the user cancelled the login?
      if (method_exists($exception, 'getSubStatus') && strpos($exception->getSubStatus(), 'AuthnFailed') !== FALSE) {
        drupal_set_message(t('Login cancelled.'), 'warning');
        $this->deleteSamlCookies();
        drupal_goto(current_path());
      }
      else {
        $msg_args = ['!exception' => var_export($exception, 1)];
        watchdog('dvg_authentication', 'Error logging into Drupal. SAML exception: !exception', $msg_args, WATCHDOG_ERROR);
        drupal_set_message($this->getErrorMessage(), 'error');
      }
    }
    else {
      $msg_args = ['!exception' => var_export($_GET['SimpleSAML_Auth_State_exceptionId'], 1)];
      watchdog('dvg_authentication', 'Unknown error logging into Drupal. SAML exception url: !exception', $msg_args, WATCHDOG_ERROR);
      drupal_set_message($this->getErrorMessage(), 'error');
    }
  }

  /**
   * {@inheritdoc}
   */
  public function logoutCallback(\stdClass $account) {
    $external_user = $this->authenticationManager->getExternalUser($account);

    // Perform a SAML logout if the user isn't a debug user and
    // the provider isn't operating in dummy mode.
    if ($external_user && !$external_user->isDebugUser() && !$this->isDummyMode()) {
      if (isset($_GET['saml-logout'])) {
        $this->deleteSamlCookies();
        return;
      }

      if ($simplesamlphp = $this->getSimpleSaml($external_user->getLevel())) {
        // If the external SAML provider doesn't support a return callback,
        // kill the drupal user session before we log the user out in the
        // external identity provider.
        if (!$this->isLogoutReturnToCallbackSupported) {
          session_destroy();
        }
        $logouturl = $simplesamlphp->getLogoutURL(url(current_path(), [
          'query' => ['saml-logout' => 0],
          'absolute' => TRUE,
        ]));
        // Perform the SAML logout at the external identity provider.
        $simplesamlphp->logout($logouturl);
      }
    }
  }

  /**
   * Get information for the DvG requirements status screen.
   *
   * Override this function to add more information for the
   * specified authentication method.
   *
   * @return array
   *   All requirements info for this AuthenticationProvider.
   */
  public function getRequirementsInfo() {
    $requirements = parent::getRequirementsInfo();

    $requirements['dvg_authentication_' . $this->getId()] = [
      'title' => t('DvG Authentication') . ' ' . $this->getLabel(),
      'value' => t('Enabled'),
      'severity' => REQUIREMENT_OK,
    ];

    $libraries = libraries_get_libraries();
    $requirements['dvg_authentication_simplesaml'] = [
      'title' => t('Simplesamlphp library'),
      'value' => t('Ok'),
      'severity' => REQUIREMENT_OK,
    ];
    if (!isset($libraries['simplesamlphp'])) {
      $requirements['dvg_authentication_simplesaml']['value'] = t('Library not found.');
      $description = t('Please refer to the installation manual to install the simplesamlphp library');
      $requirements['dvg_authentication_simplesaml']['description'] = $description;
      $requirements['dvg_authentication_simplesaml']['severity'] = REQUIREMENT_ERROR;
    }
    elseif ($this->loadSimpleSamlLibrary() && !SimpleSAML_Auth_Source::getSources()) {
      // Show as warning for dev/test environment, as SAML is usually not
      // configured there.
      $environment = variable_get('environment_indicator_overwritten_name', 'PROD');
      $is_test_env = !in_array($environment, ['ACC', 'PROD']);
      $requirements['dvg_authentication_simplesaml_authentication_services'] = [
        'title' => t('DvG Authentication SAML'),
        'value' => t('No Authentication services found in SAML configuration.'),
        'severity' => $is_test_env ? REQUIREMENT_WARNING : REQUIREMENT_ERROR,
      ];
    }

    // Show the authSource requirements per level if levels are available.
    if ($this->hasLevels()) {
      foreach ($this->getLevels() as $level => $label) {
        $this->setLevelRequirementInfo($requirements, $level);
      }
    }
    else {
      $severity = static::DUMMY_SERVICE === $this->getConfig('auth_source') ? REQUIREMENT_WARNING : REQUIREMENT_INFO;
      $requirements['dvg_authentication_' . $this->getId() . '_authentication_source'] = [
        'title' => t('DvG Authentication @provider source %level', ['@provider' => $this->getLabel(), '%level' => '']),
        'value' => $this->getConfig('auth_source'),
        'severity' => $severity,
      ];
    }

    return $requirements;
  }

  /**
   * Set the requirements info per level.
   *
   * @param array $requirements
   *   The current requirements array, passed by reference.
   * @param string $level
   *   The level to check the requirements for.
   */
  protected function setLevelRequirementInfo(array &$requirements, $level) {
    if (!$this->hasLevels()) {
      return;
    }

    $auth_source = $this->getLevelConfig($level, 'auth_source');
    // If the auth_source is not set, show a warning and bail out early.
    if (empty($auth_source)) {
      $requirements['dvg_authentication_' . $this->getId() . '_auth_source_' . $level] = [
        'title' => t('DvG Authentication @provider source %level', [
          '@provider' => $this->getLabel(),
          '%level' => $this->getLevelLabel($level),
        ]),
        'value' => t('Not configured.'),
        'severity' => REQUIREMENT_WARNING,
      ];
      return;
    }
    $severity = static::DUMMY_SERVICE === $auth_source ? REQUIREMENT_WARNING : REQUIREMENT_INFO;
    $requirements['dvg_authentication_' . $this->getId() . '_auth_source_' . $level] = [
      'title' => t('DvG Authentication @provider source %level', [
        '@provider' => $this->getLabel(),
        '%level' => $this->getLevelLabel($level),
      ]),
      'value' => $auth_source,
      'severity' => $severity,
    ];

    $sources = SimpleSAML_Auth_Source::getSources();
    // Try to fetch the Identity provider.
    $requirement_key = 'dvg_authentication_' . $this->getId() . '_idp';
    if (!isset($requirements[$requirement_key]) && in_array($auth_source, $sources, TRUE)) {
      $requirement_idp_key = 'dvg_' . $this->getId() . '_idp';
      $requirements[$requirement_idp_key] = [
        'title' => t('DvG Authentication') . ' ' . $this->getLabel() . ' Identity Provider',
        'value' => t('Configuration not found.'),
        'severity' => REQUIREMENT_ERROR,
      ];
      try {
        $source_info = SimpleSAML_Auth_Source::getById($auth_source);
        $idp = $source_info->getIdPMetadata($source_info->getMetadata()->getString('idp'));
        if ($idp_info = $idp->toArray()) {
          if (!isset($idp_info['OrganizationDisplayName']['en'])) {
            // We know the idp is valid, but we can't find a useful
            // organization name. Just remove this status line, we have
            // sufficient other debug info.
            unset($requirements[$requirement_idp_key]);
            return;
          }
          $display_name = $idp_info['OrganizationDisplayName']['en'];
          $requirements[$requirement_idp_key]['value'] = $display_name;
          $requirements[$requirement_idp_key]['severity'] = REQUIREMENT_OK;
        }
      }
      catch (\Exception $ignored) {
      }
    }
  }

}
