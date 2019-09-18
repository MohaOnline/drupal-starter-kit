<?php

/**
 * GTM Tracker tests.
 */
class GTMTrackerTest extends \DrupalUnitTestCase {

  /**
   * Test that adding the js file works.
   */
  public function testAddingJavascript() {
    $page = ['content' => []];
    campaignion_tracking_gtm_page_build($page);
    $this->assertNotEmpty($page['content']['#attached']['js']);
  }

}
