<?php

namespace Drupal\campaignion_layout;

use Upal\DrupalUnitTestCase;

/**
 * Test for the theme data wrapper.
 */
class ThemeTest extends DrupalUnitTestCase {

  /**
   * Test checking for the layout_variations feature.
   */
  public function testHasFeature() {
    $data = (object) ['info' => ['features' => ['foo', 'bar']]];
    $theme = new Theme($data);
    $this->assertFalse($theme->hasFeature());

    $data = (object) [
      'info' => ['features' => ['foo', 'layout_variations', 'baz']],
    ];
    $theme = new Theme($data);
    $this->assertTrue($theme->hasFeature());
  }

  /**
   * Test checking for layouts being enabled.
   */
  public function testIsEnabled() {
    $mock_builder = $this->getMockBuilder(Theme::class)
      ->setMethods(['hasFeature', 'setting']);

    $theme = $mock_builder->setConstructorArgs([
      (object) ['status' => 0],
    ])->getMock();
    $this->assertFalse($theme->isEnabled());

    $theme = $mock_builder->setConstructorArgs([
      (object) ['status' => 1],
    ])->getMock();
    $this->assertFalse($theme->isEnabled());

    $theme->method('hasFeature')->willReturn(TRUE);
    $this->assertFalse($theme->isEnabled());

    $theme->method('setting')->willReturn(TRUE);
    $this->assertTrue($theme->isEnabled());
  }

  /**
   * Test checking for the active theme.
   */
  public function testIsActive() {
    $theme = new Theme((object) ['name' => 'foo']);

    $this->assertTrue($theme->isActive('foo'));
    $this->assertFalse($theme->isActive('bar'));
  }

  /**
   * Test getting all enabled layouts as options.
   */
  public function testLayoutOptions() {
    $mock_builder = $this->getMockBuilder(Theme::class)
      ->setMethods(['setting', 'invokeLayoutHook']);

    $theme = $mock_builder->setConstructorArgs([
      (object) ['status' => 1, 'name' => 'foo'],
    ])->getMock();
    $theme->method('invokeLayoutHook')->willReturn([
      'foo' => ['title' => 'Foo'],
      'bar' => ['title' => 'Bar'],
      'baz' => ['title' => 'Baz'],
    ]);
    // No setting yet → No layouts activated.
    $this->assertEqual([], $theme->layoutOptions());

    // Activate the bar-layout.
    $theme->method('setting')->willReturn([
      'bar' => 'bar',
      'baz' => 0,
    ]);
    $this->assertEqual([
      'bar' => 'Bar',
    ], $theme->layoutOptions());

    // List the entire layout info with defaults added.
    $this->assertEqual([
      'foo' => ['name' => 'foo', 'title' => 'Foo', 'fields' => []],
      'bar' => ['name' => 'bar', 'title' => 'Bar', 'fields' => []],
      'baz' => ['name' => 'baz', 'title' => 'Baz', 'fields' => []],
    ], $theme->layouts(TRUE));
  }

}
