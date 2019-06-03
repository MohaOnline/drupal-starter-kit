<?php

namespace Drupal\dvg_authentication;

/**
 * Handles all default actions for external authentication providers.
 */
class AuthenticationManager {

  /**
   * The name of the user role for externally authenticated users.
   */
  public const USER_ROLE = 'external user';

  /**
   * The instance of the DvgAuthenticationManager.
   *
   * @var \Drupal\dvg_authentication\AuthenticationManager
   */
  protected static $instance;

  /**
   * A list of enabled authentication types, keyed by provider id.
   *
   * @var array
   */
  protected $authenticationProviders;

  /**
   * List of AuthenticationProvider dependencies per module.
   *
   * @var array
   */
  protected $moduleDependencies;

  /**
   * The role object of the external user role.
   *
   * @var \stdClass
   */
  protected $externalUserRole;

  /**
   * Configuration values.
   *
   * @var array
   */
  protected $configuration = [];

  /**
   * DvgAuthenticationManager constructor.
   *
   * The constructor is protected to make sure it can only be called from the
   * static::getInstance() function to force this class to work as a singleton.
   */
  protected function __construct() {
    $this->registerAuthenticationProviders();
    $this->registerModuleDependencies();
  }

  /**
   * Get an AuthenticationManager instance, create it if it doesn't exist.
   *
   * @return \Drupal\dvg_authentication\AuthenticationManager
   *   Instance of the DvgAuthenticationManager.
   */
  public static function getInstance() {
    if (!isset(static::$instance)) {
      static::$instance = new static();
    }
    return static::$instance;
  }

  /**
   * Detect and register all required providers for custom module functionality.
   */
  protected function registerModuleDependencies() {
    if ($this->moduleDependencies === NULL) {
      $this->moduleDependencies = [];
      // Get all modules that require a authentication provider
      // and there required provider.
      $hook = 'dvg_authentication_required_providers';
      foreach (module_implements($hook) as $module) {
        $providers = module_invoke($module, $hook);
        $this->moduleDependencies[$module] = [
          'providers' => [],
        ];
        foreach ($providers as $provider_id) {
          if (isset($this->authenticationProviders[$provider_id])) {
            $this->moduleDependencies[$module]['providers'][$provider_id] = $provider_id;
          }
          else {
            $missing_provider_msg = 'AuthenticationProvider ' . $provider_id . ' is not valid or not enabled.';
            watchdog('dvg_authentication', $missing_provider_msg, [], WATCHDOG_CRITICAL);
            // Also alert users with the power to do something about it,
            // or the power to complain to the right people.
            if (user_access('access administration menu')) {
              drupal_set_message($missing_provider_msg, 'error', FALSE);
            }
          }
        }
      }
    }
  }

  /**
   * Detect and register all AuthenticationProviders provided by other modules.
   */
  protected function registerAuthenticationProviders() {
    if ($this->authenticationProviders === NULL) {
      $this->authenticationProviders = [];
      // Get all AuthenticationProviders and register the modules
      // that supply them.
      $hook = 'dvg_authentication_register_providers';
      foreach (module_implements($hook) as $module) {
        $providers = module_invoke($module, $hook);
        foreach ($providers as $provider_id => $classname) {
          $this->authenticationProviders[$provider_id] = [
            'classname' => $classname,
            'module' => $module,
          ];
        }
      }
    }
  }

  /**
   * Get all enabled authentication providers provided by submodules.
   *
   * @param array $allowed_providers
   *   Optionally limit the returned providers by this list of
   *   allowed providers.
   *
   * @return \Drupal\dvg_authentication\AuthenticationProviderBase[]
   *   List with all AuthenticationProviders, keyed by the provider id.
   */
  public function getAuthenticationProviders(array $allowed_providers = []) {
    $providers = [];
    foreach (array_keys($this->authenticationProviders) as $provider_id) {
      // Don't return providers that aren't in the allowed providers list.
      if ($allowed_providers && !in_array($provider_id, $allowed_providers)) {
        continue;
      }
      $providers[$provider_id] = $this->getAuthenticationProvider($provider_id);
    }
    return $providers;
  }

  /**
   * Get an Authentication Provider object by identifier.
   *
   * @param string $provider_id
   *   The machine name of the provider.
   *
   * @return \Drupal\dvg_authentication\AuthenticationProviderBase|null
   *   Instance of the requested Authentication Provider.
   */
  public function getAuthenticationProvider($provider_id) {
    if (!empty($provider_id) && isset($this->authenticationProviders[$provider_id])) {
      $classname = $this->authenticationProviders[$provider_id]['classname'];
      /** @var \Drupal\dvg_authentication\AuthenticationProviderBase $classname */
      return $classname::getInstance($this, $this->authenticationProviders[$provider_id]['module']);
    }
    return NULL;
  }

  /**
   * Get a configuration value.
   *
   * @param string $variable
   *   Name of the config variable.
   *
   * @return mixed|null
   *   The value for the given config variable, or NULL of not found.
   */
  public function getConfig($variable) {
    return $this->configuration[$variable] ?? NULL;
  }

  /**
   * Do the actual Drupal login, creating a new temporary drupal user.
   *
   * @param \Drupal\dvg_authentication\AuthenticationProviderBase $provider
   *   The AuthenticationProvider used for authentication.
   * @param string|int $identifier
   *   Authentication type specific identifier, from the provider.
   * @param string|false $level
   *   The authentication level of the user.
   *
   * @return bool
   *   If the user is logged in.
   *
   * @throws \Exception
   */
  public function userLogin(AuthenticationProviderBase $provider, $identifier, $level = AuthenticationProviderBase::LEVEL_NONE) {
    global $user;
    if ($account = $this->getDrupalUser($provider, $identifier, $level)) {
      $user = $account;
      $edit = ['name' => $user->name];
      user_login_finalize($edit);
      drupal_set_message($provider->getLoginSuccessMessage());
      return TRUE;
    }
    drupal_set_message($provider->getLoginFailedMessage(), 'error');
    return FALSE;
  }

  /**
   * Create a new temporary user with the external user role.
   *
   * @param \Drupal\dvg_authentication\AuthenticationProviderBase $provider
   *   Identifier of the authentication provider.
   * @param string|int $identifier
   *   Authentication type specific identifier (e.g. a SSN/BSN).
   * @param string|false $level
   *   The level on which the user tries to login.
   *
   * @return bool|\stdClass
   *   The Drupal user or FALSE if it there is non.
   *
   * @throws \Exception
   */
  public function getDrupalUser(AuthenticationProviderBase $provider, $identifier, $level = AuthenticationProviderBase::LEVEL_NONE) {
    // Use the provider and the external identifier to
    // create a unique name for the user.
    $auth_name = $this->createHash($provider->getId() . ':' . $level . $identifier);

    // Load or create the account.
    if ($account = user_load_by_name($auth_name)) {
      return $account;
    }

    $mail = drupal_substr($auth_name, 0, 16) . '@' . drupal_substr($auth_name, 16) . '.external_user';
    $edit = [
      'name' => $auth_name,
      'mail' => $mail,
      'pass' => $this->createHash($auth_name . microtime() . $identifier),
      'status' => 1,
      'init' => $mail,
      'timezone' => variable_get('date_default_timezone', @date_default_timezone_get()),
      'authentication_provider' => $provider->getId(),
      'authentication_level' => $level,
    ];

    // Create the user with the "external user" role.
    if ($role = $this->getExternalUserRole()) {
      $edit['roles'][$role->rid] = $role->name;
    }

    // Save the new account.
    if ($account = user_save([], $edit)) {
      return $account;
    }

    return FALSE;
  }

  /**
   * Get the external user role.
   *
   * @return null|\stdClass
   *   A fully loaded user role object, if available.
   */
  public function getExternalUserRole() {
    if ($this->externalUserRole === NULL) {
      $this->externalUserRole = user_role_load_by_name(static::USER_ROLE);
    }
    return $this->externalUserRole;
  }

  /**
   * Check if the account has the external user role.
   *
   * @param \stdClass $account
   *   (Optional) Drupal account, defaults to the current user when omitted.
   *
   * @return bool
   *   True if the user has the external user role.
   */
  public function isExternalUser(\stdClass $account = NULL) {
    global $user;
    if (!$account) {
      $account = $user;
    }
    return \in_array(static::USER_ROLE, $account->roles, TRUE);
  }

  /**
   * Get the ExternalUser reference for the given or current user.
   *
   * @param \stdClass $account
   *   (Optional) Drupal account, defaults to the current user when omitted.
   *
   * @return \Drupal\dvg_authentication\ExternalUserBase|null
   *   The User object or NULL if the user isn't logged in with an external
   *   authentication method.
   */
  public function getExternalUser(\stdClass $account = NULL) {
    global $user;
    if (!$account) {
      $account = $user;
    }
    if (!empty($account->data['authentication_provider']) && $this->isExternalUser($account)) {
      $provider = $this->getAuthenticationProvider($account->data['authentication_provider']);
      try {
        return $provider->getUser($account);
      }
      catch (DvgAuthenticationException $e) {
        watchdog('dvg_authentication', $e->getMessage(), [], WATCHDOG_ERROR);
      }
    }
    return NULL;
  }

  /**
   * Get the module's authentication provider.
   *
   * @param string $module_name
   *   Drupal module name.
   *
   * @return \Drupal\dvg_authentication\AuthenticationProviderBase[]|null
   *   The authentication provider(s) for that module or NULL.
   */
  public function getModuleAuthenticationProviders($module_name) {
    if (!empty($this->moduleDependencies[$module_name]['providers'])) {
      return $this->moduleDependencies[$module_name]['providers'];
    }
    return NULL;
  }

  /**
   * Check if the user has access based by the allowed login providers.
   *
   * @param \stdClass $account
   *   Drupal account.
   * @param array|null $allowed_providers
   *   (optional) List of allowed providers, defaults to all
   *   available providers.
   *
   * @return bool
   *   True if the user is logged in using an allowed provider.
   */
  public function checkUserAccess(\stdClass $account, array $allowed_providers = NULL) {
    $external_user = $this->getExternalUser($account);
    if (!$external_user) {
      return FALSE;
    }
    // Default to the enabled provider if no provider is specified.
    if (empty($allowed_providers)) {
      $allowed_providers = $this->getAuthenticationProviders();
    }

    // Check if the user's authentication_provider exists in the list of
    // allowed providers.
    if (isset($allowed_providers[$external_user->getProviderId()])) {
      $provider_settings = $allowed_providers[$external_user->getProviderId()];
      if (!isset($provider_settings['level']) || $external_user->hasLevel($provider_settings['level'])) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Check if authentication is enabled on a node.
   *
   * @param \stdClass $node
   *   Node object.
   *
   * @return bool
   *   TRUE if any authentication option is enabled on this node.
   */
  public function nodeHasAuthentication(\stdClass $node) {
    // Check if authentication is enabled on this webform.
    if (isset($node->webform)) {
      $authentication_role = $this->getExternalUserRole();
      // Check if the DigiD rid is present.
      foreach ($node->webform['roles'] as $rid) {
        if ($authentication_role->rid == $rid) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Get external authentication settings for this node.
   *
   * @param \stdClass $node
   *   Node object.
   *
   * @return array
   *   Array with authentication settings for this node.
   */
  public function getNodeAuthenticationSettings(\stdClass $node) {
    if (isset($node->webform)) {
      return !empty($node->webform['dvg_authentication_settings']) ? unserialize($node->webform['dvg_authentication_settings'], ['allowed_classes' => FALSE]) : [];
    }
    return [];
  }

  /**
   * Generate safe hashes for the storage of sensitive data.
   *
   * @param string $value
   *   The value to hash.
   *
   * @return string
   *   The hashed value.
   */
  protected function createHash($value) {
    return substr(hash('sha256', $value . drupal_get_hash_salt()), 0, 32);
  }

  /**
   * Build the selection block for all allowed providers.
   *
   * @param array|null $allowed_providers
   *   (optional) List of allowed providers, default to all available providers.
   * @param \stdClass|null $node
   *   (optional) The node for which this login selection is used.
   *
   * @return array
   *   Renderable array with login buttons.
   */
  public function buildLoginSelection(array $allowed_providers = NULL, $node = NULL) {
    if (empty($allowed_providers)) {
      $allowed_providers = $this->getAuthenticationProviders();
    }

    $build = [
      '#theme' => 'authentication_login_options',
      '#title' => t('Choose a login method:'),
    ];
    foreach ($allowed_providers as $provider_id => $provider_settings) {
      $provider = $this->getAuthenticationProvider($provider_id);
      if (is_array($provider_settings) && isset($provider_settings['level'])) {
        $level = $provider_settings['level'];
      }
      else {
        // Get the most basic available level if none is selected.
        $enabled_levels = array_keys($provider->getEnabledLevels());
        $level = reset($enabled_levels);
      }
      $build['#children'][] = $provider->createLoginButton($level);
    }
    // If this selection is for webforms, it might need a skip
    // authentication option.
    if (isset($node) && $node->type === 'webform') {
      $webform_settings = $this->getNodeAuthenticationSettings($node);
      if (!empty($webform_settings['dvg_authentication_skip_authentication_enabled'])) {
        $build['#children'][] = [
          '#type' => 'authentication_login_button',
          '#title' => t('Skip authentication'),
          '#link' => l(
            t('Skip authentication'),
            current_path(),
            [
              'attributes' => [
                'class' => [
                  'btn',
                  'btn__ext-auth',
                ],
                'rel' => 'nofollow',
              ],
              'query' => [
                'skip_auth' => 1,
              ],
            ]
          ),
          '#attributes' => [
            'class' => [
              'dvgauth__skip-authentication',
            ],
          ],
        ];
      }
    }

    return $build;
  }

}
