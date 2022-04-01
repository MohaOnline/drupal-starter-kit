<?php

namespace Drupal\campaignion_layout;

use Drupal\little_helpers\Services\Container;
use Upal\DrupalUnitTestCase;

/**
 * Test interacting with the field API.
 */
class FieldIntegrationTest extends DrupalUnitTestCase {

  /**
   * Delete the test node.
   */
  public function tearDown() : void {
    Container::get()->inject('campaignion_layout.themes', NULL);
    parent::tearDown();
  }

  /**
   * Test reading a field item from a node with no field item.
   */
  public function testThemeForEntityNoItem() {
    $node = $this->nodeWithItems([]);
    // No field item → no theme.
    $this->assertNull(campaignion_layout_get_theme_for_entity('node', $node));
  }

  /**
   * Test getting a disabled theme.
   */
  public function testThemeForEntityItemDisabledTheme() {
    $node = $this->nodeWithItems([['theme' => 'foo', 'layout' => 'foo_layout']]);
    // Theme is configured in the field item but not enabled.
    $this->injectTheme(FALSE, TRUE, FALSE);
    $this->assertNull(campaignion_layout_get_theme_for_entity('node', $node));
  }

  /**
   * Test getting an enabled theme.
   */
  public function testThemeForEntityItemEnabledTheme() {
    $node = $this->nodeWithItems([['theme' => 'foo', 'layout' => 'foo_layout']]);
    // Theme is configured in the field item and enabled.
    $this->injectTheme(TRUE);
    $this->assertEqual('foo', campaignion_layout_get_theme_for_entity('node', $node));
  }

  /**
   * Test node rendering.
   */
  public function testNodePreprocess() {
    $vars['node'] = $this->nodeWithItems([['theme' => 'dontcare', 'layout' => 'foo']]);
    $vars['node']->field_main_image[LANGUAGE_NONE][0] = [
      'uri' => '/misc/druplicon.png',
    ];
    $vars['theme_hook_suggestions'] = [];
    $vars['page'] = [];
    $theme = $this->injectTheme(TRUE);
    $theme->method('getLayoutItem')->willReturn(new Item([
      'name' => 'foo',
      'fields' => [
        'field_main_image' => [
          'display' => [],
          'variable' => 'main_image',
        ],
      ],
    ], []));
    campaignion_layout_page_build($vars['page'], $vars['node'], 'foo');
    campaignion_layout_preprocess_page($vars);
    $this->assertEqual('foo', $vars['layout']);
    $this->assertEqual(['page__layout__foo'], $vars['theme_hook_suggestions']);
    $this->assertNotEmpty($vars['main_image']);
    $this->assertEqual('field', $vars['main_image']['#theme']);
  }

  /**
   * Test node rendering without layout.
   */
  public function testNodePreprocessWithoutLayout() {
    $vars['node'] = $this->nodeWithItems([]);
    $vars['theme_hook_suggestions'] = [];
    $vars['page'] = [];
    $theme = $this->injectTheme(TRUE);
    $theme->method('getLayoutItem')->willReturn(NULL);
    campaignion_layout_page_build($vars['page'], $vars['node'], 'foo');
    campaignion_layout_preprocess_page($vars);
    $this->assertNull($vars['layout']);
    $this->assertFalse($vars['reversed']);
    $this->assertEqual([], $vars['theme_hook_suggestions']);
  }

  /**
   * Test using the default layout.
   */
  public function testPageBuildDefaultLayout() {
    $node = $this->nodeWithItems([]);
    $page = [];
    $theme = $this->injectTheme(TRUE);
    $theme->expects($this->once())->method('getLayoutItem')->willReturn(new Item([
      'name' => 'foo_default',
    ], []));
    campaignion_layout_page_build($page, $node);
    $this->assertEqual('foo_default', $page['#layout']['name']);
  }

  /**
   * Create a test-node with specific items.
   */
  protected function nodeWithItems(array $items) {
    $node = (object) ['type' => 'petition', 'title' => __CLASS__];
    node_object_prepare($node);
    node_save($node);
    $node->layout[LANGUAGE_NONE] = $items;
    return $node;
  }

  /**
   * Inject a themes service that always returns a enabled/disabled theme.
   */
  protected function injectTheme($enabled, $has_feature = TRUE, $has_feature_enabled = TRUE) {
    $themes = $this->createMock(Themes::class);
    $theme = $this->createMock(Theme::class);
    $themes->method('getTheme')->willReturn($theme);
    Container::get()->inject('campaignion_layout.themes', $themes);
    $theme->method('isEnabled')->willReturn($enabled);
    $theme->method('hasFeature')->willReturn($has_feature);
    $theme->method('hasFeatureEnabled')->willReturn($has_feature_enabled);
    return $theme;
  }

}
