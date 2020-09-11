<?php

use Upal\DrupalUnitTestCase;

/**
 * Test rendering the form elements in various settings.
 */
class ThemeTest extends DrupalUnitTestCase {

  /**
   * Backup globals.
   */
  public function setUp() {
    parent::setUp();
    $this->q = $_GET['q'] ?? NULL;
    $this->lang = $GLOBALS['language_url'] ?? NULL;
  }

  /**
   * Restore globals.
   */
  public function tearDown() {
    $_GET['q'] = $this->q;
    $GLOBALS['language_url'] = $this->lang;
  }

  /**
   * Simple node with node translation.
   */
  public function testTwoLanguages() {
    $languages['en'] = (object) [
      'language' => 'en',
    ];
    $languages['de'] = (object) [
      'language' => 'de',
      'prefix' => 'de',
    ];
    $GLOBALS['language_url'] = $languages['en'];
    $_GET['q'] = 'node/1';
    $links['en'] = [
      'href' => 'node/1',
      'language' => $languages['en'],
      'title' => 'English',
      'attributes' => [
        'title' => 'English node',
      ],
    ];
    $links['de'] = [
      'href' => 'node/2',
      'language' => $languages['de'],
      'title' => 'German',
      'attributes' => [
        'title' => 'German node',
      ],
    ];
    $renderable = [
      '#theme' => 'campaignion_language_switcher',
      '#links' => $links,
    ];
    $html = render($renderable);
    $de_url = url($links['de']['href'], $links['de']);
    $expected = <<<HTML
<ul class="campaignion-language-switcher-default">
  <li class="en active"><a href="/node/1" title="English node" class="active">English</a></li>
  <li class="de"><a href="$de_url" title="German node">German</a></li>
</ul>
HTML;
    $this->assertEqual($expected, trim($html));
  }

}
