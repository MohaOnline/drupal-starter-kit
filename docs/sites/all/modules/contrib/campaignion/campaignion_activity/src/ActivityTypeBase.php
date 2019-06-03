<?php

namespace Drupal\campaignion_activity;

class ActivityTypeBase implements ActivityTypeInterface {
  public function alterQuery(\SelectQuery $query, $operator) {
  }
  public function createActivityFromRow($data) {
    return new Activity($data);
  }
}
