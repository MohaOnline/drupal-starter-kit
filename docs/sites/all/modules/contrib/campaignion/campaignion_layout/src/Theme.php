<?php

namespace Drupal\campaignion_layout;

/**
 * Interact with the configuration of a single theme.
 *
 * Themes that want to provide layouts must have the 'layout_variations' feature
 * activated. If the toggle is active the information about the provided layout
 * variations is queried by invoking THEME_campaignion_layout_info().
 *
 * The module automatically renders an additional configuration on the theme
 * settings page that allows to selectively enable or disable layouts.
 */
class Theme {

  /**
   * The theme data as provided by list_themes().
   *
   * @var object
   */
  protected $theme;

  /**
   * The base theme (if any).
   *
   * @var \Drupal\campaignion_layout\Theme
   */
  protected $base;

  /**
   * Create a new instance by passing the theme data.
   *
   * @param object $theme
   *   The theme object as given by list_themes().
   * @param \Drupal\campaignion_layout\Theme $base
   *   The theme’s base-theme if there it has one.
   */
  public function __construct($theme, Theme $base = NULL) {
    $this->theme = $theme;
    $this->base = $base;
  }

  /**
   * Check whether the theme has layouts.
   */
  public function hasFeature() {
    return in_array('layout_variations', $this->theme->info['features'] ?? []);
  }

  /**
   * Check whether the theme and its layout variations are enabled.
   */
  public function isEnabled() {
    return $this->theme->status && $this->hasFeature() && $this->setting('toggle_layout_variations');
  }

  /**
   * Check whether the theme is the current active theme.
   *
   * @param string $active
   *   Machine name of the current active theme used for testing.
   */
  public function isActive($active = NULL) {
    $active = $active ?? $GLOBALS['theme'];
    return $this->theme->name === $active;
  }

  /**
   * Get the theme’s human-readable title.
   */
  public function title() {
    return $this->theme->info['name'];
  }

  /**
   * Read a theme setting.
   */
  public function setting($setting) {
    return theme_get_setting($setting, $this->theme->name);
  }

  /**
   * Get enabled layout variations for a theme as a #options-array.
   *
   * @param bool $disabled
   *   Whether to include disabled variations.
   *
   * @return string[]
   *   Machine name as key mapped to the translated title for each enabled
   *   layout variation.
   */
  public function layoutOptions(bool $disabled = FALSE) {
    return array_map(function (array $info) {
      return $info['title'];
    }, $this->layouts($disabled));
  }

  /**
   * Get info about all enabled layout variations for a theme.
   */
  public function layouts(bool $disabled = FALSE) {
    $variations = $this->layoutInfo();
    if (!$disabled) {
      $enabled = $this->setting('layout_variations') ?? [];
      $variations = array_intersect_key($variations, array_filter($enabled));
    }
    return $variations;
  }

  /**
   * Check whether a layout is enabled.
   */
  public function getLayout($layout, $if_enabled = TRUE) {
    return $this->layouts(!$if_enabled)[$layout] ?? NULL;
  }

  /**
   * Get the theme’s declared layout metadata (with defaults).
   */
  protected function layoutInfo() {
    $info = $this->invokeLayoutHook();
    foreach ($info as $name => &$i) {
      $i += ['name' => $name, 'fields' => []];
      foreach ($i['fields'] as $field_name => &$f) {
        $f += [
          'display' => [],
          'variable' => $field_name,
        ];
      }
    }
    return $info;
  }

  /**
   * Include the theme’s template.php and invoke its hook.
   */
  protected function invokeLayoutHook() {
    $this->includeTheme();
    $func = $this->theme->name . '_campaignion_layout_info';
    return function_exists($func) ? $func() : [];
  }

  /**
   * Helper function to include a theme’s template.php.
   *
   * @see _drupal_theme_initialize()
   */
  protected function includeTheme() {
    if ($this->base) {
      $this->base->includeTheme();
    }

    // Initialize the theme.
    $theme = $this->theme;
    if (isset($theme->engine)) {
      // Include the engine.
      include_once DRUPAL_ROOT . '/' . $theme->owner;
      $theme_engine = $theme->engine;
      if (function_exists($theme_engine . '_init')) {
        call_user_func($theme_engine . '_init', $theme);
      }
    }
    else {
      // Include non-engine theme file our theme gets one too.
      if (!empty($theme->owner)) {
        include_once DRUPAL_ROOT . '/' . $theme->owner;
      }
    }
  }

}
