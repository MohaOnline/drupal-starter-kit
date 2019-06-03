<?php

namespace Drupal\campaignion_wizard;

use \Drupal\little_helpers\ArrayConfig;

use \Drupal\campaignion\Forms\EntityFieldForm;

class EmailProtestTargetStep extends WizardStep {

  protected $step  = 'target';
  protected $title = 'Target';

  protected $fieldForm = NULL;

  /**
   * Field name of the node field storing the protest options.
   *
   * @var string
   */
  protected $optionsField;

  /**
   * Field name of the node field storing the protest targets.
   *
   * @var string
   */
  protected $targetsField;

  /**
   * Extend parent constructor in order to get the field configuration.
   */
  public function __construct($wizard) {
    parent::__construct($wizard);

    $parameters = $wizard->parameters;
    $defaults['email_protest_fields'] = [
      'options' => 'thank_you_page',
      'target' => 'field_thank_you_pages',
    ];
    ArrayConfig::mergeDefaults($parameters, $defaults);
    $this->optionsField = $parameters['email_protest_fields']['options'];
    $this->targetsField = $parameters['email_protest_fields']['targets'];
  }

  /**
   * Form callback: Render the options and target field widgets.
   */
  public function stepForm($form, &$form_state) {

    $form = parent::stepForm($form, $form_state);

    $this->fieldForm = new EntityFieldForm('node', $this->wizard->node, [
      $this->optionsField,
      $this->targetsField,
    ]);
    $form += $this->fieldForm->formArray($form_state);

    return $form;
  }

  /**
   * Form validate callback: Check if at least on target was configured.
   */
  public function validateStep($form, &$form_state) {
    $this->fieldForm->validate($form, $form_state);

    $targets = $form_state['values'][$this->targetsField]['und'];
    $targets = array_filter($targets, function ($v) {
      return is_array($v) && !empty($v['target_id']);
    });
    if (empty($targets)) {
      form_error($form[$this->targetsField], t('We need at least one target for this action.'));
    }
  }

  public function submitStep($form, &$form_state) {
    $this->fieldForm->submit($form, $form_state);
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }
}

