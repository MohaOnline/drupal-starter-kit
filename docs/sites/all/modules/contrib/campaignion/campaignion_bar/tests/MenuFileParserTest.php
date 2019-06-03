<?php

namespace Drupal\campaignion_bar;

class MenuFileParserTest extends \DrupalUnitTestCase {
  public function testParserWorks() {
    $start = time();
    // import menu
    $menu_name = 'ae-menu';
    $uri = dirname(__FILE__) . '/testfiles/campaignion_bar.txt';
    $parser = new MenuFileParser($menu_name);
    $menu = $parser->parseFile($uri);
    $this->assertTrue($menu instanceof MenuItem);
    $this->assertTrue(time() - $start  < 3, "Parsing menu files isn't terribly slow.");
  }

  public function testToMenuLinks() {
    $menu_name = 'ae-menu';
    $uri = dirname(__FILE__) . '/testfiles/campaignion_bar.txt';
    $parser = new MenuFileParser($menu_name);
    $menu = $parser->fileToMenuLinks($uri);
    foreach ($menu as $key => $item) {
      $must_have = array('mlid', 'plid', 'router_path', 'hidden', 'external', 'expanded', 'weight', 'depth', 'link_title', 'options', 'customized', 'title_callback');
      foreach ($must_have as $key) {
        $this->assertTrue(isset($item[$key]), "'$key' is set for all menu items");
      }
    }
  }

  public function testEmptyFile_givesEmptyArray() {
    $menu_name = 'some';
    $uri = dirname(__FILE__) . '/testfiles/empty.txt';
    $parser = new MenuFileParser($menu_name);
    $this->assertEqual(array(), $parser->fileToMenuLinks($uri));
  }

  public function testSorting() {
    $menu_name = 'some';
    $uri = dirname(__FILE__) . '/testfiles/inverse_sort.txt';
    $parser = new MenuFileParser($menu_name);
    $result = $parser->fileToMenuLinks($uri);
    $this->assertGreaterThan($result['some_b']['weight'], $result['some_a']['weight']);
  }
}
