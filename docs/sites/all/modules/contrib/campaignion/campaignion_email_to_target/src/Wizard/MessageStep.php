<?php

namespace Drupal\campaignion_email_to_target\Wizard;

use Drupal\campaignion\Forms\EntityFieldForm;
use Drupal\campaignion_email_to_target\MessageEndpoint;
use Drupal\campaignion_wizard\WizardStep;
use Drupal\little_helpers\Services\Container;

/**
 * Wizard step for configuring the email to target messages.
 */
class MessageStep extends WizardStep {

  /**
   * The URL part used for this step.
   *
   * @var string
   */
  protected $step = 'message';

  /**
   * The menu title for this step.
   *
   * @var string
   */
  protected $title = 'Message';

  /**
   * The entity field form for the no target message.
   *
   * @var \Drupal\campaignion\Forms\EntityFieldForm
   */
  protected $fieldForm;

  /**
   * Render the message configuration form.
   */
  public function stepForm($form, &$form_state) {
    $node = $this->wizard->node;

    $form = parent::stepForm($form, $form_state);
    $form['messages'] = [
      '#type' => 'container',
      '#title' => t('Message that will be sent to target(s)'),
      '#id' => drupal_html_id('email-to-target-messages-widget'),
      '#attributes' => ['class' => ['email-to-target-messages-widget']],
    ];

    $info = token_get_info();
    $tokens = [];
    foreach (['email-to-target', 'webform-tokens', 'submission'] as $type) {
      if (!isset($info['types'][$type])) {
        continue;
      }
      $type_info = $info['types'][$type];
      $group = [
        'title' => $type_info['name'],
        'description' => $type_info['description'],
        'tokens' => [],
      ];
      foreach ($info['tokens'][$type] as $key => $token) {
        $group['tokens'][] = [
          'token' => "[$type:$key]",
          'title' => $token['name'],
          'description' => $token['description'],
        ];
      }
      $tokens[] = $group;
    }
    $settings['tokens'] = $tokens;

    $endpoint = new MessageEndpoint($node);
    $settings += $endpoint->get();
    $settings['targetAttributes'] = [];
    $dataset = $node->action->dataset();
    foreach ($dataset->attributes as $attribute) {
      $settings['targetAttributes'][] = [
        'name' => $attribute->key,
        'label' => $attribute->title,
        'description' => $attribute->description,
      ];
    }
    $settings['hardValidation'] = !$node->status;
    $settings['endpoints']['messages'] = url("node/{$node->nid}/email-to-target-messages");
    $settings['endpoints']['nodes'] = url('wizard/nodes');

    $client = Container::get()->loadService('campaignion_email_to_target.api.Client');
    $settings['endpoints']['e2t-api'] = [
      'url' => $client->getEndpoint(),
      'token' => $client->getAccessToken(),
      'dataset' => $dataset->key,
    ];

    $settings = ['campaignion_email_to_target' => $settings];
    $dir = drupal_get_path('module', 'campaignion_email_to_target');
    $form['#attached']['js'][] = ['data' => $settings, 'type' => 'setting'];
    $form['#attached']['js'][] = [
      'data' => $dir . '/js/messages_app/e2t_messages_app.vue.min.js',
      'scope' => 'footer',
      'preprocess' => FALSE,
    ];
    $form['#attached']['css'][] = [
      'data' => $dir . '/css/messages_app/e2t_messages_app.css',
      'group' => 'CSS_DEFAULT',
    ];

    $field = $this->wizard->parameters['email_to_target']['no_target_message_field'];
    $this->fieldForm = new EntityFieldForm('node', $node, [$field]);
    $form['advanced'] = [
      '#type' => 'fieldset',
      '#title' => t('No target available'),
      '#attributes' => ['class' => ['e2t-col']],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    ];
    $form['advanced'] += $this->fieldForm->formArray($form_state);

    return $form;
  }

  /**
   * Validate the submitted values.
   */
  public function validateStep($form, &$form_state) {
    $this->fieldForm->validate($form['advanced'], $form_state);
  }

  /**
   * Store the submitted values.
   */
  public function submitStep($form, &$form_state) {
    $this->fieldForm->submit($form['advanced'], $form_state);
  }

  public function checkDependencies() {
    return isset($this->wizard->node->nid);
  }

}
