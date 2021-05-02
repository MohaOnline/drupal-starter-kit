<?php

namespace Drupal\campaignion_layout;

use Upal\DrupalUnitTestCase;

/**
 * Test for the theme data wrapper.
 */
class ThemesTest extends DrupalUnitTestCase {

  /**
   * Reset static cache in the Themes class.
   */
  public function tearDown() : void {
    drupal_static_reset(Themes::class . '::declaredLayouts');
    parent::tearDown();
  }

  /**
   * Test getting a specific theme class.
   *
   * @backupGlobals enabled
   */
  public function testGetTheme() {
    $themes = new Themes([
      'foo' => (object) ['info' => ['name' => 'Foo']],
      'baz' => (object) ['info' => ['name' => 'Baz']],
    ], $this->createMock(\DrupalCacheInterface::class));
    $this->assertInstanceOf(Theme::class, $themes->getTheme('foo'));
    $this->assertEmpty($themes->getTheme('bar'));
    // Set 'foo' as the currently active theme.
    $GLOBALS['theme'] = 'foo';
    $theme = $themes->getTheme();
    $this->assertEqual('Foo', $theme->title());
  }

  /**
   * Test getting all enabled themes.
   */
  public function testEnabledThemes() {
    $data['foo'] = (object) ['name' => 'foo'];
    $data['bar'] = (object) ['name' => 'bar'];
    $data['baz'] = (object) ['name' => 'baz'];
    $cache = $this->createMock(\DrupalCacheInterface::class);
    $themes = $this->getMockBuilder(Themes::class)
      ->setConstructorArgs([$data, $cache])
      ->setMethods(['getTheme'])
      ->getMock();

    $foo = $this->createMock(Theme::class);
    $foo->method('isEnabled')->willReturn(TRUE);
    $foo->method('hasFeatureEnabled')->willReturn(TRUE);
    $bar = $this->createMock(Theme::class);
    $bar->method('isEnabled')->willReturn(FALSE);
    $bar->method('hasFeatureEnabled')->willReturn(TRUE);
    $baz = $this->createMock(Theme::class);
    $baz->method('isEnabled')->willReturn(TRUE);
    $baz->method('hasFeatureEnabled')->willReturn(FALSE);
    $themes->method('getTheme')->willReturnOnConsecutiveCalls($foo, $bar, $baz);
    $enabled_themes = $themes->enabledThemes();
    $this->assertEqual(['foo'], array_keys($enabled_themes));
  }

  /**
   * Create themes instance with a list of themes.
   */
  protected function getThemes($mock_themes) {
    $cache = $this->createMock(\DrupalCacheInterface::class);
    $themes = $this->getMockBuilder(Themes::class)
      ->setConstructorArgs([[], $cache])
      ->setMethods(['allThemes'])
      ->getMock();
    $themes->method('allThemes')->willReturn($mock_themes);
    return $themes;
  }

  /**
   * Create a mock theme.
   */
  protected function mockTheme($bases = [], $enabled = TRUE, $feature = TRUE, $layouts = [], $invoke = TRUE) {
    $theme = $this->createMock(Theme::class);
    $theme->method('isEnabled')->willReturn($enabled);
    $theme->method('hasFeature')->willReturn($feature);
    $theme->method('baseThemes')->willReturn($bases);
    $theme->expects(isset($layouts) ? $this->once() : $this->never())
      ->method('invokeLayoutHook')->willReturn($layouts);
    return $theme;
  }

  /**
   * Test getting the declared layouts with a disabled base theme.
   */
  public function testDeclaredLayoutsDisabledBase() {
    $theme['foo_base'] = $this->mockTheme([], FALSE, TRUE, []);
    $theme['foo'] = $this->mockTheme([
      'foo_base' => $theme['foo_base'],
    ], TRUE, TRUE, [
      'foo' => ['title' => 'Foo'],
    ]);
    $themes = $this->getThemes($theme);
    $this->assertEqual([
      'foo' => ['name' => 'foo', 'title' => 'Foo', 'fields' => []],
    ], $themes->declaredLayouts());
  }

  /**
   * Test getting the declared layouts with a disabled theme.
   */
  public function testDeclaredLayoutsDisabledTheme() {
    $theme['foo_base'] = $this->mockTheme([], TRUE, TRUE, []);
    $theme['foo'] = $this->mockTheme([
      'foo_base' => $theme['foo_base'],
    ], FALSE, TRUE, NULL);
    $themes = $this->getThemes($theme);
    $this->assertEqual([], $themes->declaredLayouts());
  }

  /**
   * Test that each theme gets its hook invoked only once.
   */
  public function testDeclaredLayoutsInvokesOnce() {
    $theme['base'] = $this->mockTheme([], TRUE, TRUE, []);
    $theme['foo'] = $this->mockTheme([
      'base' => $theme['base'],
    ], TRUE, TRUE, [
      'foo' => ['title' => 'Foo'],
    ]);
    $theme['bar'] = $this->mockTheme([
      'foo' => $theme['foo'],
    ], TRUE, TRUE, [
      'bar' => ['title' => 'Bar'],
    ]);
    $themes = $this->getThemes($theme);
    $this->assertEqual([
      'foo' => ['name' => 'foo', 'title' => 'Foo', 'fields' => []],
      'bar' => ['name' => 'bar', 'title' => 'Bar', 'fields' => []],
    ], $themes->declaredLayouts());
  }

  /**
   * Test getting all layouts as options.
   */
  public function testLayoutOptions() {
    $mock_themes = $this->getMockBuilder(Themes::class)
      ->setMethods(['declaredLayouts'])
      ->disableOriginalConstructor()
      ->getMock();
    $mock_themes->method('declaredLayouts')->willReturn([
      'foo' => ['title' => 'Foo'],
      'bar' => ['title' => 'Bar', 'fields' => []],
      'baz' => ['title' => 'Baz'],
    ]);
    $mock_themes->layoutOptions();
    $this->assertEqual([
      'foo' => 'Foo',
      'bar' => 'Bar',
      'baz' => 'Baz',
    ], $mock_themes->layoutOptions());
  }

}
