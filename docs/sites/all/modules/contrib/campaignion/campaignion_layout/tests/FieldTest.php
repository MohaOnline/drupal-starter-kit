<?php

namespace Drupal\campaignion_layout;

use Drupal\little_helpers\Services\Container;
use Upal\DrupalUnitTestCase;

/**
 * Test for the field integration.
 */
class FieldTest extends DrupalUnitTestCase {

  /**
   * Clean up the injected services.
   */
  public function tearDown() {
    Container::get()->inject('campaignion_layout.themes', NULL);
    parent::tearDown();
  }

  /**
   * Inject a themes service with specific metadata.
   */
  protected function injectThemes($themes = []) {
    $theme_objects = [];
    foreach ($themes as $name => $data) {
      $theme = $this->createMock(Theme::class);
      $theme->method('title')->willReturn($data['title'] ?? $name);
      $theme->method('layouts')->willReturn(array_map(function ($info) {
        return $info + ['fields' => []];
      }, $data['layouts']));
      $theme_objects[$name] = $theme;
    }
    $themes = $this->createMock(Themes::class);
    $themes->method('enabledThemes')->willReturn($theme_objects);
    Container::get()->inject('campaignion_layout.themes', $themes);
  }

  /**
   * Generate theme test data.
   */
  protected function twoThemes() {
    $themes['a']['title'] = 'Theme A';
    $themes['a']['layouts']['2col']['title'] = 'Two columns';
    $themes['a']['layouts']['banner'] = [
      'title' => 'Banner',
      'fields' => ['banner' => TRUE],
    ];
    $themes['b']['title'] = 'Theme B';
    $themes['b']['layouts']['2col']['title'] = 'Two columns';
    $themes['b']['layouts']['1col']['title'] = 'Single column';
    return $themes;
  }

  /**
   * Test rendering the field widget if no themes are available.
   */
  public function testFieldWidgetWithoutThemes() {
    $this->injectThemes([]);
    $form = [];
    $form_state = [];
    $element = campaignion_layout_field_widget_form($form, $form_state, NULL, NULL, NULL, [], 0, []);
    $this->assertEqual([], $element['values']['theme']['#options']);
    $this->assertFalse($element['#access']);
  }

  /**
   * Test rendering the field widget with themes.
   */
  public function testFieldWidgetWithThemes() {
    $this->injectThemes($this->twoThemes());
    $form = [];
    $form_state = [];
    $element = campaignion_layout_field_widget_form($form, $form_state, NULL, NULL, NULL, [], 0, []);
    $this->assertFalse($element['enabled']['#default_value']);
    $this->assertEqual([
      'a' => 'Theme A',
      'b' => 'Theme B',
    ], $element['values']['theme']['#options']);
    $this->assertNull($element['values']['theme']['#default_value']);
    $this->assertNotEmpty($element['values']['layout_a']['#options']);
    $this->assertEqual([
      '' => 'Default layout',
      '2col' => 'Two columns',
      'banner' => 'Banner',
    ], $element['values']['layout_a']['#options']);
    $this->assertEqual('', $element['values']['layout_a']['#default_value']);
    $this->assertNotEmpty($element['values']['layout_b']['#options']);
    $this->assertEqual([
      'banner' => ['#layout-a input' => ['banner']],
    ], $form_state['campaignion_layout_fields']);

    $element['#parents'] = [];
    $element['enabled']['#value'] = TRUE;
    $element['values']['theme']['#value'] = 'a';
    $element['values']['layout_a']['#value'] = 'banner';
    $element['values']['layout_b']['#value'] = '1col';
    $form_state['values'] = [];
    _campaignion_layout_field_widget_validate($element, $form_state, $form);
    $this->assertEqual('banner', $form_state['values']['layout']);

    $element['enabled']['#value'] = FALSE;
    $form_state['values'] = [];
    _campaignion_layout_field_widget_validate($element, $form_state, $form);
    $this->assertNull($form_state['values']['theme']);
    $this->assertNull($form_state['values']['layout']);
  }

  /**
   * Test default values with an unknown theme as default value.
   */
  public function testDefaultValuesUnknownTheme() {
    $this->injectThemes($this->twoThemes());
    $form = [];
    $form_state = [];
    $items[] = ['theme' => 'unknown', 'layout' => '2col'];
    $element = campaignion_layout_field_widget_form($form, $form_state, NULL, NULL, NULL, $items, 0, []);
    $this->assertFalse($element['enabled']['#default_value']);
    $this->assertNull($element['values']['theme']['#default_value']);
    $this->assertEqual('', $element['values']['layout_a']['#default_value']);
    $this->assertEqual('', $element['values']['layout_b']['#default_value']);
  }

  /**
   * Test default values with an unknown layout as default value.
   */
  public function testDefaultValuesUnknownLayout() {
    $this->injectThemes($this->twoThemes());
    $form = [];
    $form_state = [];
    $items[] = ['theme' => 'a', 'layout' => 'unknown'];
    $element = campaignion_layout_field_widget_form($form, $form_state, NULL, NULL, NULL, $items, 0, []);
    $this->assertTrue($element['enabled']['#default_value']);
    $this->assertEqual('a', $element['values']['theme']['#default_value']);
    $this->assertEqual('', $element['values']['layout_a']['#default_value']);
    $this->assertEqual('', $element['values']['layout_b']['#default_value']);
  }

  /**
   * Test default values with a default theme and layout.
   */
  public function testDefaultValues() {
    $this->injectThemes($this->twoThemes());
    $form = [];
    $form_state = [];
    $items[] = ['theme' => 'a', 'layout' => '2col'];
    $element = campaignion_layout_field_widget_form($form, $form_state, NULL, NULL, NULL, $items, 0, []);
    $this->assertTrue($element['enabled']['#default_value']);
    $this->assertEqual('a', $element['values']['theme']['#default_value']);
    $this->assertEqual('2col', $element['values']['layout_a']['#default_value']);
    $this->assertEqual('', $element['values']['layout_b']['#default_value']);
  }

  /**
   * Test field item empty check.
   */
  public function testIsEmpty() {
    $item = ['theme' => '', 'layout' => 'foo'];
    $this->assertTrue(campaignion_layout_field_is_empty($item, NULL));
    $item = ['theme' => 'bar', 'layout' => 'foo'];
    $this->assertFalse(campaignion_layout_field_is_empty($item, NULL));
  }

  /**
   * Test that #states are added to the node form.
   */
  public function testNodeFormAlter() {
    $form['layout'][LANGUAGE_NONE][0] = [
      '#type' => 'fieldset',
      '#title' => 'Theme & layout',
    ];
    $form['layout_background_image'] = [];
    $form['#node'] = (object) ['type' => 'petition'];
    $form_state = [];
    campaignion_layout_form_node_form_alter($form, $form_state);
    $this->assertFalse($form['layout_background_image']['#access']);
    unset($form['layout_background_image']['#access']);

    $form_state['campaignion_layout_fields']['layout_background_image']['#layout-a input'] = ['banner'];
    campaignion_layout_form_node_form_alter($form, $form_state);
    $expected['visible']['#layout-a input']['value'] = 'banner';
    $this->assertEqual($expected, $form['layout_background_image']['#states']);

    $this->assertEqual('fieldset', $form['layout']['#type']);
  }

}
