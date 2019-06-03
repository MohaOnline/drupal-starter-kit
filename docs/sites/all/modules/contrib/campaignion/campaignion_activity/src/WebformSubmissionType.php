<?php

namespace Drupal\campaignion_activity;

class WebformSubmissionType implements ActivityTypeInterface {
  public function alterQuery(\SelectQuery $query, $operator) {
  }
  public function createActivityFromRow($data) {
    return new WebformSubmission($data);
  }
}
