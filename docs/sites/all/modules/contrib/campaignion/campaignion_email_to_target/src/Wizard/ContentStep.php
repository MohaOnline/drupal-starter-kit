<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use \Drupal\campaignion_wizard\ContentStep as _ContentStep;

class ContentStep extends _ContentStep {
  protected $step = 'content';
  protected $title = 'Content';
  protected $nodeForm;

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);
    $p = $this->wizard->parameters['email_to_target'];
    $form[$p['no_target_message_field']]['#access'] = FALSE;
    $form[$p['options_field']]['#access'] = FALSE;
    return $form;
  }

}
