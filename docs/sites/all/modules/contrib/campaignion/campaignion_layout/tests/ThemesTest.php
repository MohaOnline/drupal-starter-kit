<?php

namespace Drupal\campaignion_layout;

use Upal\DrupalUnitTestCase;

/**
 * Test for the theme data wrapper.
 */
class ThemesTest extends DrupalUnitTestCase {

  /**
   * Test getting a specific theme class.
   */
  public function testGetTheme() {
    $themes = new Themes([
      'foo' => (object) [],
    ]);
    $this->assertInstanceOf(Theme::class, $themes->getTheme('foo'));
    $this->assertEmpty($themes->getTheme('bar'));
  }

  /**
   * Test getting all enabled themes.
   */
  public function testEnabledThemes() {
    $data['foo'] = (object) ['name' => 'foo'];
    $data['bar'] = (object) ['name' => 'bar'];
    $data['baz'] = (object) ['name' => 'baz'];
    $themes = $this->getMockBuilder(Themes::class)
      ->setConstructorArgs([$data])
      ->setMethods(['getTheme'])
      ->getMock();

    $foo = $this->createMock(Theme::class);
    $foo->method('isEnabled')->willReturn(TRUE);
    $bar = $this->createMock(Theme::class);
    $bar->method('isEnabled')->willReturn(FALSE);
    $baz = $this->createMock(Theme::class);
    $baz->method('isEnabled')->willReturn(TRUE);
    $themes->method('getTheme')->willReturnOnConsecutiveCalls($foo, $bar, $baz);
    $enabled_themes = $themes->enabledThemes();
    $this->assertEqual(['foo', 'baz'], array_keys($enabled_themes));
  }

}
