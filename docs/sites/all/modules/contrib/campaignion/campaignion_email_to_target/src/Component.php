<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\little_helpers\Services\Container;
use Drupal\little_helpers\Webform\Webform;
use Drupal\little_helpers\Webform\Submission;
use Drupal\campaignion_action\Loader;

use Drupal\campaignion_email_to_target\Channel\Email;

/**
 * Implement behavior for the email to target message webform component.
 */
class Component {

  protected $component;
  protected $webform;
  protected $action;
  protected $options;

  /**
   * Static constructor to inject dependencies based on a component array.
   */
  public static function fromComponent(array $component) {
    $node = node_load($component['nid']);
    $webform = new Webform($node);
    $action = Loader::instance()->actionFromNode($node);
    return new static($component, $webform, $action);
  }

  /**
   * Create a new component instance.
   *
   * @param array $component
   *   The webform component configuration.
   * @param \Drupal\little_helpers\Webform\Webform $webform
   *   A webform wrapper for the form’s node.
   * @param \Drupal\campaignion_email_to_target\Action $action
   *   An email_to_target action instance.
   */
  public function __construct(array $component, Webform $webform, Action $action) {
    $this->component = $component;
    $this->webform = $webform;
    $this->action = $action;
  }

  /**
   * Get a list of parent form keys for this component.
   *
   * @return array
   *   List of parent form keys - just like $element['#parents'].
   */
  public function parents($webform) {
    $parents = [$this->component['form_key']];
    $parent = $this->component;
    while ($parent['pid'] != 0) {
      $parent = $webform->component($parent['pid']);
      array_unshift($parents, $parent['form_key']);
    }
    return $parents;
  }

  /**
   * Disable submit-buttons for this form.
   */
  protected function disableSubmits(&$form) {
    $form['actions']['#access'] = FALSE;
  }

  /**
   * Save submission before redirecting.
   */
  protected function saveSubmission($form, &$original_form_state) {
    $form_state = $original_form_state;
    $form_state['save_draft'] = TRUE;
    webform_client_form_submit($form, $form_state);
    $sid = $form_state['values']['details']['sid'];
    $original_form_state['values']['details']['sid'] = $sid;
    return Submission::load($this->component['nid'], $sid);
  }

  /**
   * Execute the redirect.
   */
  protected function redirect($redirect, $form, &$form_state) {
    $form_state['redirect'] = $redirect->toFormStateRedirect();
    if (module_exists('webform_ajax') && $form['#node']->webform['webform_ajax'] != WEBFORM_AJAX_NO_AJAX) {
      $form_state['webform_completed'] = TRUE;
      unset($form_state['save_draft']);
    }
    else {
      call_user_func_array('drupal_goto', $form_state['redirect']);
    }
  }

  /**
   * Render the webform component.
   */
  public function render(&$element, &$form, &$form_state) {
    // Get list of targets for this node.
    $submission_o = $this->webform->formStateToSubmission($form_state);
    $options = $this->action->getOptions();
    $channel = $this->action->channel();

    $test_mode = !empty($form_state['test_mode']);
    $email = $submission_o->valueByKey('email');

    $element = [
      '#type' => 'fieldset',
      '#theme' => 'campaignion_email_to_target_selector_component',
    ] + $element + [
      '#theme_wrappers' => ['fieldset', 'webform_element'],
      '#title' => $this->component['name'],
      '#description' => $this->component['extra']['description'],
      '#tree' => TRUE,
      '#element_validate' => array('campaignion_email_to_target_selector_validate'),
      '#cid' => $this->component['cid'],
    ];

    if ($test_mode) {
      $element['test_mode'] = [
        '#prefix' => '<p class="test-mode-info">',
        '#markup' => t('Test-mode is active: All emails will be sent to %email.', ['%email' => $email]),
        '#suffix' => '</p>',
      ];
    }

    $element['#attributes']['class'][] = 'email-to-target-selector-wrapper';
    $element['#attributes']['class'][] = 'webform-prefill-exclude';

    try {
      $pairs_or_exclusion = $this->action->targetMessagePairs($submission_o, $test_mode);
    }
    catch (\Exception $e) {
      watchdog_exception('campaignion_email_to_target', $e);
      $element['#title'] = t('Service temporarily unavailable');
      $element['error'] = [
        '#markup' => t('We are sorry! The service is temporarily unavailable. The administrators have been informed. Please try again in a few minutes …'),
      ];
      $element['#attributes']['class'][] = 'email-to-target-error';
      $this->disableSubmits($form);
      return;
    }

    if ($pairs_or_exclusion instanceof Exclusion) {
      $exclusion = $pairs_or_exclusion;
      $element['no_target'] = $exclusion->renderable();
      $element['#attributes']['class'][] = 'email-to-target-no-targets';
      $this->disableSubmits($form);
      if ($redirect = $exclusion->redirect()) {
        $submission = $this->saveSubmission($form, $form_state);
        drupal_alter('campaignion_email_to_target_redirect', $redirect, $submission);
        $this->redirect($redirect, $form, $form_state);
      }
      return;
    }

    $pairs = $pairs_or_exclusion;
    $mode = Container::get()
      ->loadService('campaignion_email_to_target.selection_mode.loader')
      ->getSpec($options['selection_mode'])
      ->instantiate([
        'editable' => !empty($options['users_may_edit']),
        'channel' => $channel,
      ]);
    if (count($pairs) == 1) {
      $mode = $mode->singleMode();
    }
    $form_state['selection_mode'] = $mode;
    $element += $mode->formElement($pairs, $channel);
  }

  /**
   * Validate the user input to the form component.
   */
  public function validate(array $element, array &$form_state) {
    $values = &drupal_array_get_nested_value($form_state['values'], $element['#parents']);
    $values = $form_state['selection_mode']->getValues($element, $values);
  }

  /**
   * Send emails to all selected targets.
   */
  public function sendEmails($data, Submission $submission, $channel) {
    $send_count = 0;
    foreach ($data as $serialized) {
      $m = unserialize($serialized);
      if ($channel->send($m, $submission)) {
        $send_count += 1;
      }
    }
  }

}
