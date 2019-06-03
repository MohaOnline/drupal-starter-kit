<?php

/**
 * @file TFA module classes
 */

/**
 * Class Tfa
 */
class Tfa {

  /**
   * @var array
   */
  protected $context;

  /**
   * @var TfaBasePlugin
   */
  protected $validatePlugin;

  /**
   * @var array
   */
  protected $loginPlugins = array();

  /**
   * @var array
   */
  protected $fallbackPlugins = array();

  /**
   * @var bool
   */
  protected $complete = FALSE;

  /**
   * @var bool
   */
  protected $fallback = FALSE;

  /**
   * TFA constructor.
   *
   * @param TfaBasePlugin $validate
   *   Validation plugin.
   *
   * @param array $context
   *   Context of TFA process.
   *
   *   Must include key:
   *
   *     - 'uid'
   *       Account uid of user in TFA process.
   *
   *     - 'plugins'
   *       Array of plugin names indexed by type: 'validate', 'fallback',
   *       'login'.
   *
   * @param array $fallbackPlugins
   *   Optional TfaBasePlugin objects for use as fallback indexed by name.
   *
   * @param array $loginPlugins
   *   Optional TfaLoginPluginInterface objects for use during login indexed by
   *   name.
   *
   */
  public function __construct(TfaBasePlugin $validate, array $context, array $fallbackPlugins, array $loginPlugins) {
    $validate_plugin = $context['plugins']['validate'];
    $activePlugins = array('validate' => $validate_plugin);

    $this->validatePlugin = $validate;
    if (!empty($loginPlugins)) {
      foreach ($loginPlugins as $plugin_name => $loginPlugin) {
        $this->loginPlugins[$plugin_name] = $loginPlugin;
        $activePlugins['login'][] = $plugin_name;
      }
    }
    if (!empty($fallbackPlugins)) {
      foreach ($fallbackPlugins as $plugin_name => $fallbackPlugin) {
        // Skip this fallback if its same as validation.
        if ($plugin_name === $validate_plugin) {
          continue;
        }
        // Only use plugins that are ready.
        if ($fallbackPlugin->ready()) {
          $this->fallbackPlugins[$plugin_name] = $fallbackPlugin;
          $activePlugins['fallback'][] = $plugin_name;
        }
      }
      // Mark whether a fallback plugin exists.
      if (!empty($this->fallbackPlugins)) {
        $this->fallback = TRUE;
      }
    }
    $this->context = $context;
    // Save only active plugins.
    $this->context['plugins'] = $activePlugins;
  }

  /**
   * Whether authentication should be allowed and not interrupted.
   *
   * If any plugin returns TRUE then authentication is not interrupted by TFA.
   *
   * @return bool
   */
  public function loginAllowed() {
    if (!empty($this->loginPlugins)) {
      foreach ($this->loginPlugins as $class) {
        if ($class->loginAllowed()) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Determine if TFA process is ready.
   *
   * @return bool Whether process can begin or not.
   */
  public function ready() {
    return $this->validatePlugin->ready();
  }

  /**
   * Get TFA process form from plugin.
   *
   * @param array $form
   * @param array $form_state
   * @return array Form API array.
   */
  public function getForm(array $form, array &$form_state) {
    $form = $this->validatePlugin->getForm($form, $form_state);
    // Allow login plugins to modify form unless validation plugin set
    // '#tfa_no_login' to TRUE.
    if (!empty($this->loginPlugins) && (!isset($form['#tfa_no_login']) || $form['#tfa_no_login'] === FALSE)) {
      foreach ($this->loginPlugins as $class) {
        if (method_exists($class, 'getForm')) {
          $form = $class->getForm($form, $form_state);
        }
      }
    }
    return $form;
  }

  /**
   * Checks if user is allowed to continue with plugin action.
   *
   * @param string $window
   * @return bool
   */
  public function floodIsAllowed($window = '') {
    if (method_exists($this->validatePlugin, 'floodIsAllowed')) {
      return $this->validatePlugin->floodIsAllowed($window);
    }
    return TRUE;
  }

  /**
   * Validate form.
   *
   * @param array $form
   * @param array $form_state
   * @return bool
   */
  public function validateForm(array $form, array &$form_state) {
    return $this->validatePlugin->validateForm($form, $form_state);
  }

  /**
   * Return process error messages.
   *
   * @return array
   */
  public function getErrorMessages() {
    return $this->validatePlugin->getErrorMessages();
  }

  /**
   * Invoke submitForm() on plugins.
   *
   * @param array $form
   * @param array $form_state
   * @return bool Whether the validate plugin is complete.
   *   FALSE will cause tfa_form_submit() to rebuild the form for multi-step.
   */
  public function submitForm(array $form, array &$form_state) {
    // Handle fallback if set.
    if ($this->fallback && isset($form_state['values']['fallback']) && $form_state['values']['op'] === $form_state['values']['fallback']) {
      // Change context to next fallback and reset validatePlugin.
      $this->context['plugins']['validate'] = array_shift($this->context['plugins']['fallback']);
      $plugin = $this->fallbackPlugins[$this->context['plugins']['validate']];
      $this->validatePlugin = $plugin;
      if (empty($this->context['plugins']['fallback'])) {
        $this->fallback = FALSE;
      }
      // Record which plugin is activated as fallback.
      $this->context['active_fallback'] = $this->context['plugins']['validate'];
    }
    // Otherwise invoke plugin submitForm().
    elseif (method_exists($this->validatePlugin, 'submitForm')) {
      // Check if plugin is complete.
      $this->complete = $this->validatePlugin->submitForm($form, $form_state);
    }
    // Allow login plugins to handle form submit.
    if (!empty($this->loginPlugins)) {
      foreach ($this->loginPlugins as $class) {
        if (method_exists($class, 'submitForm')) {
          $class->submitForm($form, $form_state);
        }
      }
    }
    return $this->complete;
  }

  /**
   * Begin the TFA process.
   */
  public function begin() {
    // Invoke begin method on send validation plugins.
    if (method_exists($this->validatePlugin, 'begin')) {
      $this->validatePlugin->begin();
    }
  }

  /**
   * Whether the TFA process has any fallback process.
   *
   * @return bool
   */
  public function hasFallback() {
    return $this->fallback;
  }

  /**
   * Whether it's a fallback process.
   *
   * @return bool
   */
  public function isFallback() {
    return isset($this->context['active_fallback']);
  }

  /**
   * Return TFA context.
   *
   * @return array
   */
  public function getContext() {
    if (method_exists($this->validatePlugin, 'getPluginContext')) {
      $pluginContext = $this->validatePlugin->getPluginContext();
      $this->context['validate_context'] = $pluginContext;
    }
    return $this->context;
  }

  /**
   * Run TFA process finalization.
   */
  public function finalize() {
    // Invoke plugin finalize.
    if (method_exists($this->validatePlugin, 'finalize')) {
      $this->validatePlugin->finalize();
    }
    // Allow login plugins to act during finalization.
    if (!empty($this->loginPlugins)) {
      foreach ($this->loginPlugins as $class) {
        if (method_exists($class, 'finalize')) {
          $class->finalize();
        }
      }
    }
  }

}

/**
 * Class TfaSetup
 */
class TfaSetup {

  /**
   * @var TfaBasePlugin
   */
  protected $setupPlugin;

  /**
   * TFA Setup constructor.
   *
   * @param TfaBasePlugin $setupPlugin
   *   Plugin being set up.
   *
   * @param array $context
   *   Context of TFA process.
   *
   *   Must include key:
   *
   *     - 'uid'
   *       Account uid of user in TFA process.
   *
   */
  public function __construct(TfaBasePlugin $setupPlugin, array $context) {
    $this->setupPlugin = $setupPlugin;
    $this->context = $context;
  }

  /**
   * Run any begin setup processes.
   */
  public function begin() {
    // Invoke begin method on setup plugin.
    if (method_exists($this->setupPlugin, 'begin')) {
      $this->setupPlugin->begin();
    }
  }

  /**
   * Get plugin form.
   *
   * @param array $form
   * @param array $form_state
   * @return array
   */
  public function getForm(array $form, array &$form_state) {
    return $this->setupPlugin->getSetupForm($form, $form_state);
  }

  /**
   * Validate form.
   *
   * @param array $form
   * @param array $form_state
   * @return bool
   */
  public function validateForm(array $form, array &$form_state) {
    return $this->setupPlugin->validateSetupForm($form, $form_state);
  }

  /**
   * Return process error messages.
   *
   * @return array
   */
  public function getErrorMessages() {
    return $this->setupPlugin->getErrorMessages();
  }

  /**
   *
   * @param array $form
   * @param array $form_state
   * @return bool
   */
  public function submitForm(array $form, array &$form_state) {
    return $this->setupPlugin->submitSetupForm($form, $form_state);
  }

  /**
   *
   * @return array
   */
  public function getContext() {
    if (method_exists($this->setupPlugin, 'getPluginContext')) {
      $pluginContext = $this->setupPlugin->getPluginContext();
      $this->context['setup_context'] = $pluginContext;
    }
    return $this->context;
  }
}

/**
 * Base plugin class.
 */
abstract class TfaBasePlugin {

  /**
   * @var string
   */
  protected $code;

  /**
   * @var int
   */
  protected $codeLength;

  /**
   * @var array
   */
  protected $context;

  /**
   * @var array
   */
  protected $errorMessages = array();

  /**
   * @var bool
   */
  protected $isValid;

  /**
   * @var string
   */
  protected $encryptionKey;

  /**
   * Plugin constructor.
   *
   * @param array $context
   *   Context of current TFA process.
   *
   *   Must include key:
   *
   *     - 'uid'
   *       Account uid of user in TFA process.
   *
   *   May include keys:
   *
   *     - 'validate_context'
   *       Plugin-specific context for use during Tfa validation.
   *
   *     - 'setup_context'
   *       Plugin-specific context for use during TfaSetup.
   */
  public function __construct(array $context = array()) {
    $this->context = $context;
    // Default code length is 6.
    $this->codeLength = 6;
    $this->isValid = FALSE;
  }

  /**
   * Determine if the plugin can run for the current TFA context.
   *
   * @return bool
   */
  public function ready() {
    return TRUE;
  }

  /**
   * Get error messages suitable for form_set_error().
   *
   * @return array
   */
  public function getErrorMessages() {
    return $this->errorMessages;
  }

  /**
   * Submit form.
   *
   * @param array $form
   * @param array $form_state
   * @return bool Whether plugin form handling is complete.
   *   Plugins should return FALSE to invoke multi-step.
   */
  public function submitForm(array $form, array &$form_state) {
    return $this->isValid;
  }

  /**
   * Validate code.
   *
   * Note, plugins overriding validate() should be sure to set isValid property
   * correctly or else also override submitForm().
   *
   * @param string $code Code to be validated
   * @return bool Whether code is valid
   */
  protected function validate($code) {
    if ((string) $code === (string) $this->code) {
      $this->isValid = TRUE;
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Generate a random string of characters of length $this->codeLength.
   *
   * @return string
   */
  protected function generate() {
    $string = '';
    do {
      $chars = strtolower(base64_encode(drupal_random_bytes($this->codeLength)));
      // Remove some characters that are more difficult to distinguish or type.
      $string .= strtr($chars, array('+' => '', '/' => '', '=' => '', '-' => '', '_' => '', '0' => '', 'o' => ''));
    } while(strlen($string) <= $this->codeLength);

    return substr($string, 0, $this->codeLength);
  }

  /**
   * Encrypt a plaintext string.
   *
   * Should be used when writing codes to storage.
   *
   * @param string.
   * @return string
   */
  protected function encrypt($text) {
    $td = mcrypt_module_open('rijndael-128', '', 'cbc', '');
    $iv = drupal_random_bytes(mcrypt_enc_get_iv_size($td));

    $key = substr($this->encryptionKey, 0, mcrypt_enc_get_key_size($td));

    mcrypt_generic_init($td, $key, $iv);
    // Encrypt with message length so decryption can return message without
    // padding.
    $text = strlen($text) . '|' . $text;
    $data = mcrypt_generic($td, $text);

    mcrypt_generic_deinit($td);
    mcrypt_module_close($td);

    return $iv . $data;
  }

  /**
   * Decrypt a encrypted string.
   *
   * Should be used when reading codes from storage.
   *
   * @param string
   * @return string
   */
  protected function decrypt($data) {
    $td = mcrypt_module_open('rijndael-128', '', 'cbc', '');
    $iv = substr($data, 0, mcrypt_enc_get_iv_size($td));

    $data = substr($data, mcrypt_enc_get_iv_size($td));
    $key = substr($this->encryptionKey, 0, mcrypt_enc_get_key_size($td));

    mcrypt_generic_init($td, $key, $iv);

    $decrypted_text = mdecrypt_generic($td, $data);
    // Return only the message and none of its padding.
    list($length, $padded_data) = explode('|', $decrypted_text, 2);
    $text = substr($padded_data, 0, $length);

    mcrypt_generic_deinit($td);
    mcrypt_module_close($td);

    return $text;
  }

}

/**
 * Interface TfaValidationPluginInterface
 *
 * Validation plugins interact with the Tfa form processes to provide code entry
 * and validate submitted codes.
 */
interface TfaValidationPluginInterface {

  /**
   * Get TFA process form from plugin.
   *
   * @param array $form
   * @param array $form_state
   * @return array Form API array.
   */
  public function getForm(array $form, array &$form_state);

  /**
   * Validate form.
   *
   * @param array $form
   * @param array $form_state
   * @return bool Whether form passes validation or not
   */
  public function validateForm(array $form, array &$form_state);
}

/**
 * Interface TfaLoginPluginInterface
 *
 * Login plugins interact with the Tfa loginAllowed() process prior to starting
 * a TFA process.
 */
interface TfaLoginPluginInterface {

  /**
   * Whether authentication should be interrupted.
   *
   * @return bool
   */
  public function loginAllowed();
}

/**
 * Interface TfaSendPluginInterface
 *
 * Send plugins interact with the Tfa begin() process to communicate a code
 * during the start of the TFA process.
 *
 * Implementations of a send plugin should also be a validation plugin.
 */
interface TfaSendPluginInterface {

  /**
   * TFA process begin.
   */
  public function begin();
}

/**
 * Interface TfaSetupPluginInterface
 *
 * Setup plugins are used by TfaSetup for configuring a plugin.
 *
 * Implementations of a begin plugin should also be a validation plugin.
 */
interface TfaSetupPluginInterface {

  /**
   * @param array $form
   * @param array $form_state
   */
  public function getSetupForm(array $form, array &$form_state);

  /**
   * @param array $form
   * @param array $form_state
   */
  public function validateSetupForm(array $form, array &$form_state);

  /**
   * @param array $form
   * @param array $form_state
   * @return bool
   */
  public function submitSetupForm(array $form, array &$form_state);

}
