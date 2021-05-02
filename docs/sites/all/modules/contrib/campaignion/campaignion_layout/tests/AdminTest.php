<?php

namespace Drupal\campaignion_layout;

use Drupal\campaignion_layout\Tests\ThemesBaseTest;

/**
 * Tests for the theme settings page.
 */
class AdminTest extends ThemesBaseTest {

  /**
   * Test building the settings form.
   */
  public function testForm() {
    $data['a']['title'] = 'Theme A';
    $data['a']['layouts']['1col']['title'] = 'One column';
    $data['a']['layouts']['2col']['title'] = 'Two columns';
    $data['a']['layouts']['banner']['title'] = 'Banner';
    $themes = parent::injectThemes($data);
    $theme = $themes->getTheme('a');
    $theme->method('defaultLayout')->willReturn('banner');
    $theme->method('setting')->willReturn([
      '1col' => '1col',
      '2col' => 0,
    ]);

    $form = [];
    $form_state['build_info']['args'][0] = 'a';
    campaignion_layout_form_system_theme_settings_alter($form, $form_state);
    $checkboxes = $form['layout_variations']['layout_variations'];
    $this->assertEqual([
      '1col' => '1col',
      '2col' => 0,
    ], $checkboxes['#default_value']);
    $this->assertTrue($checkboxes['banner']['#disabled']);
    $this->assertEqual('banner', $checkboxes['banner']['#value']);
  }

}
