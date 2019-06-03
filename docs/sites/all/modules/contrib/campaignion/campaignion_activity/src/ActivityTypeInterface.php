<?php

namespace Drupal\campaignion_activity;

interface ActivityTypeInterface {
  public function alterQuery(\SelectQuery $query, $operator);
  public function createActivityFromRow($data);
}
