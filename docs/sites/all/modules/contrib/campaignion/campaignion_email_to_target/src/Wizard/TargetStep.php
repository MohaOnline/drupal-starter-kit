<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use Drupal\campaignion\Forms\EntityFieldForm;
use Drupal\campaignion_wizard\WizardStep;
use Drupal\little_helpers\Services\Container;

/**
 * Wizard step for choosing the action’s targets.
 */
class TargetStep extends WizardStep {

  /**
   * The URL part used for this step.
   *
   * @var string
   */
  protected $step = 'target';

  /**
   * The menu title for this step.
   *
   * @var string
   */
  protected $title = 'Target';
  protected $api;

  /**
   * Create a new step instance.
   */
  public function __construct($wizard, $api = NULL) {
    parent::__construct($wizard);
    $this->api = $api ? $api : Container::get()->loadService('campaignion_email_to_target.api.Client');
  }

  /**
   * Render the options form and the target configuration vue app.
   */
  public function stepForm($form, &$form_state) {
    $form = parent::stepForm($form, $form_state);
    $field = $this->wizard->parameters['email_to_target']['options_field'];
    $this->fieldForm = new EntityFieldForm('node', $this->wizard->node, [$field]);
    $form += $this->fieldForm->formArray($form_state);

    $settings = [];
    // Identifies contact fields within a dataset’s attributes.
    $settings['contactPrefix'] = 'contact.';
    // These are posted by the front end if a new dataset is added.
    $settings['standardColumns'] = [
      [
        'key' => 'email',
        'description' => '',
        'title' => 'Email address',
      ],
      [
        'key' => 'title',
        'description' => '',
        'title' => 'Title',
      ],
      [
        'key' => 'first_name',
        'description' => '',
        'title' => 'First name',
      ],
      [
        'key' => 'last_name',
        'description' => '',
        'title' => 'Last name',
      ],
      [
        'key' => 'salutation',
        'description' => 'This field is how the target will be addressed in the message opening so should include any appropriate titles e.g. ‘Rt Hon John Smith MP’',
        'title' => 'Salutation',
      ],
      [
        'key' => 'display_name',
        'title' => 'Display name of target',
        'description' => 'This name will be shown in the target list, so it needs to be clear to supporters.',
      ],
      [
        'key' => 'group',
        'title' => 'Group',
        'description' => 'Use this field to segment your list if you want to provide different versions of the message to different people, e.g. one CEO gets one message, another CEO gets a different message. You can set these specific messages up on the next step of the page builder.
        ',
      ],
    ];
    // Used by the front end vue app. A set of 'key' => 'regex' pairs.
    $settings['validations'] = [
      // Escape backslashes so JS won’t interpret them as escape sequence.
      'email' => '^[^\\s@]+@[^\\s@]+\\.[^\\s@]+$',
      // First and last name are:
      // - at least one non-whitespace char (which is not ,"@ too)
      // - and no ,"@ chars in the whole string.
      'first_name' => '^\\s*([^,"@\\s]\\s*)+$',
      'last_name' => '^\\s*([^,"@\\s]\\s*)+$',
      'salutation' => '\\S+',
    ];
    // Used by the front end vue app to validate field max length.
    $settings['maxFieldLengths'] = [
      'email' => 255,
      'title' => 255,
      'first_name' => 255,
      'last_name' => 255,
      'salutation' => 255,
      'display_name' => 255,
      'group' => 255,
    ];
    $settings['endpoints']['e2t-api'] = [
      'url' => $this->api->getEndpoint(),
      'token' => $this->api->getAccessToken(),
    ];
    $settings['datasetQuery'] = variable_get_value('campaignion_email_to_target_dataset_query');

    $settings = ['campaignion_email_to_target' => $settings];
    $dir = drupal_get_path('module', 'campaignion_email_to_target');
    $form['#attached']['js'][] = ['data' => $settings, 'type' => 'setting'];
    $form['#attached']['js'][] = [
      'data' => $dir . '/js/datasets_app/datasets_app.vue.min.js',
      'scope' => 'footer',
      'preprocess' => FALSE,
    ];
    $form['#attached']['css'][] = [
      'data' => $dir . '/css/datasets_app/datasets_app.css',
      'group' => 'CSS_DEFAULT',
    ];

    return $form;
  }

  /**
   * Validate the submitted values.
   */
  public function validateStep($form, &$form_state) {
    $this->fieldForm->validate($form, $form_state);
  }

  /**
   * Save the submitted values.
   */
  public function submitStep($form, &$form_state) {
    $this->fieldForm->submit($form, $form_state);
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }

}
