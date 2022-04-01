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
   * @param \Drupal\campaignion_layout\Themes $themes
   *   The themes service.
   * @param \Drupal\campaignion_layout\Theme|null $base
   *   The theme’s base-theme if there it has one, otherwise NULL.
   */
  public function __construct($theme, Themes $themes, Theme $base = NULL) {
    $this->theme = $theme;
    $this->base = $base;
    $this->themes = $themes;
  }

  /**
   * Check whether the theme has layouts.
   */
  public function hasFeature() {
    return in_array('layout_variations', $this->theme->info['features'] ?? []);
  }

  /**
   * Check whether the layout variations feature is enabled for this theme.
   */
  public function hasFeatureEnabled() {
    return $this->hasFeature() && $this->setting('toggle_layout_variations');
  }

  /**
   * Check whether the theme and its layout variations are enabled.
   */
  public function isEnabled() {
    return (bool) $this->theme->status;
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
   * Get layouts that are implemented in this theme or one of its parents.
   *
   * @return string[]
   *   An array of layouts that are declared to be implemented by this theme.
   *   Entries of the array have the layout machine name as both key and value.
   */
  public function implementedLayouts() {
    $implemented = [];
    if ($this->base) {
      $implemented += $this->base->implementedLayouts();
    }
    $info = $this->theme->info['layout'] ?? [];
    $implemented += array_combine($info, $info);
    return $implemented;
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
    $variations = $this->themes->declaredLayouts();
    $variations = array_intersect_key($variations, $this->implementedLayouts());
    if (!$disabled) {
      $enabled = $this->setting('layout_variations') ?? [];
      $default = $this->defaultLayout();
      $enabled[$default] = $default;
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
   * Get the first field item with an avaiable layout.
   *
   * If no matching field item is found then an item with the default layout is
   * returned.
   *
   * @param iterable $items
   *   A list of field items for the current entity.
   *
   * @return \Drupal\campaignion_layout\Item
   *   An Item object representing the matching item and layout.
   */
  public function getLayoutItem(iterable $items) : ?Item {
    foreach ($items as $item) {
      if ($layout = $this->getLayout($item['layout'])) {
        return new Item($layout, $item);
      }
    }
    if ($layout = $this->getLayout($this->defaultLayout(), FALSE)) {
      return new Item($layout, []);
    }
    return NULL;
  }

  /**
   * Get the theme’s default layout.
   *
   * The default layout of a theme can’t be deactivated.
   *
   * @return string
   *   Machine name of the default layout.
   */
  public function defaultLayout() {
    return $this->theme->info['layout_default'] ?? ($this->base ? $this->base->defaultLayout() : 'default');
  }

  /**
   * Get an array of this theme and all its bases keyed by machine name.
   */
  public function baseThemes($include_self = FALSE) {
    $themes = $this->base ? $this->base->baseThemes(TRUE) : [];
    if ($include_self) {
      $themes[$this->theme->name] = $this;
    }
    return $themes;
  }

  /**
   * Include the theme’s template.php and invoke its hook.
   */
  public function invokeLayoutHook() {
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
