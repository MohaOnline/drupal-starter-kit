<?php

namespace Drupal\campaignion_bar;

function _campaignion_bar_sort_weight($a, $b) {
  return $a->weight == $b->weight ? 0 : (( $a->weight > $b->weight ) ? 1 : -1);
}

class MenuItem {
  public $mlid;
  public $link_title;
  public $children = array();
  public $parent = NULL;
  public $weight = 0;
  public $external = FALSE;
  public $hidden = FALSE;
  public $expanded = FALSE;
  public $options = array();
  public $customized = FALSE;
  public $depth = 0;
  public $language = NULL;
  public $link_path;
  public $placeholder = FALSE;
  public $name = NULL;

  public function __construct($values = array()) {
    foreach ($values as $key => &$val) {
      $this->$key = &$val;
    }
  }

  public function setName($name) {
    $this->name = drupal_clean_css_identifier($name);
    if ($this->parent) {
      $this->mlid = $this->parent->mlid . '_' . $this->name;
    } else {
      $this->mlid = $this->name;
    }
  }

  public function setParent(MenuItem $parent) {
    $this->parent = $parent;
    $this->depth = $parent->depth + 1;
    $this->parent->children[$this->name] = $this;
    $this->mlid = $this->parent->mlid . '_' . $this->name;
  }

  public function depthFirstList(&$list = array()) {
    if (!$this->placeholder)
      $list[] = $this;
    usort($this->children, __NAMESPACE__ . '\\_campaignion_bar_sort_weight');
    foreach ($this->children as $child) {
      $child->depthFirstList($list);
    }
    return $list;
  }

  public function setPath($path) {
    if ($this->external = url_is_external($path)) {
      $this->link_path = $path;
    }
    else {
      $this->link_path = $this->lookupPath($path);
    }
    $this->router_path = _menu_find_router_path($this->link_path);
  }

  /**
   * Helper function to find unaliased path for a given path.
   *
   * @param $path
   *   Path (node/* or any other) as described in the input file.
   *
   * @return
   *   Unaliased path.
   */
  protected function lookupPath($path) {
    if (empty($path) || $path == '<front>') {
      $path = variable_get('site_frontpage', 'node');
    }

    // Search by alias by default, but use only
    // path without query/argument part.
    $query = '';
    $path = explode('?', $path);
    if (count($path) != 1) {
      $query = '?' . $path[1];
    }
    $path = $path[0];

    // Does an alias exist in the system?
    $system_url = drupal_lookup_path('source', $path);
    if (!$system_url) {
      $system_url = $path;
    }
    return $system_url;
  }

  public function toArray() {
    $data = (array) $this;
    unset($data['children']);
    unset($data['parent']);
    unset($data['id']);
    unset($data['description']);
    $data['plid']  = $this->depth > 0 ? $this->parent->mlid : $this->mlid;
    if (!empty($this->description)) {
      $data['options']['attributes']['title'] =  $this->description;
    }
    return $data;
  }

  public function mergeChildren($item) {
    foreach ($item->children as $child) {
      if (!$child->placeholder) {
        $child->setParent($this);
      }
      if (isset($this->children[$child->name])) {
        $this->children[$child->name]->mergeChildren($child);
      }
    }
  }
}
