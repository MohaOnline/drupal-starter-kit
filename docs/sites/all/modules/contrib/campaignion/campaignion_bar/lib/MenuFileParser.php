<?php

namespace Drupal\campaignion_bar;

class MenuFileParser {
  protected $menu_name;
  protected $options;
  public function __construct($menu_name, $options = array()) {
    module_load_include('inc', 'menu');
    $this->options = $options;
    $this->menu_name = $menu_name;
  }
  /**
   * File parser function. Reads through the text file and constructs the menu.
   *
   * @param $uri
   *   uri of the uploaded file.
   * @param $menu_name
   *   internal name of existing menu.
   * @return array
   *   array structure of menu.
   */
  public function parseFile($uri) {
    // Keep track of actual weights per level.
    // We have to append to existing items not to mess up the menu.
    $weights = array(0 => 0);
    // Keep track of actual parents per level.
    $menu = new MenuItem();
    $menu->setName($this->menu_name);
    $menu->placeholder = TRUE;
    $parents = array(0 => $menu);
    $errors = array();

    $handle = fopen($uri, "r");
    if (!$handle) {
      throw new ParseException(t("Couldn't open the uploaded file for reading."));
    }

    $level = $current_line = 0;
    while ($line = fgets($handle)) {
      $current_line++;

      // Skip empty lines.
      if (preg_match('/^\s*$/', $line)) {
        continue;
      }
      try {
        $item = $this->parseLine($line, $level, $weights, $parents, $current_line);

        $level = $item->depth;
        $parents[$level] = $item;
        $weights[$level] = $item->weight;
      } catch(ParseLineException $e) {
        $errors[] = t('Error on line @line_number: @error.', array('@line_number' => $current_line, '@error' => $e->getMessage()));
      }
    }

    fclose($handle);
    if (!empty($errors)) {
      throw new ParseException($errors);
    }

    return $menu;
  }

  /**
   * Parse a line of text containing the menu structure.
   * Only * and - are allowed as indentation characters.
   * Menu item definition may or may not contain details in JSON format.
   *
   * @param $line
   *   One line from input file.
   * @param $prev_level
   *   Previous level to build ierarchy.
   * @param $weights
   *   Array of menu items' weights.
   * @param $parents
   *   Array of menu items' parents.
   *
   * @return
   *   Array representing a menu item.
   */
  protected function parseLine($line, $prev_level, array $weights, array $parents, $line_nr) {
    $item = new MenuItem();

    // Set default language
    $langs = array_keys(language_list());
    $path = 'node';

    // JSON is used.
    if (($json_start = strpos($line, '{"')) != 0) {
      $json = substr($line, $json_start);
      $details = json_decode($json);

      // Parse structure and title.
      $base_info = substr($line, 0, $json_start);

      if (is_null($details)) {
        throw new ParseLineException(t('Malformed item details.'));
      }

      // Extract details.
      $path              = !empty($details->url) ? trim($details->url) : $path;
      $item->description = !empty($details->description) ? trim($details->description) : '';
      $item->expanded    = !empty($details->expanded);
      $item->hidden      = !empty($details->hidden);
      $item->language    = !empty($details->lang) && in_array($details->lang, $langs) ? $details->lang : NULL;
      $item->placeholder = !empty($details->placeholder);
    }
    // No JSON is provided, only level and title are specified.
    else {
      $base_info = $line;
    }
    $level = $this->parseLevelTitle($base_info, $item);

    // Make sure this item is only 1 level below the last item.
    if ($level > $prev_level + 1) {
      throw new ParseLineException(t('wrong indentation'));
    }

    if (isset($details->weight)) {
      $item->weight = $details->weight;
    }
    else {
      $weight = isset($weights[$level]) && !($level > $prev_level) ? $weights[$level] + 1 : 0;
      $item->weight = $weight;
    }
    $item->setParent($parents[$level]);
    $item->setPath($path);

    if ($item->language) {
      // Important when setting the language, otherwise it'll be ignored.
      $item->customized = 1;
    }

    return $item;
  }

  /**
   * Parse indentation and title information from a menu item definition line.
   *
   * @param string level and title part of the menu line
   * @return level.
   */
  protected function parseLevelTitle($line, $item) {
    $matches = array();
    if (preg_match('/^([\-]+|[\*]+)?(\s+)?(.*)$/', $line, $matches)) {
      $item->link_title = check_plain(trim($matches[3]));
      $item->setName(strtolower($item->link_title));
      return strlen($matches[1]); // No sense to use drupal_strlen on indentation.
    }
    else {
      throw new ParseLineException(t('missing title or wrong indentation'));
    }
  }


  public function fileToMenuLinks($uri) {
    $menu_tree = $this->parseFile($uri);
    drupal_alter('campaignion_bar_tree', $menu_tree, $this->menu_name);
    
    $items_by_path = array();
    $router_paths  = array();
    $menu = array();
    
    foreach ($menu_tree->depthFirstList() as $item) {
      $array_item = $item->toArray();
      if (count(explode('/', $item->link_path)) > count(explode('/', $item->router_path))) {
        continue;
      }
      $menu[$item->mlid] = $array_item;
      $items_by_path[$item->router_path] = &$menu[$item->mlid];
      $router_paths[] = $item->router_path;
    }

    if (empty($router_paths)) {
      return $menu;
    }

    $query = db_select('menu_router', 'm', array('fetch' => \PDO::FETCH_ASSOC));
    $query->fields('m', array(
      'path',
      'load_functions',
      'to_arg_functions',
      'access_callback',
      'access_arguments',
      'page_callback',
      'page_arguments',
      'delivery_callback',
      'tab_parent',
      'tab_root',
      'title',
      'title_callback',
      'title_arguments',
      'theme_callback',
      'theme_arguments',
      'type',
      'description',
    ))->condition('m.path', $router_paths);
    $router_paths = $query->execute()->fetchAllAssoc('path');
    
    foreach ($menu as $mlid => &$array_item) {
      if (isset($router_paths[$array_item['router_path']])) {
        $array_item += $router_paths[$array_item['router_path']];
      } else {
        unset($menu[$mlid]);
      }
    }
    
    return $menu;
  }
}
