<?php

/**
 * @file
 * The HostingDeployNode class.
 */

class HostingDeployNode extends HostingNode {
  protected $info_fields = ['field_deployment_strategy'];

  public function nodeView() {
    parent::nodeView();
    $strategies = HostingDeployForm::getStrategyLabels();
    if ($strategy = $this->getFieldValue('field_deployment_strategy')) {
      $this->node->content['info']['field_deployment_strategy']['#markup'] = $strategies[$strategy];
    }
  }

}
