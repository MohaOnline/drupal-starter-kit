<?php

namespace Drupal\dvg_authentication_auto_logout;

/**
 * Manages auto logout parameters and actions.
 *
 * E.g. when to log out a user or refresh the timeout counter.
 */
class AutoLogout {

  /**
   * The AutoLogout instance.
   *
   * @var \Drupal\dvg_authentication_auto_logout\AutoLogout
   */
  private static $instance;

  /**
   * Configuration settings of this plugin.
   *
   * @var array
   */
  protected $configuration;

  /**
   * Custom configuration dependant on the profile.
   *
   * Keyed by profile.
   *
   * @var array
   */
  protected $profileConfigurations;

  /**
   * List of custom profiles.
   *
   * @var array
   *   Keyed by profile name.
   *   Value is settings for the profile containing
   *   a callback, title, optional data and optional login message callback.
   */
  protected $profiles;

  /**
   * List of custom profiles that are currently enabled.
   *
   * @var array
   *   Keyed by profile name.
   *   Value is settings for the profile containing
   *   a callback, title, optional data and optional login message callback.
   */
  protected $activeProfiles;

  /**
   * AutoLogout constructor.
   */
  public function __construct() {
    // Load the configuration for this plugin.
    $this->configuration = variable_get('dvg_authentication_auto_logout', DVG_AUTHENTICATION_AUTO_LOGOUT_DEFAULTS);

    $this->profiles = module_invoke_all('auto_logout_profiles');
    drupal_alter('auto_logout_profiles', $this->profiles);
    $this->activeProfiles = [];
    foreach ($this->profiles as $profile => $details) {
      // Remove all empty values, because those are overridden by the
      // default settings.
      $custom_config = array_filter(variable_get('dvg_authentication_auto_logout_' . $profile, []));
      $this->profileConfigurations[$profile] = array_merge($this->configuration, $custom_config);
      if (variable_get('dvg_authentication_auto_logout_enabled_' . $profile, $details['default_enabled'] ?? FALSE)) {
        $this->activeProfiles[$profile] = $details;
      }
    }
  }

  /**
   * Get an instance of AutoLogout, create it if it doesn't exist.
   *
   * The only use of the singleton pattern is to provide easy static caching
   * of an instance. It is however safe to use the constructor itself.
   *
   * @return \Drupal\dvg_authentication_auto_logout\AutoLogout
   *   Instance of the AutoLogout.
   */
  public static function getInstance() {
    if (!isset(static::$instance)) {
      static::$instance = new static();
    }
    return static::$instance;
  }

  /**
   * List of custom profiles.
   *
   * @return array
   *   Keyed by profile name.
   *   Value is settings for the profile containing
   *   a callback, title and optional data.
   */
  public function getProfiles(): array {
    return $this->profiles;
  }

  /**
   * List of custom profiles that are currently enabled.
   *
   * @return array
   *   Keyed by profile name.
   *   Value is settings for the profile containing
   *   a callback, title and optional data.
   */
  public function getActiveProfiles(): array {
    return $this->activeProfiles;
  }

  /**
   * Determines the auto logout profile of the user.
   *
   * Returns NULL if the user does not belong to any profile.
   *
   * @param \stdClass $user
   *   (optional) Drupal user object.
   *
   * @return string|null
   *   Name of the auto logout profile or NULL.
   */
  public function getUserProfile(\stdClass $user = NULL) {
    if (!$user) {
      global $user;
    }
    // Cache profile in the user object.
    if (!isset($user->auto_logout_profile)) {
      foreach ($this->activeProfiles as $profile_name => $details) {
        $belongs_to_profile = $details['callback'];
        if ($belongs_to_profile($user, $details['data'] ?? NULL)) {
          $user->auto_logout_profile = $profile_name;
          break;
        }
      }
    }

    return $user->auto_logout_profile ?? NULL;
  }

  /**
   * Retrieve an auto logout config field.
   *
   * Automatically handles the active profile for the user.
   *
   * @param string $setting
   *   Name of the setting to retrieve.
   * @param \stdClass $user
   *   (optional) Drupal user object.
   *
   * @return mixed|null
   *   The value for the setting or NULL if not available.
   */
  public function getConfig($setting, \stdClass $user = NULL) {
    $profile = $this->getUserProfile($user);
    return $this->profileConfigurations[$profile][$setting] ?? NULL;
  }

  /**
   * Check if the auto logout functionality is active for the user.
   *
   * @param \stdClass $user
   *   (optional) The user to check for.
   *
   * @return bool
   *   TRUE if the current user is an external user.
   */
  public function isActive(\stdClass $user = NULL) {
    return !empty($this->getUserProfile($user));
  }

  /**
   * Get the time remaining before logout.
   *
   * @param \stdClass $user
   *   (optional) Drupal user object.
   *
   * @return int
   *   Number of seconds remaining.
   *   A zero or negative number indicates an expired session.
   */
  public function getRemainingTime(\stdClass $user = NULL) {
    if (!$this->isActive($user)) {
      return 0;
    }

    $activity_left = $this->getRemainingActivityTime($user);
    $session_left = $this->getRemainingMaxSessionTime($user);

    return min($activity_left, $session_left);
  }

  /**
   * Get remaining time based on the users last activity time.
   *
   * @param \stdClass $user
   *   (optional) Drupal user object.
   *
   * @return int
   *   Number of seconds remaining.
   */
  protected function getRemainingActivityTime(\stdClass $user = NULL) {
    $timeout = $this->getConfig('timeout', $user);
    $last_active = $_SESSION['auto_logout_last_activity'] ?? $_SESSION['auto_logout_start'] ?? $timeout;
    return $timeout - (REQUEST_TIME - $last_active);
  }

  /**
   * Get remaining time based on the users max session time.
   *
   * @param \stdClass $user
   *   (optional) Drupal user object.
   *
   * @return int
   *   Number of seconds remaining.
   */
  protected function getRemainingMaxSessionTime(\stdClass $user = NULL) {
    $max_session_time = $this->getConfig('max_session_time', $user);
    $session_start = $_SESSION['auto_logout_start'] ?? $max_session_time;
    return $max_session_time - (REQUEST_TIME - $session_start);
  }

  /**
   * Determine if session should be kept alive.
   *
   * @return bool
   *   TRUE if something about the current context
   *   should keep the connection open. FALSE and
   *   the standard countdown to auto logout applies.
   */
  public function keepAlive() {
    $keep_alive = &drupal_static(__FUNCTION__);
    if (!isset($keep_alive)) {
      $keep_alive = !empty(array_filter(module_invoke_all('auto_logout_keep_alive')));
    }
    return $keep_alive;
  }

  /**
   * Determine if the session can be extended by refreshing.
   *
   * If the remaining time is limited by the max session time there is no
   * point in refreshing. When the time runs out the user will be logged out
   * regardless of recent activity.
   *
   * @param \stdClass $user
   *   (optional) Drupal user object.
   *
   * @return bool
   *   If the session can be extended by refreshing.
   */
  public function canRefresh(\stdClass $user = NULL) {
    return $this->getRemainingMaxSessionTime($user) > $this->getRemainingActivityTime($user);
  }

  /**
   * Determine if auto logout should be prevented.
   *
   * @return bool
   *   TRUE if there is a reason not to auto logout
   *   the current user on the current page.
   */
  public function preventAutoLogout() {
    return !empty(array_filter(module_invoke_all('prevent_auto_logout')));
  }

  /**
   * Get the target attribute for the logout-link in the status bar.
   *
   * @return string
   *   The link target attribute.
   */
  public function getLinkTarget() {
    $link_target = '_self';
    $profile = $this->getUserProfile();
    if (isset($this->activeProfiles[$profile]) && !empty($this->activeProfiles[$profile]['logout_open_blank'])) {
      $link_target = '_blank';
    }
    return $link_target;
  }

  /**
   * Log out the user.
   *
   * Redirect to configured page or the front page.
   */
  public function logout() {
    global $user;
    if ($this->getConfig('auto_logout_use_watchdog')) {
      $msg = 'Session automatically closed for %name by auto logout.';
      $msg_args = ['%name' => $user->name];
      watchdog('dvg_authentication_auto_logout', $msg, $msg_args);
    }

    $inactivity_message = $this->getConfig('inactivity_message');
    if ($inactivity_message) {
      drupal_set_message($inactivity_message);
    }

    $options = [];
    $goto = $this->getConfig('redirect_url') ?: '';
    if ($goto && isset($_GET['destination'])) {
      $options['query']['destination'] = $_GET['destination'];
      unset($_GET['destination']);
    }
    module_load_include('inc', 'user', 'user.pages');
    if (user_is_logged_in()) {
      user_logout_current_user();
    }
    drupal_goto($goto, $options);
  }

  /**
   * Get the login message for the active profile.
   *
   * @param \stdClass $user
   *   (optional) Drupal user object.
   *
   * @return string
   *   Login message for use in
   */
  public function getLoginMessage(\stdClass $user = NULL) {
    if (!$user) {
      global $user;
    }
    $active_profile = $this->getUserProfile($user);
    if (!empty($this->activeProfiles[$active_profile]['login_message_callback'])) {
      $details = $this->activeProfiles[$active_profile];
      $message_callback = $details['login_message_callback'];
      return $message_callback($user, $details['data'] ?? NULL);
    }
    else {
      return t('You are logged into @site.', ['@site' => variable_get('site_name', t('Drupal'))]);
    }
  }

}
