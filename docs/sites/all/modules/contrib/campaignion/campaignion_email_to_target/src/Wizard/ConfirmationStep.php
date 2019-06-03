<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use \Drupal\campaignion_wizard\ConfirmStep;
use \Drupal\campaignion_action\ActionBase;

class ConfirmationStep extends ConfirmStep {
  protected $step = 'confirm';
  protected $title = 'Confirm';
  protected $nodeForm;

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);
    $c = &$form['confirm_container'];
    $i = 0;
    foreach (element_children($c) as $key) {
      $c[$key]['#weight'] = $i += 1;

    }
    $action = $this->wizard->node->action;
    $link = $action->testLink('this test-mode link');
    $link = drupal_render($link);
    $c['test'] = [
      '#type' => 'container',
      '#attributes' => ['class' => ['confirm-edit-wrapper']],
      'caption' => ['#markup' => '<h2>Test your action</h2>'],
      'description' => [
        '#markup' => format_string('<p>You can send !link to your beta-testers to see whether everything works as expected. Everyone with this link will be able to access the action even it is not published and all protest-emails will be sent to the user filling out the form.</p>', ['!link' => $link]),
      ],
      '#weight' => 50,
    ];
    $c['buttons']['#weight'] = 100;
    return $form;
  }

}
