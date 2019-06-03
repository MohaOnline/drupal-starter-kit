<?php

namespace Drupal\dvg_authentication;

/**
 * Base class for DvgAuthenticationProviders.
 */
abstract class AuthenticationProviderBase implements ConfigInterface {

  /**
   * Used for authentication providers that don't have more than one level.
   */
  public const LEVEL_NONE = FALSE;

  /**
   * Instances of the AuthenticationProviderBase subclasses.
   *
   * @var \Drupal\dvg_authentication\AuthenticationProviderBase[]
   */
  protected static $instances;

  /**
   * The AuthenticationManager.
   *
   * @var \Drupal\dvg_authentication\AuthenticationManager
   */
  protected $authenticationManager;

  /**
   * Name of the module that provides this AuthenticationProvider.
   *
   * @var string
   */
  protected $moduleName;

  /**
   * Configuration settings of this plugin.
   *
   * @var array
   */
  protected $configuration;

  /**
   * If the provider has an extra logo per authentication level.
   *
   * @var bool
   */
  protected $useLevelLogos = FALSE;

  /**
   * AuthenticationProviderBase constructor.
   *
   * @param \Drupal\dvg_authentication\AuthenticationManager $authentication_manager
   *   The AuthenticationManager.
   * @param string $module_name
   *   Name of the module that provides this AuthenticationManager.
   *
   * @see AuthenticationProviderBase::getInstance()
   */
  public function __construct(AuthenticationManager $authentication_manager, $module_name) {
    $this->authenticationManager = $authentication_manager;
    $this->moduleName = $module_name;
    // Load the configuration for this plugin.
    $this->configuration = variable_get('dvg_authentication_' . $this->getId(), $this->getDefaultConfiguration());
  }

  /**
   * Get the instance of the called child class of AuthenticationProviderBase.
   *
   * @param \Drupal\dvg_authentication\AuthenticationManager $authentication_manager
   *   The AuthenticationManager instance.
   * @param string $module_name
   *   Name of the module that supplied this provider.
   *
   * @return \Drupal\dvg_authentication\AuthenticationProviderBase
   *   Instance of the AuthenticationProviderBase.
   */
  public static function getInstance(AuthenticationManager $authentication_manager, $module_name) {
    $called_class = get_called_class();
    if (!isset(static::$instances[$called_class])) {
      static::$instances[$called_class] = new static($authentication_manager, $module_name);
    }
    return static::$instances[$called_class];
  }

  /**
   * Get the identifier (machine name) of this AuthenticationProvider.
   *
   * @return string
   *   Identifier of the AuthenticationProvider.
   */
  abstract public function getId();

  /**
   * Get the human readable label of the AuthenticationProvider.
   *
   * @return string
   *   Translated text label.
   */
  abstract public function getLabel();

  /**
   * Get the available levels for this Authentication method.
   *
   * If there's only one level available, an array with level 'standard'
   * is returned.
   *
   * @return array
   *   List of levels for this provider, keyed by the level identifier,
   *   the value is the translatable label for the level. e.g.:
   *   ['level_1' => t('Level 1'), 'level_2' => t('Level 2')];
   */
  public function getLevels() {
    return [static::LEVEL_NONE => t('None')];
  }

  /**
   * Get all enabled levels.
   *
   * @return array
   *   Enabled levels.
   */
  public function getEnabledLevels() {
    return $this->getLevels();
  }

  /**
   * Check if authentication levels are enabled.
   *
   * @return bool
   *   TRUE if there are levels set.
   */
  public function hasLevels() {
    $levels = $this->getLevels();
    unset($levels[static::LEVEL_NONE]);
    return !empty($levels);
  }

  /**
   * Get the label of the specified level.
   *
   * @param string $level
   *   The authentication level.
   *
   * @return string|null
   *   Label of the authentication level or NULL when the level is
   *   not available or equal to LEVEL_NONE.
   */
  public function getLevelLabel($level) {
    $levels = $this->getLevels();
    if ($level && array_key_exists($level, $levels)) {
      return $levels[$level];
    }
    return NULL;
  }

  /**
   * Get a description to show next to the login button.
   *
   * @return string
   *   The description to show with the button.
   */
  public function getButtonDescription() {
    return '';
  }

  /**
   * Get the functional content id for the error page.
   *
   * @return string
   *   functional content key.
   */
  public function getErrorPageFunctionalContentId() {
    return 'dvg_authentication_' . $this->getId() . '__error_page';
  }

  /**
   * Get the error message of the AuthenticationProvider.
   *
   * @return string
   *   The error message.
   */
  abstract public function getErrorMessage();

  /**
   * Get the administrative description, for use in the admin backend.
   *
   * @return string
   *   Translated description of the authentication provider.
   */
  public function getAdminDescription() {
    return t('Settings for %provider (%module_name).', ['%provider' => $this->getLabel(), '%module_name' => $this->getModuleName()]);
  }

  /**
   * Get the name of the module that provides this AuthenticationProvider.
   *
   * @return string
   *   Module name.
   */
  public function getModuleName() {
    return $this->moduleName;
  }

  /**
   * Build the configuration form for the admin page.
   *
   * @return array
   *   Form fields for the configuration of this form.
   */
  protected function buildConfigurationForm() {
    $form = [];
    $form['logo'] = [
      '#title' => t('Logo'),
      '#type' => 'managed_file',
      '#default_value' => $this->getConfig('logo'),
      '#description' => t('This logo is used on the authentication selection screen.'),
      '#upload_validators' => [
        'file_validate_extensions' => ['png jpg jpeg svg'],
      ],
      '#required' => TRUE,
      '#upload_location' => 'public://',
      '#weight' => -1,
    ];

    if ($this->hasLevels()) {
      $form['levels'] = [
        '#tree' => 'true',
        '#title' => t('Level settings'),
      ];
      foreach ($this->getLevels() as $level => $label) {
        $form['levels'][$level] = $this->buildLevelConfigFields($level);
      }
    }

    return $form;
  }

  /**
   * Build the fields for the configuration form for an authentication level.
   *
   * @param string $level
   *   Identifier of the level.
   *
   * @return array
   *   Render array with the form fields.
   */
  protected function buildLevelConfigFields($level) {

    $fields = [
      '#type' => 'fieldset',
      '#title' => $this->getLabel() . ' ' . $this->getLevelLabel($level),
    ];

    if ($this->useLevelLogos) {
      $fields['logo'] = [
        '#title' => t('Logo'),
        '#type' => 'managed_file',
        '#default_value' => $this->getLevelConfig($level, 'logo'),
        '#required' => TRUE,
        '#upload_location' => 'public://',
        '#description' => t('This will be shown next to the login button.'),
        '#upload_validators' => [
          'file_validate_extensions' => ['png jpg jpeg svg'],
        ],
      ];
    }

    return $fields;
  }

  /**
   * Get the configuration form structure.
   *
   * @param array $form
   *   An associative array containing the initial structure of the form.
   * @param array $form_state
   *   The current state of the form.
   *
   * @return array
   *   The form structure.
   */
  public function getConfigurationForm(array $form, array &$form_state) {
    // Add the provider id to the config form so we can use it in the
    // submit handler to trigger the right callback function.
    $form['provider_id'] = [
      '#type' => 'value',
      '#value' => $this->getId(),
    ];

    // Wrap all settings so they will be stored as an array in one variable.
    $configuration_form = $this->buildConfigurationForm();
    $form['dvg_authentication_' . $this->getId()] = $configuration_form;
    $form['dvg_authentication_' . $this->getId()]['#tree'] = TRUE;
    return $form;
  }

  /**
   * Validator for the configurationForm.
   *
   * @param array $form
   *   The form.
   * @param array $form_state
   *   The current form state.
   */
  public function configurationFormValidate(array $form, array &$form_state) {
    foreach ($this->getLevels() as $level => $label) {
      $level_settings = $form_state['values']['dvg_authentication_' . $this->getId()]['levels'][$level];
      if (
        $this->useLevelLogos
        && !empty($level_settings['auth_source'])
        && empty($level_settings['logo'])
      ) {
        $element_parents = 'dvg_authentication_' . $this->getId() . '][levels][' . $level . '][logo';
        $field_name = ['!name' => t('Logo !level_label', ['!level_label' => $label])];
        form_set_error($element_parents, t('!name field is required.', $field_name));
      }
    }
  }

  /**
   * Submit handler for the configurationForm.
   *
   * @param array $form
   *   The form.
   * @param array $form_state
   *   The current form state.
   */
  public function configurationFormSubmit(array $form, array &$form_state) {
    if (!empty($form_state['values']['dvg_authentication_' . $this->getId()]['logo'])) {
      $this->addFileUsage($form_state['values']['dvg_authentication_' . $this->getId()]['logo']);
    }

    if ($this->useLevelLogos && $this->hasLevels()) {
      // Add usage to the level indicators to prevent it from
      // disappearing on cron.
      $values = $form_state['values']['dvg_authentication_' . $this->getId()]['levels'];
      foreach ($this->getLevels() as $level => $label) {
        if (!empty($values[$level]['logo'])) {
          $this->addFileUsage($values[$level]['logo']);
        }
      }
    }
  }

  /**
   * Add usage to the logo to prevent it from disappearing on cron.
   *
   * @param int $fid
   *   Id of the file.
   */
  protected function addFileUsage($fid) {
    $file = file_load($fid);
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'dvg_authentication', 'dvg_authentication_' . $this->getId(), 0);
  }

  /**
   * Get the default configuration for this plugin.
   *
   * @return array
   *   Default settings for this plugin.
   */
  protected function getDefaultConfiguration() {
    return [];
  }

  /**
   * Get a setting from the plugin configuration.
   *
   * @param string $setting
   *   Name of the setting to fetch.
   *
   * @return mixed|null
   *   The value or NULL if the setting doesn't exist.
   */
  public function getConfig($setting) {
    return $this->configuration[$setting] ?? NULL;
  }

  /**
   * Get a setting from the plugin configuration for an authentication level.
   *
   * @param string $level
   *   Id of the authentication level.
   * @param string $setting
   *   Name of the setting to fetch.
   *
   * @return mixed|null
   *   The value or NULL if the setting doesn't exist.
   */
  public function getLevelConfig($level, $setting) {
    return $this->configuration['levels'][$level][$setting] ?? NULL;
  }

  /**
   * Perform the login action for this AuthenticationProvider plugin.
   *
   * This can be a redirect to the authentication broker or service.
   *
   * @param string|bool $level
   *   The level on which the user tries to login.
   *
   * @return mixed
   *   A boolean value when the login is executed directly indicating failure or
   *   success, or a page callback, e.g. a confirmation page to show the user
   *   before the actual login is performed.
   */
  abstract public function login($level = AuthenticationProviderBase::LEVEL_NONE);

  /**
   * Callback to perform when hook_user_logout() is called.
   *
   * @param \stdClass $account
   *   Drupal user account.
   *
   * @see hook_user_logout()
   */
  public function logoutCallback(\stdClass $account) {
    // No special actions on logout by default.
  }

  /**
   * Get the external user object.
   *
   * @param \stdClass $account
   *   A Drupal user object.
   *
   * @return \Drupal\dvg_authentication\ExternalUserBase
   *   The external user object.
   *
   * @throws \Drupal\dvg_authentication\DvgAuthenticationException
   */
  abstract public function getUser(\stdClass $account);

  /**
   * Relative path to the login callback.
   *
   * @param string $level
   *   (Optional) the level to generate the login path for. If the supplied
   *   level doesn't exists for this provider, the path is skipped.
   *
   * @return string
   *   The path.
   */
  public function getLoginPath($level = NULL) {
    $path = 'user/external/' . $this->getId();
    if ($level && array_key_exists($level, $this->getLevels())) {
      $path .= '/' . $level;
    }
    return $path;
  }

  /**
   * Create a login button with the logo and optional message.
   *
   * @param string $level
   *   (optional) The level of the Authentication provider.
   *
   * @return array
   *   Renderable array for the button.
   */
  public function createLoginButton($level = NULL) {
    $build = [
      '#type' => 'authentication_login_button',
      '#title' => $this->getLabel(),
      '#link' => l(
        t('Log in with @provider', ['@provider' => $this->getLabel()]),
        $this->getLoginPath($level),
        [
          'attributes' => [
            'class' => [
              'btn',
              'btn__dvgauth',
            ],
            'rel' => 'nofollow',
          ],
          'query' => drupal_get_destination(),
        ]
      ),
    ];

    // Add the description.
    $build['#description'] = $this->getButtonDescription();

    // Add the logo if available.
    if ($fid = $this->getConfig('logo')) {
      $title_text = t('@provider logo', ['@provider' => $this->getLabel()]);
      $attributes = [
        'class' => [
          'dvgauth__logo',
        ],
      ];
      $build['#logo'] = $this->getLogo($fid, $title_text, $attributes);
    }

    // Add the level indicator logo.
    if ($level) {
      if ($level_indicator = $this->getLevelConfig($level, 'logo')) {
        $level_title_text = $this->getLabel() . ' ' . $this->getLevelLabel($level);
        $attributes = [
          'class' => [
            'dvgauth__logo',
          ],
        ];
        $build['#title'] = $level_title_text;
        $build['#level_indicator'] = $this->getLogo($level_indicator, $level_title_text, $attributes);
      }
    }

    return $build;
  }

  /**
   * Get a logo based on the drupal file id.
   *
   * @param int $fid
   *   The file id.
   * @param string $title
   *   Value for the 'title' and 'alt' attributes of the image.
   * @param array $attributes
   *   Other attributes for the image, e.g. 'class'.
   *
   * @return null|string
   *   NULL if the file doesn't exists or can't be created,
   *   otherwise a rendered image is returned.
   */
  public function getLogo($fid, $title, array $attributes) {
    if (!$fid) {
      return NULL;
    }
    if ($file = file_load($fid)) {
      $variables = [
        'path' => $file->uri,
        'width' => NULL,
        'height' => NULL,
        'alt' => $title,
        'title' => $title,
        'attributes' => $attributes,
      ];

      // Svg logo's can't be rendered as an image style,
      // so directly output the svg.
      if (pathinfo($file->filename, PATHINFO_EXTENSION) === 'svg') {
        return theme('image', $variables);
      }

      // @Todo: replace with a more generic image style for dvg_authentication logos.
      $variables['style_name'] = 'digid_logo';
      return theme('image_style', $variables);
    }
    return NULL;
  }

  /**
   * Get the message the user sees when login is successful.
   *
   * Can be overridden by child classes to make the message specific for
   * the used authentication method.
   *
   * @return string
   *   The message.
   */
  public function getLoginSuccessMessage() {
    return t('You have successfully logged into @site using @authentication_provider.', [
      '@site' => variable_get('site_name', t('Drupal')),
      '@authentication_provider' => $this->getLabel(),
    ]);
  }

  /**
   * Get the message the user sees when login has failed.
   *
   * Can be overridden by child classes to make the message specific for
   * the used authentication method.
   *
   * @return string
   *   The message.
   */
  public function getLoginFailedMessage() {
    return t('Error during login in using @authentication_provider.', [
      '@authentication_provider' => $this->getLabel(),
    ]);
  }

  /**
   * Get the additional fields for the webform configure form.
   *
   * @param array $webform_settings
   *   The current webform settings, needed to set previously configured values.
   *
   * @return array
   *   Renderable array with settings fields.
   */
  public function getWebformConfiguration(array $webform_settings) {
    $provider_settings = $webform_settings['methods'][$this->getId()] ?? [];

    $build = [];
    $build['enabled'] = [
      '#id' => 'dvg-authentication-provider-' . $this->getId() . '-enabled',
      '#type' => 'checkbox',
      '#title' => $this->getLabel(),
      '#default_value' => !empty($provider_settings['enabled']),
    ];

    // Display the level selection, when more than one level is available.
    if ($this->hasLevels()) {
      $levels = $this->getEnabledLevels();
      $level_options = [
        '' => t('Select required level'),
      ];
      $level_options += $levels;
      $selected_level = $provider_settings['level'] ?? '';
      $build['level'] = [
        '#type' => 'select',
        '#options' => $level_options,
        '#default_value' => $selected_level,
        '#states' => [
          'visible' => [
            '#dvg-authentication-provider-' . $this->getId() . '-enabled' => [
              'checked' => TRUE,
            ],
          ],
        ],
      ];
    }

    return $build;
  }

  /**
   * Validate the webform configuration settings.
   *
   * @param array $values
   *   The authentication provider values from the form_state array.
   */
  public function validateWebformConfigureSettings(array $values) {
    // Check if the mandatory level is selected.
    if ($values['enabled'] && $this->hasLevels() && empty($values['level'])) {
      form_set_error('dvg_authentication_settings', t('The authentication level is required for !provider.', ['!provider' => $this->getId()]));
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
    $requirements = [];
    $requirements['dvg_authentication_' . $this->getId()] = [
      'title' => t('DvG Authentication @provider', ['@provider' => $this->getLabel()]),
      'value' => t('Enabled'),
      'severity' => REQUIREMENT_OK,
    ];

    $logo_title = [
      t('DvG Authentication'),
      $this->getLabel(),
      t('Logo'),
    ];
    $requirements['dvg_authentication_' . $this->getId() . '_logo'] = [
      'title' => implode(' ', $logo_title),
      'value' => t('Ok'),
      'severity' => REQUIREMENT_OK,
    ];
    if (empty($this->getConfig('logo'))) {
      $requirements['dvg_authentication_' . $this->getId() . '_logo']['value'] = t('No logo uploaded.');
      $link_text = t('Upload a @provider logo.', ['@provider' => $this->getLabel()]);
      $link_path = 'admin/config/services/dvg-authentication/' . $this->getId();
      $requirements['dvg_authentication_' . $this->getId() . '_logo']['description'] = l($link_text, $link_path);
      $requirements['dvg_authentication_' . $this->getId() . '_logo']['severity'] = REQUIREMENT_ERROR;
    }

    return $requirements;
  }

}
