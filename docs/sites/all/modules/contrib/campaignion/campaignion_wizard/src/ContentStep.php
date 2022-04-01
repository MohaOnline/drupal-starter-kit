<?php

namespace Drupal\campaignion_wizard;

use \Drupal\campaignion\Forms\EmbeddedNodeForm;

class ContentStep extends WizardStep {
  protected $step = 'content';
  protected $title = 'Content';
  protected $nodeForm;

  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);
    $node = $this->wizard->node;

    // New translations should always be unpublished.
    if (empty($node->nid) && !empty($_GET['translation'])) {
      $node->status = 0;
    }

    // load original node form
    $form_state['embedded']['#wizard_type'] = 'content';
    $this->nodeForm = new EmbeddedNodeForm($this->wizard->node, $form_state);
    // Don’t re-execute alter-functions that listen to this property.
    $form['#node_edit_form'] = FALSE;
    $form += $this->nodeForm->formArray($form);

    // we don't want the webform_template selector to show up here.
    unset($form['webform_template']);

    $form[$this->wizard->parameters['thank_you_page']['reference']]['#access'] = FALSE;

    $form['actions']['#access'] = FALSE;
    $form['options']['#access'] = TRUE;
    unset($form['#metatags']);

    // don't publish per default
    if (!isset($this->wizard->node->nid)) {
      $form['options']['status']['#default_value'] = 0;
      $form['options']['promote']['#default_value'] = 0;
    }

    // secondary container
    $form['wizard_secondary'] = array(
      '#type' => 'container',
      '#weight' => 3001,
    );

    $wizard_secondary_used = false;
    // move specific items to secondary container
    if (isset($form['field_main_image'])) {
      $form['field_main_image']['#wizard_secondary'] = TRUE;
    }
    foreach (element_children($form) as $key) {
      if (!empty($form[$key]['#wizard_secondary'])) {
        $form['wizard_secondary'][$key] = $form[$key];
        hide($form[$key]);
        $wizard_secondary_used = true;
      }
    }
    if ($wizard_secondary_used) {
      $form['#attributes']['class'][] = 'wizard-secondary-container';
    }

    return $form;
  }

  public function validateStep($form, &$form_state) {
    $this->nodeForm->validate($form, $form_state);
  }

  public function submitStep($form, &$form_state) {
    parent::submitStep($form, $form_state);
    $this->nodeForm->submit($form, $form_state);
    $node = $form_state['embedded']['node'];
    $form_state['form_info']['path'] = 'node/' . $node->nid . '/wizard/%step';
    $form_state['form_info']['return path'] = 'node/' . $node->nid;
  }

  public function status() {
    return array(
      'caption' => t('Your copy is great'),
      'message' => t('You have added content, a nice picture and a convincing title.'),
    );
  }
}
