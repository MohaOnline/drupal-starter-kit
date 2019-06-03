<?php

namespace Drupal\campaignion_manage\Query;

class Template extends Content {

  public function __construct() {
    parent::__construct();
    $this->query->innerJoin('field_data_action_template', 'fat', 'fat.entity_id = n.nid');
    $this->query->condition('fat.action_template_value', 1);
    $this->reset();
  }

  public function pagerQuery() {
    $query = parent::pagerQuery();
    $query->innerJoin('field_data_action_template', 'fat', 'fat.entity_id = n.nid');
    $query->condition('fat.action_template_value', 1);
    return $query;
  }
}
