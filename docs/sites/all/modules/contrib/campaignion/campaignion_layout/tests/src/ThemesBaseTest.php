<?php

namespace Drupal\campaignion_layout\Tests;

use Drupal\little_helpers\Services\Container;
use Upal\DrupalUnitTestCase;

use Drupal\campaignion_layout\Theme;
use Drupal\campaignion_layout\Themes;

/**
 * Base test for testing theme & layout handling.
 *
 * The test class provides a simple way of injecting a Themes service with
 * themes and layouts configured.
 */
abstract class ThemesBaseTest extends DrupalUnitTestCase {

  /**
   * Cleanup the injected service.
   */
  public function tearDown() : void {
    Container::get()->inject('campaignion_layout.themes', NULL);
    parent::tearDown();
  }

  /**
   * Inject a themes service with specific theme and layout data.
   */
  protected function injectThemes(array $theme_data = [], array $layouts = []) {
    $themes = $this->getMockBuilder(Themes::class)
      ->disableOriginalConstructor()
      ->setMethods(['enabledThemes', 'declaredLayouts', 'getTheme'])
      ->getMock();
    $theme_objects = [];
    $add_layout_defaults = function ($info) {
      return $info + ['fields' => []];
    };
    foreach ($theme_data as $name => $data) {
      $data += ['layouts' => [], 'name' => $name, 'info' => []];
      $data['info'] += [
        'features' => ['layout_variations'],
        'layout' => array_keys($data['layouts']),
      ];
      $theme = $this->getMockBuilder(Theme::class)
        ->setConstructorArgs([(object) $data, $themes, NULL])
        ->setMethods(['title', 'layouts', 'defaultLayout', 'setting'])
        ->getMock();
      $theme->method('title')->willReturn($data['title'] ?? $name);
      $theme->method('layouts')
        ->willReturn(array_map($add_layout_defaults, $data['layouts']));
      $theme_objects[$name] = $theme;
      $layouts += $data['layouts'];
    }
    $themes->method('enabledThemes')->willReturn($theme_objects);
    $themes->method('declaredLayouts')
      ->willReturn(array_map($add_layout_defaults, $layouts));
    $themes->method('getTheme')->will($this->returnCallback(function($name) use ($theme_objects) {
      return $theme_objects[$name] ?? NULL;
    }));
    Container::get()->inject('campaignion_layout.themes', $themes);
    return $themes;
  }

}
