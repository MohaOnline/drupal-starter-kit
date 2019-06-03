<?php

namespace Drupal\campaignion_action;

use \Drupal\campaignion_wizard\WebformTemplateWizard;

class Template extends TypeBase {
  public function wizard($node = NULL) {
    return new WebformTemplateWizard($this->parameters, $node, $this->type);
  }
}
