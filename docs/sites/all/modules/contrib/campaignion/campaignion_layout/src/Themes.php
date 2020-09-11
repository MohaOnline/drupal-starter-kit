<?php

namespace Drupal\campaignion_layout;

/**
 * Service for instantiating theme classes.
 */
class Themes {

  /**
   * Theme data for all available themes.
   *
   * @var object[]
   *
   * @see list_themes()
   */
  protected $themes;

  /**
   * Create a new instance by reading the theme data from list_themes().
   */
  public static function fromConfig() {
    return new static(list_themes());
  }

  /**
   * Create a new instance by passing the theme data.
   */
  public function __construct(array $themes) {
    $this->themes = $themes;
  }

  /**
   * Create instance for a single theme.
   */
  public function getTheme($theme_name) {
    if ($theme = $this->themes[$theme_name] ?? NULL) {
      $base = isset($theme->base_theme) ? $this->getTheme($theme->base_theme) : NULL;
      return new Theme($theme, $base);
    }
  }

  /**
   * Get all enabled themes.
   */
  public function enabledThemes() {
    $self = $this;
    $all_themes = array_map(function ($theme) use ($self) {
      return $self->getTheme($theme->name);
    }, $this->themes);
    return array_filter($all_themes, function ($theme) {
      return $theme->isEnabled();
    });
  }

}
