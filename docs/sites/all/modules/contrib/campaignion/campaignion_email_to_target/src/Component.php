<?php

namespace Drupal\campaignion_email_to_target;

use Drupal\little_helpers\Webform\Webform;
use Drupal\little_helpers\Webform\Submission;
use Drupal\campaignion_action\Loader;

use Drupal\campaignion_email_to_target\Loader as ModeLoader;

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
        drupal_alter('webform_redirect', $redirect, $submission);
        $this->redirect($redirect, $form, $form_state);
      }
      return;
    }

    $pairs = $pairs_or_exclusion;
    $class = ModeLoader::instance()->getMode($options['selection_mode']);
    $mode = new $class(!empty($options['users_may_edit']));
    if (count($pairs) == 1) {
      $mode = $mode->singleMode();
    }
    $form_state['selection_mode'] = $mode;
    $element += $mode->formElement($pairs);
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
  public function sendEmails($data, Submission $submission) {
    $nid = $submission->nid;
    $node = $submission->webform->node;
    $root_node = $node->tnid ? node_load($node->tnid) : $node;
    $send_count = 0;

    foreach ($data as $serialized) {
      $m = unserialize($serialized);
      $message = new Message($m['message']);
      $message->replaceTokens(NULL, $submission);
      unset($m);

      // Set the HTML property based on availablity of MIME Mail.
      $email['html'] = FALSE;
      // Pass through the theme layer.
      $t = 'campaignion_email_to_target_mail';
      $theme_d = ['message' => $message, 'submission' => $submission];
      $email['message'] = theme([$t, $t . '_' . $nid], $theme_d);

      $email['from'] = $message->from;
      $email['subject'] = $message->subject;

      $email['headers'] = [
        'X-Mail-Domain' => variable_get('site_mail_domain', 'supporter.campaignion.org'),
        'X-Action-UUID' => $root_node->uuid,
      ];

      // Verify that this submission is not attempting to send any spam hacks.
      if (_webform_submission_spam_check($message->to, $email['subject'], $email['from'], $email['headers'])) {
        watchdog('campaignion_email_to_target', 'Possible spam attempt from @remote !message',
                array('@remote' => ip_address(), '!message' => "<br />\n" . nl2br(htmlentities($email['message']))));
        drupal_set_message(t('Illegal information. Data not submitted.'), 'error');
        return FALSE;
      }

      $language = $GLOBALS['language'];
      $mail_params = array(
        'message' => $email['message'],
        'subject' => $email['subject'],
        'headers' => $email['headers'],
        'submission' => $submission,
        'email' => $email,
      );

      // Mail the submission.
      $m = $this->mail($message->to, $language, $mail_params, $email['from']);
      if ($m['result']) {
        $send_count += 1;
      }
    }
  }

  /**
   * Wrapper for drupal_mail().
   */
  protected function mail($to, $language, $mail_params, $from) {
    return drupal_mail('campaignion_email_to_target', 'email_to_target', $to, $language, $mail_params, $from);
  }

}
