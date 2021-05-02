<?php

namespace Drupal\campaignion_manage;

/**
 * Test theme function for campaignion_manage.
 */
class ThemeTest extends \DrupalUnitTestCase {

  /**
   * Test parse_url with absolute URL.
   */
  public function testParseUrlAbsolute() {
    $element = [
      '#theme' => 'campaignion_manage_pager',
      '#element' => pager_default_initialize(20, 5),
    ];
    $_GET['q'] = 'system/ajax';
    $_SERVER['HTTP_REFERER'] = 'http://example.com/path';
    $html = drupal_render($element);
    $this->assertStringContainsString('href="/path?page=1"', $html);
  }

}
