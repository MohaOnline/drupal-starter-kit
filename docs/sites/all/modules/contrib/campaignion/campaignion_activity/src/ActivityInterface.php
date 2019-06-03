<?php

namespace Drupal\campaignion_activity;

interface ActivityInterface {
  public static function load($id);
  public function save();
  public function delete();
}
